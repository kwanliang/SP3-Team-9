#include "CollisionManager.h"

hitbox hitbox::generatehitbox(Vector3 minValue, Vector3 maxValue)
{
    hitbox NewBox;
    NewBox.m_vecMin = minValue;
    NewBox.m_vecMax = maxValue;
    return NewBox;
}

hitbox hitbox::generatehitbox(Vector3 pos, float length, float height, float width, Vector3 objectVelocity)
{
    hitbox NewBox;

    NewBox.m_position = pos;

    NewBox.m_velocity = objectVelocity;

    NewBox.m_length = length;
    NewBox.m_height = height;
    NewBox.m_width = width;

    NewBox.m_vecMin = Vector3(
        pos.x - (length * 0.5),
        pos.y - (height * 0.5),
        pos.z - (width * 0.5)
        );
    NewBox.m_vecMax = Vector3(
        pos.x + (length * 0.5),
        pos.y + (height * 0.5),
        pos.z + (width * 0.5)
        );

    return NewBox;
}

void hitbox::updatehitbox(hitbox &box, Vector3 newpos)
{
    box.m_position = newpos;

    box.m_vecMin = Vector3(
        (box.m_position.x - (box.m_length * 0.5)),
        (box.m_position.y - (box.m_height * 0.5)),
        (box.m_position.z - (box.m_width * 0.5))
        );

    box.m_vecMax = Vector3(
        (box.m_position.x + (box.m_length * 0.5)),
        (box.m_position.y + (box.m_height * 0.5)),
        (box.m_position.z + (box.m_width * 0.5))
        );
}

hitbox2 hitbox2::generatehitbox(Vector3 pos, float length, float height, float width)
{
    hitbox2 NewBox;

    NewBox.m_position = pos;

    NewBox.m_length = length;
    NewBox.m_height = height;
    NewBox.m_width = width;

    NewBox.m_point[0] = Vector3(pos.x - (length * 0.5), pos.y - (height * 0.5), pos.z - (width * 0.5));
    NewBox.m_point[1] = Vector3(pos.x + (length * 0.5), pos.y - (height * 0.5), pos.z - (width * 0.5));
    NewBox.m_point[2] = Vector3(pos.x + (length * 0.5), pos.y - (height * 0.5), pos.z + (width * 0.5));
    NewBox.m_point[3] = Vector3(pos.x - (length * 0.5), pos.y - (height * 0.5), pos.z + (width * 0.5));

    NewBox.m_point[4] = Vector3(pos.x - (length * 0.5), pos.y + (height * 0.5), pos.z - (width * 0.5));
    NewBox.m_point[5] = Vector3(pos.x + (length * 0.5), pos.y + (height * 0.5), pos.z - (width * 0.5));
    NewBox.m_point[6] = Vector3(pos.x + (length * 0.5), pos.y + (height * 0.5), pos.z + (width * 0.5));
    NewBox.m_point[7] = Vector3(pos.x - (length * 0.5), pos.y + (height * 0.5), pos.z + (width * 0.5));
    return NewBox;
}

void hitbox2::updatehitbox(hitbox2 &box, Vector3 newpos)
{
    Vector3 displacement = box.m_position - newpos;

    box.m_position = newpos;

    for (unsigned i = 0; i < 8; i++)
    {
        box.m_point[i] -= displacement;

    }
}

TSphere TSphere::generatehitbox(Vector3 m_position, float m_Radius)
{
    TSphere NewBox;
    NewBox.m_position = m_position;
    NewBox.m_Radius = m_Radius;
    return NewBox;
}

void TSphere::updatehitbox(TSphere &box, Vector3 newpos)
{
    box.m_position = newpos;
}