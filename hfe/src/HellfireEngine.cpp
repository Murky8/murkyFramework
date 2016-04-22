#include "HellfireEngine.h"
#include "geomFuncts.h"
#include "inputDevices.h"

//#define DRAWPOINTS
#define DRAWMESH

void doHFE()
{
	//vec v0(0,0,0);
	//vec v1(1,1,0);
	//render.draw.line(v0, v1, 0xffffff);
	const int nSectsPerEdge=10;
	const int nVertsPerSlice=nSectsPerEdge*4;
	const int nSlices=50;

	float rad=2.0f;
	
	float dadi = PI_x2/(4.0f*nSectsPerEdge);
	float ang;
	float rotation=0;
	vec pos, offv;
	

	//int j[10];

	Carray <vec> verts;
	verts.alloc(nSectsPerEdge*4*nSlices);


	float alpha=0.5f; //alpha =0 -> sharp rotor
	//if( inputDevices.key(KEY_RIGHT) != false )rotation+=0.01f;
	//if( inputDevices.key(KEY_LEFT) != false )rotation-=0.01f;


	float depth = 0;					
	LOOPL(iSlice, 0, nSlices )
	{
		float r, r1;
		r = rad*(1.0f-alpha);
		r1 = rad-r;

		ang = -PI_x2/8.0f +rotation;	
		float off = (r-r1)*sqrt(2.0f)/2.0f;
		
		offv=vec(-sin(rotation), -cos(rotation),0.0f)*off;
		LOOPL(i,0, nSectsPerEdge)
		{
			float x, y;

			x= r*sin(ang);
			y= r*cos(ang);
						
			pos = vec(x,y,depth)+offv;

			verts.add(pos);		

			#ifdef DRAWPOINTS 	
			render.drawDeb.crosshair(pos, 0.01, 0xff8080);			
			#endif

			ang+=dadi;
		}


		offv=vec(cos(rotation), -sin(rotation),0.0f)*off;
		LOOPL(i,0, nSectsPerEdge)
		{
			float x, y;

			x= r1*sin(ang);
			y= r1*cos(ang);
						
			pos = vec(x,y,depth)+offv;

			verts.add(pos);
			
#ifdef DRAWPOINTS 	
			render.drawDeb.crosshair(pos, 0.01, 0xff8080);			
#endif
		
			ang+=dadi;
		}

		
		offv=vec(sin(rotation), cos(rotation),0.0f)*off;
		LOOPL(i,0, nSectsPerEdge)
		{
			float x, y;

			x= r*sin(ang);
			y= r*cos(ang);
								
			pos = vec(x,y,depth)+offv;

			verts.add( pos);		
#ifdef DRAWPOINTS 	
			render.drawDeb.crosshair(pos, 0.01, 0xff8080);			
#endif
		

			ang+=dadi;
		}

		
		offv=vec(-cos(rotation), sin(rotation),0.0f)*off;
		LOOPL(i,0, nSectsPerEdge)
		{
			float x, y;
			
			x= r1*sin(ang);
			y= r1*cos(ang);
					
			pos = vec(x,y,depth)+offv;

			verts.add(pos);
		
#ifdef DRAWPOINTS 	
			render.drawDeb.crosshair(pos, 0.01, 0xff8080);			
#endif
		
			ang+=dadi;
		}

	
		// form
		if(iSlice<10)
		{				
		rotation+=	0.2f;
		depth	+= 0.3;
		alpha -= (0.5/10);
		}
		else
			/*
			if(iSlice<30)
			{
				float a = (PI/(30-10-1))*(iSlice-10);
				alpha = sinf(a)*sinf(a)*0.2f;

				rotation+=	0.002f;
				depth	+= 0.01;
			}
			else
			*/
				if(iSlice<40)
				{
					rotation+=	PI_x2/(19*2);					
					depth	+= 0.3;
				}								
		// form

	}
	


	// dump mesh
	Carray<STLFace> faces(nVertsPerSlice*(nSlices-1)*2, true);
	LOOPL(i, 0, nSlices-1)
	{
		LOOPL(j, 0, nVertsPerSlice)
		{
		STLFace face0, face1;

		int jPlus1 = (j+1)%nVertsPerSlice;

		face0.p[0] = verts[i*nVertsPerSlice +j];
		face0.p[1] = verts[i*nVertsPerSlice +jPlus1];
		face0.p[2] = verts[(i+1)*nVertsPerSlice +j];

		face1.p[0] = verts[i*nVertsPerSlice +jPlus1];
		face1.p[1] = verts[(i+1)*nVertsPerSlice +j];
		face1.p[2] = verts[(i+1)*nVertsPerSlice +jPlus1];
										 
		faces.add(face0);
		faces.add(face1);
		}
	}

	// draw
#ifdef DRAWMESH
	for( STLFace* ITERATE(face, faces))
	{
		render.draw.wireTriangle( face->p[0],face->p[1], face->p[2],0xf808080 );
	}
#endif
		

	//saveMeshAsSTL( "HFERotor.stl", faces );
	faces.free();
	verts.free();
	//exit2();
}


void	saveMeshAsSTL( char *_fileName, Carray<STLFace> &faces )
{
	wickedLibMisc::setCurrentDirFromGameRoot("");
		
	Carray< char > text; 
	text.alloc( 32*1024*1024, true );

	text.addText("solid HFE \n");
	for( STLFace* ITERATE(face, faces) )
	{
		vec norm = geomFuncts.getFaceNormal( face->p[0], face->p[1], face->p[2]);

		text.addText("  facet normal %e %e %e \n", norm.x, norm.y, norm.z);
		text.addText("    outer loop\n");
		text.addText("      vertex %e %e %e\n", face->p[0].x, face->p[0].z, face->p[0].y);
		text.addText("      vertex %e %e %e\n", face->p[1].x, face->p[1].z, face->p[1].y);
		text.addText("      vertex %e %e %e\n", face->p[2].x, face->p[2].z, face->p[2].y);
		text.addText("    endloop\n");
		text.addText("  endfacet\n");
	}
	text.addText("endsolid HFE \n");	
	text.saveFile( _fileName );
	text.free();
}


