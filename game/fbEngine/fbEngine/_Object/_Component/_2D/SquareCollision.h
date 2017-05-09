#pragma once
#include "Collisiont2D.h"

//�l�p�`�̂����蔻��
class SquareCollision :public Collison2D
{
public:
	SquareCollision(GameObject* g, Transform* t) : Collison2D(g, t, typeid(this).name())
	{
		_Size = Vector2(1, 1);
	}
	//�l�p�`�Ɠ_�̔���
	const bool Judgment(const Vector2& point);
	//�l�p�`���m�̔���(������)
	const bool Judgment(const SquareCollision* Square);
	//
	void SetSize(const Vector2& size)
	{
		_Size = size;
	}
private:
	//���ĕ��A����
	Vector2 _Size;
};