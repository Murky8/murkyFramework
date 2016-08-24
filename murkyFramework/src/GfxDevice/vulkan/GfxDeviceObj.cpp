//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11. vulkan
#include <murkyFramework/src/pch.hpp>


#define ERR_EXIT(err_msg, err_class) \
    triggerBreakpoint();

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

#define GET_INSTANCE_PROC_ADDR(inst, entrypoint)                               \
    {                                                                          \
        demo->fp##entrypoint =                                                 \
            (PFN_vk##entrypoint)vkGetInstanceProcAddr(inst, "vk" #entrypoint); \
        if (demo->fp##entrypoint == NULL) triggerBreakpoint() ;\
    }


static PFN_vkGetDeviceProcAddr g_gdpa = NULL;

#define GET_DEVICE_PROC_ADDR(dev, entrypoint)                                  \
    {                                                                          \
        if (!g_gdpa)                                                           \
            g_gdpa = (PFN_vkGetDeviceProcAddr)vkGetInstanceProcAddr(           \
                demo->inst, "vkGetDeviceProcAddr");                            \
        demo->fp##entrypoint =                                                 \
            (PFN_vk##entrypoint)g_gdpa(dev, "vk" #entrypoint);                 \
        if (demo->fp##entrypoint == NULL) \
		{                                    \
			triggerBreakpoint();\
        }                                                                      \
    }

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

		struct SwapchainBuffers
		{
			VkImage image;
			VkCommandBuffer cmd;
			VkImageView view;
		};

		struct Gfxinfo
		{
			VkInstance			inst = {};
			VkDevice			device = {};
			VkPhysicalDevice	gpu = {};
			PFN_vkCreateDebugReportCallbackEXT	CreateDebugReportCallback = {};
			PFN_vkDestroyDebugReportCallbackEXT DestroyDebugReportCallback = {};
			VkDebugReportCallbackEXT			msg_callback = {};
			PFN_vkDebugReportMessageEXT			DebugReportMessage = {};
			bool								use_break = true;
			bool validate = true;
			uint32_t enabled_extension_count;
			uint32_t enabled_layer_count;
			char *extension_names[64];
			char *enabled_layers[64];
			VkPhysicalDeviceProperties gpu_props;
			uint32_t queue_count;
			VkQueueFamilyProperties *queue_props;

			PFN_vkGetPhysicalDeviceSurfaceSupportKHR
				fpGetPhysicalDeviceSurfaceSupportKHR;
			PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR
				fpGetPhysicalDeviceSurfaceCapabilitiesKHR;
			PFN_vkGetPhysicalDeviceSurfaceFormatsKHR
				fpGetPhysicalDeviceSurfaceFormatsKHR;
			PFN_vkGetPhysicalDeviceSurfacePresentModesKHR
				fpGetPhysicalDeviceSurfacePresentModesKHR;
			PFN_vkCreateSwapchainKHR fpCreateSwapchainKHR;
			PFN_vkDestroySwapchainKHR fpDestroySwapchainKHR;
			PFN_vkGetSwapchainImagesKHR fpGetSwapchainImagesKHR;
			PFN_vkAcquireNextImageKHR fpAcquireNextImageKHR;
			PFN_vkQueuePresentKHR fpQueuePresentKHR;

			VkSurfaceKHR	surface;
			bool			use_xlib = false;
			uint32_t		graphics_queue_node_index;
			VkQueue			queue;

			int width, height;
			VkFormat format;
			VkColorSpaceKHR color_space;
			VkPhysicalDeviceMemoryProperties memory_properties;

			VkCommandPool cmd_pool;
			uint32_t swapchainImageCount;
			VkSwapchainKHR swapchain;
			SwapchainBuffers *buffers;
			uint32_t current_buffer;

			struct 
			{
				VkFormat format;

				VkImage image;
				VkMemoryAllocateInfo mem_alloc;
				VkDeviceMemory mem;
				VkImageView view;
			} depth;

			VkCommandBuffer cmd; // Buffer for initialization commands
		}gfxinfo;
		
		typedef  Gfxinfo Demo;
		
		bool memory_type_from_properties(Demo *demo, uint32_t typeBits,
			VkFlags requirements_mask,
			uint32_t *typeIndex)
		{
			// Search memtypes to find first index with those properties
			for (uint32_t i = 0; i < VK_MAX_MEMORY_TYPES; i++)
			{
				if ((typeBits & 1) == 1)
				{
					// Type is available, does it match user properties?
					if ((demo->memory_properties.memoryTypes[i].propertyFlags &
						requirements_mask) == requirements_mask)
					{
						*typeIndex = i;
						return true;
					}
				}
				typeBits >>= 1;
			}
			// No memory types matched, return failure
			return false;
		}

		void demo_set_image_layout(Demo *demo, VkImage image,
			VkImageAspectFlags aspectMask,
			VkImageLayout old_image_layout,
			VkImageLayout new_image_layout,
			VkAccessFlagBits srcAccessMask)
		{
			VkResult err;

			if (demo->cmd == VK_NULL_HANDLE)
			{
				VkCommandBufferAllocateInfo cmd = {};
				cmd.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
				cmd.pNext = NULL;
				cmd.commandPool = demo->cmd_pool;
				cmd.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
				cmd.commandBufferCount = 1;

				err = vkAllocateCommandBuffers(demo->device, &cmd, &demo->cmd);
				assert(!err);
				VkCommandBufferBeginInfo cmd_buf_info = {};
				cmd_buf_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
				cmd_buf_info.pNext = NULL;
				cmd_buf_info.flags = 0;
				cmd_buf_info.pInheritanceInfo = NULL;

				err = vkBeginCommandBuffer(demo->cmd, &cmd_buf_info);
				assert(!err);
			}

			VkImageMemoryBarrier image_memory_barrier = {};
			image_memory_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			image_memory_barrier.pNext = NULL;
			image_memory_barrier.srcAccessMask = srcAccessMask;
			image_memory_barrier.dstAccessMask = 0;
			image_memory_barrier.oldLayout = old_image_layout;
			image_memory_barrier.newLayout = new_image_layout;
			image_memory_barrier.image = image;
			image_memory_barrier.subresourceRange = { aspectMask, 0, 1, 0, 1 };
			if (new_image_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
			{
				/* Make sure anything that was copying from this image has completed */
				image_memory_barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
			}

			if (new_image_layout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)
			{
				image_memory_barrier.dstAccessMask =
					VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			}

			if (new_image_layout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
			{
				image_memory_barrier.dstAccessMask =
					VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
			}

			if (new_image_layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
			{
				/* Make sure any Copy or CPU writes to image are flushed */
				image_memory_barrier.dstAccessMask =
					VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_INPUT_ATTACHMENT_READ_BIT;
			}

			VkImageMemoryBarrier *pmemory_barrier = &image_memory_barrier;

			VkPipelineStageFlags src_stages = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			VkPipelineStageFlags dest_stages = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;

			vkCmdPipelineBarrier(demo->cmd, src_stages, dest_stages, 0, 0, NULL, 0,
				NULL, 1, pmemory_barrier);
		}
		

		VKAPI_ATTR VkBool32 VKAPI_CALL
			BreakCallback(VkFlags msgFlags, VkDebugReportObjectTypeEXT objType,
				uint64_t srcObject, size_t location, int32_t msgCode,
				const char *pLayerPrefix, const char *pMsg, void *pUserData)
		{
#ifndef WIN32
			raise(SIGTRAP);
#else
			DebugBreak();
#endif

			return false;
		}


		static int validation_error = 0;
		VKAPI_ATTR VkBool32 VKAPI_CALL
			dbgFunc(VkFlags msgFlags, VkDebugReportObjectTypeEXT objType,
				uint64_t srcObject, size_t location, int32_t msgCode,
				const char *pLayerPrefix, const char *pMsg, void *pUserData)
		{
			char *message = (char *)malloc(strlen(pMsg) + 100);

			assert(message);

			validation_error = 1;

			if (msgFlags & VK_DEBUG_REPORT_ERROR_BIT_EXT)
			{
				sprintf(message, "ERROR: [%s] Code %d : %s", pLayerPrefix, msgCode,
					pMsg);
			}
			else if (msgFlags & VK_DEBUG_REPORT_WARNING_BIT_EXT)
			{
				sprintf(message, "WARNING: [%s] Code %d : %s", pLayerPrefix, msgCode,
					pMsg);
			}
			else
			{
				return false;
			}

#ifdef _WIN32
			struct demo *demo = (struct demo*) pUserData;
			//if (!demo->suppress_popups)
				//MessageBox(NULL, message, "Alert", MB_OK);
#else
			printf("%s\n", message);
			fflush(stdout);
#endif
			free(message);

			/*
			* false indicates that layer should not bail-out of an
			* API call that had validation failures. This may mean that the
			* app dies inside the driver due to invalid parameter(s).
			* That's what would happen without validation layers, so we'll
			* keep that behavior here.
			*/
			return false;
		}

		static VkBool32 demo_check_layers(uint32_t check_count, char **check_names,
			uint32_t layer_count,
			VkLayerProperties *layers)
		{
			for (uint32_t i = 0; i < check_count; i++)
			{
				VkBool32 found = 0;
				for (uint32_t j = 0; j < layer_count; j++)
				{
					if (!strcmp(check_names[i], layers[j].layerName))
					{
						found = 1;
						break;
					}
				}
				if (!found)
				{
					fprintf(stderr, "Cannot find layer: %s\n", check_names[i]);
					return 0;
				}
			}
			return 1;
		}

		void demo_prepare_buffers(Demo *demo)
		{
			VkResult err;
			VkSwapchainKHR oldSwapchain = demo->swapchain;

			// Check the surface capabilities and formats
			VkSurfaceCapabilitiesKHR surfCapabilities;
			err = demo->fpGetPhysicalDeviceSurfaceCapabilitiesKHR(
				demo->gpu, demo->surface, &surfCapabilities);
			assert(!err);

			uint32_t presentModeCount;
			err = demo->fpGetPhysicalDeviceSurfacePresentModesKHR(
				demo->gpu, demo->surface, &presentModeCount, NULL);
			assert(!err);
			VkPresentModeKHR *presentModes =
				(VkPresentModeKHR *)malloc(presentModeCount * sizeof(VkPresentModeKHR));
			assert(presentModes);
			err = demo->fpGetPhysicalDeviceSurfacePresentModesKHR(
				demo->gpu, demo->surface, &presentModeCount, presentModes);
			assert(!err);

			VkExtent2D swapchainExtent;
			// width and height are either both -1, or both not -1.
			if (surfCapabilities.currentExtent.width == (uint32_t)-1)
			{
				// If the surface size is undefined, the size is set to
				// the size of the images requested.
				swapchainExtent.width = demo->width;
				swapchainExtent.height = demo->height;
			}
			else
			{
				// If the surface size is defined, the swap chain size must match
				swapchainExtent = surfCapabilities.currentExtent;
				demo->width = surfCapabilities.currentExtent.width;
				demo->height = surfCapabilities.currentExtent.height;
			}

			// If mailbox mode is available, use it, as is the lowest-latency non-
			// tearing mode.  If not, try IMMEDIATE which will usually be available,
			// and is fastest (though it tears).  If not, fall back to FIFO which is
			// always available.
			VkPresentModeKHR swapchainPresentMode = VK_PRESENT_MODE_FIFO_KHR;
			for (size_t i = 0; i < presentModeCount; i++)
			{
				if (presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
				{
					swapchainPresentMode = VK_PRESENT_MODE_MAILBOX_KHR;
					break;
				}
				if ((swapchainPresentMode != VK_PRESENT_MODE_MAILBOX_KHR) &&
					(presentModes[i] == VK_PRESENT_MODE_IMMEDIATE_KHR))
				{
					swapchainPresentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
				}
			}

			// Determine the number of VkImage's to use in the swap chain (we desire to
			// own only 1 image at a time, besides the images being displayed and
			// queued for display):
			uint32_t desiredNumberOfSwapchainImages =
				surfCapabilities.minImageCount + 1;
			if ((surfCapabilities.maxImageCount > 0) &&
				(desiredNumberOfSwapchainImages > surfCapabilities.maxImageCount))
			{
				// Application must settle for fewer images than desired:
				desiredNumberOfSwapchainImages = surfCapabilities.maxImageCount;
			}

			VkSurfaceTransformFlagsKHR preTransform;
			if (surfCapabilities.supportedTransforms &
				VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
			{
				preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
			}
			else
			{
				preTransform = surfCapabilities.currentTransform;
			}

			VkSwapchainCreateInfoKHR swapchain = {};
			swapchain.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
			swapchain.pNext = NULL,
			swapchain.surface = demo->surface,
			swapchain.minImageCount = desiredNumberOfSwapchainImages,
			swapchain.imageFormat = demo->format,
			swapchain.imageColorSpace = demo->color_space,
			swapchain.imageExtent = {};
			swapchain.imageExtent.width = swapchainExtent.width;
			swapchain.imageExtent.height = swapchainExtent.height;
			swapchain.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
			swapchain.preTransform = (VkSurfaceTransformFlagBitsKHR)preTransform; //NOTE: check (24/8/2016)
			swapchain.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
			swapchain.imageArrayLayers = 1;
			swapchain.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			swapchain.queueFamilyIndexCount = 0;
			swapchain.pQueueFamilyIndices = NULL;
			swapchain.presentMode = swapchainPresentMode;
			swapchain.oldSwapchain = oldSwapchain;
			swapchain.clipped = true;
			
			uint32_t i;

			err = demo->fpCreateSwapchainKHR(demo->device, &swapchain, NULL,
				&demo->swapchain);
			assert(!err);

			// If we just re-created an existing swapchain, we should destroy the old
			// swapchain at this point.
			// Note: destroying the swapchain also cleans up all its associated
			// presentable images once the platform is done with them.
			if (oldSwapchain != VK_NULL_HANDLE)
			{
				demo->fpDestroySwapchainKHR(demo->device, oldSwapchain, NULL);
			}

			err = demo->fpGetSwapchainImagesKHR(demo->device, demo->swapchain,
				&demo->swapchainImageCount, NULL);
			assert(!err);

			VkImage *swapchainImages =
				(VkImage *)malloc(demo->swapchainImageCount * sizeof(VkImage));
			assert(swapchainImages);
			err = demo->fpGetSwapchainImagesKHR(demo->device, demo->swapchain,
				&demo->swapchainImageCount,
				swapchainImages);
			assert(!err);

			demo->buffers = (SwapchainBuffers *)malloc(sizeof(SwapchainBuffers) *
				demo->swapchainImageCount);
			assert(demo->buffers);

			for (i = 0; i < demo->swapchainImageCount; i++)
			{
				VkImageViewCreateInfo color_image_view = {};
				color_image_view.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
				color_image_view.pNext = NULL,
				color_image_view.format = demo->format,
				
				color_image_view.components = {};
				color_image_view.components.r = VK_COMPONENT_SWIZZLE_R;
				color_image_view.components.g = VK_COMPONENT_SWIZZLE_G;
				color_image_view.components.b = VK_COMPONENT_SWIZZLE_B;
				color_image_view.components.a = VK_COMPONENT_SWIZZLE_A;

				color_image_view.subresourceRange = {};
				color_image_view.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				color_image_view.subresourceRange.baseMipLevel = 0;
				color_image_view.subresourceRange.levelCount = 1;
				color_image_view.subresourceRange.baseArrayLayer = 0;
				color_image_view.subresourceRange.layerCount = 1;
				color_image_view.viewType = VK_IMAGE_VIEW_TYPE_2D;
				color_image_view.flags = 0;
				
				demo->buffers[i].image = swapchainImages[i];

				color_image_view.image = demo->buffers[i].image;

				err = vkCreateImageView(demo->device, &color_image_view, NULL,
					&demo->buffers[i].view);
				assert(!err);
			}
			
			if (NULL != presentModes)
			{
				free(presentModes);
			}
		}

		void demo_prepare_depth(Demo *demo)
		{
			const VkFormat depth_format = VK_FORMAT_D16_UNORM;
			VkImageCreateInfo image = {};
			image.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
			image.pNext = NULL;
			image.imageType = VK_IMAGE_TYPE_2D;
			image.format = depth_format;
			image.extent = {};
			image.extent.width = demo->width;
			image.extent.height = demo->height;
			image.extent.depth = 1;
		
			image.mipLevels = 1;
			image.arrayLayers = 1;
			image.samples = VK_SAMPLE_COUNT_1_BIT;
			image.tiling = VK_IMAGE_TILING_OPTIMAL;
			image.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
			image.flags = 0;
			
			VkImageViewCreateInfo view = {};
			view.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			view.pNext = NULL;
			view.image = VK_NULL_HANDLE;
			view.format = depth_format;
			view.subresourceRange = {};
			view.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
			view.subresourceRange.baseMipLevel = 0;
			view.subresourceRange.levelCount = 1;
			view.subresourceRange.baseArrayLayer = 0;
			view.subresourceRange.layerCount = 1;
			view.flags = 0;
			view.viewType = VK_IMAGE_VIEW_TYPE_2D;
			
			VkMemoryRequirements mem_reqs;
			VkResult err;
			bool pass;

			demo->depth.format = depth_format;

			/* create image */
			err = vkCreateImage(demo->device, &image, NULL, &demo->depth.image);
			assert(!err);

			vkGetImageMemoryRequirements(demo->device, demo->depth.image, &mem_reqs);
			assert(!err);

			demo->depth.mem_alloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			demo->depth.mem_alloc.pNext = NULL;
			demo->depth.mem_alloc.allocationSize = mem_reqs.size;
			demo->depth.mem_alloc.memoryTypeIndex = 0;

			pass = memory_type_from_properties(demo, mem_reqs.memoryTypeBits,
				0, /* No requirements */
				&demo->depth.mem_alloc.memoryTypeIndex);
			assert(pass);

			/* allocate memory */
			err = vkAllocateMemory(demo->device, &demo->depth.mem_alloc, NULL,
				&demo->depth.mem);
			assert(!err);

			/* bind memory */
			err =
				vkBindImageMemory(demo->device, demo->depth.image, demo->depth.mem, 0);
			assert(!err);

			demo_set_image_layout(demo, demo->depth.image, VK_IMAGE_ASPECT_DEPTH_BIT,
				VK_IMAGE_LAYOUT_UNDEFINED,
				VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
				(VkAccessFlagBits)0);

			/* create image view */
			view.image = demo->depth.image;
			err = vkCreateImageView(demo->device, &view, NULL, &demo->depth.view);
			assert(!err);
		}


		GfxDeviceObj::GfxDeviceObj(GfxDeviceObj_initStruct *const initStruct) :
			hDC(initStruct->windowsSpecific->gethDC())
		{
			g_appDebug->render->gfxDevice = this; // warning: see g_aapDebug usage notes: for development only, remove!
			systemSpecific::WindowsSpecific *const windowsSpecific = initStruct->windowsSpecific;

			VkResult err;
			VkResult res;
			Gfxinfo* demo = &gfxinfo;
			bool enableValidation = false;
			uint32_t instance_extension_count = 0;
			uint32_t instance_layer_count = 0;
			uint32_t validation_layer_count = 0;
			char **instance_validation_layers = NULL;
			gfxinfo.enabled_extension_count = 0;
			gfxinfo.enabled_layer_count = 0;

			char *instance_validation_layers_alt1[] = {
				"VK_LAYER_LUNARG_standard_validation"
			};

			char *instance_validation_layers_alt2[] = {
				"VK_LAYER_GOOGLE_threading",       "VK_LAYER_LUNARG_parameter_validation",
				"VK_LAYER_LUNARG_object_tracker",  "VK_LAYER_LUNARG_image",
				"VK_LAYER_LUNARG_core_validation", "VK_LAYER_LUNARG_swapchain",
				"VK_LAYER_GOOGLE_unique_objects"
			};

			/* Look for validation layers */
			VkBool32 validation_found = 0;
			if (demo->validate)
			{
				err = vkEnumerateInstanceLayerProperties(&instance_layer_count, NULL);
				assert(!err);

				instance_validation_layers = instance_validation_layers_alt1;
				if (instance_layer_count > 0)
				{
					VkLayerProperties *instance_layers = (VkLayerProperties *)
						malloc(sizeof(VkLayerProperties) * instance_layer_count);
					err = vkEnumerateInstanceLayerProperties(&instance_layer_count,
						instance_layers);
					assert(!err);


					validation_found = demo_check_layers(
						ARRAY_SIZE(instance_validation_layers_alt1),
						instance_validation_layers, instance_layer_count,
						instance_layers);
					if (validation_found)
					{
						demo->enabled_layer_count = ARRAY_SIZE(instance_validation_layers_alt1);
						demo->enabled_layers[0] = "VK_LAYER_LUNARG_standard_validation";
						validation_layer_count = 1;
					}
					else
					{
						// use alternative set of validation layers
						instance_validation_layers = instance_validation_layers_alt2;
						demo->enabled_layer_count = ARRAY_SIZE(instance_validation_layers_alt2);
						validation_found = demo_check_layers(
							ARRAY_SIZE(instance_validation_layers_alt2),
							instance_validation_layers, instance_layer_count,
							instance_layers);
						validation_layer_count =
							ARRAY_SIZE(instance_validation_layers_alt2);
						for (uint32_t i = 0; i < validation_layer_count; i++)
						{
							demo->enabled_layers[i] = instance_validation_layers[i];
						}
					}
					free(instance_layers);
				}

				if (!validation_found)
				{
					ERR_EXIT("vkEnumerateInstanceLayerProperties failed to find "
						"required validation layer.\n\n"
						"Please look at the Getting Started guide for additional "
						"information.\n",
						"vkCreateInstance Failure");
				}
			}

			/* Look for instance extensions */
			VkBool32 surfaceExtFound = 0;
			VkBool32 platformSurfaceExtFound = 0;

			memset(demo->extension_names, 0, sizeof(demo->extension_names));

			err = vkEnumerateInstanceExtensionProperties(
				NULL, &instance_extension_count, NULL);
			assert(!err);

			if (instance_extension_count > 0)
			{
				VkExtensionProperties *instance_extensions = (VkExtensionProperties*)
					malloc(sizeof(VkExtensionProperties) * instance_extension_count);
				err = vkEnumerateInstanceExtensionProperties(
					NULL, &instance_extension_count, instance_extensions);
				assert(!err);
				for (uint32_t i = 0; i < instance_extension_count; i++)
				{
					if (!strcmp(VK_KHR_SURFACE_EXTENSION_NAME,
						instance_extensions[i].extensionName))
					{
						surfaceExtFound = 1;
						demo->extension_names[demo->enabled_extension_count++] =
							VK_KHR_SURFACE_EXTENSION_NAME;
					}
#if defined(VK_USE_PLATFORM_WIN32_KHR)
					if (!strcmp(VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
						instance_extensions[i].extensionName))
					{
						platformSurfaceExtFound = 1;
						demo->extension_names[demo->enabled_extension_count++] =
							VK_KHR_WIN32_SURFACE_EXTENSION_NAME;
					}
#endif
#if defined(VK_USE_PLATFORM_XLIB_KHR)
					if (!strcmp(VK_KHR_XLIB_SURFACE_EXTENSION_NAME,
						instance_extensions[i].extensionName))
					{
						platformSurfaceExtFound = 1;
						xlibSurfaceExtFound = 1;
						demo->extension_names[demo->enabled_extension_count++] =
							VK_KHR_XLIB_SURFACE_EXTENSION_NAME;
					}
#endif
#if defined(VK_USE_PLATFORM_XCB_KHR)
					if (!strcmp(VK_KHR_XCB_SURFACE_EXTENSION_NAME,
						instance_extensions[i].extensionName))
					{
						platformSurfaceExtFound = 1;
						demo->extension_names[demo->enabled_extension_count++] =
							VK_KHR_XCB_SURFACE_EXTENSION_NAME;
					}
#endif
#if defined(VK_USE_PLATFORM_WAYLAND_KHR)
					if (!strcmp(VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME,
						instance_extensions[i].extensionName))
					{
						platformSurfaceExtFound = 1;
						demo->extension_names[demo->enabled_extension_count++] =
							VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME;
					}
#endif
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
					if (!strcmp(VK_KHR_ANDROID_SURFACE_EXTENSION_NAME,
						instance_extensions[i].extensionName))
					{
						platformSurfaceExtFound = 1;
						demo->extension_names[demo->enabled_extension_count++] =
							VK_KHR_ANDROID_SURFACE_EXTENSION_NAME;
					}
#endif
					if (!strcmp(VK_EXT_DEBUG_REPORT_EXTENSION_NAME,
						instance_extensions[i].extensionName))
					{
						if (demo->validate)
						{
							demo->extension_names[demo->enabled_extension_count++] =
								VK_EXT_DEBUG_REPORT_EXTENSION_NAME;
						}
					}
					assert(demo->enabled_extension_count < 64);
				}

				free(instance_extensions);
			}

			if (!surfaceExtFound)
			{
				ERR_EXIT("vkEnumerateInstanceExtensionProperties failed to find "
					"the " VK_KHR_SURFACE_EXTENSION_NAME
					" extension.\n\nDo you have a compatible "
					"Vulkan installable client driver (ICD) installed?\nPlease "
					"look at the Getting Started guide for additional "
					"information.\n",
					"vkCreateInstance Failure");
			}
			if (!platformSurfaceExtFound)
			{
#if defined(VK_USE_PLATFORM_WIN32_KHR)
				ERR_EXIT("vkEnumerateInstanceExtensionProperties failed to find "
					"the " VK_KHR_WIN32_SURFACE_EXTENSION_NAME
					" extension.\n\nDo you have a compatible "
					"Vulkan installable client driver (ICD) installed?\nPlease "
					"look at the Getting Started guide for additional "
					"information.\n",
					"vkCreateInstance Failure");
#elif defined(VK_USE_PLATFORM_XCB_KHR)
				ERR_EXIT("vkEnumerateInstanceExtensionProperties failed to find "
					"the " VK_KHR_XCB_SURFACE_EXTENSION_NAME
					" extension.\n\nDo you have a compatible "
					"Vulkan installable client driver (ICD) installed?\nPlease "
					"look at the Getting Started guide for additional "
					"information.\n",
					"vkCreateInstance Failure");
#elif defined(VK_USE_PLATFORM_WAYLAND_KHR)
				ERR_EXIT("vkEnumerateInstanceExtensionProperties failed to find "
					"the " VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME
					" extension.\n\nDo you have a compatible "
					"Vulkan installable client driver (ICD) installed?\nPlease "
					"look at the Getting Started guide for additional "
					"information.\n",
					"vkCreateInstance Failure");
#elif defined(VK_USE_PLATFORM_ANDROID_KHR)
				ERR_EXIT("vkEnumerateInstanceExtensionProperties failed to find "
					"the " VK_KHR_ANDROID_SURFACE_EXTENSION_NAME
					" extension.\n\nDo you have a compatible "
					"Vulkan installable client driver (ICD) installed?\nPlease "
					"look at the Getting Started guide for additional "
					"information.\n",
					"vkCreateInstance Failure");
#endif
			}
#if defined(VK_USE_PLATFORM_XLIB_KHR)
			if (demo->use_xlib && !xlibSurfaceExtFound)
			{
				ERR_EXIT("vkEnumerateInstanceExtensionProperties failed to find "
					"the " VK_KHR_XLIB_SURFACE_EXTENSION_NAME
					" extension.\n\nDo you have a compatible "
					"Vulkan installable client driver (ICD) installed?\nPlease "
					"look at the Getting Started guide for additional "
					"information.\n",
					"vkCreateInstance Failure");
			}
#endif
			VkApplicationInfo app = {};
			app.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
			app.pNext = NULL;
			app.pApplicationName = "vulkan murky";
			app.applicationVersion = VK_MAKE_VERSION(1, 0, 0);;
			app.pEngineName = "vulkan murky";
			app.engineVersion = VK_MAKE_VERSION(1, 0, 0);;
			app.apiVersion = VK_MAKE_VERSION(1, 0, 0);;
			
			VkInstanceCreateInfo inst_info = {};
			inst_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			inst_info.pNext = NULL;
			inst_info.pApplicationInfo = &app;
			inst_info.enabledLayerCount = demo->enabled_layer_count;
			inst_info.ppEnabledLayerNames = (const char *const *)instance_validation_layers;
			inst_info.enabledExtensionCount = demo->enabled_extension_count;
			inst_info.ppEnabledExtensionNames = (const char *const *)demo->extension_names;
							
			/*
			* This is info for a temp callback to use during CreateInstance.
			* After the instance is created, we use the instance-based
			* function to register the final callback.
			*/
			VkDebugReportCallbackCreateInfoEXT dbgCreateInfo;
			if (demo->validate)
			{
				dbgCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
				dbgCreateInfo.pNext = NULL;
				dbgCreateInfo.pfnCallback = demo->use_break ? BreakCallback : dbgFunc;
				dbgCreateInfo.pUserData = demo;
				dbgCreateInfo.flags =
					VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
				inst_info.pNext = &dbgCreateInfo;
			}
			
			uint32_t gpu_count;

			err = vkCreateInstance(&inst_info, NULL, &demo->inst);
			if (err == VK_ERROR_INCOMPATIBLE_DRIVER)
			{
				ERR_EXIT("Cannot find a compatible Vulkan installable client driver "
					"(ICD).\n\nPlease look at the Getting Started guide for "
					"additional information.\n",
					"vkCreateInstance Failure");
			}
			else if (err == VK_ERROR_EXTENSION_NOT_PRESENT)
			{
				ERR_EXIT("Cannot find a specified extension library"
					".\nMake sure your layers path is set appropriately.\n",
					"vkCreateInstance Failure");
			}
			else if (err)
			{
				ERR_EXIT("vkCreateInstance failed.\n\nDo you have a compatible Vulkan "
					"installable client driver (ICD) installed?\nPlease look at "
					"the Getting Started guide for additional information.\n",
					"vkCreateInstance Failure");
			}

			/* Make initial call to query gpu_count, then second call for gpu info*/
			err = vkEnumeratePhysicalDevices(demo->inst, &gpu_count, NULL);
			assert(!err && gpu_count > 0);

			if (gpu_count > 0)
			{
				VkPhysicalDevice *physical_devices = (VkPhysicalDevice*)malloc(sizeof(VkPhysicalDevice) * gpu_count);
				err = vkEnumeratePhysicalDevices(demo->inst, &gpu_count, physical_devices);
				assert(!err);
				/* For cube demo we just grab the first physical device */
				demo->gpu = physical_devices[0];
				free(physical_devices);
			}
			else
			{
				ERR_EXIT("vkEnumeratePhysicalDevices reported zero accessible devices.\n\n"
					"Do you have a compatible Vulkan installable client driver (ICD) "
					"installed?\nPlease look at the Getting Started guide for "
					"additional information.\n",
					"vkEnumeratePhysicalDevices Failure");
			}

			/* Look for device extensions */
			uint32_t device_extension_count = 0;
			VkBool32 swapchainExtFound = 0;
			demo->enabled_extension_count = 0;
			memset(demo->extension_names, 0, sizeof(demo->extension_names));

			err = vkEnumerateDeviceExtensionProperties(demo->gpu, NULL,
				&device_extension_count, NULL);
			assert(!err);

			if (device_extension_count > 0)
			{
				VkExtensionProperties *device_extensions =
					(VkExtensionProperties*)malloc(sizeof(VkExtensionProperties) * device_extension_count);
				err = vkEnumerateDeviceExtensionProperties(
					demo->gpu, NULL, &device_extension_count, device_extensions);
				assert(!err);

				for (uint32_t i = 0; i < device_extension_count; i++)
				{
					if (!strcmp(VK_KHR_SWAPCHAIN_EXTENSION_NAME,
						device_extensions[i].extensionName))
					{
						swapchainExtFound = 1;
						demo->extension_names[demo->enabled_extension_count++] =
							VK_KHR_SWAPCHAIN_EXTENSION_NAME;
					}
					assert(demo->enabled_extension_count < 64);
				}

				free(device_extensions);
			}

			if (!swapchainExtFound)
			{
				ERR_EXIT("vkEnumerateDeviceExtensionProperties failed to find "
					"the " VK_KHR_SWAPCHAIN_EXTENSION_NAME
					" extension.\n\nDo you have a compatible "
					"Vulkan installable client driver (ICD) installed?\nPlease "
					"look at the Getting Started guide for additional "
					"information.\n",
					"vkCreateInstance Failure");
			}

			if (demo->validate)
			{
				demo->CreateDebugReportCallback =
					(PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(
						demo->inst, "vkCreateDebugReportCallbackEXT");
				demo->DestroyDebugReportCallback =
					(PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(
						demo->inst, "vkDestroyDebugReportCallbackEXT");
				if (!demo->CreateDebugReportCallback)
				{
					ERR_EXIT(
						"GetProcAddr: Unable to find vkCreateDebugReportCallbackEXT\n",
						"vkGetProcAddr Failure");
				}
				if (!demo->DestroyDebugReportCallback)
				{
					ERR_EXIT(
						"GetProcAddr: Unable to find vkDestroyDebugReportCallbackEXT\n",
						"vkGetProcAddr Failure");
				}
				demo->DebugReportMessage =
					(PFN_vkDebugReportMessageEXT)vkGetInstanceProcAddr(
						demo->inst, "vkDebugReportMessageEXT");
				if (!demo->DebugReportMessage)
				{
					ERR_EXIT("GetProcAddr: Unable to find vkDebugReportMessageEXT\n",
						"vkGetProcAddr Failure");
				}

				VkDebugReportCallbackCreateInfoEXT dbgCreateInfo;
				PFN_vkDebugReportCallbackEXT callback;
				callback = demo->use_break ? BreakCallback : dbgFunc;
				dbgCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
				dbgCreateInfo.pNext = NULL;
				dbgCreateInfo.pfnCallback = callback;
				dbgCreateInfo.pUserData = demo;
				dbgCreateInfo.flags =
					VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
				err = demo->CreateDebugReportCallback(demo->inst, &dbgCreateInfo, NULL,
					&demo->msg_callback);
				switch (err)
				{
				case VK_SUCCESS:
					break;
				case VK_ERROR_OUT_OF_HOST_MEMORY:
					ERR_EXIT("CreateDebugReportCallback: out of host memory\n",
						"CreateDebugReportCallback Failure");
					break;
				default:
					ERR_EXIT("CreateDebugReportCallback: unknown failure\n",
						"CreateDebugReportCallback Failure");
					break;
				}
			}
			vkGetPhysicalDeviceProperties(demo->gpu, &demo->gpu_props);

			/* Call with NULL data to get count */
			vkGetPhysicalDeviceQueueFamilyProperties(demo->gpu, &demo->queue_count,
				NULL);
			assert(demo->queue_count >= 1);

			demo->queue_props = (VkQueueFamilyProperties *)malloc(
				demo->queue_count * sizeof(VkQueueFamilyProperties));
			vkGetPhysicalDeviceQueueFamilyProperties(demo->gpu, &demo->queue_count,
				demo->queue_props);
			// Find a queue that supports gfx
			uint32_t gfx_queue_idx = 0;
			for (gfx_queue_idx = 0; gfx_queue_idx < demo->queue_count;
				gfx_queue_idx++)
			{
				if (demo->queue_props[gfx_queue_idx].queueFlags & VK_QUEUE_GRAPHICS_BIT)
					break;
			}
			assert(gfx_queue_idx < demo->queue_count);
			// Query fine-grained feature support for this device.
			//  If app has specific feature requirements it should check supported
			//  features based on this query
			VkPhysicalDeviceFeatures physDevFeatures;
			vkGetPhysicalDeviceFeatures(demo->gpu, &physDevFeatures);

			GET_INSTANCE_PROC_ADDR(demo->inst, GetPhysicalDeviceSurfaceSupportKHR);
			GET_INSTANCE_PROC_ADDR(demo->inst, GetPhysicalDeviceSurfaceCapabilitiesKHR);
			GET_INSTANCE_PROC_ADDR(demo->inst, GetPhysicalDeviceSurfaceFormatsKHR);
			GET_INSTANCE_PROC_ADDR(demo->inst, GetPhysicalDeviceSurfacePresentModesKHR);
			GET_INSTANCE_PROC_ADDR(demo->inst, GetSwapchainImagesKHR);
			// end of cube.c/demo_init_vk()

			// start of cube.c/demo_create_window()
			// done elsewheerre
			// end of cube.c/demo_create_window()

			// start of cube.c/demo_init_vk_swapchain(&demo);
			uint32_t i;

			// Create a WSI surface for the window:
#if defined(VK_USE_PLATFORM_WIN32_KHR)
			VkWin32SurfaceCreateInfoKHR createInfo;
			createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
			createInfo.pNext = NULL;
			createInfo.flags = 0;
			createInfo.hinstance = initStruct->windowsSpecific->gethInstance();
			createInfo.hwnd = initStruct->windowsSpecific->gethWnd();

			err =
				vkCreateWin32SurfaceKHR(demo->inst, &createInfo, NULL, &demo->surface);
#elif defined(VK_USE_PLATFORM_WAYLAND_KHR) && !defined(VK_USE_PLATFORM_XCB_KHR)
			VkWaylandSurfaceCreateInfoKHR createInfo;
			createInfo.sType = VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR;
			createInfo.pNext = NULL;
			createInfo.flags = 0;
			createInfo.display = demo->display;
			createInfo.surface = demo->window;

			err = vkCreateWaylandSurfaceKHR(demo->inst, &createInfo, NULL,
				&demo->surface);
#elif defined(VK_USE_PLATFORM_ANDROID_KHR)
			VkAndroidSurfaceCreateInfoKHR createInfo;
			createInfo.sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR;
			createInfo.pNext = NULL;
			createInfo.flags = 0;
			createInfo.window = (ANativeWindow*)(demo->window);

			err = vkCreateAndroidSurfaceKHR(demo->inst, &createInfo, NULL, &demo->surface);
#endif
			if (demo->use_xlib)
			{
#if defined(VK_USE_PLATFORM_XLIB_KHR)
				VkXlibSurfaceCreateInfoKHR createInfo;
				createInfo.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
				createInfo.pNext = NULL;
				createInfo.flags = 0;
				createInfo.dpy = demo->display;
				createInfo.window = demo->xlib_window;

				err = vkCreateXlibSurfaceKHR(demo->inst, &createInfo, NULL,
					&demo->surface);
#endif
			}
			else
			{
#if defined(VK_USE_PLATFORM_XCB_KHR)
				VkXcbSurfaceCreateInfoKHR createInfo;
				createInfo.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
				createInfo.pNext = NULL;
				createInfo.flags = 0;
				createInfo.connection = demo->connection;
				createInfo.window = demo->xcb_window;

				err = vkCreateXcbSurfaceKHR(demo->inst, &createInfo, NULL, &demo->surface);
#endif
			}
			assert(!err);

			// Iterate over each queue to learn whether it supports presenting:
			VkBool32 *supportsPresent =
				(VkBool32 *)malloc(demo->queue_count * sizeof(VkBool32));
			for (i = 0; i < demo->queue_count; i++)
			{
				demo->fpGetPhysicalDeviceSurfaceSupportKHR(demo->gpu, i, demo->surface,
					&supportsPresent[i]);
			}

			// Search for a graphics and a present queue in the array of queue
			// families, try to find one that supports both
			uint32_t graphicsQueueNodeIndex = UINT32_MAX;
			uint32_t presentQueueNodeIndex = UINT32_MAX;
			for (i = 0; i < demo->queue_count; i++)
			{
				if ((demo->queue_props[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0)
				{
					if (graphicsQueueNodeIndex == UINT32_MAX)
					{
						graphicsQueueNodeIndex = i;
					}

					if (supportsPresent[i] == VK_TRUE)
					{
						graphicsQueueNodeIndex = i;
						presentQueueNodeIndex = i;
						break;
					}
				}
			}
			if (presentQueueNodeIndex == UINT32_MAX)
			{
				// If didn't find a queue that supports both graphics and present, then
				// find a separate present queue.
				for (uint32_t i = 0; i < demo->queue_count; ++i)
				{
					if (supportsPresent[i] == VK_TRUE)
					{
						presentQueueNodeIndex = i;
						break;
					}
				}
			}
			free(supportsPresent);

			// Generate error if could not find both a graphics and a present queue
			if (graphicsQueueNodeIndex == UINT32_MAX ||
				presentQueueNodeIndex == UINT32_MAX)
			{
				ERR_EXIT("Could not find a graphics and a present queue\n",
					"Swapchain Initialization Failure");
			}

			// TODO: Add support for separate queues, including presentation,
			//       synchronization, and appropriate tracking for QueueSubmit.
			// NOTE: While it is possible for an application to use a separate graphics
			//       and a present queues, this demo program assumes it is only using
			//       one:
			if (graphicsQueueNodeIndex != presentQueueNodeIndex)
			{
				ERR_EXIT("Could not find a common graphics and a present queue\n",
					"Swapchain Initialization Failure");
			}

			demo->graphics_queue_node_index = graphicsQueueNodeIndex;
			
			//start of cube.c/demo_create_device(demo);
			float queue_priorities[1] = { 0.0 };
			VkDeviceQueueCreateInfo queue = {};
			queue.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queue.pNext = NULL;
			queue.queueFamilyIndex = demo->graphics_queue_node_index;
			queue.queueCount = 1;
			queue.pQueuePriorities = queue_priorities;

			VkDeviceCreateInfo deviceinfo = {};
			deviceinfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
			deviceinfo.pNext = NULL;
			deviceinfo.queueCreateInfoCount = 1;
			deviceinfo.pQueueCreateInfos = &queue;
			deviceinfo.enabledLayerCount = 0;
			deviceinfo.ppEnabledLayerNames = NULL;
			deviceinfo.enabledExtensionCount = demo->enabled_extension_count;
			deviceinfo.ppEnabledExtensionNames = (const char *const *)demo->extension_names;
			deviceinfo.pEnabledFeatures = NULL; // If specific features are required, pass them in here

			err = vkCreateDevice(demo->gpu, &deviceinfo, NULL, &demo->device);
			assert(!err);			
			//end of cube.c/demo_create_device(demo);

			GET_DEVICE_PROC_ADDR(demo->device, CreateSwapchainKHR);
			GET_DEVICE_PROC_ADDR(demo->device, DestroySwapchainKHR);
			GET_DEVICE_PROC_ADDR(demo->device, GetSwapchainImagesKHR);
			GET_DEVICE_PROC_ADDR(demo->device, AcquireNextImageKHR);
			GET_DEVICE_PROC_ADDR(demo->device, QueuePresentKHR);

			vkGetDeviceQueue(demo->device, demo->graphics_queue_node_index, 0,
				&demo->queue);

			// Get the list of VkFormat's that are supported:
			uint32_t formatCount;
			err = demo->fpGetPhysicalDeviceSurfaceFormatsKHR(demo->gpu, demo->surface,
				&formatCount, NULL);
			assert(!err);
			VkSurfaceFormatKHR *surfFormats =
				(VkSurfaceFormatKHR *)malloc(formatCount * sizeof(VkSurfaceFormatKHR));
			err = demo->fpGetPhysicalDeviceSurfaceFormatsKHR(demo->gpu, demo->surface,
				&formatCount, surfFormats);
			assert(!err);
			// If the format list includes just one entry of VK_FORMAT_UNDEFINED,
			// the surface has no preferred format.  Otherwise, at least one
			// supported format will be returned.
			if (formatCount == 1 && surfFormats[0].format == VK_FORMAT_UNDEFINED)
			{
				demo->format = VK_FORMAT_B8G8R8A8_UNORM;
			}
			else
			{
				assert(formatCount >= 1);
				demo->format = surfFormats[0].format;
			}
			demo->color_space = surfFormats[0].colorSpace;

			//demo->quit = false;
			//demo->curFrame = 0;

			// Get Memory information and properties
			vkGetPhysicalDeviceMemoryProperties(demo->gpu, &demo->memory_properties);
			// end of cube.c/demo_init_vk_swapchain(&demo);

//crt st		// start of cube.c/demo_prepare(&demo);
			VkCommandPoolCreateInfo cmd_pool_info = {};
			cmd_pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
			cmd_pool_info.pNext = NULL;
			cmd_pool_info.queueFamilyIndex = demo->graphics_queue_node_index;
			cmd_pool_info.flags = 0;
			
			err = vkCreateCommandPool(demo->device, &cmd_pool_info, NULL,
				&demo->cmd_pool);
			assert(!err);

			VkCommandBufferAllocateInfo cmd = {};
			cmd.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			cmd.pNext = NULL;
			cmd.commandPool = demo->cmd_pool;
			cmd.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
			cmd.commandBufferCount = 1;
			
			demo_prepare_buffers(demo);
			demo_prepare_depth(demo);
			//crt
			/*
			demo_prepare_textures(demo);
			demo_prepare_cube_data_buffer(demo);

			demo_prepare_descriptor_layout(demo);
			demo_prepare_render_pass(demo);
			demo_prepare_pipeline(demo);

			for (uint32_t i = 0; i < demo->swapchainImageCount; i++)
			{
				err =
					vkAllocateCommandBuffers(demo->device, &cmd, &demo->buffers[i].cmd);
				assert(!err);
			}

			demo_prepare_descriptor_pool(demo);
			demo_prepare_descriptor_set(demo);

			demo_prepare_framebuffers(demo);

			for (uint32_t i = 0; i < demo->swapchainImageCount; i++)
			{
				demo->current_buffer = i;
				demo_draw_build_cmd(demo, demo->buffers[i].cmd);
			}

			
			// Prepare functions above may generate pipeline commands
			// that need to be flushed before beginning the render loop.
			
			demo_flush_init_cmd(demo);

			demo->current_buffer = 0;
			demo->prepared = true;
			*/
//crt end			// end of cube.c/demo_prepare(&demo);
				
			// new stuff^
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