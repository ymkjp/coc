#ifndef __GRAVE_H__
#define __GRAVE_H__

#include "Definitions.h"
#include "Tmx.h"

class Grave : public Node
{
public:
    static Grave* create(Tmx* tmx, Vec2 coord) {
        auto p = new Grave();
        if (p->init(tmx, coord)) {
            p->autorelease();
            return p;
        }
        CC_SAFE_DELETE(p);
        return nullptr;
    }
    
    virtual bool init(Tmx* tmx, Vec2 coord);
    
    
protected:
    Tmx* tmx;
    Vec2 coord;
};

#endif // __GRAVE_H__
