#include "global.h"
#include "randomizer.h"
#include "battle.h"
#include "event_data.h"
#include "pokemon.h"
#include "random.h"
#include "string_util.h"
#include "data.h"
#include "item.h"
#include "constants/items.h"
#include "constants/species.h"
#include "constants/trainers.h"

// FRLG Legacy: save-file-based runtime species randomizer.
// A 32-bit seed + settings bitfield stored in save vars drive a deterministic
// permutation of the 386 real species, rebuilt into EWRAM on new game and on
// every continue. Hooks in wild/trainer/gift/static creation paths remap
// species through Randomizer_MapSpecies, so the same save always produces the
// same randomization and pre-feature saves are untouched.

#define RANDO_BST_WINDOW 20

enum
{
    PARTITION_ALL,
    PARTITION_NORMALS,
    PARTITION_LEGENDS,
};

EWRAM_DATA u16 gOakSpeechRandoSettings = 0;
EWRAM_DATA u32 gOakSpeechRandoSeed = 0;

// EWRAM is nearly full in this hack (~1.3 KB free after these), so the build
// uses a single scratch list and shuffles the map entries in place.
#define POOL_MAX 386

static EWRAM_DATA u16 sSpeciesMap[NUM_SPECIES] = {};
static EWRAM_DATA bool8 sMapBuilt = FALSE;
static EWRAM_DATA u16 sLastGiftVanilla = SPECIES_NONE;
static EWRAM_DATA u16 sLastGiftMapped = SPECIES_NONE;
// Scratch for building the permutation (only live during Randomizer_BuildTables).
static EWRAM_DATA u16 sPoolList[POOL_MAX] = {};

static u32 sRandoRngState;

#include "data/randomizer_item_pool.h"

static const u16 sLegendaries[] =
{
    SPECIES_ARTICUNO, SPECIES_ZAPDOS, SPECIES_MOLTRES, SPECIES_MEWTWO, SPECIES_MEW,
    SPECIES_RAIKOU, SPECIES_ENTEI, SPECIES_SUICUNE, SPECIES_LUGIA, SPECIES_HO_OH,
    SPECIES_CELEBI, SPECIES_REGIROCK, SPECIES_REGICE, SPECIES_REGISTEEL,
    SPECIES_LATIAS, SPECIES_LATIOS, SPECIES_KYOGRE, SPECIES_GROUDON,
    SPECIES_RAYQUAZA, SPECIES_JIRACHI, SPECIES_DEOXYS,
};

// Local ISO LCG so the permutation is deterministic from the saved seed and
// never perturbs gRngValue (same pattern as WildEncounterRandom).
static u16 RandoRand(void)
{
    sRandoRngState = ISO_RANDOMIZE1(sRandoRngState);
    return sRandoRngState >> 16;
}

static bool8 IsRealSpecies(u16 species)
{
    if (species == SPECIES_NONE || species >= NUM_SPECIES)
        return FALSE;
    if (species >= SPECIES_OLD_UNOWN_B && species <= SPECIES_OLD_UNOWN_Z)
        return FALSE;
    return TRUE;
}

static bool8 IsLegendary(u16 species)
{
    u32 i;

    for (i = 0; i < ARRAY_COUNT(sLegendaries); i++)
    {
        if (sLegendaries[i] == species)
            return TRUE;
    }
    return FALSE;
}

static bool8 IsBossTrainerClass(u8 trainerClass)
{
    switch (trainerClass)
    {
    case TRAINER_CLASS_LEADER:      // Kanto leaders + rematches, Johto/Hoenn isle leaders
    case TRAINER_CLASS_ELITE_FOUR:
    case TRAINER_CLASS_CHAMPION:
    case TRAINER_CLASS_BOSS:        // Rocket-boss Giovanni
    case TRAINER_CLASS_RIVAL_EARLY:
    case TRAINER_CLASS_RIVAL_LATE:
    case TRAINER_CLASS_PKMN_PROF:   // Prof. Oak postgame fight
    case TRAINER_CLASS_AQUA_LEADER: // repurposed as Janine's NINJA class
    case TRAINER_CLASS_NURSE:       // secret postgame boss Nurse Joy
    case TRAINER_CLASS_OFFICER:     // secret postgame boss Officer Jenny
        return TRUE;
    }
    return FALSE;
}

static u16 BstOf(u16 species)
{
    const struct SpeciesInfo *info = &gSpeciesInfo[species];

    return info->baseHP + info->baseAttack + info->baseDefense
         + info->baseSpeed + info->baseSpAttack + info->baseSpDefense;
}

// 1, 2 or 3 by National Dex number.
static u8 GenOf(u16 species)
{
    u16 dex = SpeciesToNationalPokedexNum(species);

    if (dex <= 151)
        return 1;
    if (dex <= 251)
        return 2;
    return 3;
}

static bool8 IsGenAllowed(u16 species, u16 settings)
{
    static const u16 sGenBit[] = {0, RANDO_F_NO_GEN1, RANDO_F_NO_GEN2, RANDO_F_NO_GEN3};

    return !(settings & sGenBit[GenOf(species)]);
}

static bool8 InPartition(u16 species, u8 which)
{
    if (!IsRealSpecies(species))
        return FALSE;
    if (which == PARTITION_NORMALS && IsLegendary(species))
        return FALSE;
    if (which == PARTITION_LEGENDS && !IsLegendary(species))
        return FALSE;
    return TRUE;
}

// Generation-restricted global swap: every species in the partition maps to one
// fixed species from the allowed gens. A bijection is impossible when the pool
// is smaller, so allowed targets are reused evenly (or, with SIMILAR BST, the
// target is drawn from the allowed species closest in base stat total).
static void MapPartitionRestricted(u8 which, u16 settings)
{
    u32 i, j, nA = 0, k = 0;
    u16 species;

    for (species = 1; species < NUM_SPECIES; species++)
    {
        if (InPartition(species, which) && IsGenAllowed(species, settings))
            sPoolList[nA++] = species;
    }
    if (nA == 0)
        return; // nothing allowed in this partition: leave it unchanged

    if (settings & RANDO_F_SIMILAR_BST)
    {
        for (i = 1; i < nA; i++) // sort allowed pool by BST
        {
            u16 s = sPoolList[i];
            u16 b = BstOf(s);

            for (j = i; j > 0; j--)
            {
                u16 prevB = BstOf(sPoolList[j - 1]);

                if (prevB < b || (prevB == b && sPoolList[j - 1] < s))
                    break;
                sPoolList[j] = sPoolList[j - 1];
            }
            sPoolList[j] = s;
        }
        for (species = 1; species < NUM_SPECIES; species++)
        {
            u16 b;
            u32 lo, hi, pos, window;

            if (!InPartition(species, which))
                continue;
            b = BstOf(species);
            for (pos = 0; pos < nA && BstOf(sPoolList[pos]) < b; pos++)
                ;
            window = RANDO_BST_WINDOW < nA ? RANDO_BST_WINDOW : nA;
            lo = pos >= window / 2 ? pos - window / 2 : 0;
            if (lo + window > nA)
                lo = nA - window;
            hi = lo + window;
            sSpeciesMap[species] = sPoolList[lo + RandoRand() % (hi - lo)];
        }
    }
    else
    {
        for (i = nA - 1; i > 0; i--) // shuffle the allowed pool
        {
            j = RandoRand() % (i + 1);
            species = sPoolList[i];
            sPoolList[i] = sPoolList[j];
            sPoolList[j] = species;
        }
        for (species = 1; species < NUM_SPECIES; species++)
        {
            if (InPartition(species, which))
                sSpeciesMap[species] = sPoolList[k++ % nA];
        }
    }
}

// Swap the map targets of two partition members. sSpeciesMap starts as the
// identity, so Fisher-Yates over map entries yields a uniform permutation of
// the partition without a separate codomain array.
static void SwapMapEntries(u16 domainA, u16 domainB)
{
    u16 tmp = sSpeciesMap[domainA];

    sSpeciesMap[domainA] = sSpeciesMap[domainB];
    sSpeciesMap[domainB] = tmp;
}

static void ShufflePartition(u8 which, u16 settings)
{
    u32 i, j, n = 0;
    u16 species;

    for (species = 1; species < NUM_SPECIES; species++)
    {
        if (!IsRealSpecies(species))
            continue;
        if (which == PARTITION_NORMALS && IsLegendary(species))
            continue;
        if (which == PARTITION_LEGENDS && !IsLegendary(species))
            continue;
        sPoolList[n++] = species;
    }
    if (n < 2)
        return;

    if (settings & RANDO_F_SIMILAR_BST)
    {
        // Sort the partition by BST (species id tiebreak keeps it deterministic;
        // BST recomputed per compare to save EWRAM — one-time cost at load),
        // then shuffle within a small window so every source maps to a species
        // of similar strength while the whole map stays a bijection.
        for (i = 1; i < n; i++)
        {
            u16 s = sPoolList[i];
            u16 b = BstOf(s);

            for (j = i; j > 0; j--)
            {
                u16 prevB = BstOf(sPoolList[j - 1]);

                if (prevB < b || (prevB == b && sPoolList[j - 1] < s))
                    break;
                sPoolList[j] = sPoolList[j - 1];
            }
            sPoolList[j] = s;
        }
        for (i = 0; i + 1 < n; i++)
        {
            u32 window = RANDO_BST_WINDOW;

            if (window > n - i)
                window = n - i;
            j = i + RandoRand() % window;
            SwapMapEntries(sPoolList[i], sPoolList[j]);
        }
    }
    else
    {
        for (i = n - 1; i > 0; i--)
        {
            j = RandoRand() % (i + 1);
            SwapMapEntries(sPoolList[i], sPoolList[j]);
        }
    }
}

bool8 Randomizer_IsActive(void)
{
    return (VarGet(VAR_RANDOMIZER_SETTINGS) & RANDO_ACTIVE_MASK) != 0;
}

u32 Randomizer_GetSeed(void)
{
    return ((u32)VarGet(VAR_RANDOMIZER_SEED_HI) << 16) | VarGet(VAR_RANDOMIZER_SEED_LO);
}

void Randomizer_BuildTables(void)
{
    u32 i;
    u16 settings = VarGet(VAR_RANDOMIZER_SETTINGS);

    sMapBuilt = TRUE;
    sLastGiftVanilla = SPECIES_NONE;
    sLastGiftMapped = SPECIES_NONE;
    for (i = 0; i < NUM_SPECIES; i++)
        sSpeciesMap[i] = i;
    if (!(settings & RANDO_ACTIVE_MASK))
        return;

    sRandoRngState = Randomizer_GetSeed();
    if (settings & RANDO_GEN_MASK)
    {
        // Some gens excluded: map everything onto the allowed gens.
        if (settings & RANDO_F_LEGENDS)
        {
            MapPartitionRestricted(PARTITION_ALL, settings);
        }
        else
        {
            MapPartitionRestricted(PARTITION_NORMALS, settings);
            MapPartitionRestricted(PARTITION_LEGENDS, settings);
        }
    }
    else if (settings & RANDO_F_LEGENDS)
    {
        // All gens: the original bijective shuffle (existing saves keep their world).
        ShufflePartition(PARTITION_ALL, settings);
    }
    else
    {
        // Legendaries only remap among themselves and never replace a non-legend.
        ShufflePartition(PARTITION_NORMALS, settings);
        ShufflePartition(PARTITION_LEGENDS, settings);
    }
}

// Hash of (seed, category, slot, attempt) for EVERY SLOT mode.
static u32 SlotHash(u32 slotKey, u8 category, u32 attempt)
{
    u32 h = Randomizer_GetSeed() ^ (slotKey * 2654435761u);

    h = ISO_RANDOMIZE1(h ^ ((u32)category * 0x85EBCA6Bu));
    h = ISO_RANDOMIZE1(h ^ (h >> 15) ^ (attempt * 0x9E3779B9u));
    h = ISO_RANDOMIZE1(h ^ (h >> 13));
    return h >> 8;
}

// EVERY SLOT mode: each slot draws its own species, honoring the same
// legendary partition and (optionally) a +/-10% base-stat-total band.
static u16 PickSlotSpecies(u16 species, u8 category, u32 slotKey, u16 settings)
{
    bool8 legendSource = !(settings & RANDO_F_LEGENDS) && IsLegendary(species);
    u16 srcBst = BstOf(species);
    u16 tolerance = srcBst / 10;
    u32 attempt;

    for (attempt = 0; attempt < 256; attempt++)
    {
        u32 h = SlotHash(slotKey, category, attempt);
        u16 candidate;

        if (legendSource)
        {
            candidate = sLegendaries[h % ARRAY_COUNT(sLegendaries)];
        }
        else
        {
            candidate = 1 + h % (NUM_SPECIES - 1);
            if (!IsRealSpecies(candidate))
                continue;
            if (!(settings & RANDO_F_LEGENDS) && IsLegendary(candidate))
                continue;
        }
        if (!IsGenAllowed(candidate, settings))
            continue;
        if (settings & RANDO_F_SIMILAR_BST)
        {
            u16 bst = BstOf(candidate);

            if (bst + tolerance < srcBst || bst > srcBst + tolerance)
                continue;
        }
        return candidate;
    }
    return sSpeciesMap[species]; // global map is already partition/BST/gen-safe
}

// Current map as the high half of a slot key.
u32 Randomizer_LocationKey(void)
{
    return ((u32)gSaveBlock1Ptr->location.mapGroup << 24)
         | ((u32)gSaveBlock1Ptr->location.mapNum << 16);
}

u16 Randomizer_MapSpecies(u16 species, u8 category)
{
    return Randomizer_MapSpeciesAt(species, category, species);
}

// slotKey identifies the encounter slot / party slot / gift; it only matters
// in EVERY SLOT mode (otherwise one global 1:1 swap applies everywhere).
u16 Randomizer_MapSpeciesAt(u16 species, u8 category, u32 slotKey)
{
    static const u16 sCategoryBit[] =
    {
        [RCAT_WILD]    = RANDO_F_WILD,
        [RCAT_TRAINER] = RANDO_F_TRAINERS,
        [RCAT_BOSS]    = RANDO_F_BOSSES,
        [RCAT_GIFT]    = RANDO_F_GIFTS,
        [RCAT_STATIC]  = RANDO_F_STATICS,
    };
    u16 settings = VarGet(VAR_RANDOMIZER_SETTINGS);

    if (!(settings & RANDO_ACTIVE_MASK))
        return species;
    if (category >= ARRAY_COUNT(sCategoryBit) || !(settings & sCategoryBit[category]))
        return species;
    if (!IsRealSpecies(species))
        return species;
    if (!sMapBuilt) // backstop for any load path that skipped the explicit rebuild
        Randomizer_BuildTables();
    if (settings & RANDO_F_PER_SLOT)
        return PickSlotSpecies(species, category, slotKey, settings);
    return sSpeciesMap[species];
}

u16 Randomizer_MapTrainerMonSpecies(u16 trainerNum, u8 partyIndex, u16 species)
{
    u8 category = IsBossTrainerClass(gTrainers[trainerNum].trainerClass) ? RCAT_BOSS : RCAT_TRAINER;

    return Randomizer_MapSpeciesAt(species, category, ((u32)trainerNum << 8) | partyIndex);
}

void Randomizer_SetLastGift(u16 vanillaSpecies, u16 mappedSpecies)
{
    sLastGiftVanilla = vanillaSpecies;
    sLastGiftMapped = mappedSpecies;
}

// bufferspeciesname immediately after a gift buffers the gift's vanilla
// constant; swap in the mapped species so "received X!" matches the mon,
// without touching unrelated hint dialogue.
u16 Randomizer_RemapGiftText(u16 species)
{
    if (!(VarGet(VAR_RANDOMIZER_SETTINGS) & RANDO_F_GIFTS))
        return species;
    if (species != SPECIES_NONE && species == sLastGiftVanilla)
        return sLastGiftMapped;
    return species;
}

// Oak's Lab starter choice: preview the species the chosen ball will really
// give. VAR_0x8004 = species to show, STR_VAR_2 = its name, VAR_RESULT = TRUE
// when the randomizer changed it (the script then uses neutral wording).
void Randomizer_PreviewStarter(void)
{
    u16 vanilla = VarGet(VAR_TEMP_2);
    // Same key ScriptGiveMon uses, so the preview matches the mon received.
    u16 species = Randomizer_MapSpeciesAt(vanilla, RCAT_GIFT, Randomizer_LocationKey() | vanilla);

    gSpecialVar_0x8004 = species;
    gSpecialVar_Result = (species != vanilla);
    StringCopy(gStringVar2, gSpeciesNames[species]);
}

// Item randomizer: each item location (keyed by map + object, or by the hidden
// item's flag) gets a fixed random item from sRandoItemPool, derived from the
// save's seed. Key items and HMs are never moved, so progression is safe.
u16 Randomizer_MapFieldItem(u16 item, u32 locationKey)
{
    u32 h;

    if (!(VarGet(VAR_RANDOMIZER_SETTINGS) & RANDO_F_ITEMS) || item == ITEM_NONE)
        return item;
    if (ItemId_GetPocket(item) == POCKET_KEY_ITEMS)
        return item;
    if (item >= ITEM_HM01 && item <= ITEM_HM08)
        return item;
    h = Randomizer_GetSeed() ^ (locationKey * 2654435761u);
    h = ISO_RANDOMIZE1(h);
    h = ISO_RANDOMIZE1(h ^ (h >> 15));
    return sRandoItemPool[(h >> 8) % ARRAY_COUNT(sRandoItemPool)];
}

// Std_FindItem: VAR_0x8000 holds the ball's item; key it by map + object.
void Randomizer_RandomizeItemBall(void)
{
    u32 key = Randomizer_LocationKey() | gSpecialVar_LastTalked;

    gSpecialVar_0x8000 = Randomizer_MapFieldItem(gSpecialVar_0x8000, key);
}

// A randomized save that can produce Gen 2/3 Pokémon gets the National Dex from
// the start (otherwise those catches aren't even recorded). Called on new game
// and on every continue, so existing randomized saves pick it up too.
void Randomizer_TryGrantNationalDex(void)
{
    u16 settings = VarGet(VAR_RANDOMIZER_SETTINGS);

    if (!(settings & RANDO_SPECIES_MASK))
        return;
    if ((settings & RANDO_F_NO_GEN2) && (settings & RANDO_F_NO_GEN3))
        return;
    if (!IsNationalPokedexEnabled())
        EnableNationalPokedex();
}
