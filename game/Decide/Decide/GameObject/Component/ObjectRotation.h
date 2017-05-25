#pragma once
#include "fbEngine\_Object\_Component\Component.h"

class ObjectRotation :public Component {
public:
	ObjectRotation(GameObject* g, Transform* t) :Component(g, t, typeid(this).name()) {};
	~ObjectRotation() {};
public:
	// オブジェクトの方向を向く関数(Y軸無視)。
	// 引数：	見たいオブジェクト。
	void RotationToObject_XZ(const GameObject& Object);
	// 指定した方向を向く関数(Y軸無視)。
	// 引数：	向きベクトル。
	void RotationToDirection_XZ(const Vector3& Dir);
	// 指定した軸と角度を用いて回転する関数。
	// 引数：	軸(単位ベクトル)。
	//			回転角度(ラジアン)。
	void RotationAxis(const Vector3& axis, float angle);
};