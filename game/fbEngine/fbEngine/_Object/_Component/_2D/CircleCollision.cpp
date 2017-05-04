#include "CircleCollision.h"

const bool CircleCollision::Judgment(const Vector2& point)
{
	//ベクトルを求める
	Vector2 pos = { transform->GetPosition().x,transform->GetPosition().y };
	Vector2 vec = point - pos;
	//長さが半径以下ならヒット
	return (vec.Length() <= _Radius);
}

const bool CircleCollision::Judgment(const CircleCollision * circle)
{
	Vector2 posA, posB;
	posA = { transform->GetPosition().x,transform->GetPosition().y };
	posB = { circle->transform->GetPosition().x,circle->transform->GetPosition().y };
	Vector2 vec = posB - posA;
	float SumRadius = this->_Radius + circle->_Radius;
	//ベクトルの長さがお互いの半径の和以上なら衝突
	return (vec.Length() <= SumRadius);
}
