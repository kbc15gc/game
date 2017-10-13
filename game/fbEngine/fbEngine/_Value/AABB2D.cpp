#include "fbstdafx.h"
#include "AABB2D.h"

void AABB2D::SetUpVertex(Vector2 rightup, Vector2 leftdown)
{
	_Rect.right = rightup.x;
	_Rect.left = leftdown.x;
	_Rect.top = rightup.y;
	_Rect.bottom = leftdown.y;
}

void AABB2D::SetUpVertex(const AABB& aabb)
{
	//‰EãA¶‰ºB
	Vector2 ru = Vector2(-FLT_MAX,-FLT_MAX), ld = Vector2(FLT_MAX, FLT_MAX);
	FOR(idx,8)
	{
		auto pos = aabb.GetVertexPosition(idx);
		ru.x = max(pos.x, ru.x);
		ru.y = max(pos.y, ru.y);

		ld.x = min(pos.x, ld.x);
		ld.y = min(pos.y, ld.y);
	}
	SetUpVertex(ru, ld);
}

bool AABB2D::IsHit(const AABB2D& aabb)
{
	auto obj1 = this->_Rect, obj2 = aabb._Rect;

	return ((obj1.right  > obj2.left) &&
		(obj1.left   < obj2.right) &&
		(obj1.top    > obj2.bottom) &&
		(obj1.bottom < obj2.top));
}
