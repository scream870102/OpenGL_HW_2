#pragma once
//class IScene
//{
//public:
//	IScene();
//	~IScene();
//
//private:
//
//};
//
//IScene::IScene()
//{
//}
//
//IScene::~IScene()
//{
//}
class Scene
{
public:
	Scene();
	~Scene();
	void Update(float delta);
	void Draw();
	void Init();
private:
	void UpdateCamera();

};