#pragma once
#include "Vector.h"

struct Rect
{
	float right;
	float left;
	float top;
	float bottom;
};

//AABBの2D版。
class AABB2D {
public:
	AABB2D() {};
	//頂点を設定する。
	//[in] 右上。
	//[in] 左下。
	void SetUpVertex(Vector2 rightup,Vector2 leftdown);
	//頂点を設定する。
	void SetUpVertex(const AABB& aabb);

	//衝突しているかどうか？
	bool IsHit(const AABB2D& aabb);
private:
	Rect _Rect;
};