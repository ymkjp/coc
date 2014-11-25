#include "Building.h"

bool Building::init(TMXTiledMap* tiledMap, ValueMap objectInfo)
{
    _tiledMap = tiledMap;
    cache = SpriteFrameCache::getInstance();
    _objectInfo = objectInfo;
    this->addBuilding();
    return true;
}

void Building::addBuilding()
{
    auto gid = _objectInfo.at("gid").asInt();
    auto pos = this->convertToIso(Vec2(_objectInfo.at("x").asInt(), _objectInfo.at("y").asInt()));
    
    auto file = "stage/field/08.png";
    
    auto tmp = Vec2(_objectInfo.at("x").asInt(), _objectInfo.at("y").asInt());
    auto tmpFixed = Vec2(tmp.x, _tiledMap->getContentSize().height - tmp.y);
    // xxx
    switch (gid) {
        case 3:
            file = "stage/archer_tower/1041.0.png";
            break;
        case 4:
            file = "stage/wall/1027.0.png";
            break;
        case 5:
            file = "stage/wall/1029.0.png";
            break;
        case 6:
            file = "stage/wall/1030.0.png";
            break;
        case 7:
            file = "stage/wall/1028.0.png";
            break;
        case 8:
            file = "stage/wall/1000.0.png";
            break;
        default:
            break;
    }
    sprite = Sprite::createWithSpriteFrameName(file);
    sprite->setPosition(pos);
    return;
}

inline Vec2 Building::convertToIso(Vec2 rawPos)
{
    Size tileSize = _tiledMap->getTileSize();
    Size mapSize = _tiledMap->getMapSize();
    auto pos = Vec2(rawPos.x, mapSize.height - rawPos.y);
    
    float ratio = tileSize.width / tileSize.height;
    float x = tileSize.width * 0.5 * (pos.x/(tileSize.width / ratio) - pos.y/tileSize.height);
    float y = tileSize.height * 0.5 * (mapSize.height - pos.x/(tileSize.width / ratio) - pos.y/tileSize.height);
    return Vec2(x, y) + Vec2(mapSize.width * 0.65, mapSize.height);
}
