#include "Scene.h"
#include "Utils/Input.h"
//#include "../Common/TypeDefine.h"
//#include "../Common/TypeDefine.h"
//#include "../Common/CQuad.h"
#include "../Common/CSolidCube.h"
//#include "../Common/CSolidSphere.h"
#include "../Common/CWireSphere.h"
//#include "../Common/CWireCube.h"
//#include "../Common/CChecker.h"
#include "../Common/CCamera.h"
#include "../Common/CShape.h"
//#define SPACE_KEY 32
#define SCREEN_SIZE 800
#define HALF_SIZE SCREEN_SIZE /2 
//#define VP_HALFWIDTH  20.0f
//#define VP_HALFHEIGHT 20.0f
//#define GRID_SIZE 20 // must be an even number
//
//#define SETTING_MATERIALS 
//// For Model View and Projection Matrix
//mat4 g_mxModelView(1.0f);
//mat4 g_mxProjection;
//
//// For Objects
//CChecker* g_pChecker;
CSolidCube* g_pCube;
//CSolidSphere* g_pSphere;
//
//CQuad* g_LeftWall, * g_RightWall;
//CQuad* g_FrontWall, * g_BackWall;
//
//// For View Point
GLfloat g_fRadius = 10.0;
GLfloat g_fTheta = 60.0f * DegreesToRadians;
GLfloat g_fPhi = 45.0f * DegreesToRadians;

//point4  g_vEye(g_fRadius* sin(g_fTheta)* cos(g_fPhi), g_fRadius* sin(g_fTheta)* sin(g_fPhi), g_fRadius* cos(g_fTheta), 1.0);
//point4  g_vAt(0.0, 0.0, 0.0, 1.0);
//vec4    g_vUp(0.0, 1.0, 0.0, 0.0);
//
////----------------------------------------------------------------------------
//// Part 2 : for single light source
//bool g_bAutoRotating = false;
//float g_fElapsedTime = 0;
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
	45.0f,	// spotCutoff;	// (range: [0.0, 90.0], 180.0)  spot ���ө��d��
	1.0f	,	// spotCosCutoff; // (range: [1.0,0.0],-1.0), �ө���V�P�Q�ө��I���������ר� cos ��, cut off ����
	1	,	// constantAttenuation	(a + bd + cd^2)^-1 ���� a, d ��������Q�ө��I���Z��
	0	,	// linearAttenuation	    (a + bd + cd^2)^-1 ���� b
	0		// quadraticAttenuation (a + bd + cd^2)^-1 ���� c
};

CWireSphere* g_pLight;
Scene::Scene(){
}

Scene::~Scene(){
}

void Scene::Update(float delta){
	UpdateCamera();
	mat4 mvx;	// view matrix & projection matrix
	bool bVDirty;	// view �P projection matrix �O�_�ݭn��s������
	auto camera = CCamera::getInstance();
	mvx = camera->getViewMatrix(bVDirty);
	if (bVDirty) {
	//	g_pChecker->SetViewMatrix(mvx);
		g_pCube->SetViewMatrix(mvx);
	//	g_pSphere->SetViewMatrix(mvx);
	//	g_LeftWall->SetViewMatrix(mvx);
	//	g_RightWall->SetViewMatrix(mvx);
	//	g_FrontWall->SetViewMatrix(mvx);
	//	g_BackWall->SetViewMatrix(mvx);
		g_pLight->SetViewMatrix(mvx);

	}

	//if (g_bAutoRotating) { // Part 2 : ���s�p�� Light ����m
	//	//UpdateLightPosition(delta);
	//}
	//// �p�G�ݭn���s�p��ɡA�b�o��p��C�@�Ӫ����C��
	//g_pChecker->Update(delta, g_Light1);
	g_pCube->Update(delta, g_Light1);
	//g_pSphere->Update(delta, g_Light1);
	//g_LeftWall->Update(delta, g_Light1);
	//g_RightWall->Update(delta, g_Light1);
	//g_FrontWall->Update(delta, g_Light1);
	//g_BackWall->Update(delta, g_Light1);
	g_pLight->Update(delta);
}



void Scene::Init(){
//
	mat4 mxT;
	vec4 vT, vColor;
//	// ���ͩһݤ� Model View �P Projection Matrix
//
	point4  eye(g_fRadius * sin(g_fTheta) * sin(g_fPhi), g_fRadius * cos(g_fTheta), g_fRadius * sin(g_fTheta) * cos(g_fPhi), 1.0f);
	point4  at(0.0f, 0.0f, 0.0f, 1.0f);

auto camera = CCamera::create();
	camera->updateViewLookAt(eye, at);
	camera->updatePerspective(60.0, (GLfloat)SCREEN_SIZE / (GLfloat)SCREEN_SIZE, 1.0, 1000.0);
//
//	// ���ͪ��󪺹���
//	g_pChecker = new CChecker(GRID_SIZE);
//	// Part 3 : materials
//#ifdef SETTING_MATERIALS
//	g_pChecker->SetMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0, 0.85f, 0, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
//	g_pChecker->SetKaKdKsShini(0, 0.8f, 0.5f, 1);
//#endif
//	g_pChecker->SetShadingMode(GOURAUD_SHADING);
//	g_pChecker->SetShader();
//
//	vT.x = -10.0f; vT.y = 10.0f; vT.z = 0;
//	mxT = Translate(vT);
//	g_LeftWall = new CQuad;
//	g_LeftWall->SetMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0, 0.85f, 0, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
//	g_LeftWall->SetShadingMode(GOURAUD_SHADING);
//	g_LeftWall->SetShader();
//	g_LeftWall->SetColor(vec4(0.6f));
//	g_LeftWall->SetTRSMatrix(mxT * RotateZ(-90.0f) * Scale(20.0f, 1, 20.0f));
//	g_LeftWall->SetKaKdKsShini(0, 0.8f, 0.5f, 1);
//
//	vT.x = 10.0f; vT.y = 10.0f; vT.z = 0;
//	mxT = Translate(vT);
//	g_RightWall = new CQuad;
//	g_RightWall->SetMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0, 0.85f, 0, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
//	g_RightWall->SetShadingMode(GOURAUD_SHADING);
//	g_RightWall->SetShader();
//	g_RightWall->SetColor(vec4(0.6f));
//	g_RightWall->SetTRSMatrix(mxT * RotateZ(90.0f) * Scale(20.0f, 1, 20.0f));
//	g_RightWall->SetKaKdKsShini(0, 0.8f, 0.5f, 1);
//
//	vT.x = 0.0f; vT.y = 10.0f; vT.z = 10.0f;
//	mxT = Translate(vT);
//	g_FrontWall = new CQuad;
//	g_FrontWall->SetMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0, 0.85f, 0, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
//	g_FrontWall->SetShadingMode(GOURAUD_SHADING);
//	g_FrontWall->SetShader();
//	g_FrontWall->SetColor(vec4(0.6f));
//	g_FrontWall->SetTRSMatrix(mxT * RotateX(-90.0f) * Scale(20.0f, 1, 20.0f));
//	g_FrontWall->SetKaKdKsShini(0, 0.8f, 0.5f, 1);
//
//	vT.x = 0.0f; vT.y = 10.0f; vT.z = -10.0f;
//	mxT = Translate(vT);
//	g_BackWall = new CQuad;
//	g_BackWall->SetMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0, 0.85f, 0, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
//	g_BackWall->SetShadingMode(GOURAUD_SHADING);
//	g_BackWall->SetShader();
//	g_BackWall->SetColor(vec4(0.6f));
//	g_BackWall->SetTRSMatrix(mxT * RotateX(90.0f) * Scale(20.0f, 1, 20.0f));
//	g_BackWall->SetKaKdKsShini(0, 0.8f, 0.5f, 1);
//
//	// Part 3 : materials
//
	g_pCube = new CSolidCube;
//	// Part 3 : materials
//#ifdef SETTING_MATERIALS
	g_pCube->SetMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0, 0.85f, 0, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	g_pCube->SetKaKdKsShini(0.15f, 0.8f, 0.2f, 2);
//#endif
	g_pCube->SetShader();
//	// �]�w Cube
	vT.x = 1.5; vT.y = 0.5; vT.z = -1.5;
	mxT = Translate(vT);
	g_pCube->SetTRSMatrix(mxT);
	g_pCube->SetShadingMode(GOURAUD_SHADING);
//
//	g_pSphere = new CSolidSphere(1, 16, 16);
//	// Part 3 : materials
//#ifdef SETTING_MATERIALS
//	g_pSphere->SetMaterials(vec4(0.15f, 0.15f, 0.15f, 1.0f), vec4(0, 0, 0.85f, 1), vec4(1.0f, 1.0f, 1.0f, 1.0f));
//	g_pSphere->SetKaKdKsShini(0.15f, 0.45f, 0.55f, 5);
//#endif
//	g_pSphere->SetShader();
//	// �]�w Sphere
//	vT.x = -1.5; vT.y = 1.0; vT.z = 1.5;
//	mxT = Translate(vT);
//	g_pSphere->SetTRSMatrix(mxT);
//	g_pSphere->SetShadingMode(GOURAUD_SHADING);
//
	// �]�w �N�� Light �� WireSphere
	g_pLight = new CWireSphere(0.25f, 6, 3);
	g_pLight->SetShader();
	mxT = Translate(g_Light1.position);
	g_pLight->SetTRSMatrix(mxT);
	g_pLight->SetColor(g_Light1.diffuse);
//#ifdef LIGHTING_WITHGPU
	g_pLight->SetLightingDisable();
//#endif
//
//	// �]�����d�Ҥ��|�ʨ� Projection Matrix �ҥH�b�o�̳]�w�@���Y�i
//	// �N���g�b OnFrameMove ���C���� Check
	bool bPDirty;
	mat4 mpx = camera->getProjectionMatrix(bPDirty);
//	g_pChecker->SetProjectionMatrix(mpx);
	g_pCube->SetProjectionMatrix(mpx);
//	g_pSphere->SetProjectionMatrix(mpx);
	g_pLight->SetProjectionMatrix(mpx);
//	g_LeftWall->SetProjectionMatrix(mpx);
//	g_RightWall->SetProjectionMatrix(mpx);
//	g_FrontWall->SetProjectionMatrix(mpx);
//	g_BackWall->SetProjectionMatrix(mpx);
}

void Scene::UpdateCamera(){
	g_fPhi = (float)-M_PI * (Input::GetInstance()->mouseX - HALF_SIZE) / (HALF_SIZE);  // �ഫ�� g_fPhi ���� -PI �� PI ���� (-180 ~ 180 ����)
	g_fTheta = (float)M_PI * (float)Input::GetInstance()->mouseY / SCREEN_SIZE;;
	point4  eye(g_fRadius * sin(g_fTheta) * sin(g_fPhi), g_fRadius * cos(g_fTheta), g_fRadius * sin(g_fTheta) * cos(g_fPhi), 1.0f);
	point4  at(0.0f, 0.0f, 0.0f, 1.0f);
	auto camera = CCamera::getInstance();
	camera->updateViewLookAt(eye, at);
}


void Scene::Draw() {
	//g_pChecker->Draw();
	//g_pSphere->Draw();
	g_pCube->Draw();
	g_pLight->Draw();
	//g_LeftWall->Draw();
	//g_RightWall->Draw();
	//g_FrontWall->Draw();
	//g_BackWall->Draw();
}
