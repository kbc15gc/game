#include"stdafx.h"
#include "SearchViewAngle.h"

bool SearchViewAngle::IsDiscovery(const Vector3& pos1, const Vector3& pos2, const Vector3& Direction, float viewAngle, float range) {
	Vector3 Vec = pos2 - pos1;	// �������瑊��ւ̃x�N�g���B
	float Length = Vec.Length();
	if (Length <= range) {
		// �����鋗���ɓ������B

		// Y�������������B
		Vec.y = 0.0f;
		Vec.Normalize();
		Vector3 dir = Direction;
		dir.y = 0.0f;
		dir.Normalize();

		float angle = fabsf(acosf(Vec.Dot(dir)));	// �p�x�Z�o�B
		if (angle <= D3DXToRadian(viewAngle) * 0.5f) {
			// ����p�ɓ������B

			return true;
		}
	}
	return false;
}
