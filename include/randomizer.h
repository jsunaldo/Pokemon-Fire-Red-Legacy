#ifndef GUARD_RANDOMIZER_H
#define GUARD_RANDOMIZER_H

#include "global.h"
#include "constants/vars.h"

// FRLG Legacy: save-file-based runtime species randomizer.
// Seed + settings live in unused save vars, so pre-feature saves (all zero)
// behave exactly as before. Set only during NewGameInitData (after ClearSav1).
#define VAR_RANDOMIZER_SEED_LO   VAR_0x408C
#define VAR_RANDOMIZER_SEED_HI   VAR_0x408D
#define VAR_RANDOMIZER_SETTINGS  VAR_0x408E

#define RANDO_F_WILD        (1 << 0) // wild land/water/rock-smash/fishing encounters
#define RANDO_F_TRAINERS    (1 << 1) // non-boss trainer parties
#define RANDO_F_BOSSES      (1 << 2) // leaders/E4/Champion/rival/Giovanni/Oak parties
#define RANDO_F_GIFTS       (1 << 3) // givemon/giveegg/in-game trades (incl. starter)
#define RANDO_F_STATICS     (1 << 4) // setwildbattle statics + roamer
#define RANDO_F_SIMILAR_BST (1 << 5) // remap within similar base-stat totals
#define RANDO_F_LEGENDS     (1 << 6) // allow legendaries in the general pool
#define RANDO_F_ITEMS       (1 << 7) // item balls + hidden items (key items/HMs stay put)
#define RANDO_F_PER_SLOT    (1 << 8) // roll every slot independently instead of one global 1:1 swap
// Generation filter, stored as EXCLUDE bits so older saves (bits clear) keep all
// three gens and their existing shuffle. Applies only to what is randomized.
#define RANDO_F_NO_GEN1     (1 << 9)
#define RANDO_F_NO_GEN2     (1 << 10)
#define RANDO_F_NO_GEN3     (1 << 11)
#define RANDO_GEN_MASK      (RANDO_F_NO_GEN1 | RANDO_F_NO_GEN2 | RANDO_F_NO_GEN3)
// Species-changing categories (items don't count for gens / National Dex).
#define RANDO_SPECIES_MASK  (RANDO_F_WILD | RANDO_F_TRAINERS | RANDO_F_BOSSES | RANDO_F_GIFTS | RANDO_F_STATICS)
// Any content bit set => the randomizer is active for this save.
#define RANDO_ACTIVE_MASK   (RANDO_F_WILD | RANDO_F_TRAINERS | RANDO_F_BOSSES | RANDO_F_GIFTS | RANDO_F_STATICS | RANDO_F_ITEMS)

enum RandomizerCategory
{
    RCAT_WILD,
    RCAT_TRAINER,
    RCAT_BOSS,
    RCAT_GIFT,
    RCAT_STATIC,
};

// Written by the New Game intro UI, consumed by NewGameInitData (flags/vars
// are wiped by ClearSav1, so the choice has to ride EWRAM like gOakSpeechHardMode).
extern u16 gOakSpeechRandoSettings;
extern u32 gOakSpeechRandoSeed;

bool8 Randomizer_IsActive(void);
void Randomizer_TryGrantNationalDex(void);
u32 Randomizer_GetSeed(void);
void Randomizer_BuildTables(void);
u16 Randomizer_MapSpecies(u16 species, u8 category);
u16 Randomizer_MapSpeciesAt(u16 species, u8 category, u32 slotKey);
u32 Randomizer_LocationKey(void);
u16 Randomizer_MapTrainerMonSpecies(u16 trainerNum, u8 partyIndex, u16 species);
void Randomizer_SetLastGift(u16 vanillaSpecies, u16 mappedSpecies);
u16 Randomizer_RemapGiftText(u16 species);
void Randomizer_PreviewStarter(void);
u16 Randomizer_MapFieldItem(u16 item, u32 locationKey);
void Randomizer_RandomizeItemBall(void);

#endif // GUARD_RANDOMIZER_H
