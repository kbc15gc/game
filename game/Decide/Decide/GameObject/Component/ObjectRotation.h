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

	// オブジェクトの方向を向く関数(Y軸無視)。
	// 引数：	見たいオブジェクト。
	void RotationToObject_XZ(const GameObject* Object);
	// オブジェクトの方向を向く関数(Y軸無視、補間)。
	// 引数：	見たいオブジェクト。
	//			何秒で回転終了するか。
	void RotationToObjectInterpolate_XZ(const GameObject* Object,float time);

	// 指定した方向を向く関数(Y軸無視)。
	// 引数：	向きベクトル。
	void RotationToDirection_XZ(const Vector3& Dir);
	// 指定した方向を向く関数(Y軸無視、補間)。
	// 引数：	向きベクトル。
	//			何秒で回転終了するか。
	void RotationToDirectionInterpolation_XZ(const Vector3& Dir, float time);

	// 指定した軸と角度を用いて回転する関数。
	// 引数：	軸(単位ベクトル)。
	//			回転角度(ラジアン)。
	void RotationAxis(const Vector3& axis, float angle);
	// 指定した軸と角度を用いて回転する関数(補間)。
	// 引数：	軸(単位ベクトル)。
	//			回転角度(ラジアン)。
	//			何秒で回転終了するか。
	void RotationAxisInterpolate(const Vector3& axis, float angle,float time);

	// 自由回転開始。
	// 引数：	回転量(一秒ごと、ラジアン)。
	//			回転軸。
	// ※回転を止めるにはこのクラスのStopFreeRotationを呼び出す。
	void StartFreeRotation(float rad, const Vector3 axis);

	// すべての自由回転(補間しないものを除く)を強制的に終了。
	inline void StopFreeRotation() {
		_isFreeRotation = false;
	}

	// 半回転より大きい角度を半回転より小さい角度と方向に変換して返却。
	float Optimization(float angle);

	// マイナスありの角度(-179〜180の角度)をマイナスのない値(0〜360の角度)に変換して返却。
	float ConvertAngle_From0Through360(float angle);

	// マイナスなしの角度(0〜360の角度)をマイナスのある値(-179〜180の角度)に変換して返却。
	float ConvertAngle_FromNegative179Through180(float angle);

private:
	void _EndRotation();
private:
	Vector3 _axis;
	float _targetAngle;	// 目標角度。
	float _angle;	// 現在の角度。
	float _power;	// 1フレームでの回転量。
	bool _isInterpolate = false;	// 補間中か。
	bool _isFreeRotation = false;

	bool _test = false;
};