#include "GameObject.h"
#include "CollisionManager.h"

GameObject::GameObject()
    : objectType(SEACREATURE)
    , pos(0, 0, 0)
    , vel(0, 0, 0)
    , scale(1, 1, 1)
    , active(false)
{
}
GameObject::GameObject(OBJECT_TYPE objectType, Vector3 pos, Vector3 vel, Vector3 scale, bool active)
    : objectType(objectType)
    , pos(pos)
    , vel(vel)
    , scale(scale)
    , active(active)
{
}

GameObject::~GameObject()
{
}