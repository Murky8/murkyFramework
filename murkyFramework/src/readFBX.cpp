//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/src/pch.hpp>

namespace murkyFramework
{	
	void loadFBX_tris(const std::wstring &filePathName,	std::vector<Triangle_pct> &tris)
	{				
        triggerBreakpoint(L"fix");

		std::unique_ptr<qdev::BinaryFileLoader> pBinaryFile(new qdev::BinaryFileLoader(filePathName));
		if (pBinaryFile->pdata == nullptr)
		{// unsuccessful			
		}

		std::string temp(pBinaryFile->pdata, pBinaryFile->getDataLength());
		std::wstring text = s2ws(temp);

		// parse text

		// get num verts
		auto it = text.cbegin();
		if (findNextText_incIt(std::wstring(L"Vertices:"), it, text.end() ) == false)
		{
			triggerBreakpoint();
		}

		int nVerts;
		nVerts = getNextInt_incIt(it, text.end()) / 3;

		std::vector<Vert_pct >	verts;
		
		// read verts
		for (int i = 0; i < nVerts; i++)
		{
			float x = getNextFloat_incIt(it, text.end());
			float z = getNextFloat_incIt(it, text.end());
			float y = getNextFloat_incIt(it, text.end());
			verts.push_back
				(
					Vert_pct{ vec3(x, y, z), vec3(1, 1, 1), vec2(1, 1) }
			);
			//debugLog << i << x << y << z << L"\n";
		}
		// get num faces
        if (findNextText_incIt(std::wstring(L"PolygonVertexIndex:"), it, text.end()) == false)		
			triggerBreakpoint();
		
		int nFaces = getNextInt_incIt(it, text.end()) / 3;

		// get face indices
		for (int i = 0; i < nFaces; i++)
		{
			int i0 = getNextInt_incIt(it, text.end());
			int i1 = getNextInt_incIt(it, text.end());
			int i2 = ~getNextInt_incIt(it, text.end()); // note: FBX weirdness -1

			tris.push_back(
				Triangle_pct
				(
					verts[i0],
					verts[i1],
					verts[i2]

					));
		}

        // get UVs
        std::vector<vec2> uvs;

        if (findNextText_incIt(std::wstring(L"UV:"), it, text.end()) == false)
            triggerBreakpoint();

        int nUVs = getNextInt_incIt(it, text.end());

        for (int i = 0; i < nUVs/2; i++)
        {
            vec2 v;
            v.x = getNextFloat_incIt(it, text.end());
            v.y = getNextFloat_incIt(it, text.end());

            uvs.push_back(v);
        }

        if (findNextText_incIt(std::wstring(L"UVIndex:"), it, text.end()) == false)
            triggerBreakpoint(L"meh");        
        {
            int nUVIndicies = getNextInt_incIt(it, text.end());
            if(nUVIndicies!=nFaces*3)
                triggerBreakpoint();
        }

        for (int i = 0; i < nFaces; i++)
        {
            int i0 = getNextInt_incIt(it, text.end());
            int i1 = getNextInt_incIt(it, text.end());
            int i2 = getNextInt_incIt(it, text.end()); // note: NOT FBX weirdness -1

            tris[i].v[0].textCoords = uvs[i0];
            tris[i].v[1].textCoords = uvs[i1];
            tris[i].v[2].textCoords = uvs[i2];
        }			
	}

    void serializeTris(const std::wstring &filePathName, std::vector<Triangle_pct> &tris)
    {
        std::fstream ofile;
        ofile.open(filePathName, std::ios::out + std::ios::binary + std::ios::trunc);
        u32 nTris = tris.size();

        ofile.write(reinterpret_cast<char*>(&nTris), sizeof(nTris));
        ofile.write(reinterpret_cast<char*>(tris.data()), tris.size() *sizeof(Triangle_pct));
        /*for(Triangle_pct & tri : tris)
        {
        ofile << tri.v[0].pos.x;
        ofile << tri.v[0].pos.y;
        ofile << tri.v[0].pos.z;

        }*/
        ofile.close();
    }

    void deserializeTris(const std::wstring &filePathName, std::vector<Triangle_pct> &tris)
    {
        std::ifstream file;
        file.open(filePathName, std::ios::in + std::ios::binary);

        u32 nTris = tris.size();
        file.read(reinterpret_cast<char*>(&nTris), sizeof(nTris));
        for (int i = 0; i < nTris; ++i)
        {
            Triangle_pct newTri;
            file.read(reinterpret_cast<char*>(&newTri), sizeof(Triangle_pct));
            tris.push_back(newTri);
        }
    }

    void loadFBX(const std::wstring &filePathName, std::vector<Vert_pct> &vertices, std::vector<u16> &indices )
    {
        std::unique_ptr<qdev::BinaryFileLoader> pBinaryFile(new qdev::BinaryFileLoader(filePathName));
        if (pBinaryFile->pdata == nullptr)
        {// unsuccessful
        }

        auto temp = new std::string(pBinaryFile->pdata, pBinaryFile->getDataLength());
        auto text = new std::wstring();
            *text = s2ws(*temp);

        delete temp;
        // parse text

        // get num verts
        auto it = text->cbegin();
        if (findNextText_incIt(std::wstring(L"Vertices:"), it, text->end()) == false)
        {
            triggerBreakpoint();
        }

        int nVerts;
        nVerts = getNextInt_incIt(it, text->end()) / 3;        

        // read verts
        for (int i = 0; i < nVerts; i++)
        {
            float x = getNextFloat_incIt(it, text->end());
            float z = getNextFloat_incIt(it, text->end());
            float y = getNextFloat_incIt(it, text->end());
            vertices.push_back
                (
                    Vert_pct{ vec3(x, y, z), vec3(1, 1, 1), vec2(1, 1) }
            );
            //debugLog << i << x << y << z << L"\n";
        }
        // get num faces
        if (findNextText_incIt(std::wstring(L"PolygonVertexIndex:"), it, text->end()) == false)
            triggerBreakpoint();

        int nFaces = getNextInt_incIt(it, text->end()) / 3;

        // get face indices
        for (int i = 0; i < nFaces; i++)
        {
            int i0 = getNextInt_incIt(it, text->end());
            int i1 = getNextInt_incIt(it, text->end());
            int i2 = ~getNextInt_incIt(it, text->end()); // note: FBX weirdness -1
            //int i2 = getNextInt_incIt(it, text->end()); // note: FBX weirdness -1


            if (i0 < 0 || i1 < 0 || i2 < 0)
               triggerBreakpoint();

            indices.push_back(i0);
            indices.push_back(i1);
            indices.push_back(i2);
        }

        // get UVs
        auto uvs = new std::vector<vec2>();

        if (findNextText_incIt(std::wstring(L"UV:"), it, text->end()) == false)
            triggerBreakpoint();

        int nUVs = getNextInt_incIt(it, text->end());

        for (int i = 0; i < nUVs / 2; i++)
        {
            vec2 v;
            v.x = getNextFloat_incIt(it, text->end());
            v.y = getNextFloat_incIt(it, text->end());

            uvs->push_back(v);
        }

        if (findNextText_incIt(std::wstring(L"UVIndex:"), it, text->end()) == false)
            triggerBreakpoint(L"meh");
        {
            int nUVIndicies = getNextInt_incIt(it, text->end());
            //if (nUVIndicies != nFaces * 3)
                //triggerBreakpoint();
        }

        for (int i = 0; i < nFaces; i++)
        {
            int i0 = getNextInt_incIt(it, text->end());
            int i1 = getNextInt_incIt(it, text->end());
            int i2 = getNextInt_incIt(it, text->end()); // note: NOT FBX weirdness -1

            vertices[indices[i*3 + 0]].textCoords = (*uvs)[i0];
            vertices[indices[i*3 + 1]].textCoords = (*uvs)[i1];
            vertices[indices[i*3 + 2]].textCoords = (*uvs)[i2];            
        }        
        delete uvs;
        delete text;
    }

    void serializeTris(const std::wstring &filePathName, std::vector<Vert_pct> &vertices, std::vector<u16> &indices)
    {
        std::fstream ofile;
        ofile.open(filePathName, std::ios::out + std::ios::binary + std::ios::trunc);
        
        u32 nVerts = vertices.size();
        ofile.write(reinterpret_cast<char*>(&nVerts), sizeof(nVerts));
        ofile.write(reinterpret_cast<char*>(vertices.data()), vertices.size() *sizeof(Vert_pct));

        u32 nIndices = indices.size();
        ofile.write(reinterpret_cast<char*>(&nIndices), sizeof(nIndices));
        ofile.write(reinterpret_cast<char*>(indices.data()), indices.size() *sizeof(u16));
        
        ofile.close();
    }

    void deserializeTris(const std::wstring &filePathName, std::vector<Vert_pct> &vertices, std::vector<u16> &indices)
    {
        std::ifstream file;
        file.open(filePathName, std::ios::in + std::ios::binary);

        u32 nVerts;
        file.read(reinterpret_cast<char*>(&nVerts), sizeof(nVerts));
        for (int i = 0; i < nVerts; ++i)
        {
            Vert_pct newVert;
            file.read(reinterpret_cast<char*>(&newVert), sizeof(Vert_pct));
            vertices.push_back(newVert);
        }

        u32 nIndices;
        file.read(reinterpret_cast<char*>(&nIndices), sizeof(nIndices));
        for (int i = 0; i < nIndices; ++i)
        {
            u16 newIndex;
            file.read(reinterpret_cast<char*>(&newIndex), sizeof(newIndex));
            indices.push_back(newIndex);
        }
    }

    void compileFBX(FilePathSplit pathSplit)
    {
        std::wstring binPath = makePathString(pathSplit.directoryPath, pathSplit.fileName, L"bin");

        if (fileNeedsCompiling(pathSplit))
        {// compile to .bin
            debugLog << L"bin is not current. compiling \n";
        }
        else
        {
            debugLog << L"bin is current \n";
  //          murkyFramework::deserializeTris(binPath, gdeb_tris);
        }
    }

}//namespace murkyFramework