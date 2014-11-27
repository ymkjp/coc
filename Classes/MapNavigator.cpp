#include "MapNavigator.h"

USING_NS_CC;

bool MapNavigator::init(TMXTiledMap* _tiledMap)
{
    tiledMap = _tiledMap;
    domainTMXLayer = tiledMap->getLayer("Domain");
    wallTMXLayer = tiledMap->getLayer("Wall");
    return true;
}

inline bool MapNavigator::isTravelable(Vec2 pos)
{
    for (std::string name: blockLayers) {
        if (0 != tiledMap->getLayer(name.c_str())->getTileGIDAt(pos)) {
            return false;
        }
    }
    return true;
}

/**
 @param Vec2 goalPoint should be __STATUS::NONE
 @link http://2dgames.jp/2012/05/22/a-starar/
 */
std::stack<Vec2>* MapNavigator::navigate(Vec2 startPoint, Vec2 goalPoint)
{
    //    CCLOG("=== Nav started ===");
    AStar* openedNode = this->findLastNode(startPoint, goalPoint);
    
    AStar* paths = openedNode;
    while (paths->parent != nullptr) {
        pathToGoal.push(paths->pos);
        paths = paths->parent;
    }
    //    CCLOG("=== Nav finished ===");
    
    return &pathToGoal;
}

AStar* MapNavigator::findLastNode(Vec2 startPoint, Vec2 goalPoint)
{
    std::set<Vec2> openSet;
    
    AStar worldGrid[WORLD_MAP_WIDTH][WORLD_MAP_HEIGHT] = {};
    
    for (int x = 0; x < sizeof(worldGrid) / sizeof(worldGrid[0]); ++x) {
        for (int y = 0; y < sizeof(worldGrid[x]) / sizeof(worldGrid[x][0]); ++y) {
            AStar plainNode;
            plainNode.status = (this->isTravelable(Vec2(x,y))) ? AStar::__STATUS::NONE : AStar::__STATUS::UNABLE;
            plainNode.pos = Vec2(x,y);
            worldGrid[x][y] = plainNode;
        }
    }
    
    AStar startNode;
    startNode.status = AStar::__STATUS::OPEN;
    startNode.cost = 0;
    startNode.distance = startPoint.getDistanceSq(goalPoint);
    startNode.parent = nullptr;
    startNode.pos = startPoint;
    worldGrid[static_cast<int>(startPoint.x)][static_cast<int>(startPoint.y)] = startNode;
    openSet.insert(startPoint);
    
    // Open first 8 nodes
    Vec2 targetPoint;
    for (std::set<Vec2>::iterator i = surround.begin(); i != surround.end(); ++i)
    {
        targetPoint = startPoint + *i;
        if (isInMapRange(targetPoint) && worldGrid[static_cast<int>(targetPoint.x)][static_cast<int>(targetPoint.y)].status == AStar::__STATUS::NONE)
        {
            AStar openNode;
            openNode.cost = 0;
            openNode.status = AStar::__STATUS::OPEN;
            openNode.distance = targetPoint.getDistanceSq(goalPoint);
            openNode.pos = targetPoint;
            openNode.parent = &startNode;
            worldGrid[static_cast<int>(targetPoint.x)][static_cast<int>(targetPoint.y)] = openNode;
            openSet.insert(targetPoint);
        }
    }
    worldGrid[static_cast<int>(startPoint.x)][static_cast<int>(startPoint.y)].status = AStar::__STATUS::CLOSE;
    openSet.erase(startPoint);
    
    // Find goal from openSet
    int isFound = 0;
    int steps = 0;
    Vec2 bestPoint;
    Vec2 nextPoint;
    Vec2 openPoint;
    AStar* bestNode = nullptr;
    AStar* openedNode = nullptr;
    while (isFound == 0 && steps < WORLD_MAP_HEIGHT * WORLD_MAP_WIDTH) {
        ++steps;
        for (std::set<Vec2>::iterator openedPoint = openSet.begin(); openedPoint != openSet.end(); ++openedPoint)
        {
            openedNode = &worldGrid[static_cast<int>(openedPoint->x)][static_cast<int>(openedPoint->y)];
            if (*openedPoint == goalPoint) {
                isFound = 1;
                break;
            } else if (bestNode == nullptr || openedNode->GetScore() < bestNode->GetScore() ) {
                bestPoint = *openedPoint;
                bestNode = openedNode;
            } else if (openedNode->GetScore() == bestNode->GetScore() && arc4random_uniform(2) == 1) { /** @fixme **/
                bestPoint = *openedPoint;
                bestNode = openedNode;
            }
        }
        if (isFound) { break; }
        for (std::set<Vec2>::iterator i = surround.begin(); i != surround.end(); ++i)
        {
            nextPoint = bestPoint + Vec2(i->x,i->y);
            if (isInMapRange(nextPoint)
                && worldGrid[static_cast<int>(nextPoint.x)][static_cast<int>(nextPoint.y)].status == AStar::__STATUS::NONE
                && openSet.find(nextPoint) == openSet.end()
                ) {
                AStar nextNode;
                nextNode.status = AStar::__STATUS::OPEN;
                nextNode.cost = steps;
                nextNode.distance = nextPoint.getDistanceSq(goalPoint);
                nextNode.pos = nextPoint;
                nextNode.parent = bestNode;
                worldGrid[static_cast<int>(nextPoint.x)][static_cast<int>(nextPoint.y)] = nextNode;
                openSet.insert(nextPoint);
            }
        }
        worldGrid[static_cast<int>(bestPoint.x)][static_cast<int>(bestPoint.y)].status = AStar::__STATUS::CLOSE;
        openSet.erase(bestPoint);
        bestNode = {};
    }
    return openedNode;
}
