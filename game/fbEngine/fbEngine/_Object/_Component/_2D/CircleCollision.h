#pragma once
#include "Collisiont2D.h"

//‰~‚Ì‚ ‚½‚è”»’è
class CircleCollision :public Collison2D
{
public:
	CircleCollision(GameObject* g, Transform* t) : Collison2D(g, t, typeid(this).name())
	{
		
	}
	//‰~‚Æ“_‚Ì‚ ‚½‚è”»’è
	const bool Judgment(const Vector2& point);
	//ƒT[ƒNƒ‹“¯m‚Ì‚ ‚½‚è”»’è
	const bool Judgment(const CircleCollision* circle);
	//”¼Œaİ’è
	void SetRadius(float r)
	{
		_Radius = r;
	}
private:
	float _Radius;
};