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
	Vector2 posA, posB;
	posA = { transform->GetPosition().x,transform->GetPosition().y };
	posB = { circle->transform->GetPosition().x,circle->transform->GetPosition().y };
	Vector2 vec = posB - posA;
	float SumRadius = this->_Radius + circle->_Radius;
	//�x�N�g���̒��������݂��̔��a�̘a�ȏ�Ȃ�Փ�
	return (vec.Length() <= SumRadius);
}
