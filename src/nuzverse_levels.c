#include "global.h"
#include "constants/region_map_sections.h"
#include "constants/opponents.h"
#include "nuzverse_config.h"
#include "nuzverse_levels.h"

// ============================================================================
// Curva livelli Kaizo ibrida. Boss = ace esatto (tabella per ID, #if IS_FRLG);
// trainer generici = range per area corrente, scalato nel party.
// Kill-switch: NV_DEBUG_ENEMY_LV1=1 forza Lv1 (debug).
// ============================================================================

struct NvBoss { u16 id; u8 ace; };

#if IS_FRLG
// ---------------- KANTO ----------------
static const struct NvBoss sBosses[] = {
    { TRAINER_LEADER_BROCK, 23 },
    { TRAINER_LEADER_MISTY, 29 },
    { TRAINER_LEADER_LT_SURGE, 35 },
    { TRAINER_LEADER_ERIKA, 41 },
    { TRAINER_LEADER_KOGA, 51 },
    { TRAINER_LEADER_SABRINA, 57 },
    { TRAINER_LEADER_BLAINE, 62 },
    { TRAINER_LEADER_GIOVANNI, 64 },
    { TRAINER_BOSS_GIOVANNI, 46 },
    { TRAINER_BOSS_GIOVANNI_2, 57 },
    { TRAINER_ELITE_FOUR_LORELEI, 85 },
    { TRAINER_ELITE_FOUR_BRUNO, 86 },
    { TRAINER_ELITE_FOUR_AGATHA, 87 },
    { TRAINER_ELITE_FOUR_LANCE, 88 },
    { TRAINER_CHAMPION_FIRST_SQUIRTLE, 90 },
    { TRAINER_CHAMPION_FIRST_BULBASAUR, 90 },
    { TRAINER_CHAMPION_FIRST_CHARMANDER, 90 },
    { TRAINER_RIVAL_OAKS_LAB_SQUIRTLE, 11 },
    { TRAINER_RIVAL_OAKS_LAB_BULBASAUR, 11 },
    { TRAINER_RIVAL_OAKS_LAB_CHARMANDER, 11 },
};
static u8 NvAreaRange(u16 sec, u8 *lo, u8 *hi)
{
    switch (sec)
    {
    case MAPSEC_VIRIDIAN_FOREST: *lo = 8; *hi = 18; return 1;
    case MAPSEC_MT_MOON: case MAPSEC_ROUTE_4: case MAPSEC_ROUTE_24: case MAPSEC_ROUTE_25: *lo = 24; *hi = 27; return 1;
    case MAPSEC_S_S_ANNE: case MAPSEC_ROUTE_5: case MAPSEC_ROUTE_6: *lo = 30; *hi = 33; return 1;
    case MAPSEC_ROCK_TUNNEL: case MAPSEC_ROUTE_9: case MAPSEC_ROUTE_10: case MAPSEC_ROUTE_8: case MAPSEC_ROUTE_7: *lo = 36; *hi = 39; return 1;
    case MAPSEC_ROCKET_HIDEOUT: case MAPSEC_POKEMON_TOWER: case MAPSEC_ROUTE_11: case MAPSEC_ROUTE_12: *lo = 42; *hi = 45; return 1;
    case MAPSEC_ROUTE_16: case MAPSEC_ROUTE_17: case MAPSEC_ROUTE_18: case MAPSEC_ROUTE_13: case MAPSEC_ROUTE_14: case MAPSEC_ROUTE_15: *lo = 47; *hi = 50; return 1;
    case MAPSEC_SILPH_CO: case MAPSEC_SAFFRON_CITY: *lo = 52; *hi = 56; return 1;
    case MAPSEC_POKEMON_MANSION: case MAPSEC_CINNABAR_ISLAND: case MAPSEC_ROUTE_19: case MAPSEC_ROUTE_20: case MAPSEC_ROUTE_21: *lo = 58; *hi = 61; return 1;
    case MAPSEC_VIRIDIAN_CITY: case MAPSEC_ROUTE_22: case MAPSEC_ROUTE_23: *lo = 63; *hi = 64; return 1;
    case MAPSEC_KANTO_VICTORY_ROAD: *lo = 64; *hi = 65; return 1;
    default: return 0;
    }
}
#else
// ---------------- HOENN ----------------
static const struct NvBoss sBosses[] = {
    { TRAINER_ROXANNE_1, 23 },
    { TRAINER_BRAWLY_1, 29 },
    { TRAINER_WATTSON_1, 35 },
    { TRAINER_FLANNERY_1, 41 },
    { TRAINER_NORMAN_1, 46 },
    { TRAINER_WINONA_1, 51 },
    { TRAINER_TATE_AND_LIZA_1, 57 },
    { TRAINER_JUAN_1, 64 },
    { TRAINER_SIDNEY, 85 },
    { TRAINER_PHOEBE, 86 },
    { TRAINER_GLACIA, 87 },
    { TRAINER_DRAKE, 88 },
    { TRAINER_WALLACE, 90 },
    { TRAINER_STEVEN, 90 },
    { TRAINER_BRENDAN_ROUTE_103_MUDKIP, 11 },
    { TRAINER_BRENDAN_ROUTE_103_TREECKO, 11 },
    { TRAINER_BRENDAN_ROUTE_103_TORCHIC, 11 },
    { TRAINER_MAY_ROUTE_103_MUDKIP, 11 },
    { TRAINER_MAY_ROUTE_103_TREECKO, 11 },
    { TRAINER_MAY_ROUTE_103_TORCHIC, 11 },
};
static u8 NvAreaRange(u16 sec, u8 *lo, u8 *hi)
{
    switch (sec)
    {
    case MAPSEC_PETALBURG_WOODS: case MAPSEC_ROUTE_104: *lo = 8; *hi = 18; return 1;
    case MAPSEC_GRANITE_CAVE: case MAPSEC_DEWFORD_TOWN: case MAPSEC_ROUTE_106: case MAPSEC_ROUTE_107: case MAPSEC_ROUTE_108: case MAPSEC_ROUTE_109: *lo = 24; *hi = 27; return 1;
    case MAPSEC_SLATEPORT_CITY: case MAPSEC_ROUTE_110: *lo = 30; *hi = 33; return 1;
    case MAPSEC_MT_CHIMNEY: case MAPSEC_FIERY_PATH: case MAPSEC_JAGGED_PASS: case MAPSEC_ROUTE_111: case MAPSEC_ROUTE_112: *lo = 36; *hi = 39; return 1;
    case MAPSEC_PETALBURG_CITY: case MAPSEC_ROUTE_117: *lo = 42; *hi = 45; return 1;
    case MAPSEC_ROUTE_119: case MAPSEC_ROUTE_118: case MAPSEC_FORTREE_CITY: *lo = 47; *hi = 50; return 1;
    case MAPSEC_MT_PYRE: case MAPSEC_ROUTE_120: case MAPSEC_ROUTE_121: case MAPSEC_ROUTE_122: case MAPSEC_ROUTE_123: case MAPSEC_MOSSDEEP_CITY: *lo = 52; *hi = 56; return 1;
    case MAPSEC_SEAFLOOR_CAVERN: case MAPSEC_ROUTE_124: case MAPSEC_ROUTE_125: case MAPSEC_ROUTE_126: case MAPSEC_ROUTE_127: case MAPSEC_ROUTE_128: *lo = 58; *hi = 61; return 1;
    case MAPSEC_SOOTOPOLIS_CITY: case MAPSEC_CAVE_OF_ORIGIN: *lo = 63; *hi = 64; return 1;
    case MAPSEC_VICTORY_ROAD: *lo = 64; *hi = 65; return 1;
    default: return 0;
    }
}
#endif

static u8 NvBossAce(u16 trainerId)
{
    u32 i;
    for (i = 0; i < ARRAY_COUNT(sBosses); i++)
        if (sBosses[i].id == trainerId)
            return sBosses[i].ace;
    return 0;
}

u8 NvKaizoLevel(u8 defaultLevel, u16 trainerId, u8 monIndex, u8 partySize)
{
#if NV_DEBUG_ENEMY_LV1
    return 1; // kill-switch debug
#else
    u8 ace, lo, hi, back;
    if (partySize == 0)
        partySize = 1;
    ace = NvBossAce(trainerId);
    if (ace)
    {
        back = (partySize - 1 > monIndex) ? (u8)(partySize - 1 - monIndex) : 0; // 0 = ace (ultimo mon)
        if (back > 5)
            back = 5;
        return (ace > back) ? (u8)(ace - back) : ace;
    }
    if (NvAreaRange(gMapHeader.regionMapSectionId, &lo, &hi))
    {
        if (partySize <= 1)
            return hi;
        return (u8)(lo + ((hi - lo) * monIndex) / (partySize - 1)); // scala lo->hi nel party
    }
    return defaultLevel; // area non mappata (side content): livello vanilla
#endif
}
