#include <iostream>
#include <fstream>
#include "LoadHmap.h"
#include "Vector3.h"

bool LoadHeightMap(const char *file_path, std::vector<unsigned char> &heightMap)
{
	std::cout << "Loading \"" << file_path << "\"... ";

	std::ifstream fileStream(file_path, std::ios::binary);
	if(!fileStream.is_open())
	{
		std::cout << " File not found.\n";
		return false;
	}

    fileStream.seekg(0, std::ios::end); // (offset, end of stream)
    std::streampos fsize = (unsigned)fileStream.tellg(); // get fsize = ? bytes
	
	fileStream.seekg(0, std::ios::beg); // beginning
	heightMap.resize((unsigned)fsize); // allocate memory
	fileStream.read((char *)&heightMap[0], fsize); // reading
	
	fileStream.close();

	std::cout << " done." << std::endl;

	return true;
}

float ReadHeightMap(std::vector<unsigned char> &heightMap, float x, float z)
{
	if(x < -0.5f || x > 0.5f || z < -0.5f || z > 0.5f)
		return 0;
	if(heightMap.size() == 0)
		return 0;

	unsigned terrainSize = (unsigned)sqrt((double)heightMap.size());
	
	unsigned zCoord = (unsigned)((z + 0.5f) * terrainSize);
	unsigned xCoord = (unsigned)((x + 0.5f) * terrainSize);

	return (float)heightMap[zCoord * terrainSize + xCoord] / 100.f;
}

Vector3 ReadHeightMap2(std::vector<unsigned char> &heightMap, float x, float z)//trying to use barycentric to return a point
{
	if (heightMap.size() == 0)
		return 0;

	//get x & z coords relative to terrain
	//float terrainx = x - terrain posx;
	//float terrainz = z - terrain posz;

	//get the size of a single square
	//float tilesize = 100/height.length() -1;//where height is the height array

	//find out whichs square the player is on
	//int gridx = terrainx/tilesize;
	//int gridy = terrainz/tilesize;

	//find out where the player is on the grid square
	//float xCoord = (terrainx % tilesize )/tilesize;//  xCoord>=0<=1
	//float yCoord = (terrainy % tilesize )/tilesize;//  yCoord>=0<=1

	//find out which triangle the player is on
	//float side


	//if(xCoord <= (1-zCoord))//top side
	{
			/*barryCentric(Vector3(0, heights[gridX][gridZ], 0), Vector3(1,
			heights[gridX + 1][gridZ], 0), Vector3(0,
			heights[gridX][gridZ + 1], 1), Vector3(xCoord, zCoord,0));*/
	}
	//else//bottom
	{
			/*barryCentric(Vector3(0, heights[gridX][gridZ], 0), Vector3(1,
			heights[gridX + 1][gridZ], 0), Vector3(0,
			heights[gridX][gridZ + 1], 1), Vector3(xCoord, zCoord, 0));*/
	}
}

float barryCentric(Vector3 p1, Vector3 p2, Vector3 p3, Vector3 pos) {
	float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
	float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
	float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
	float l3 = 1.0f - l1 - l2;
	return l1 * p1.y + l2 * p2.y + l3 * p3.y;
}
