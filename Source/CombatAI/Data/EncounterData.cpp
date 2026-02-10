// Copyright CombatAI Project. All Rights Reserved.

#include "EncounterData.h"

int32 UEncounterData::GetTotalEnemyCount() const
{
	int32 Total = 0;
	for (const FEncounterWave& Wave : Waves)
	{
		Total += Wave.Enemies.Num();
	}
	return Total;
}
