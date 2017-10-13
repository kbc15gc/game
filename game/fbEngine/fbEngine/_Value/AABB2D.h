#pragma once
#include "Vector.h"

struct Rect
{
	float right;
	float left;
	float top;
	float bottom;
};

//AABB��2D�ŁB
class AABB2D {
public:
	AABB2D() {};
	//���_��ݒ肷��B
	//[in] �E��B
	//[in] �����B
	void SetUpVertex(Vector2 rightup,Vector2 leftdown);
	//���_��ݒ肷��B
	void SetUpVertex(const AABB& aabb);

	//�Փ˂��Ă��邩�ǂ����H
	bool IsHit(const AABB2D& aabb);
private:
	Rect _Rect;
};