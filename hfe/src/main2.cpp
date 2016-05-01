#include <murkyFramework/src/pch.hpp>
#define _USE_MATH_DEFINES
#include <math.h>
          
namespace murkyFramework
{
    void  main_noGfx()
    {           
        f32 deltaTime = g_appDebug->lastFrameDuration;
    }

    void hfe_createLayerVerts(vec4 *verts, f32 rMajor, f32 rMinor, f32 phase, f32 z, 
        int nMajorSegments, int nMinorSegments)
    {
        f32 r = rMajor + rMinor;

        vec basisX = { cos(phase), sin(phase), 0 };
        vec basisY = { -sin(phase), cos(phase), 0 };

        float dtheta = 3.141f*0.5f / ((f32)nMinorSegments);

        f32 theta;
        int i;

        // right minor        
        for (theta = 0, i = 0; i < nMinorSegments; ++i)
        {
            vec offset = { r*0.5f - rMinor, r*0.5f - rMinor, 0.0f };
            f32 x = rMinor*sin(theta);
            f32 y = rMinor*cos(theta);

            vec vert = basisX*(x + offset.x) + basisY*(y + offset.y);
            vert.z = z;

            theta += dtheta;
            *verts = vert;
            verts++;
        }
        // bottom major        
        for (theta = 0, i = 0; i < nMajorSegments; ++i)
        {
            vec offset = { -1.0f*(rMajor - 0.5f*r), 1.0f*(rMajor - 0.5f*r), 0.0f };
            f32 x = rMajor*sin(theta + M_PI_2);
            f32 y = rMajor*cos(theta + M_PI_2);

            vec vert = basisX*(x + offset.x) + basisY*(y + offset.y);
            vert.z = z;

            theta += dtheta;            
            *verts = vert;
            verts++;
        }

        // left minor        
        for (theta = 0, i = 0; i < nMinorSegments; ++i)
        {
            vec offset = { -1.0f*(r*0.5f - rMinor), -1.0f*(r*0.5f - rMinor), 0.0f };
            f32 x = rMinor*sin(theta + M_PI);
            f32 y = rMinor*cos(theta + M_PI);

            vec vert = basisX*(x + offset.x) + basisY*(y + offset.y);
            vert.z = z;

            theta += dtheta;
            *verts = vert;
            verts++;            
        }

        // top major        
        for (theta = 0, i = 0; i < nMajorSegments; ++i)
        {
            vec offset = { rMajor - 0.5f*r, -1.0f*(rMajor - 0.5f*r), 0.0f };
            f32 x = rMajor*sin(theta - M_PI_2);
            f32 y = rMajor*cos(theta - M_PI_2);

            vec vert = basisX*(x + offset.x) + basisY*(y + offset.y);
            vert.z = z;

            theta += dtheta;
            *verts = vert;
            verts++;            
        }

    }
    
    void  main_gfx()
    {                  
        auto &lines = g_appDebug->render->defaultLines;
        // make  helfire rotor

        f32 rMajor = 25.0f;
        f32 rMinor = 0.0f;
        //f32 r = rMajor + rMinor;
        int nMajorSegments = 10;
        int nMinorSegments = 10;

        static f32 phase = 0.0f;
        phase += 0.001f;        

        f32 tphase = phase;
        
        const int nSliceVerts = 100;
        const int nSlices = 10;

        boost::multi_array<vec4, 2> verts(boost::extents[nSlices][nSliceVerts]);

        for (int z = 0;z < nSlices;z++)
        {
            hfe_createLayerVerts(&(verts[z][0]), rMajor, rMinor, tphase, (f32)z*1.1f,
                nSliceVerts/4, nSliceVerts/4);

            tphase += 0.1f;
        }

        for (int z = 0;z < nSlices;z++)
        {                            
            for (int i = 0;i < nSliceVerts;i++)
            {
                //Render::drawCrosshair(verts[z][i], vec3(1, 1, 1), 0.1f, lines);
                Render::drawLine(
                    Vert_pc{ verts[z][i], vec3(1, 1, 1) }, 
                    Vert_pc{ verts[z][(i+1)%nSliceVerts], vec3(1, 1, 1) }, 
                    lines);
            }
            
        }            
    }  
}// namespace murkyFramework

using namespace murkyFramework;

int main() // can't be in a namespace :(
{
    
    //     

    //skool();    
    AppFramework_initStruct appInit;
    appInit.main_noGfx  = main_noGfx;
    appInit.main_gfx    = main_gfx;


    AppFramework *const app = new AppFramework(appInit);
 
    // initialise
    g_appDebug->game->cursorPosOri = Render::makeLookAtPosOri(
        vec4(0, 0, -20), vec4(0, 0, 0));

    app->run();
    murkyFramework::debugLog << L"Finished\n";
     
}