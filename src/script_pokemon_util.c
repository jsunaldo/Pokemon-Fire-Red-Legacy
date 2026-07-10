#include "global.h"
#include "gflib.h"
#include "berry.h"
#include "daycare.h"
#include "event_data.h"
#include "load_save.h"
#include "overworld.h"
#include "party_menu.h"
#include "pokedex.h"
#include "randomizer.h"
#include "script_pokemon_util.h"
#include "constants/items.h"
#include "constants/pokemon.h"

static void CB2_ReturnFromChooseHalfParty(void);
static void CB2_ReturnFromChooseBattleTowerParty(void);

void HealPlayerParty(void)
{
    u8 i, j;
    u8 ppBonuses;
    u8 arg[4];

    // restore HP.
    for(i = 0; i < gPlayerPartyCount; i++)
    {
        u16 maxHP;
        // FRLG Legacy Hard Mode: a fainted Pokemon is dead for good — the Pokemon
        // Center (and any scripted heal) leaves it at 0 HP. Swap in a living mon
        // from the PC to keep going.
        if (IsMonPermanentlyDead(&gPlayerParty[i]))
            continue;
        maxHP = GetMonData(&gPlayerParty[i], MON_DATA_MAX_HP);
        arg[0] = maxHP;
        arg[1] = maxHP >> 8;
        SetMonData(&gPlayerParty[i], MON_DATA_HP, arg);
        ppBonuses = GetMonData(&gPlayerParty[i], MON_DATA_PP_BONUSES);

        // restore PP.
        for(j = 0; j < MAX_MON_MOVES; j++)
        {
            arg[0] = CalculatePPWithBonus(GetMonData(&gPlayerParty[i], MON_DATA_MOVE1 + j), ppBonuses, j);
            SetMonData(&gPlayerParty[i], MON_DATA_PP1 + j, arg);
        }

        // since status is u32, the four 0 assignments here are probably for safety to prevent undefined data from reaching SetMonData.
        arg[0] = 0;
        arg[1] = 0;
        arg[2] = 0;
        arg[3] = 0;
        SetMonData(&gPlayerParty[i], MON_DATA_STATUS, arg);
    }
}

// FRLG Legacy: count the player's Gym Badges into VAR_RESULT. The flexible gyms
// (Erika/Koga/Sabrina/Blaine) use this to pick a leader team scaled to the
// player's progress, so those four can be cleared in any order and still fight
// at an appropriate level.
void GetPlayerGymBadgeCount(void)
{
    u32 flag;
    u8 count = 0;

    for (flag = FLAG_BADGE01_GET; flag <= FLAG_BADGE08_GET; flag++)
    {
        if (FlagGet(flag))
            count++;
    }
    gSpecialVar_Result = count;
}

// FRLG Legacy: VAR_RESULT = TRUE only if every Kanto gym leader has been re-beaten
// in the current rematch stage (the 8 contiguous KANTO_REMATCH flags). Gates the
// Elite Four rematch, and (after the Johto Isles) the Tier 3 Elite Four.
void CheckAllKantoGymsRematched(void)
{
    u32 flag;

    gSpecialVar_Result = TRUE;
    for (flag = FLAG_KANTO_REMATCH_BROCK; flag <= FLAG_KANTO_REMATCH_GIOVANNI; flag++)
    {
        if (!FlagGet(flag))
        {
            gSpecialVar_Result = FALSE;
            break;
        }
    }
}

u8 ScriptGiveMon(u16 species, u8 level, u16 item, u32 unused1, u32 unused2, u8 unused3)
{
    u16 nationalDexNum;
    int sentToPc;
    u8 heldItem[2];
    u16 vanillaSpecies = species;
    struct Pokemon *mon = AllocZeroed(sizeof(struct Pokemon));

    species = Randomizer_MapSpecies(species, RCAT_GIFT);
    // Gift scripts buffer the vanilla constant right after givemon; remember the
    // pair so bufferspeciesname can print the species actually received.
    Randomizer_SetLastGift(vanillaSpecies, species);
    CreateMon(mon, species, level, 32, 0, 0, OT_ID_PLAYER_ID, 0);
    heldItem[0] = item;
    heldItem[1] = item >> 8;
    SetMonData(mon, MON_DATA_HELD_ITEM, heldItem);
    sentToPc = GiveMonToPlayer(mon);
    nationalDexNum = SpeciesToNationalPokedexNum(species);

    switch(sentToPc)
    {
    case MON_GIVEN_TO_PARTY:
    case MON_GIVEN_TO_PC:
        GetSetPokedexFlag(nationalDexNum, FLAG_SET_SEEN);
        GetSetPokedexFlag(nationalDexNum, FLAG_SET_CAUGHT);
        break;
    }

    Free(mon);
    return sentToPc;
}

u8 ScriptGiveEgg(u16 species)
{
    struct Pokemon *mon = AllocZeroed(sizeof(struct Pokemon));
    bool8 isEgg;
    bool8 sentToPc;

    // Remap here, not in CreateEgg — daycare eggs already carry bred species.
    species = Randomizer_MapSpecies(species, RCAT_GIFT);
    CreateEgg(mon, species, TRUE);
    isEgg = TRUE;
    SetMonData(mon, MON_DATA_IS_EGG, &isEgg);

    sentToPc = GiveMonToPlayer(mon);
    Free(mon);
    return sentToPc;
}

void HasEnoughMonsForDoubleBattle(void)
{
    switch (GetMonsStateToDoubles())
    {
    case PLAYER_HAS_TWO_USABLE_MONS:
        gSpecialVar_Result = PLAYER_HAS_TWO_USABLE_MONS;
        break;
    case PLAYER_HAS_ONE_MON:
        gSpecialVar_Result = PLAYER_HAS_ONE_MON;
        break;
    case PLAYER_HAS_ONE_USABLE_MON:
        gSpecialVar_Result = PLAYER_HAS_ONE_USABLE_MON;
        break;
    }
}

static bool8 CheckPartyMonHasHeldItem(u16 item)
{
    int i;

    for(i = 0; i < PARTY_SIZE; i++)
    {
        u16 species = GetMonData(&gPlayerParty[i], MON_DATA_SPECIES_OR_EGG);
        if (species != SPECIES_NONE && species != SPECIES_EGG && GetMonData(&gPlayerParty[i], MON_DATA_HELD_ITEM) == item)
            return TRUE;
    }
    return FALSE;
}

bool8 DoesPartyHaveEnigmaBerry(void)
{
    bool8 hasItem = CheckPartyMonHasHeldItem(ITEM_ENIGMA_BERRY);
    if (hasItem == TRUE)
        GetBerryNameByBerryType(ItemIdToBerryType(ITEM_ENIGMA_BERRY), gStringVar1);

    return hasItem;
}

void CreateScriptedWildMon(u16 species, u8 level, u16 item)
{
    u8 heldItem[2];

    species = Randomizer_MapSpecies(species, RCAT_STATIC);
    ZeroEnemyPartyMons();
    CreateMon(&gEnemyParty[0], species, level, 32, 0, 0, OT_ID_PLAYER_ID, 0);
    if (item)
    {
        heldItem[0] = item;
        heldItem[1] = item >> 8;
        SetMonData(&gEnemyParty[0], MON_DATA_HELD_ITEM, heldItem);
    }
}

void ScriptSetMonMoveSlot(u8 monIndex, u16 move, u8 slot)
{
    if (monIndex > PARTY_SIZE)
        monIndex = gPlayerPartyCount - 1;

    SetMonMoveSlot(&gPlayerParty[monIndex], move, slot);
}

// Note: When control returns to the event script, gSpecialVar_Result will be
// TRUE if the party selection was successful.
void ChooseHalfPartyForBattle(void)
{
    gMain.savedCallback = CB2_ReturnFromChooseHalfParty;
//    VarSet(VAR_FRONTIER_FACILITY, FACILITY_MULTI_OR_EREADER);
    InitChooseMonsForBattle(CHOOSE_MONS_FOR_CABLE_CLUB_BATTLE);
}

static void CB2_ReturnFromChooseHalfParty(void)
{
    switch (gSelectedOrderFromParty[0])
    {
    case 0:
        gSpecialVar_Result = FALSE;
        break;
    default:
        gSpecialVar_Result = TRUE;
        break;
    }

    SetMainCallback2(CB2_ReturnToFieldContinueScriptPlayMapMusic);
}

void ChooseBattleTowerPlayerParty(void)
{
    gMain.savedCallback = CB2_ReturnFromChooseBattleTowerParty;
    InitChooseMonsForBattle(CHOOSE_MONS_FOR_BATTLE_TOWER);
}

static void CB2_ReturnFromChooseBattleTowerParty(void)
{
    switch (gSelectedOrderFromParty[0])
    {
    case 0:
        LoadPlayerParty();
        gSpecialVar_Result = FALSE;
        break;
    default:
        ReducePlayerPartyToThree();
        gSpecialVar_Result = TRUE;
        break;
    }

    SetMainCallback2(CB2_ReturnToFieldContinueScriptPlayMapMusic);
}

void ReducePlayerPartyToThree(void)
{
    struct Pokemon * party = AllocZeroed(3 * sizeof(struct Pokemon));
    int i;

    // copy the selected pokemon according to the order.
    for (i = 0; i < 3; i++)
        if (gSelectedOrderFromParty[i]) // as long as the order keeps going (did the player select 1 mon? 2? 3?), do not stop
            party[i] = gPlayerParty[gSelectedOrderFromParty[i] - 1]; // index is 0 based, not literal

    CpuFill32(0, gPlayerParty, sizeof gPlayerParty);

    // overwrite the first 3 with the order copied to.
    for (i = 0; i < 3; i++)
        gPlayerParty[i] = party[i];

    CalculatePlayerPartyCount();
    Free(party);
}
