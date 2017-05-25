#pragma once
#include "fbEngine\_Object\_Component\Component.h"

class ObjectRotation :public Component {
public:
	ObjectRotation(GameObject* g, Transform* t) :Component(g, t, typeid(this).name()) {};
	~ObjectRotation() {};
public:
	// �I�u�W�F�N�g�̕����������֐�(Y������)�B
	// �����F	�������I�u�W�F�N�g�B
	void RotationToObject_XZ(const GameObject& Object);
	// �w�肵�������������֐�(Y������)�B
	// �����F	�����x�N�g���B
	void RotationToDirection_XZ(const Vector3& Dir);
	// �w�肵�����Ɗp�x��p���ĉ�]����֐��B
	// �����F	��(�P�ʃx�N�g��)�B
	//			��]�p�x(���W�A��)�B
	void RotationAxis(const Vector3& axis, float angle);
};