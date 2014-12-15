#include "UnitGiant.h"

USING_NS_CC;

void UnitGiant::playStartAttackingVoice()
{
    tmx->playSE(unitSmallCaseNameByType.at(type) + "_attack_start");
};