#pragma once
#include "_Object\_Component\Component.h"

class Vertex;

class BitmapFont :public Component
{
public:
	BitmapFont(GameObject* g, Transform* t);
	void Start()override;
private:
	//’¸“_
	static Vertex* _Vertex;
};