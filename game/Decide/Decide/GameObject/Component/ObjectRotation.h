#pragma once
#include "fbEngine\Component.h"

class ObjectRotation :public Component {
public:
	ObjectRotation(GameObject* g, Transform* t) :Component(g, t, typeid(this).name()) {};
	~ObjectRotation() {};
public:
	// オブジェクトの方向を向く関数(Y軸無視)。
	// 引数：	見たいオブジェクト。
	void RotationToObject_XZ(const GameObject& Object);
};