#pragma once

// ����p����N���X�B
class SearchViewAngle {
public:
	// ����p�ɓ����Ă��邩����B
	// �����F	�����̈ʒu�B
	//			����p�ɓ����Ă��邩���肵�����I�u�W�F�N�g�̈ʒu�B
	//			�����B
	//			����p(�x)�B
	//			�����鋗���B
	bool IsDiscovery(const Vector3& pos1, const Vector3& pos2, const Vector3& Direction, float angle, float range);
};