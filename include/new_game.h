#ifndef GUARD_NEW_GAME_H
#define GUARD_NEW_GAME_H

extern bool8 gDifferentSaveFile;
// Shortcuts some randomness in berry_blender.c, and enables debug printing
// in contest.c.
extern bool8 gEnableContestDebugging;

void SetTrainerId(u32 trainerId, u8 *dst);
u32 GetTrainerId(u8 *trainerId);
void CopyTrainerId(u8 *dst, u8 *src);
void NewGameInitData(void);
void NvBuildInjectedParty(void);   // Nuzverse: costruisce gParties[player] da gNvInjectParty (special, sfide Torre/Arena)
void NvBuildInjectedFoe(void);     // Nuzverse: costruisce gParties[OPPONENT_A] da gNvInjectFoe (special, Arena ghost)
bool8 NvHasInjectedFoe(void);      // Nuzverse: TRUE se c'e' una squadra ghost iniettata (Arena)
void NvBuildRandomPlayerTeam(u32 count, u32 level); // Nuzverse Torre: squadra player random gen9 (special)
void NvBuildRandomFoe(u32 count, u32 level);        // Nuzverse Torre: foe random gen9 level-matched
void NvScriptBuildRandomTeam(void);                 // Nuzverse Torre: special wrapper (legge VAR_0x8005=count)
void ResetMenuAndMonGlobals(void);
void Sav2_ClearSetDefault(void);

#endif // GUARD_NEW_GAME_H
