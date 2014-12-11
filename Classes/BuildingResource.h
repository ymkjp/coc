#ifndef __BUILDING_RESOURCE_H__
#define __BUILDING_RESOURCE_H__

#include "cocos2d.h"
USING_NS_CC;

#include "Building.h"

class BuildingResource : public Building
{
public:
    float getStorageCapacity();

protected:
    Node* damagedEffectNode;
    timeline::ActionTimeline* damagedEffectAction;
    
    ScoreType scoreType;
    float remainingStorageAmmount;

    // Buildingのメソッドを上書き
    void initOwn();
    void damagedEffect();
    void increaseResourceScore(float damage);
    
    virtual void adjustScale() {};
    
    const std::map<BuildingType, ScoreType> scoreTypeByBuildingType = {
        {ElixerTank, ElixerScore},
        {GoldBank,   CoinScore},
    };
    
    const std::map<BuildingType, float> storageCapacityByType = {
        {ElixerTank, 25000},
        {GoldBank,   25000},
    };
    
    const std::map<BuildingType, std::string> motionNameByType = {
        {ElixerTank, "res/ElixerBubble.csb"},
        {GoldBank,   "res/CoinBubble.csb"},
    };
    
    const std::map<BuildingType, std::string> soundNameByType = {
        {ElixerTank, "elixir_steal"},
        {GoldBank,   "coin_steal"},
    };
};

#endif // __BUILDING_RESOURCE_H__
