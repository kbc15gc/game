#include "stdafx.h"
#include "CircleCollision.h"

const bool CircleCollision::Judgment(const Vector2& point)
{
	//�x�N�g�������߂�
	Vector2 pos = { transform->GetPosition().x,transform->GetPosition().y };
	Vector2 vec = point - pos;
	//���������a�ȉ��Ȃ�q�b�g
	return (vec.Length() <= _Radius);
}

const bool CircleCollision::Judgment(const CircleCollision * circle)
{
	return false;
}
