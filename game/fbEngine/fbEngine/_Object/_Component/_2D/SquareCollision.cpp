#include"fbstdafx.h"
#include "SquareCollision.h"

const bool SquareCollision::Judgment(const Vector2& point)
{
	Vector2 pos = Vector2(transform->GetPosition().x, transform->GetPosition().y);
	RECT rect = { 
		pos.x - _Size.x / 2,	//ç∂
		pos.y - _Size.y / 2,	//è„
		pos.x + _Size.x / 2,	//âE
		pos.y + _Size.y / 2		//â∫
	};
	
	return (rect.top < point.y && point.y < rect.bottom &&
		rect.left < point.x && point.x < rect.right);
}

const bool SquareCollision::Judgment(const SquareCollision * Square)
{
	return false;
}
