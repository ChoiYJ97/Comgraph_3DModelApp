////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"

GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_Model[num] = { 0, };

	m_LightShader = 0;
	m_Light = 0;
	m_Text = 0;
	vertexCount = 0;
	m_SkyDome = 0;
	m_SkyDomeShader = 0;
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
			result = m_Model[i]->Initialize(m_D3D->GetDevice(), "../Engine/data/sun.obj", L"../Engine/data/Sun.jpg");
		else if (i == 1)
			result = m_Model[i]->Initialize(m_D3D->GetDevice(), "../Engine/data/mercury.obj", L"../Engine/data/Mercury.jpg");
		else if (i == 2)
			result = m_Model[i]->Initialize(m_D3D->GetDevice(), "../Engine/data/moon.obj", L"../Engine/data/Moon.dds");
		else if (i == 3)
			result = m_Model[i]->Initialize(m_D3D->GetDevice(), "../Engine/data/venus.obj", L"../Engine/data/VenusSurface.jpg");
		else if (i == 4)
			result = m_Model[i]->Initialize(m_D3D->GetDevice(), "../Engine/data/earth.obj", L"../Engine/data/EarthDayMap.jpg");
		else if (i == 5)
			result = m_Model[i]->Initialize(m_D3D->GetDevice(), "../Engine/data/mars.obj", L"../Engine/data/Mars.jpg");
		else if (i == 6)
			result = m_Model[i]->Initialize(m_D3D->GetDevice(), "../Engine/data/jupiter.obj", L"../Engine/data/Jupiter.jpg");
		else if (i == 7)
			result = m_Model[i]->Initialize(m_D3D->GetDevice(), "../Engine/data/saturn.obj", L"../Engine/data/Saturn.jpg");
		else if (i == 8)
			result = m_Model[i]->Initialize(m_D3D->GetDevice(), "../Engine/data/uranus.obj", L"../Engine/data/Uranus.jpg");
		else if (i == 9)
			result = m_Model[i]->Initialize(m_D3D->GetDevice(), "../Engine/data/neptune.obj", L"../Engine/data/Neptune.jpg");

		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
			return false;
		}
	}
	getPolygonCount("../Engine/data/moon.obj");
	getPolygonCount("../Engine/data/earth.obj");
	getPolygonCount("../Engine/data/venus.obj");
	getPolygonCount("../Engine/data/mars.obj");
	getPolygonCount("../Engine/data/jupiter.obj");
	getPolygonCount("../Engine/data/saturn.obj");
	getPolygonCount("../Engine/data/uranus.obj");
	getPolygonCount("../Engine/data/neptune.obj");
	getPolygonCount("../Engine/data/sun.obj");
	getPolygonCount("../Engine/data/mercury.obj");


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
	m_Light->SetAmbientColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(1.0f, -1.0f, 1.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(32.0f);


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

	// Create the sky dome object.
	m_SkyDome = new SkyDomeClass;
	if (!m_SkyDome)
	{
		return false;
	}

	// Initialize the sky dome object.
	result = m_SkyDome->Initialize(m_D3D->GetDevice());
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the sky dome object.", L"Error", MB_OK);
		return false;
	}

	// Create the sky dome shader object.
	m_SkyDomeShader = new SkyDomeShaderClass;
	if (!m_SkyDomeShader)
	{
		return false;
	}

	// Initialize the sky dome shader object.
	result = m_SkyDomeShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the sky dome shader object.", L"Error", MB_OK);
		return false;
	}

	return true;
}
void GraphicsClass::SetAmbient(bool on)
{
	if (on) 
	{
		m_Light->SetAmbientColor(0.5f, 0.5f, 0.5f, 0.5f);
	}
	else
	{
		m_Light->SetAmbientColor(0, 0, 0, 0);
	}
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
	if (on) 
	{
		m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
		m_Light->SetSpecularPower(32.0f);
	}
	else
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
void GraphicsClass::RotateCamera(float dir) {
	m_Camera->Rotate(dir);
}
void GraphicsClass::LookatChange(int x, int y)
{
	m_Camera->LookAtChange(x, y);
	m_Camera->Render();
}
void GraphicsClass::Shutdown()
{
	// Release the sky dome shader object.
	if (m_SkyDomeShader)
	{
		m_SkyDomeShader->Shutdown();
		delete m_SkyDomeShader;
		m_SkyDomeShader = 0;
	}

	// Release the sky dome object.
	if (m_SkyDome)
	{
		m_SkyDome->Shutdown();
		delete m_SkyDome;
		m_SkyDome = 0;
	}


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

	// Release the model object.
	//for(int i = 0; i < 3; i++)
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
	rotation += (float)D3DX_PI * 0.00015f;
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
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix, worldMatrix1;
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

		// Render the sky dome using the sky dome shader.
		m_SkyDome->Render(m_D3D->GetDeviceContext());
	m_SkyDomeShader->Render(m_D3D->GetDeviceContext(), m_SkyDome->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_SkyDome->GetApexColor(), m_SkyDome->GetCenterColor(), m_SkyDome->GetTexture());

		// Turn back face culling back on.
		m_D3D->TurnOnCulling();

	// Turn the Z buffer back on.
	m_D3D->TurnZBufferOn();

	// Reset the world matrix.
	m_D3D->GetWorldMatrix(worldMatrix);
	D3DXMatrixRotationY(&worldMatrix1, rotation);
	for (int i = 0; i < num; i++) {
		m_Model[i]->Render(m_D3D->GetDeviceContext());
		// Render the model using the light shader.
		if (i == 0) {
			// Render the model using the light shader.
			result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model[i]->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
				m_Model[i]->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
				m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
		}
		else
			// Render the model using the light shader.
			result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model[i]->GetIndexCount(), worldMatrix1*i, viewMatrix, projectionMatrix,
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
