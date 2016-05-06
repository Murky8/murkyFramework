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

        f32 rMajor = 15.0f;
        f32 rMinor = 15.0f;
        //f32 r = rMajor + rMinor;
        int nMajorSegments = 10;
        int nMinorSegments = 10;

        static f32 phase = 0.0f;
        phase += 0.001f;        

        f32 tphase = phase;
        
        const int nSliceVerts = 100;
        const int nSlices = 20;

        boost::multi_array<vec4, 2> verts(boost::extents[nSlices][nSliceVerts]);

        //create
        f32 dr = (f32)rMinor / (nSlices-1);
        dr *= 0.99f; // to avoid singulatrity

        for (int z = 0;z < nSlices;z++)
        {
            if (rMajor == 0)triggerBreakpoint(L"singulatity");
            if (rMinor == 0)triggerBreakpoint(L"singulatity");

            hfe_createLayerVerts(&(verts[z][0]), rMajor, rMinor, tphase, (f32)z*1.1f,
                nSliceVerts/4, nSliceVerts/4);
            
            //tphase += 0.1f;
            rMajor += dr;
            rMinor -= dr;
        }

        // display
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

        //save .STL
        /*
        solid Object01
            facet normal 9.848077e-001 - 1.736483e-001 8.225585e-007
            outer loop
            vertex 5.876782e+001 2.518892e+000 0.000000e+000
            vertex 5.590727e+001 - 1.370409e+001 0.000000e+000
            vertex 5.590728e+001 - 1.370409e+001 - 9.571788e+000
            endloop
            endfacet
            ...
            endsolid Object01
            */
        std::ofstream text;
        text.open(L"asdfg.STL");

        text.setf(std::ios::scientific);
        text << "solid Object01" << std::endl;
 
        for (int z = 0;z < nSlices-1;z++)
        {
            for (int i = 0;i < nSliceVerts;i++)
            {
                vec3 q[4] =
                {
                    verts[z][i],
                    verts[z][(i + 1) % nSliceVerts],
                    verts[z + 1][i],
                    verts[z + 1][(i + 1) % nSliceVerts]
                };

                //int it = 0;
                for(int it=0;it<=1;it++)
                {
                    vec3 t[3];
                    if (it == 0)
                    {
                        t[0] = q[0];
                        t[1] = q[2];
                        t[2] = q[1];
                    }
                    else
                    {
                        t[0] = q[3];
                        t[1] = q[1];
                        t[2] = q[2];
                    }

                    vec normal = (cross(t[0] - t[2], t[0] - t[1])).unitDir();

                    //text << "\tfacet normal 0.000000e+000 0.000000e+000 0.000000e+000" << std::endl;
                    text << "  facet normal " << 
                        normal.x << " " <<
                        normal.y << " " <<
                        normal.z << " " <<
                        std::endl;

                    text << "    outer loop" << std::endl;
                
                    text << "      vertex " << t[0].x << " " << t[0].y << " " << t[0].z << std::endl;
                    text << "      vertex " << t[1].x << " " << t[1].y << " " << t[1].z << std::endl;
                    text << "      vertex " << t[2].x << " " << t[2].y << " " << t[2].z << std::endl;                
                
                    text << "    endloop" << std::endl;
                    text << "  endfacet" << std::endl;                
                }
            }
        }
        text << "endsolid Object01" << std::endl;


        text.close();        
        exit(0);
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