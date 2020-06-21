////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_


/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = true;
const bool VSYNC_ENABLED = false;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3dclass.h"
#include "cameraclass.h"
#include "textclass.h"
#include "modelclass.h"
#include "lightshaderclass.h"
#include "lightclass.h"

#include "skydomeclass.h"
#include "skydomeshaderclass.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////
class GraphicsClass
{
public:	
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame(int, int, float, bool, bool, bool);
	bool Render(float);
	void getPolygonCount(char*);
	void SetAmbient(bool);
	void SetDiffuse(bool);
	void SetSpecular(bool);
	void MoveCameraX(float);
	void MoveCameraZ(float);
	void MoveCameraY(float);
	void RotateXCamera(float);
	void RotateYCamera(float);
	void LookatChange(int, int);
	void modelRendering(int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);
	void CameraTranslate();

	void SetModelIndex(int);
	int GetModelIndex();

private:
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	TextClass* m_Text;
	ModelClass* m_skydome;
	ModelClass* m_Model[11];
	
	LightShaderClass* m_LightShader;
	LightClass* m_Light;
	int vertexCount;

	int num = 11, m_index = 9;
	bool Init, CameraMovement;
	
};

#endif