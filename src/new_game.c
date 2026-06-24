#include "global.h"
#include "clock.h"
#include "new_game.h"
#include "random.h"
#include "pokemon.h"
#include "roamer.h"
#include "pokemon_size_record.h"
#include "script.h"
#include "lottery_corner.h"
#include "play_time.h"
#include "mauville_old_man.h"
#include "match_call.h"
#include "lilycove_lady.h"
#include "load_save.h"
#include "pokeblock.h"
#include "dewford_trend.h"
#include "berry.h"
#include "rtc.h"
#include "easy_chat.h"
#include "event_data.h"
#include "constants/maps.h"
#include "constants/flags.h"
#include "constants/vars.h"
#include "money.h"
#include "trainer_hill.h"
#include "trainer_tower.h"
#include "tv.h"
#include "coins.h"
#include "text.h"
#include "overworld.h"
#include "mail.h"
#include "battle_records.h"
#include "item.h"
#include "pokedex.h"
#include "apprentice.h"
#include "frontier_util.h"
#include "pokedex.h"
#include "save.h"
#include "link_rfu.h"
#include "main.h"
#include "contest.h"
#include "item_menu.h"
#include "pokemon_storage_system.h"
#include "pokemon_jump.h"
#include "decoration_inventory.h"
#include "secret_base.h"
#include "string_util.h"
#include "player_pc.h"
#include "field_specials.h"
#include "berry_powder.h"
#include "mystery_gift.h"
#include "union_room_chat.h"
#include "constants/map_groups.h"
#include "constants/items.h"
#include "difficulty.h"
#include "follower_npc.h"

extern const u8 EventScript_ResetAllMapFlags[];
extern const u8 EventScript_ResetAllMapFlagsFrlg[];

static void ClearFrontierRecord(void);
static void WarpToTruck(void);
static void ResetMiniGamesRecords(void);
static void ResetItemFlags(void);
static void ResetDexNav(void);

EWRAM_DATA bool8 gDifferentSaveFile = FALSE;
EWRAM_DATA bool8 gEnableContestDebugging = FALSE;

static const struct ContestWinner sContestWinnerPicDummy =
{
    .monName = _(""),
    .trainerName = _("")
};

void SetTrainerId(u32 trainerId, u8 *dst)
{
    dst[0] = trainerId;
    dst[1] = trainerId >> 8;
    dst[2] = trainerId >> 16;
    dst[3] = trainerId >> 24;
}

u32 GetTrainerId(u8 *trainerId)
{
    return (trainerId[3] << 24) | (trainerId[2] << 16) | (trainerId[1] << 8) | (trainerId[0]);
}

void CopyTrainerId(u8 *dst, u8 *src)
{
    s32 i;
    for (i = 0; i < TRAINER_ID_LENGTH; i++)
        dst[i] = src[i];
}

static void InitPlayerTrainerId(void)
{
    u32 trainerId = gIronmonFixedSeed /*IronMon Nuzlocke EM: seed per-run*/;
    SetTrainerId(trainerId, gSaveBlock2Ptr->playerTrainerId);
}

// L=A isnt set here for some reason.
static void SetDefaultOptions(void)
{
    gSaveBlock2Ptr->optionsTextSpeed = OPTIONS_TEXT_SPEED_MID;
    gSaveBlock2Ptr->optionsWindowFrameType = 0;
    gSaveBlock2Ptr->optionsSound = OPTIONS_SOUND_MONO;
    gSaveBlock2Ptr->optionsBattleStyle = OPTIONS_BATTLE_STYLE_SHIFT;
    gSaveBlock2Ptr->optionsBattleSceneOff = FALSE;
    gSaveBlock2Ptr->regionMapZoom = FALSE;
}

static void ClearPokedexFlags(void)
{
    gUnusedPokedexU8 = 0;
    memset(&gSaveBlock1Ptr->dexCaught, 0, sizeof(gSaveBlock1Ptr->dexCaught));
    memset(&gSaveBlock1Ptr->dexSeen, 0, sizeof(gSaveBlock1Ptr->dexSeen));
}

void ClearAllContestWinnerPics(void)
{
    s32 i;

    ClearContestWinnerPicsInContestHall();

    // Clear Museum paintings
    for (i = MUSEUM_CONTEST_WINNERS_START; i < NUM_CONTEST_WINNERS; i++)
        gSaveBlock1Ptr->contestWinners[i] = sContestWinnerPicDummy;
}

static void ClearFrontierRecord(void)
{
    CpuFill32(0, &gSaveBlock2Ptr->frontier, sizeof(gSaveBlock2Ptr->frontier));

    gSaveBlock2Ptr->frontier.opponentNames[0][0] = EOS;
    gSaveBlock2Ptr->frontier.opponentNames[1][0] = EOS;
}

static void WarpToTruck(void)
{
    if (IS_FRLG)
    {
        // Nuzverse kanto skip-intro: spawn nel lab di Oak, pronti a scegliere lo starter.
        VarSet(VAR_MAP_SCENE_PALLET_TOWN_PROFESSOR_OAKS_LAB, 2);
        FlagClear(FLAG_HIDE_OAK_IN_HIS_LAB);
        FlagClear(FLAG_HIDE_RIVAL_IN_LAB);
        FlagClear(FLAG_HIDE_BULBASAUR_BALL);
        FlagClear(FLAG_HIDE_SQUIRTLE_BALL);
        FlagClear(FLAG_HIDE_CHARMANDER_BALL);
        SetWarpDestination(MAP_GROUP(MAP_PALLET_TOWN_PROFESSOR_OAKS_LAB), MAP_NUM(MAP_PALLET_TOWN_PROFESSOR_OAKS_LAB), WARP_ID_NONE, 6, 11);
    }
    else
    {
        // Nuzverse hoenn skip-intro: spawn su Route 101, SOLO la borsa (Birch nascosto = start pulito).
        VarSet(VAR_ROUTE101_STATE, 2);
        FlagSet(FLAG_HIDE_ROUTE_101_BIRCH_ZIGZAGOON_BATTLE);
        FlagSet(FLAG_HIDE_ROUTE_101_ZIGZAGOON);
        FlagClear(FLAG_HIDE_ROUTE_101_BIRCH_STARTERS_BAG);
        SetWarpDestination(MAP_GROUP(MAP_ROUTE101), MAP_NUM(MAP_ROUTE101), WARP_ID_NONE, 7, 15);
    }
    WarpIntoMap();
}

void Sav2_ClearSetDefault(void)
{
    ClearSav2();
    SetDefaultOptions();
}

void ResetMenuAndMonGlobals(void)
{
    gDifferentSaveFile = FALSE;
    ResetPokedexScrollPositions();
    ZeroPlayerPartyMons();
    ZeroEnemyPartyMons();
    ResetBagScrollPositions();
    ResetPokeblockScrollPositions();
}

void NewGameInitData(void)
{
#if IS_FRLG
    u8 rivalName[PLAYER_NAME_LENGTH + 1];
#endif
    if (gSaveFileStatus == SAVE_STATUS_EMPTY || gSaveFileStatus == SAVE_STATUS_CORRUPT)
        RtcReset();

#if IS_FRLG
    // Nuzverse instant-start: sigla/naming saltati -> gSaveBlock1Ptr->rivalName puo' NON
    // essere terminato (garbage) e StringCopy sforerebbe il buffer locale da 8B (stack
    // overflow -> reset -> LOOP del logo GBA su Kanto). Copia LIMITATA + EOS = sicura,
    // a prova di stato del SaveBlock e indipendente dal timing del menu.
    StringCopyN(rivalName, gSaveBlock1Ptr->rivalName, PLAYER_NAME_LENGTH);
    rivalName[PLAYER_NAME_LENGTH] = EOS;
#endif
    gDifferentSaveFile = TRUE;
    gSaveBlock2Ptr->encryptionKey = 0;
    ZeroPlayerPartyMons();
    ZeroEnemyPartyMons();
    ResetPokedex();
    ClearFrontierRecord();
    ClearSav1();
    ClearSav3();
    ClearAllMail();
    gSaveBlock2Ptr->specialSaveWarpFlags = 0;
    gSaveBlock2Ptr->gcnLinkFlags = 0;
    InitPlayerTrainerId();
    PlayTimeCounter_Reset();
    ClearPokedexFlags();
    InitEventData();
    ClearTVShowData();
    ResetGabbyAndTy();
    ClearSecretBases();
    ClearBerryTrees();
    SetMoney(&gSaveBlock1Ptr->money, 3000);
    SetCoins(0);
    ResetLinkContestBoolean();
    ResetGameStats();
    ClearAllContestWinnerPics();
    ClearPlayerLinkBattleRecords();
    InitSeedotSizeRecord();
    InitLotadSizeRecord();
    gPartiesCount[B_TRAINER_PLAYER] = 0;
    ZeroPlayerPartyMons();
    ResetPokemonStorageSystem();
    DeactivateAllRoamers();
    gSaveBlock1Ptr->registeredItem = ITEM_NONE;
    ClearBag();
    NewGameInitPCItems();
    ClearPokeblocks();
    ClearDecorationInventories();
    InitEasyChatPhrases();
    SetMauvilleOldMan();
    InitDewfordTrend();
    ResetFanClub();
    ResetLotteryCorner();
    UpdateDailySeed();
    WarpToTruck();
    if (IS_FRLG)
        RunScriptImmediately(EventScript_ResetAllMapFlagsFrlg);
    else
        RunScriptImmediately(EventScript_ResetAllMapFlags);
#if IS_FRLG
        StringCopy(gSaveBlock1Ptr->rivalName, rivalName);
#endif
    ResetMiniGamesRecords();
    InitUnionRoomChatRegisteredTexts();
    InitLilycoveLady();
    ResetAllApprenticeData();
    ClearRankingHallRecords();
    InitMatchCallCounters();
    ClearMysteryGift();
    WipeTrainerNameRecords();
    ResetTrainerHillResults();
    ResetTrainerTowerResults();
    ResetContestLinkResults();

    gNvRunResult = 0; // Nuzverse: nuova run attiva (reset del marker fine-run per il webapp)
    // ===== Nuzverse mai-bloccante (preset trama) =====
#if NV_NO_BLOCKERS
#if GAME_VERSION == VERSION_EMERALD
    // -- Hoenn --
    VarSet(VAR_PETALBURG_CITY_STATE, 3);   // tutorial Wally fatto
    VarSet(VAR_PETALBURG_GYM_STATE, 2);    // Norman: 4 badge => battaglia
    VarSet(VAR_SOOTOPOLIS_CITY_STATE, 6);  // crisi legendari risolta, gym aperta
    // Nuzverse: climax leggendari (Kyogre/Groudon/Rayquaza) RIMOSSO -> dungeon percorribili,
    // palestra di Juan aperta, zero cutscene. Lo state=6 sopra disinnesca le scene di Sootopolis;
    // qui sblocchiamo la porta della palestra (murata via metatile finche' il flag e' unset) e
    // disinneschiamo i trigger residui di Seafloor/Sky Pillar.
    FlagSet(FLAG_SOOTOPOLIS_ARCHIE_MAXIE_LEAVE);   // apre porta palestra + case (fix softlock)
    FlagSet(FLAG_KYOGRE_ESCAPED_SEAFLOOR_CAVERN);  // coerenza SootopolisCity_OnLoad
    VarSet(VAR_SKY_PILLAR_RAYQUAZA_CRY_DONE, 1);   // niente cutscene/risveglio Rayquaza in cima
    // Niente sprite leggendari/Magma nella camera di Seafloor Room9 (raggiungibile in Dive
    // in qualsiasi momento): nascondi Kyogre (dormiente+sveglio), Maxie e i grunt Magma.
    FlagSet(FLAG_HIDE_SEAFLOOR_CAVERN_ROOM_9_KYOGRE_ASLEEP);
    FlagSet(FLAG_HIDE_SEAFLOOR_CAVERN_ROOM_9_KYOGRE);
    FlagSet(FLAG_HIDE_SEAFLOOR_CAVERN_ROOM_9_MAXIE);
    FlagSet(FLAG_HIDE_SEAFLOOR_CAVERN_ROOM_9_MAGMA_GRUNTS);
    FlagSet(FLAG_HIDE_ROUTE_112_TEAM_MAGMA);   // funivia libera
    FlagSet(FLAG_HIDE_MT_CHIMNEY_TEAM_MAGMA);  // vetta senza meteorite
    FlagSet(FLAG_HIDE_MT_CHIMNEY_TEAM_AQUA);
    FlagSet(FLAG_HIDE_ROUTE_119_TEAM_AQUA);    // ponte Istituto Meteo libero
    FlagSet(FLAG_HIDE_RUSTURF_TUNNEL_ROCK_1);  // scorciatoia Verdanturf<->Rustboro
    FlagSet(FLAG_HIDE_RUSTURF_TUNNEL_ROCK_2);
    FlagSet(FLAG_HIDE_LITTLEROOT_TOWN_BRENDANS_HOUSE_TRUCK); // Nuzverse: niente camion del trasloco a Littleroot
    FlagSet(FLAG_HIDE_LITTLEROOT_TOWN_MAYS_HOUSE_TRUCK);     // (instant-start: il trasloco non avviene mai)
    // Nuzverse lineare: salta cutscene-storia filler (solo dialogo, NON gate di progressione)
    VarSet(VAR_LITTLEROOT_TOWN_STATE, 4);  // intro Littleroot (gemelli/scarpe) gia' fatta
    VarSet(VAR_LITTLEROOT_RIVAL_STATE, 3); // incontro rivale in cameretta saltato
    // Nuzverse: la scena Pokédex del lab di Birch e' saltata -> settiamo a mano i flag che
    // quella scena imposta. Senza, Oldale BLOCCA l'uscita ovest (gate FLAG_ADVENTURE_STARTED)
    // e si e' forzati a tornare al lab dopo il rivale di Route103. Cosi' si prosegue subito.
    FlagSet(FLAG_ADVENTURE_STARTED);            // sblocca Oldale ovest (niente ritorno forzato)
    FlagSet(FLAG_SYS_POKEDEX_GET);              // Pokédex gia' in mano
    FlagSet(FLAG_RECEIVED_POKEDEX_FROM_BIRCH);  // scena consegna Pokédex marcata fatta
    VarSet(VAR_ROUTE116_STATE, 2);         // Briney "Peeko rapito" (solo dialogo)
    VarSet(VAR_ROUTE118_STATE, 1);         // Steven sul dislivello (solo flavor)
    VarSet(VAR_ROUTE121_STATE, 1);         // grunt Aqua che chiacchierano e se ne vanno
#else
    // -- Kanto (FRLG): rimuove roadblock e gate-oggetto del percorso principale --
    FlagSet(FLAG_HIDE_ROUTE_12_SNORLAX);   // Snorlax Route 12
    FlagSet(FLAG_HIDE_ROUTE_16_SNORLAX);   // Snorlax Route 16
    FlagSet(FLAG_FOUND_BOTH_VERMILION_GYM_SWITCHES); // niente puzzle cestini Surge
    VarSet(VAR_MAP_SCENE_VICTORY_ROAD_1F, 100);          // Via Vittoria: barriere mai disegnate (niente puzzle massi/interruttori)
    VarSet(VAR_MAP_SCENE_VICTORY_ROAD_2F_BOULDER1, 100);
    VarSet(VAR_MAP_SCENE_VICTORY_ROAD_2F_BOULDER2, 100);
    VarSet(VAR_MAP_SCENE_VICTORY_ROAD_3F, 100);
    FlagSet(FLAG_RESCUED_MR_FUJI);         // coerenza Torre/Flauto
    FlagSet(FLAG_GOT_POKE_FLUTE);
    AddBagItem(ITEM_SILPH_SCOPE, 1);       // fantasmi Torre Pokémon
    AddBagItem(ITEM_SECRET_KEY, 1);        // palestra di Blaine
    AddBagItem(ITEM_TEA, 1);               // guardie di Zafferano
    AddBagItem(ITEM_POKE_FLUTE, 1);
    AddBagItem(ITEM_SS_TICKET, 1);
    // Nuzverse lineare: salta cutscene-storia filler del main path (non-gate; key item gia' in borsa)
    VarSet(VAR_MAP_SCENE_PALLET_TOWN_OAK, 3);                    // Oak non ti ferma sulla Route 1
    // Nuzverse: la scena Pokédex (consegna "pacco di Oak") e' saltata -> settiamo a mano i
    // flag che dava, altrimenti manca il Pokédex e la quest pacco/Oak resta aperta (NPC che
    // blocca a fine Viridian finche' non torni da Oak). Cosi' Pokédex in mano e quest fatta.
    FlagSet(FLAG_SYS_POKEDEX_GET);     // Pokédex in mano
    FlagSet(FLAG_ADVENTURE_STARTED);   // quest pacco/Oak completata (0x74 = "RECEIVED Pokédex")
    VarSet(VAR_MAP_SCENE_VIRIDIAN_CITY_OLD_MAN, 2);             // Route 2 libera + niente tutorial cattura
    VarSet(VAR_MAP_SCENE_PEWTER_CITY, 2);                       // niente guida-palestra forzata
    VarSet(VAR_MAP_SCENE_CERULEAN_CITY_RIVAL, 1);              // niente rivale all'incrocio
    VarSet(VAR_MAP_SCENE_CERULEAN_CITY_ROCKET, 1);            // niente grunt della casa derubata
    VarSet(VAR_MAP_SCENE_ROUTE22, 2);                          // salta SOLO il primo rivale (il tardo pre-Lega resta)
    VarSet(VAR_MAP_SCENE_ROUTE24, 1);                          // niente cutscene del Ponte Pepita
    FlagSet(FLAG_HIDE_NUGGET_BRIDGE_ROCKET);                   // nasconde il Rocket sul ponte
    VarSet(VAR_MAP_SCENE_ROUTE5_ROUTE6_ROUTE7_ROUTE8_GATES, 1); // guardie gate Saffron via (Te' gia' in borsa)
#endif
#endif
#if NV_KAIZO
    // Kaizo: borsa di partenza — 10 Master Ball (catture sicure nelle prime 3 route) + 1 Pozione.
    AddBagItem(ITEM_MASTER_BALL, 10);
    AddBagItem(ITEM_POTION, 1);
    AddBagItem(ITEM_DEVON_SCOPE, 1); // "Repel Switch": interruttore incontri (no step count)
#endif
    // Nuzverse QoL (la QoL retail lato-client e' saltata sulle variant fork): testo
    // veloce, niente animazioni di battaglia, stile SET e scarpe da corsa. Con
    // IsRunningDisallowed gia' forzata a FALSE -> si corre OVUNQUE (indoor incluso).
    gSaveBlock2Ptr->optionsTextSpeed = OPTIONS_TEXT_SPEED_FAST;
    gSaveBlock2Ptr->optionsBattleSceneOff = TRUE;
    gSaveBlock2Ptr->optionsBattleStyle = OPTIONS_BATTLE_STYLE_SET;
    FlagSet(FLAG_SYS_B_DASH);
    // ==========================================
    SetCurrentDifficultyLevel(DIFFICULTY_NORMAL);
    ResetItemFlags();
    ResetDexNav();
    ClearFollowerNPCData();
}

static void ResetMiniGamesRecords(void)
{
    CpuFill16(0, &gSaveBlock2Ptr->berryCrush, sizeof(struct BerryCrush));
    SetBerryPowder(&gSaveBlock2Ptr->berryCrush.berryPowderAmount, 0);
    ResetPokemonJumpRecords();
    CpuFill16(0, &gSaveBlock2Ptr->berryPick, sizeof(struct BerryPickingResults));
}

static void ResetItemFlags(void)
{
#if OW_SHOW_ITEM_DESCRIPTIONS == OW_ITEM_DESCRIPTIONS_FIRST_TIME
    memset(&gSaveBlock3Ptr->itemFlags, 0, sizeof(gSaveBlock3Ptr->itemFlags));
#endif
}

static void ResetDexNav(void)
{
#if USE_DEXNAV_SEARCH_LEVELS == TRUE
    memset(gSaveBlock3Ptr->dexNavSearchLevels, 0, sizeof(gSaveBlock3Ptr->dexNavSearchLevels));
#endif
    gSaveBlock3Ptr->dexNavChain = 0;
}
