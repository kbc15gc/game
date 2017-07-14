#include"stdafx.h"
#include "SearchViewAngle.h"

bool SearchViewAngle::IsDiscovery(const Vector3& pos1, const Vector3& pos2, const Vector3& Direction, float viewAngle, float range) {
	Vector3 Vec = pos2 - pos1;	// 自分から相手へのベクトル。
	float Length = Vec.Length();
	if (Length <= range) {
		// 見える距離に入った。

		// Y軸成分を除く。
		Vec.y = 0.0f;
		Vec.Normalize();
		Vector3 dir = Direction;
		dir.y = 0.0f;
		dir.Normalize();

		float angle = fabsf(acosf(Vec.Dot(dir)));	// 角度算出。
		if (angle <= D3DXToRadian(viewAngle) * 0.5f) {
			// 視野角に入った。

			return true;
		}
	}
	return false;
}
