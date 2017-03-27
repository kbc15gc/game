#pragma once
#include "Collisiont2D.h"

//�~�̂����蔻��
class CircleCollision :public Collison2D
{
public:
	CircleCollision(GameObject* g, Transform* t) : Collison2D(g, t, typeid(this).name())
	{
		
	}
	//�~�Ɠ_�̂����蔻��
	const bool Judgment(const Vector2& point);
	//������
	const bool Judgment(const CircleCollision* circle);
	//���a�ݒ�
	void SetRadius(float r)
	{
		_Radius = r;
	}
private:
	float _Radius;
};