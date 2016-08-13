//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11. vulkan
#include <murkyFramework/src/pch.hpp>

namespace murkyFramework {
    namespace GfxDevice {
        void GfxDeviceObj::setUniform_projectionMatrix(const float *pMat, int slot)
        {
            // note: OGL, this accepts row-major, pre-multiplying of verts and post-multi in vertex shader.
            // ie no need to transpose if post-multi (Mv) in vertex shader.

            projectionMatricies[slot] = *(mat4*)pMat;            
        }

        void GfxDeviceObj::setCurrentSlot(int in_slot)
        {
            /*glUseProgram(shaderManager.get(L"posColTex").value);
            glUniformMatrix4fv(GfxDevice::Shaders::uniformHandle_projectionMatrix, 1, false, (f32*)&projectionMatricies[in_slot]);
            glUseProgram(0);

            glUseProgram(shaderManager.get(L"posCol").value);
            glUniformMatrix4fv(GfxDevice::Shaders::uniformHandle_projectionMatrix, 1, false, (f32*)&projectionMatricies[in_slot]);
            glUseProgram(0);*/
        }

        GfxDeviceObj::GfxDeviceObj(GfxDeviceObj_initStruct *const initStruct) :
            hDC(initStruct->windowsSpecific->gethDC())
        {
            g_appDebug->render->gfxDevice = this; // warning: see g_aapDebug usage notes: for development only, remove!

			VkResult res;
			bool enableValidation = false;
			systemSpecific::WindowsSpecific *const windowsSpecific = initStruct->windowsSpecific;

			VkApplicationInfo applicationInfo = {};
			applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
			applicationInfo.pApplicationName = "vulkan murky";
			applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
			applicationInfo.pEngineName = "vulkan murky";
			applicationInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
			applicationInfo.apiVersion = VK_MAKE_VERSION(1, 0, 0);

			std::vector<const char*> enabledExtensions = { VK_KHR_SURFACE_EXTENSION_NAME };
#if defined(_WIN32)
			enabledExtensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#endif

			VkInstanceCreateInfo instanceCreateInfo = {};
			instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			instanceCreateInfo.pNext = nullptr;
			instanceCreateInfo.flags = 0;
			instanceCreateInfo.pApplicationInfo = &applicationInfo;
			instanceCreateInfo.enabledLayerCount = 0;
			instanceCreateInfo.ppEnabledLayerNames = nullptr;
			instanceCreateInfo.enabledExtensionCount = enabledExtensions.size();
			instanceCreateInfo.ppEnabledExtensionNames = enabledExtensions.data();

			if (enableValidation)
			{
				enabledExtensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);				
				//instanceCreateInfo.enabledLayerCount = vkDebug::validationLayerCount;
				//instanceCreateInfo.ppEnabledLayerNames = vkDebug::validationLayerNames;
			}

			VkInstance instance;
			res = vkCreateInstance(&instanceCreateInfo, nullptr, &instance);
			if (res == VK_ERROR_INCOMPATIBLE_DRIVER)
			{
				triggerBreakpoint(L"Cannot find a compatible Vulkan installable client driver \
					(ICD).\n\nPlease look at the Getting Started guide for \
					additional information.\n \
					vkCreateInstance Failure");
			}

			if (res == VK_ERROR_EXTENSION_NOT_PRESENT) 
			{
				triggerBreakpoint(L"Cannot find a specified extension library \
					.\nMake sure your layers path is set appropriately\n \
					vkCreateInstance Failure");
			}
			
			if (res) 
			{
				triggerBreakpoint(L"vkCreateInstance failed.\n\nDo you have a compatible Vulkan \
					installable client driver (ICD) installed?\nPlease look at \
					the Getting Started guide for additional information.\n \
					vkCreateInstance Failure");
			}

			uint32_t gpu_count;
			res = vkEnumeratePhysicalDevices(instance, &gpu_count, NULL);
			if (gpu_count == 0)
				triggerBreakpoint();


			VkPhysicalDevice *physical_devices = (VkPhysicalDevice*)malloc(sizeof(VkPhysicalDevice) * gpu_count);

			res = vkEnumeratePhysicalDevices(instance, &gpu_count, physical_devices);
			if (res)
				triggerBreakpoint();

			/* For tri demo we just grab the first physical device */
			//demo->gpu = physical_devices[0];
			free(physical_devices);

        }

        void GfxDeviceObj::initialise()
        {
        }

        GfxDeviceObj::~GfxDeviceObj()
        {
        }

        void GfxDeviceObj::drawBegin()
        {
            /*glViewport(0, 0, g_appDebug->screenResX, g_appDebug->screenResY);

            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
*/
            /*
            // pixella moof test
            glBindTexture(GL_TEXTURE_2D, textureManager.get(L"font 4c").value);

            const auto subDiv = 256;            
            boost::multi_array<u8, 3> t(boost::extents[subDiv][subDiv][4]);

            for (auto i = 0; i < subDiv; ++i)
            for (auto j = 0; j < subDiv; ++j)
            {
            
            t[j][i][0] = i*i + j*j;
            t[j][i][1] = i*i * 2 + j*j * 2;
            t[j][i][2] = 30;
            }
            
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, (void*)t.data());
            onGfxDeviceErrorTriggerBreakpoint();
            
            glBindTexture(GL_TEXTURE_2D, 0);              
            */
            GfxDevice::onGfxDeviceErrorTriggerBreakpoint();
        }

        void GfxDeviceObj::drawEnd()
        {
            //glFlush();
            SwapBuffers(g_appDebug->render->gfxDevice->hDC);
        }

        void GfxDeviceObj::loadTexturesInDir(std::wstring directoryName)
        {
            FileDirectoryWalker fileWalker(directoryName, L"\\.png$");

            while (fileWalker.findNext())
            {
                debugLog << L"RenderObj::loadTexturesInDir loaded " << fileWalker.findData.cFileName << "\n";
                FilePathSplit pathBits(std::wstring(fileWalker.findData.cFileName));

                GfxDevice::TextureWrapper newt = GfxDevice::createTextureObjectFromFile(
                    directoryName, pathBits.fileName, pathBits.extensionName);

                textureManager.add(pathBits.fileName, newt);
            }
        }


        // temp forward dec
        u32 createShader(const char* sourceText, u32 type);
        u32	    createProgram(u32 vertexShader, u32 fragmentShader);

        void GfxDeviceObj::loadShadersInDir(std::wstring directoryName)
        {
            FileDirectoryWalker fileWalker(directoryName, L"\\.vsh$");

            while (fileWalker.findNext())
            {
                debugLog << L"gfxDevice::loadShadersInDir loaded " << fileWalker.findData.cFileName << "\n";
                FilePathSplit pathBits(std::wstring(fileWalker.findData.cFileName));

     //           {
     //               std::wstring vsPath = directoryName + L"/" + pathBits.fileName + L".vsh";
     //               qdev::BinaryFileLoader vs_text_temp(vsPath);
     //               int nChars = vs_text_temp.getDataLength();
     //               char *vs_text = new char[nChars + 1];
     //               memcpy(vs_text, vs_text_temp.data(), nChars);
     //               vs_text[nChars] = 0;    // text needs to be null terminted

     //               std::wstring fsPath = directoryName + L"/" + pathBits.fileName + L".fsh";
     //               qdev::BinaryFileLoader fs_text_temp(fsPath);
     //               int nCharsFs = fs_text_temp.getDataLength();
     //               char *fs_text = new char[nCharsFs + 1];
     //               memcpy(fs_text, fs_text_temp.data(), nCharsFs);
     //               fs_text[nCharsFs] = 0;  // text needs to be null terminted

					//triggerBreakpoint();
     //               u32 vs = GfxDevice::createShader(vs_text, GL_VERTEX_SHADER);
     //               u32 fs = GfxDevice::createShader(fs_text, GL_FRAGMENT_SHADER);

     //               GLuint p = GfxDevice::createProgram(vs, fs);
					//triggerBreakpoint();
     //               shaderManager.add(pathBits.fileName, GfxDevice::ShaderWrapper{});

     //               delete[] fs_text;
     //               delete[] vs_text;
     //           }
            }
        }

        void onGfxDeviceErrorTriggerBreakpoint()
        {
            /*GLenum err;
            bool stop = false;
            while ((err = glGetError()) != GL_NO_ERROR)
            {
                std::wstring s;
                switch (err)
                {
                case 0x0500: s = L"GL_INVALID_ENUM"; break;
                case 0x0501: s = L"GL_INVALID_VALUE"; break;
                case 0x0502: s = L"GL_INVALID_OPERATION"; break;
                case 0x0503: s = L"GL_STACK_OVERFLOW"; break;
                case 0x0504: s = L"GL_STACK_UNDERFLOW"; break;
                case 0x0505: s = L"GL_OUT_OF_MEMORY"; break;
                default: s = L"" + err;
                }
                debugLog << L"GL error: " << s << L"\n";
                stop = true;
            }

            if (stop == true)*/
                triggerBreakpoint();
        }
   
        void GfxDeviceObj::uploadDynamicTexture(void* p)
        {
			triggerBreakpoint();
            /*glBindTexture(GL_TEXTURE_2D, textureManager.get(L"dynamic").value);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1024, 1024, 0, GL_RGBA, GL_UNSIGNED_BYTE, p);
            onGfxDeviceErrorTriggerBreakpoint();            
            glBindTexture(GL_TEXTURE_2D, 0);            */
        }
    }//namespace GfxDevice 
}//namespace murkyFramework