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

GameObject::~GameObject()
{
}