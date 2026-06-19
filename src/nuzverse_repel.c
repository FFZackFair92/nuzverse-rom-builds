#include "global.h"
#include "event_data.h"
#include "nuzverse_config.h"
#include "nuzverse_repel.h"

// Stato dell'interruttore repellente. Flag salvato (FLAG_NV_REPEL_TOGGLE) => persiste a save/load.
bool32 NvRepelToggleOn(void)
{
    return FlagGet(FLAG_NV_REPEL_TOGGLE);
}
