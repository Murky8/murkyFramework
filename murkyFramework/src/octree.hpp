#pragma once
//------------------------------------------------------------------------------
// 2015 J. Coelho.
// C++11. Windows. /*Android.*/
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <queue>

namespace murkyFramework {
    
    using namespace std;  

    class Entity;
    class EntityRef
    {
        int index;
    };

    class CubeIntLocation
    {
    public:
        veci_u16 gridCoords;
        int subDivLevel;
        CubeIntLocation() {}
        CubeIntLocation(veci_u16 gridCoords, int subDivLevel) :
            gridCoords(gridCoords), subDivLevel(subDivLevel)
        {}
    };

    class CubeRef
    {
    public:
        unsigned short index;
        CubeRef() {}
        CubeRef(unsigned short index) : index(index)
        {
        }
    };



    class Cube
    {
    public:
        std::list<EntityRef> entities;
        bool hasChildren;

        Cube()
        {
            hasChildren = false;
        }

        CubeRef children8;
    };


    class Octree
    {
    public:

        float rootCubeDim = { 100.f };
        Octree(float rootCubeDim) : rootCubeDim(rootCubeDim)
        {}

        std::vector<Cube> cubes;

        Cube&	getCube(CubeRef it)
        {
            return cubes[it.index];
        }

        Cube& operator[](CubeRef it)
        {
            return getCube(it);
        }

        CubeRef rootCube()
        {
            return CubeRef(0);
        }

        void addCube(Cube& newCube)
        {
            cubes.push_back(newCube);
        }

        void	subDivideCube(CubeRef it)
        {
            Cube &motherCube = getCube(it);

            if (motherCube.hasChildren == true)
                exit(0);

            motherCube.children8 = CubeRef(cubes.size());
            for (int i = 0;i<8;i++)
            {
                Cube newCube;
                addCube(newCube);
            }
        }

        void calcBoxMinMax(CubeIntLocation location, vec4& out_min, vec4& out_max)
        {
            float boxLength = rootCubeDim / (float)(0b1 << location.subDivLevel);
            out_min.x = (float)location.gridCoords.ix*boxLength;
            out_max.x = out_min.x + boxLength;

            out_min.y = (float)location.gridCoords.iy*boxLength;
            out_max.y = out_min.y + boxLength;

            out_min.z = (float)location.gridCoords.iz*boxLength;
            out_max.z = out_min.z + boxLength;
        }
    };

    class TreeWalker
    {
    public:
        Octree &octree;
        vec4 min;
        vec4 max;

        int ix, iy, iz;
        //float x, y, z;
        int subDivLevel; //root = 0
        CubeRef cubeRef;

        TreeWalker(Octree &octree) : octree(octree)
        {
        }

        void moveToChild(int index)
        {
        }
    };

    CubeRef findCubeAt(vec4 pos, Octree &octree)
    {
        //	TreeWalker it(octree);
        CubeRef cube = octree.rootCube();
        vec4 min = { 0.f, 0.f, 0.f };
        float cubeDim = octree.rootCubeDim;
        vec4 max = { cubeDim,cubeDim, cubeDim };

        while (octree.getCube(cube).hasChildren == true)
        {
            vec4 mid{ 0,0,0 };// = (max-min)/2.0f;	
            int addr = 0;

            if (pos.x>mid.x)
            {
                addr += 0b001;
                min.x = mid.x;
            }
            else
            {
                max.x = mid.x;
            }

            if (pos.y>mid.y)
            {
                addr += 0b010;
                min.y = mid.y;
            }
            else
            {
                max.y = mid.y;
            }

            if (pos.z>mid.z)
            {
                addr += 0b100;
                min.z = mid.z;
            }
            else
            {
                max.z = mid.z;
            }
            cube.index = octree.getCube(cube).children8.index + addr;
        }
        return cube;
    }

    struct CubeTempInfo
    {
        CubeRef cubeRef;
        CubeIntLocation cubeIntLocation;
    };

    void getChildCubesInBox(std::vector<CubeRef> &foundCubes, Octree &octree,
        vec4 boxMin, vec4 boxMax)
    {
        std::queue<CubeTempInfo> fifob;

        CubeTempInfo c;
        c.cubeRef = octree.rootCube();

        c.cubeIntLocation.gridCoords.ix = 0;
        c.cubeIntLocation.gridCoords.iy = 0;
        c.cubeIntLocation.gridCoords.iz = 0;
        c.cubeIntLocation.subDivLevel = 0;
        fifob.push(c);

        int deb = 99;
        while (fifob.empty() == false)
        {
            deb--;
            if (deb < 0)
                triggerBreakpoint(L"getChildCubesInBox error");

            CubeTempInfo &cubeTempInfo = fifob.front();
            fifob.pop();
            vec4 cubeMin, cubeMax;
            octree.calcBoxMinMax(cubeTempInfo.cubeIntLocation, cubeMin, cubeMax);
            if (!(boxMax.x<cubeMin.x || boxMin.x>cubeMax.x))
                if (!(boxMax.y<cubeMin.y || boxMin.y>cubeMax.y))
                    if (!(boxMax.z<cubeMin.z || boxMin.z>cubeMax.z))
                    {
                        if (octree.getCube(cubeTempInfo.cubeRef).hasChildren == true)
                        {
                        }
                        else
                        {
                        }
                        //fifb.push
                    }
        }
    }


    class GridElement
    {
    public:
        std::list<void*> entityRefs;
    };

    class Grid
    {
    public:
        GridElement* gridData;
        int xDim;
        int yDim;
        int zDim;

        Grid(int x, int y, int z)
        {
            gridData = new GridElement[xDim*yDim*zDim];
        }

        ~Grid()
        {
            delete[] gridData;
        }

        GridElement& getElementAt(int x, int y, int z)
        {
            return gridData[z*xDim*yDim + y*xDim + x];
        }
    };

}//namespace murkyFramework