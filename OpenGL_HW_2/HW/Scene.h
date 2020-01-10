#pragma once
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