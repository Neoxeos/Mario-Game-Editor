#include "Physics.h"

Vec2f Physics::GetOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
{
    if (!a->hasComponent<CBoundingBox>() || !b->hasComponent<CBoundingBox>()) return Vec2f(0, 0);
    float x1 = a->getComponent<CTransform>().pos.x;
    float y1 = a->getComponent<CTransform>().pos.y;
    float x2 = b->getComponent<CTransform>().pos.x;
    float y2 = b->getComponent<CTransform>().pos.y;

    float h1 = a->getComponent<CBoundingBox>().halfSize.y;
    float w1 = a->getComponent<CBoundingBox>().halfSize.x;
    float h2 = b->getComponent<CBoundingBox>().halfSize.y;
    float w2 = b->getComponent<CBoundingBox>().halfSize.x;

    Vec2f delta = { (float)abs(x1 - x2), (float)abs(y1 - y2) };
    float ox = w1 + w2 - delta.x;
    float oy = h1 + h2 - delta.y;

    return Vec2f(ox, oy);
}

Vec2f Physics::GetPreviousOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
{
    if (!a->hasComponent<CBoundingBox>() || !b->hasComponent<CBoundingBox>()) return Vec2f(0, 0);
    float x1 = a->getComponent<CTransform>().prevPos.x;
    float y1 = a->getComponent<CTransform>().prevPos.y;
    float x2 = b->getComponent<CTransform>().prevPos.x;
    float y2 = b->getComponent<CTransform>().prevPos.y;

    float h1 = a->getComponent<CBoundingBox>().halfSize.y;
    float w1 = a->getComponent<CBoundingBox>().halfSize.x;
    float h2 = b->getComponent<CBoundingBox>().halfSize.y;
    float w2 = b->getComponent<CBoundingBox>().halfSize.x;

    Vec2f delta = { (float)abs(x1 - x2), (float)abs(y1 - y2) };
    float ox = w1 + w2 - delta.x;
    float oy = h1 + h2 - delta.y;

    return Vec2f(ox, oy);
}