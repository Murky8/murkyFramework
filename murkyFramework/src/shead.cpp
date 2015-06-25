
#include <windows.h>
#include <iostream>
#include <vector>
#include <thread>

#include <glew/include/GL/glew.h> 
#include <glew/include/GL/wglew.h>

#include <murkyFramework/include/debugUtils.hpp>
#include <murkyFramework/include/types.hpp>
#include <murkyFramework/include/debugUtils.hpp>
#include <murkyFramework/include/loadSaveFile.hpp>
#include <murkyFramework/include/system.hpp>
#include <murkyFramework/include/gfxLowLevel/gfxPrimativeTypes.hpp>
#include <murkyFramework/include/gfxLowLevel/gfxLowLevel.hpp>
#include <murkyFramework/include/appFramework.hpp>


namespace
{
	HDC			hDC;		// Private GDI Device Context
	HGLRC		hRC;		// Permanent Rendering Context
	HWND		hWnd;	    // Holds Our Window Handle
	HINSTANCE	hInstance;	// Holds The Instance Of The Application
	u64         frameStartTime = 0;
	bool        wndProcCalled = false;
}

void mainLoop();
void init();
bool createWindow(LPCWSTR title, int width, int height);


int WINAPI WinMain(HINSTANCE	hInstance,			// Instance
	HINSTANCE	hPrevInstance,		// Previous Instance
	LPSTR		lpCmdLine,			// Command Line Parameters
	int			nCmdShow)			// Window Show State
{
	MSG		msg;
	init();

	while (!Gapp.exitWholeApp)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{	// Is There A Message Waiting?        
			if (msg.message == WM_QUIT)
			{				// Have We Received A Quit Message?
				Gapp.exitWholeApp = TRUE;							// If So done=TRUE
			}
			else
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else
		{										// If There Are No Messages        

			mainLoop();
		}
	}

	wglMakeCurrent(hDC, 0); // Remove the rendering context from our device context
	wglDeleteContext(hRC); // Delete our rendering context

	ReleaseDC(hWnd, hDC); // Release the device context from our window
	debugLog << L"Finished\n";
}



GLuint vbo, vao, vs, fs, prog;
GfxLowLevel::TextureRef *texture0;
GfxLowLevel::TextureRef *texture1;
GLuint textureSamplerID;

const char* vertex_shader =
"#version 400 core\n"
"layout(location = 0) in vec3 in_pos;"
"layout(location = 1) in vec3 in_col;"
"layout(location = 2) in vec2 in_textCoords;"
"out vec3 colour;"
"out vec2 textCoords;"
""
"void main()"
"{"
"	colour = in_col;"
"	textCoords = in_textCoords;"
"	gl_Position = vec4(in_pos, 1.0);"
"};";

const char* fragment_shader =
"#version 400 core\n"
"in vec3 colour;"
"in vec2 textCoords;"
"uniform sampler2D textureSamplerID;"
"out vec4 frag_colour;"
""
"void main ()"
"{"
"  frag_colour = vec4 (colour, 1.0)*texture( textureSamplerID, textCoords );"
"}";


void init()
{

	wchar_t wcstring[] = L"Murky8";
	debugLog << L"Start\n";
	qdev::setCurrentDirectoryToAppRoot();

	auto res = createWindow(wcstring, 800, 800);
	
	GfxLowLevel::onGfxDeviceErrorTriggerBreakpoint();

	texture0 = new GfxLowLevel::TextureRef(L"data/font.png");
	texture1 = new GfxLowLevel::TextureRef(L"data/t0.png");

	// buffers etc
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// dont need to actually put anything in buffer yet
	//glBufferData(GL_ARRAY_BUFFER, tris.size()*sizeof(Triangle_pct), tris.data(), GL_DYNAMIC_DRAW);
	
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// layout
	int szVertex = sizeof(Vert_pct);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, szVertex, 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, szVertex, (void*)(sizeof(vec3)));//col      

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, szVertex, (void*)(sizeof(vec3) + sizeof(vec3)));//tex        
	
	// shader
	//vs = glCreateShader(GL_VERTEX_SHADER);
	//glShaderSource(vs, 1, &vertex_shader, NULL);
	//glCompileShader(vs);
	vs = GfxLowLevel::createShader(vertex_shader, GL_VERTEX_SHADER);
	fs = GfxLowLevel::createShader(fragment_shader, GL_FRAGMENT_SHADER);	
	prog = GfxLowLevel::createProgram(vs, fs);
	textureSamplerID = glGetUniformLocation(prog, "textureSamplerID");
	
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	GfxLowLevel::onGfxDeviceErrorTriggerBreakpoint();

}

void mainLoop()
{
	GfxLowLevel::onGfxDeviceErrorTriggerBreakpoint();

	glViewport(0, 0, 800, 800);

	glClearColor(0.4f, 0.6f, 0.9f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	////
	
	glUseProgram(prog);
	glBindVertexArray(vao);
	
	// change data
	std::vector<Triangle_pct> dtris;
	float v = (float)(Gapp.frameCounter % 60) / 60.f;
	Triangle_pct tri0
	{
		{ { v, 0.0f, 1.0f }, { 1.0f, 1.0f, 0.0f }, { 0.0f, 1.0f } },
		{ { 0.0f, 0.5f, 1.0f }, { 1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
		{ { 0.5f, 0.0f, 1.0f }, { 0.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } },
	};
	dtris.push_back(tri0);

	Triangle_pct tri1
	{
		{ { 0.0f, 0.5f, 1.0f }, { 1.0f, 1.0f, 0.0f }, { 0.0f, 0.0f } },
		{ { 0.5f, 0.5f, 1.0f }, { 1.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } },
		{ { 0.5f, 0.0f, 1.0f }, { 0.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } },
	};
	dtris.push_back(tri1);

	glBufferData(GL_ARRAY_BUFFER, dtris.size()*sizeof(Triangle_pct), dtris.data(), GL_DYNAMIC_DRAW);
	// change data

	glBindTexture( GL_TEXTURE_2D, texture1->getHandle() );
	glDrawArrays( GL_TRIANGLES, 0, 6 );
	glBindTexture(GL_TEXTURE_2D, 0);
	////
	glFlush();
	SwapBuffers(hDC);
	Gapp.frameCounter++;

	::Sleep(1000 / 60);
	GfxLowLevel::onGfxDeviceErrorTriggerBreakpoint();
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	wndProcCalled = true;
	switch (message)
	{
	
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			Gapp.exitWholeApp = true;
		}
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

bool create40Context()
{
	hDC = GetDC(hWnd); // Get the device context for our window

	PIXELFORMATDESCRIPTOR pfd; // Create a new PIXELFORMATDESCRIPTOR (PFD)
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR)); // Clear our  PFD
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR); // Set the size of the PFD to the size of the class
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW; // Enable double buffering, opengl support and drawing to a window
	pfd.iPixelType = PFD_TYPE_RGBA; // Set our application to use RGBA pixels
	pfd.cColorBits = 32; // Give us 32 bits of color information (the higher, the more colors)
	pfd.cDepthBits = 32; // Give us 32 bits of depth information (the higher, the more depth levels)
	pfd.iLayerType = PFD_MAIN_PLANE; // Set the layer of the PFD

	int nPixelFormat = ChoosePixelFormat(hDC, &pfd); // Check if our PFD is valid and get a pixel format back
	if (nPixelFormat == 0) // If it fails
		return false;

	bool bResult = (bool)SetPixelFormat(hDC, nPixelFormat, &pfd); // Try and set the pixel format based on our PFD
	if (!bResult) // If it fails
		return false;

	HGLRC tempOpenGLContext = wglCreateContext(hDC); // Create an OpenGL 2.1 context for our device context
	wglMakeCurrent(hDC, tempOpenGLContext); // Make the OpenGL 2.1 context current and active

	glewExperimental = GL_TRUE;
	GLenum error = glewInit(); // Enable GLEW
	if (error != GLEW_OK) // If GLEW fails
		return false;

	int attributes[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, 4, // Set the MAJOR version of OpenGL to 4
		WGL_CONTEXT_MINOR_VERSION_ARB, 0, // Set the MINOR version of OpenGL to 0
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB, // Set our OpenGL context to be forward compatible
		0
	};

	if (wglewIsSupported("WGL_ARB_create_context") == 1)
	{ // If the OpenGL 3.x context creation extension is available
		hRC = wglCreateContextAttribsARB(hDC, NULL, attributes); // Create and OpenGL 3.x context based on the given attributes
		wglMakeCurrent(NULL, NULL); // Remove the temporary context from being active
		wglDeleteContext(tempOpenGLContext); // Delete the temporary OpenGL 2.1 context
		wglMakeCurrent(hDC, hRC); // Make our OpenGL 3.0 context current
	}
	else
	{
		hRC = tempOpenGLContext; // If we didn't have support for OpenGL 3.x and up, use the OpenGL 2.1 context
		triggerBreakpoint();
	}

	int glVersion[2] = { -1, -1 }; // Set some default values for the version
	glGetIntegerv(GL_MAJOR_VERSION, &glVersion[0]); // Get back the OpenGL MAJOR version we are using
	glGetIntegerv(GL_MINOR_VERSION, &glVersion[1]); // Get back the OpenGL MAJOR version we are using

	std::cout << "Using OpenGL: " << glVersion[0] << "." << glVersion[1] << std::endl; // Output which version of OpenGL we are using

	return true; // We have successfully created a context, return true
}

bool createWindow(LPCWSTR title, int width, int height)
{
	WNDCLASS windowClass;
	DWORD dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;

	hInstance = GetModuleHandle(NULL);

	windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	windowClass.lpfnWndProc = (WNDPROC)WndProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = hInstance;
	windowClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.hbrBackground = NULL;
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = title;

	if (!RegisterClass(&windowClass))
	{
		return false;
	}

	hWnd = CreateWindowEx(dwExStyle, title, title, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, width, height, NULL, NULL, hInstance, NULL);

	bool res = create40Context(); // Create our OpenGL context on the given window we just created
	if (!res)
	{
		return false;
	}

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	return true;
}
/*
class Wobject
{
std::Vector<*void> subObjects;
void serialize(char* p, int capacity );
int size();

}

*/