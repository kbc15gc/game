#pragma once
#include "Collisiont2D.h"

//円のあたり判定
class CircleCollision :public Collison2D
{
public:
	CircleCollision(GameObject* g, Transform* t) : Collison2D(g, t, typeid(this).name())
	{
		
	}
	//円と点のあたり判定
	const bool Judgment(const Vector2& point);
	//サークル同士のあたり判定
	const bool Judgment(const CircleCollision* circle);
	//半径設定
	void SetRadius(float r)
	{
		_Radius = r;
	}
private:
	float _Radius;
};