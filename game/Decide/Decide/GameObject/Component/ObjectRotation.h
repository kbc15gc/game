#pragma once
#include "fbEngine\Component.h"

class ObjectRotation :public Component {
public:
	ObjectRotation(GameObject* g, Transform* t) :Component(g, t, typeid(this).name()) {};
	~ObjectRotation() {};
public:
	// �I�u�W�F�N�g�̕����������֐�(Y������)�B
	// �����F	�������I�u�W�F�N�g�B
	void RotationToObject_XZ(const GameObject& Object);
};