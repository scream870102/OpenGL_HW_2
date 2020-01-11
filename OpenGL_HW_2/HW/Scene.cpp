#include "Scene.h"
#include "Utils/Input.h"
#include "../Common/CQuad.h"
#include "../Common/CSolidCube.h"
#include "../Common/CWireSphere.h"
#include "../Common/CCamera.h"
#include "../Common/CShape.h"
#define SCREEN_SIZE 800
#define HALF_SIZE SCREEN_SIZE /2 
//// For Objects
CSolidCube* g_pCube;
//// For View Point
GLfloat g_fRadius = 10.0;
GLfloat g_fTheta = 60.0f * DegreesToRadians;
GLfloat g_fPhi = 45.0f * DegreesToRadians;

float g_fLightRadius = 6;
float g_fLightTheta = 0;

float g_fLightR = 0.95f;
float g_fLightG = 0.95f;
float g_fLightB = 0.95f;
//Update Camera and Light here
LightSource g_Light1 = {
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // ambient 
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // diffuse
	color4(g_fLightR, g_fLightG, g_fLightB, 1.0f), // specular
	point4(g_fLightRadius, g_fLightRadius, 0.0f, 1.0f),   // position
	point4(0.0f, 0.0f, 0.0f, 1.0f),   // halfVector
	vec3(0.0f, 0.0f, 0.0f),			  //spotTarget
	vec3(0.0f, 0.0f, 0.0f),			  //spotDirection
	1.0f	,	// spotExponent(parameter e); cos^(e)(phi) 
	45.0f,	// spotCutoff;	// (range: [0.0, 90.0], 180.0)  spot 的照明範圍
	1.0f	,	// spotCosCutoff; // (range: [1.0,0.0],-1.0), 照明方向與被照明點之間的角度取 cos 後, cut off 的值
	1	,	// constantAttenuation	(a + bd + cd^2)^-1 中的 a, d 為光源到被照明點的距離
	0	,	// linearAttenuation	    (a + bd + cd^2)^-1 中的 b
	0		// quadraticAttenuation (a + bd + cd^2)^-1 中的 c
};

CWireSphere* g_pLight;
Scene::Scene() {
}

Scene::~Scene() {
}

void Scene::Update(float delta) {
	UpdateCamera();
	//Update camera position due to input
	if (Input::GetInstance()->IsGetKey('w')) {
		CCamera::getInstance()->Move(CCamera::Direction::FORWARD);
	}
	else if (Input::GetInstance()->IsGetKey('s')) {
		CCamera::getInstance()->Move(CCamera::Direction::BACKWARD);
	}
	else if (Input::GetInstance()->IsGetKey('a')) {
		CCamera::getInstance()->Move(CCamera::Direction::LEFT);
	}
	else if (Input::GetInstance()->IsGetKey('d')) {
		CCamera::getInstance()->Move(CCamera::Direction::RIGHT);
	}
	mat4 mvx;	// view matrix & projection matrix
	bool bVDirty;	// view 與 projection matrix 是否需要更新給物件
	auto camera = CCamera::getInstance();
	mvx = camera->getViewMatrix(bVDirty);
	if (bVDirty) {
		g_pCube->SetViewMatrix(mvx);
		g_pLight->SetViewMatrix(mvx);

	}

	//// 如果需要重新計算時，在這邊計算每一個物件的顏色
	g_pCube->Update(delta, g_Light1);
	g_pLight->Update(delta);
}



void Scene::Init() {
	//
	mat4 mxT;
	vec4 vT, vColor;
	//	// 產生所需之 Model View 與 Projection Matrix
	//
	point4  eye(g_fRadius * sin(g_fTheta) * sin(g_fPhi), g_fRadius * cos(g_fTheta), g_fRadius * sin(g_fTheta) * cos(g_fPhi), 1.0f);
	point4  at(0.0f, 0.0f, 0.0f, 1.0f);

	auto camera = CCamera::create();
	camera->updateViewLookAt(eye, at);
	camera->updatePerspective(60.0, (GLfloat)SCREEN_SIZE / (GLfloat)SCREEN_SIZE, 1.0, 1000.0);
	//	// Part 3 : materials
	//
	g_pCube = new CSolidCube;
	//	// Part 3 : materials
	//#ifdef SETTING_MATERIALS
	g_pCube->SetMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0, 0.85f, 0, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pCube->SetKaKdKsShini(0.15f, 0.8f, 0.2f, 2);
	//#endif
	g_pCube->SetShader();
	//	// 設定 Cube
	vT.x = 1.5; vT.y = 0.5; vT.z = -1.5;
	mxT = Translate(vT);
	g_pCube->SetTRSMatrix(mxT);
	g_pCube->SetShadingMode(GOURAUD_SHADING);

	// 設定 代表 Light 的 WireSphere
	g_pLight = new CWireSphere(0.25f, 6, 3);
	g_pLight->SetShader();
	mxT = Translate(g_Light1.position);
	g_pLight->SetTRSMatrix(mxT);
	g_pLight->SetColor(g_Light1.diffuse);
	//#ifdef LIGHTING_WITHGPU
	g_pLight->SetLightingDisable();
	//#endif
	//
	//	// 因為本範例不會動到 Projection Matrix 所以在這裡設定一次即可
	//	// 就不寫在 OnFrameMove 中每次都 Check
	bool bPDirty;
	mat4 mpx = camera->getProjectionMatrix(bPDirty);
	g_pCube->SetProjectionMatrix(mpx);
	g_pLight->SetProjectionMatrix(mpx);
}

void Scene::UpdateCamera() {
	g_fPhi = (float)-M_PI * (Input::GetInstance()->mouseX - HALF_SIZE) / (HALF_SIZE);  // 轉換成 g_fPhi 介於 -PI 到 PI 之間 (-180 ~ 180 之間)
	g_fTheta = (float)M_PI * (float)Input::GetInstance()->mouseY / SCREEN_SIZE;;
	//point4  eye(g_fRadius * sin(g_fTheta) * sin(g_fPhi), g_fRadius * cos(g_fTheta), g_fRadius * sin(g_fTheta) * cos(g_fPhi), 1.0f);
	point4 at(g_fRadius * sin(g_fTheta) * sin(g_fPhi), g_fRadius * cos(g_fTheta), g_fRadius * sin(g_fTheta) * cos(g_fPhi), 1.0f);
	CCamera::getInstance()->updateLookAt(at);
}


void Scene::Draw() {
	g_pCube->Draw();
	g_pLight->Draw();
}
