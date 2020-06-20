////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"

GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_Model[num] = { 0, };
	m_skydome = 0;
	m_LightShader = 0;
	m_Light = 0;

	m_Text = 0;
	vertexCount = 0;
}
GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}

GraphicsClass::~GraphicsClass()
{
}

void GraphicsClass::getPolygonCount(char* filename) {
	ifstream fin;
	char input;
	int vCount;

	fin.open(filename);

	if (fin.fail())
	{
		return;
	}

	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	fin >> vCount;

	vertexCount += vCount;

	fin.close();
}

bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;
	D3DXMATRIX baseViewMatrix;


	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if (!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}

	// Initialize a base view matrix with the camera for 2D user interface rendering.
	m_Camera->SetPosition(0.0f, 10.0f, -60.0f);
	m_Camera->Render();
	m_Camera->GetViewMatrix(baseViewMatrix);
	
	m_skydome = new ModelClass;
	if (!m_skydome)
	{
		return false;
	}
	result = m_skydome->Initialize(m_D3D->GetDevice(), "../Engine/data/sky.obj", L"../Engine/data/Background.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Create the model object.
	//for (int i = 0; i < 3; i++)
	for (int i = 0; i < num; i++)
	{
		m_Model[i] = new ModelClass;
		if (!m_Model)
		{
			return false;
		}

		// Initialize the model object.
		if(i == 0)
			result = m_Model[i]->Initialize(m_D3D->GetDevice(), "../Engine/data/Sun.obj", L"../Engine/data/Sun.jpg");
		else if (i == 1)
			result = m_Model[i]->Initialize(m_D3D->GetDevice(), "../Engine/data/Mercury.obj", L"../Engine/data/Mercury.jpg");
		else if (i == 2)
			result = m_Model[i]->Initialize(m_D3D->GetDevice(), "../Engine/data/Moon.obj", L"../Engine/data/Moon.dds");
		else if (i == 3)
			result = m_Model[i]->Initialize(m_D3D->GetDevice(), "../Engine/data/Venus.obj", L"../Engine/data/VenusSurface.jpg");
		else if (i == 4)
			result = m_Model[i]->Initialize(m_D3D->GetDevice(), "../Engine/data/Earth.obj", L"../Engine/data/EarthDayMap.jpg");
		else if (i == 5)
			result = m_Model[i]->Initialize(m_D3D->GetDevice(), "../Engine/data/Mars.obj", L"../Engine/data/Mars.jpg");
		else if (i == 6)
			result = m_Model[i]->Initialize(m_D3D->GetDevice(), "../Engine/data/Jupiter.obj", L"../Engine/data/Jupiter.jpg");
		else if (i == 7)
			result = m_Model[i]->Initialize(m_D3D->GetDevice(), "../Engine/data/Saturn.obj", L"../Engine/data/Saturn.jpg");
		else if (i == 8)
			result = m_Model[i]->Initialize(m_D3D->GetDevice(), "../Engine/data/Uranus.obj", L"../Engine/data/Uranus.jpg");
		else if (i == 9)
			result = m_Model[i]->Initialize(m_D3D->GetDevice(), "../Engine/data/Neptune.obj", L"../Engine/data/Neptune.jpg");

		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
			return false;
		}
	}
	getPolygonCount("../Engine/data/Moon.obj");
	getPolygonCount("../Engine/data/earth.obj");
	getPolygonCount("../Engine/data/Venus.obj");
	getPolygonCount("../Engine/data/Mars.obj");
	getPolygonCount("../Engine/data/Jupiter.obj");
	getPolygonCount("../Engine/data/Saturn.obj");
	getPolygonCount("../Engine/data/Uranus.obj");
	getPolygonCount("../Engine/data/Neptune.obj");
	getPolygonCount("../Engine/data/Sun.obj");
	getPolygonCount("../Engine/data/Mercury.obj");


	// Create the light shader object.
	m_LightShader = new LightShaderClass;
	if (!m_LightShader)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the light object.
	m_Light = new LightClass;
	if (!m_Light)
	{
		return false;
	}

	// Initialize the light object.
	m_Light->SetAmbientColor(20.0f, 20.0f, 20.0f, 50.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(0,0,0);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(100000.0f);


	// Create the text object.
	m_Text = new TextClass;
	if (!m_Text)
	{
		return false;
	}

	// Initialize the text object.
	result = m_Text->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}
	return true;
}
void GraphicsClass::SetAmbient(bool on)
{
	/*if (on) 
	{
		m_Light->SetAmbientColor(0.5f, 0.5f, 0.5f, 0.5f);
	}
	else
	{
		m_Light->SetAmbientColor(0, 0, 0, 0);
	}*/
}
void GraphicsClass::SetDiffuse(bool on) 
{
	if (on) {
		m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else
	{
		m_Light->SetDiffuseColor(0, 0, 0, 0);
	}
}
void GraphicsClass::SetSpecular(bool on) 
{
	/*if (on) 
	{
		m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
		m_Light->SetSpecularPower(32.0f);
	}
	else*/
	{
		m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
		m_Light->SetSpecularPower(100000.0f);
	}
}
void GraphicsClass::MoveCameraZ(float dir) {
	m_Camera->MoveZ(dir);
}
void GraphicsClass::MoveCameraX(float dir) {
	m_Camera->MoveX(dir);
}
void GraphicsClass::MoveCameraY(float dir) {
	m_Camera->MoveY(dir);
}
void GraphicsClass::RotateXCamera(float dir) {
	m_Camera->RotateX(dir);
}
void GraphicsClass::RotateYCamera(float dir) {
	m_Camera->RotateY(dir);
}

void GraphicsClass::LookatChange(int x, int y)
{
	m_Camera->LookAtChange(x, y);
	m_Camera->Render();
}
void GraphicsClass::Shutdown()
{
	// Release the light object.
	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the light shader object.
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	if (m_skydome)
	{
		m_skydome->Shutdown();
		delete m_skydome;
		m_skydome = 0;
	}
	// Release the model object.
	for (int i = 0; i < num; i++)
		if (m_Model)
		{
			m_Model[i]->Shutdown();
			delete m_Model[i];
			m_Model[i] = 0;
		}

	// Release the text object.
	if (m_Text)
	{
		m_Text->Shutdown();
		delete m_Text;
		m_Text = 0;
	}

	// Release the camera object.
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the D3D object.
	if (m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	return;
}
bool GraphicsClass::Frame(int fps, int cpu, float frameTime, bool onoff_ambi, bool onoff_diff, bool onoff_spec)
{
	bool result;
	static float rotation = 0.0f;

	// Update the rotation variable each frame.
	rotation += (float)D3DX_PI * 0.000015f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}
	SetAmbient(onoff_ambi);
	SetDiffuse(onoff_diff);
	SetSpecular(onoff_spec);
	// Render the graphics scene.
	result = Render(rotation);
	if (!result)
	{
		return false;
	}

	// Set the frames per second.
	result = m_Text->SetFps(fps, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	// Set the cpu usage.
	result = m_Text->SetCpu(cpu, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	result = m_Text->SetVerCount(vertexCount/3, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}
	return true;
}


bool GraphicsClass::Render(float rotation)
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	D3DXMATRIX worldMatrix1, WM_sun, WM_moon, WM_planet;
	bool result;
	D3DXVECTOR3 cameraPosition;

	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the view, projection, and world matrices from the camera and D3D objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);

	// Get the position of the camera.
	cameraPosition = m_Camera->GetPosition();

	// Translate the sky dome to be centered around the camera position.
	D3DXMatrixTranslation(&worldMatrix, cameraPosition.x, cameraPosition.y, cameraPosition.z);

	// Turn off back face culling.
	m_D3D->TurnOffCulling();

	// Turn off the Z buffer.
	m_D3D->TurnZBufferOff();

	// Turn back face culling back on.
	m_D3D->TurnOnCulling();

	// Turn the Z buffer back on.
	m_D3D->TurnZBufferOn();

	//D3DXVECTOR3 Earthpos;
	D3DXMatrixIdentity(&WM_sun);
	D3DXMatrixIdentity(&WM_planet);
	// Reset the world matrix.
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetWorldMatrix(WM_moon);
	m_D3D->GetWorldMatrix(worldMatrix1);

	D3DXMatrixTranspose(&WM_moon, &WM_moon);
	D3DXMatrixTranspose(&WM_planet, &WM_planet);
	//D3DXMatrixDecompose(&D3DXVECTOR3(), &D3DXQUATERNION(), &Earthpos, &worldMatrix1);

	//D3DXMatrixIdentity
	D3DXMatrixRotationY(&worldMatrix1, -rotation);
	D3DXMatrixRotationX(&WM_moon, rotation*10);
	D3DXMatrixRotationY(&WM_sun, -rotation);
	//D3DXMatrixRotationZ(&WM_planet, rotation);

	m_skydome->Render(m_D3D->GetDeviceContext());
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_skydome->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_skydome->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

	for (int i = 0; i < num; i++) {

		m_Model[i]->Render(m_D3D->GetDeviceContext());

		if (i == 0) {
			result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model[i]->GetIndexCount(), WM_sun, viewMatrix, projectionMatrix,
				m_Model[i]->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
				m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

		}
		else if (i == 2) {
			result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model[i]->GetIndexCount(), WM_moon*worldMatrix1, viewMatrix, projectionMatrix,
				m_Model[i]->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
				m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
		}
		else
			result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model[i]->GetIndexCount(), worldMatrix1, viewMatrix, projectionMatrix,
				m_Model[i]->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
				m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

		if (!result)
		{
			return false;
		}
	}

	// Turn off the Z buffer to begin all 2D rendering.
	m_D3D->TurnZBufferOff();

	// Turn on the alpha blending before rendering the text.
	m_D3D->TurnOnAlphaBlending();

	// Render the text strings.
	result = m_Text->Render(m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	// Turn off alpha blending after rendering the text.
	m_D3D->TurnOffAlphaBlending();

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_D3D->TurnZBufferOn();

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}
