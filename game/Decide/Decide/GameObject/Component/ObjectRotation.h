#pragma once
#include "fbEngine\_Object\_Component\Component.h"

class ObjectRotation :public Component {
public:
	ObjectRotation(GameObject* g, Transform* t) :Component(g, t, typeid(this).name()) {
#ifdef _DEBUG
		mbstowcs_s(nullptr, name, typeid(*this).name(), strlen(typeid(*this).name()));
#endif

	};
	~ObjectRotation() {};
public:
	void Update()override;

	// �I�u�W�F�N�g�̕����������֐�(Y������)�B
	// �����F	�������I�u�W�F�N�g�B
	void RotationToObject_XZ(const GameObject* Object);
	// �I�u�W�F�N�g�̕����������֐�(Y�������A���)�B
	// �����F	�������I�u�W�F�N�g�B
	//			���b�ŉ�]�I�����邩�B
	void RotationToObjectInterpolate_XZ(const GameObject* Object,float time);

	// �w�肵�������������֐�(Y������)�B
	// �����F	�����x�N�g���B
	void RotationToDirection_XZ(const Vector3& Dir);
	// �w�肵�������������֐�(Y�������A���)�B
	// �����F	�����x�N�g���B
	//			���b�ŉ�]�I�����邩�B
	void RotationToDirectionInterpolation_XZ(const Vector3& Dir, float time);

	// �w�肵�����Ɗp�x��p���ĉ�]����֐��B
	// �����F	��(�P�ʃx�N�g��)�B
	//			��]�p�x(���W�A��)�B
	void RotationAxis(const Vector3& axis, float angle);
	// �w�肵�����Ɗp�x��p���ĉ�]����֐�(���)�B
	// �����F	��(�P�ʃx�N�g��)�B
	//			��]�p�x(���W�A��)�B
	//			���b�ŉ�]�I�����邩�B
	void RotationAxisInterpolate(const Vector3& axis, float angle,float time);

	// ���R��]�J�n�B
	// �����F	��]��(��b���ƁA���W�A��)�B
	//			��]���B
	// ����]���~�߂�ɂ͂��̃N���X��StopFreeRotation���Ăяo���B
	void StartFreeRotation(float rad, const Vector3 axis);

	// ���ׂĂ̎��R��](��Ԃ��Ȃ����̂�����)�������I�ɏI���B
	inline void StopFreeRotation() {
		_isFreeRotation = false;
	}

	// ����]���傫���p�x�𔼉�]��菬�����p�x�ƕ����ɕϊ����ĕԋp�B
	float Optimization(float angle);

	// �}�C�i�X����̊p�x(-179�`180�̊p�x)���}�C�i�X�̂Ȃ��l(0�`360�̊p�x)�ɕϊ����ĕԋp�B
	float ConvertAngle_From0Through360(float angle);

	// �}�C�i�X�Ȃ��̊p�x(0�`360�̊p�x)���}�C�i�X�̂���l(-179�`180�̊p�x)�ɕϊ����ĕԋp�B
	float ConvertAngle_FromNegative179Through180(float angle);

private:
	void _EndRotation();
private:
	Vector3 _axis;
	float _targetAngle;	// �ڕW�p�x�B
	float _angle;	// ���݂̊p�x�B
	float _power;	// 1�t���[���ł̉�]�ʁB
	bool _isInterpolate = false;	// ��Ԓ����B
	bool _isFreeRotation = false;

	bool _test = false;
};