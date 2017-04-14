#include "SearchViewAngle.h"

bool SearchViewAngle::IsDiscovery(const Vector3& pos1, const Vector3& pos2, const Vector3& Direction, float viewAngle, float range) {
	Vector3 Vec = pos2 - pos1;	// 自分から相手へのベクトル。
	float Length = Vec.Length();
	if (Length <= range) {
		// 見える距離に入った。

		Vec.Normalize();
		float angle = fabsf(acosf(Vec.Dot(Direction)));	// 角度算出。
		if (angle <= D3DXToRadian(viewAngle)) {
			// 視野角に入った。

			return true;
		}
	}
	return false;
}
