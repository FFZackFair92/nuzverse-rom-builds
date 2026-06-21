#include "global.h"
#include "item_ball.h"
#include "event_data.h"
#include "constants/event_objects.h"
#include "constants/items.h"

static u32 GetItemBallAmountFromTemplate(u32);
static u32 GetItemBallIdFromTemplate(u32);

static u32 GetItemBallAmountFromTemplate(u32 itemBallId)
{
    u32 amount = gMapHeader.events->objectEvents[itemBallId].movementRangeX;

    if (amount > MAX_BAG_ITEM_CAPACITY)
        return MAX_BAG_ITEM_CAPACITY;

    return (amount == 0) ? 1 : amount;
}

static u32 GetItemBallIdFromTemplate(u32 itemBallId)
{
    enum Item itemId = gMapHeader.events->objectEvents[itemBallId].trainerRange_berryTreeId;

    return (itemId >= ITEMS_COUNT) ? (ITEM_NONE + 1) : itemId;
}

void GetItemBallIdAndAmountFromTemplate(void)
{
    u32 itemBallId = (gSpecialVar_LastTalked - 1);
    gSpecialVar_Result = GetItemBallIdFromTemplate(itemBallId);
    gSpecialVar_0x8009 = GetItemBallAmountFromTemplate(itemBallId);
}

// Nuzverse (Hoenn): le ex-bacche sono diventate poke-ball con uno strumento RANDOM.
// Item seedato dal trainerId (cambia per-run, come specie/mosse) ma deterministico per-spot
// (mappa + local id) cosi' ogni spot da' un oggetto stabile nella stessa run. Pool curato.
// Mette item+quantita' in gSpecialVar_Result/0x8009; lo script poi usa `finditem` (give+hide).
void NvGetBerryRandomItem(void)
{
    static const u16 sPool[] = {
        ITEM_POTION, ITEM_SUPER_POTION, ITEM_HYPER_POTION, ITEM_MAX_POTION, ITEM_FULL_RESTORE,
        ITEM_REVIVE, ITEM_MAX_REVIVE, ITEM_FULL_HEAL, ITEM_ANTIDOTE, ITEM_PARALYZE_HEAL,
        ITEM_AWAKENING, ITEM_BURN_HEAL, ITEM_ICE_HEAL, ITEM_ETHER, ITEM_MAX_ETHER,
        ITEM_ELIXIR, ITEM_MAX_ELIXIR, ITEM_PP_UP, ITEM_RARE_CANDY, ITEM_HP_UP,
        ITEM_PROTEIN, ITEM_IRON, ITEM_CALCIUM, ITEM_CARBOS, ITEM_ZINC,
        ITEM_GREAT_BALL, ITEM_ULTRA_BALL, ITEM_NUGGET,
    };
    const u8 *tid = gSaveBlock2Ptr->playerTrainerId;
    u32 seed = (u32)tid[0] | ((u32)tid[1] << 8) | ((u32)tid[2] << 16) | ((u32)tid[3] << 24);
    u32 spot = ((u32)gSaveBlock1Ptr->location.mapGroup << 16)
             | ((u32)gSaveBlock1Ptr->location.mapNum << 8)
             | (u32)gSpecialVar_LastTalked;
    u32 h = seed ^ (spot * 2654435761u);
    h ^= h >> 13; h *= 0x5bd1e995u; h ^= h >> 15;
    gSpecialVar_Result = sPool[h % ARRAY_COUNT(sPool)];
    gSpecialVar_0x8009 = 1;
}
