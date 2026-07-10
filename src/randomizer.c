#include "global.h"
#include "randomizer.h"
#include "battle.h"
#include "event_data.h"
#include "pokemon.h"
#include "random.h"
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
    if (settings & RANDO_F_LEGENDS)
    {
        ShufflePartition(PARTITION_ALL, settings);
    }
    else
    {
        // Legendaries only remap among themselves and never replace a non-legend.
        ShufflePartition(PARTITION_NORMALS, settings);
        ShufflePartition(PARTITION_LEGENDS, settings);
    }
}

u16 Randomizer_MapSpecies(u16 species, u8 category)
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
    return sSpeciesMap[species];
}

u16 Randomizer_MapTrainerMonSpecies(u16 trainerNum, u16 species)
{
    u8 category = IsBossTrainerClass(gTrainers[trainerNum].trainerClass) ? RCAT_BOSS : RCAT_TRAINER;

    return Randomizer_MapSpecies(species, category);
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
