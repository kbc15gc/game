#include "SearchViewAngle.h"

bool SearchViewAngle::IsDiscovery(const Vector3& pos1, const Vector3& pos2, const Vector3& Direction, float viewAngle, float range) {
	Vector3 Vec = pos2 - pos1;	// �������瑊��ւ̃x�N�g���B
	float Length = Vec.Length();
	if (Length <= range) {
		// �����鋗���ɓ������B

		Vec.Normalize();
		float angle = fabsf(acosf(Vec.Dot(Direction)));	// �p�x�Z�o�B
		if (angle <= D3DXToRadian(viewAngle)) {
			// ����p�ɓ������B

			return true;
		}
	}
	return false;
}
