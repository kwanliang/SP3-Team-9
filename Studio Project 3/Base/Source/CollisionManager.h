#ifndef COLLISIONMANAGER_H
#define COLLISIONMANAGER_H

#include "Vector3.h"
#include <vector>
#include "LoadHmap.h"


struct hitbox
{
	Vector3 m_position;
	Vector3 m_vecMax;
	Vector3 m_vecMin;
    //Vector3 m_target;

	Vector3 m_velocity;

	float m_height;
	float m_length;
	float m_width;	

    static hitbox generatehitbox(Vector3 minValue, Vector3 maxValue);
    static hitbox generatehitbox(Vector3 pos, float length, float height, float width, Vector3 objectVelocity);
    static void updatehitbox(hitbox &box, Vector3 newpos);
};

struct hitbox2
{
	Vector3 m_position;

	Vector3 m_point[8];

	float m_height;
	float m_length;
	float m_width;

    static hitbox2 generatehitbox(Vector3 pos, float length, float height, float width);
    static void updatehitbox(hitbox2 &box, Vector3 newpos);
};

struct TSphere
{
    Vector3 m_position;
    float m_Radius;

    static TSphere generatehitbox(Vector3 m_position, float m_Radius);
    static void updatehitbox(TSphere &box, Vector3 newpos);
};

static bool terraincollision(const hitbox2& hitbox, std::vector<unsigned char> heightmap)
{
    for (unsigned i = 0; i < 8; i++)
    {
        float height = (350.f * ReadHeightMap(heightmap, hitbox.m_point[i].x / 3000.f, hitbox.m_point[i].z / 3000.f));

        if (hitbox.m_point[i].y < height)
            return true;
    }
    return false;
}

static bool terraincollision(const Vector3& pos, std::vector<unsigned char> heightmap)
{
    float height = (350.f * ReadHeightMap(heightmap, pos.x / 3000.f, pos.z / 3000.f));

    if (pos.y < height)
        return true;

    return false;
}

static bool collision(const hitbox2& lhsBox, const hitbox2& rhsBox)
{		//for AABB to AABB collision
    //Check if lhsBox's max is greater than rhsBox's min and lhsBox's min is less than rhsBox's max
    if (lhsBox.m_point[6].x > rhsBox.m_point[0].x &&
        lhsBox.m_point[0].x < rhsBox.m_point[6].x &&
        lhsBox.m_point[6].y > rhsBox.m_point[0].y &&
        lhsBox.m_point[0].y < rhsBox.m_point[6].y &&
        lhsBox.m_point[6].z > rhsBox.m_point[0].z &&
        lhsBox.m_point[0].z < rhsBox.m_point[6].z){
        return true;
    }
    else
        return false;
}

static bool collision(const hitbox& lhsBox, const hitbox& rhsBox)
{		//for AABB to AABB collision
    //Check if lhsBox's max is greater than rhsBox's min and lhsBox's min is less than rhsBox's max
    if (lhsBox.m_vecMax.x > rhsBox.m_vecMin.x &&
        lhsBox.m_vecMin.x < rhsBox.m_vecMax.x &&
        lhsBox.m_vecMax.y > rhsBox.m_vecMin.y &&
        lhsBox.m_vecMin.y < rhsBox.m_vecMax.y &&
        lhsBox.m_vecMax.z > rhsBox.m_vecMin.z &&
        lhsBox.m_vecMin.z < rhsBox.m_vecMax.z){
        return true;
    }
    else
        return false;
}

static bool collision(const hitbox& hitbox, const Vector3& vecPoint)
{
    //Check if the point is less than max and greater than min
    if (vecPoint.x > hitbox.m_vecMin.x && vecPoint.x < hitbox.m_vecMax.x &&
        vecPoint.y > hitbox.m_vecMin.y && vecPoint.y < hitbox.m_vecMax.y &&
        vecPoint.z > hitbox.m_vecMin.z && vecPoint.z < hitbox.m_vecMax.z){
        return true;
    }
    else
        return false;
}

static bool collision(const TSphere& tSph, const Vector3& vecPoint)
{
    //Calculate the squared distance from the point to the center of the sphere
    Vector3 vecDist(tSph.m_position - vecPoint);
    float fDistSq = vecDist.LengthSquared();

    //Calculate if the squared distance between the sphere's center and the point
    //is less than the squared radius of the sphere
    if (vecDist.Length() < tSph.m_Radius)
    {
        return true;
    }
    else
        return false;
}


static bool collision(const hitbox& lhsBox, const hitbox2& rhsBox){

	if (lhsBox.m_vecMax.x > rhsBox.m_point[0].x &&
		lhsBox.m_vecMin.x < rhsBox.m_point[6].x &&
		lhsBox.m_vecMax.y > rhsBox.m_point[0].y &&
		lhsBox.m_vecMin.y < rhsBox.m_point[6].y &&
		lhsBox.m_vecMax.z > rhsBox.m_point[0].z &&
		lhsBox.m_vecMin.z <rhsBox.m_point[6].z){
		return true;
	}
	else
		return false;

}

static bool collision(const hitbox2& hitbox, const Vector3& vecPoint)
{
	//Check if the point is less than max and greater than min
	if (vecPoint.x > hitbox.m_point[0].x && vecPoint.x < hitbox.m_point[6].x &&
		vecPoint.y > hitbox.m_point[0].y && vecPoint.y < hitbox.m_point[6].y &&
		vecPoint.z > hitbox.m_point[0].z && vecPoint.z < hitbox.m_point[6].z){
		return true;
	}
	else
		return false;
}

#endif;