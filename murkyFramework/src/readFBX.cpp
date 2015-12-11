//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/src/pch.hpp>

namespace murkyFramework
{
//http://www.cheatography.com/davechild/cheat-sheets/regular-expressions/

	bool done = false;

	int getNextPosInt_incIt(std::wstring::const_iterator &it, std::wstring::const_iterator  &end)
	{
		static std::wregex regx_int(L"[0-9]+");

		int i{ 0 };
		std::wsmatch match;

		if (std::regex_search(it, end, match, regx_int))
		{
			std::wstring substr(match[0].first, match[0].second);
			i = _wtoi(substr.c_str());

			it = match[0].second;
		}
		return i;
	}

	int getNextInt_incIt(std::wstring::const_iterator &it, std::wstring::const_iterator  &end)
	{
		static std::wregex regx_int(L"[-\\+]?[0-9]+");

		int i{ 0 };
		std::wsmatch match;

		if (std::regex_search(it, end, match, regx_int))
		{
			std::wstring substr(match[0].first, match[0].second);
			i = _wtoi(substr.c_str());

			it = match[0].second;
		}

		return i;
	}

	float getNextFloat_incIt(std::wstring::const_iterator &it, std::wstring::const_iterator  &end)
	{
		// "-+[0-9]+\\.+[0-9]+"
		static std::wregex regx_float(L"[-\\+]?[0-9]*\\.?[0-9]+");

		float f{ 0 };
		std::wsmatch match;

		if (std::regex_search(it, end, match, regx_float))
		{
			std::wstring substr(match[0].first, match[0].second);
			f = (float)_wtof(substr.c_str());

			it = match[0].second;
		}

		return f;
	}

	bool findNextText_incIt(std::wstring &strToFind, std::wstring::const_iterator &it, std::wstring::const_iterator  &end)
	{
        std::wstring regex_string;
        regex_string += L"[\\s]";
        regex_string += strToFind;
        std::wregex regx_text(regex_string);
                                       
        std::wsmatch match;

        if (std::regex_search(it, end, match, regx_text))
        {
            std::wstring substr(match[0].first, match[0].second);            
            it = match[0].second;
    		return true;
        }
        else
        {
            return false;
        }
	}
		
	void loadFBX_tris(const std::wstring &filePathName,	std::vector<Triangle_pct> &tris)
	{				
		std::unique_ptr<qdev::BinaryFileLoader> pBinaryFile(new qdev::BinaryFileLoader(filePathName));
		if (pBinaryFile->pdata == nullptr)
		{// unsuccessful
			done = true;		
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

		// parse text todo: remove!!!
		done = true;		
	}

    /*
    void loadFBX_tris(const std::wstring &filePathName, std::vector<Vert_pct> &verts, std::vector<u16> &indexes )
    {
        std::unique_ptr<qdev::BinaryFileLoader> pBinaryFile(new qdev::BinaryFileLoader(filePathName));
        if (pBinaryFile->pdata == nullptr)
        {// unsuccessful
            done = true;
        }

        std::string temp(pBinaryFile->pdata, pBinaryFile->getDataLength());
        std::wstring text = s2ws(temp);

        // parse text

        // get num verts
        auto it = text.cbegin();
        if (findNextText_incIt(text, L"Vertices:", it) == false)
        {
            triggerBreakpoint();
        }

        int nVerts;
        nVerts = getNextInt_incIt(it, text.end()) / 3;
       
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
        if (findNextText_incIt(text, L"PolygonVertexIndex:", it) == false)
        {
            triggerBreakpoint();
        }
        int nFaces = getNextInt_incIt(it, text.end()) / 3;

        // get face indices
        for (int i = 0; i < nFaces; i++)
        {
            int i0 = getNextInt_incIt(it, text.end());
            int i1 = getNextInt_incIt(it, text.end());
            int i2 = ~getNextInt_incIt(it, text.end()); // note: FBX weirdness -1

            indexes.push_back(i0);
            indexes.push_back(i1);
            indexes.push_back(i2);
        }

        // parse text
        done = true;
    }
*/
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
		for (int i = 0; i++ < nTris; )
		{
			Triangle_pct newTri;
			file.read(reinterpret_cast<char*>(&newTri), sizeof(Triangle_pct));
			tris.push_back(newTri);
		}
	}

	/*void loadFBX_threaded(const std::wstring &dirName, const std::wstring &fileName, const std::wstring &extensionName,
		bool &done)
	{
		void loadFBX(const std::wstring &dirName, const std::wstring &fileName, const std::wstring &extensionName)
	}
*/
	/*bool doFBX(const std::wstring &dirName, const std::wstring &fileName, const std::wstring &extensionName)
	{
		bool res = loadFBX(dirName, fileName, extensionName);
		if (res == false)
			triggerBreakpoint();

		return true;
	}
*/
}//namespace murkyFramework