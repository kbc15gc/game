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
	// �w�肵�����Ɗp�x��p���ĉ�]����֐��B
	// �����F	��(�P�ʃx�N�g��)�B
	//			��]�p�x(���W�A��)�B
	//			���b�ŉ�]�I�����邩�B
	void RotationAxisInterpolate(const Vector3& axis, float angle,float time);


private:
	float _counter = 0.0f;
	float _interval = 0.0f;
	Quaternion _rot;
	bool _isInterpolate = false;	// ��Ԓ����B
};