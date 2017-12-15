#include"stdafx.h"
#include "ObjectRotation.h"


void ObjectRotation::Update() {
	if (_isFreeRotation) {
		float delta = Time::DeltaTime();

		if (_isInterpolate) {
			_counter += delta;

			if (_counter > _interval) {
				// 回転終了。

				_isInterpolate = false;
				_isFreeRotation = false;
			}
		}
		Quaternion work = transform->GetRotation();
		Quaternion work2 = Quaternion::Identity;
		work2.SetRotation(_axis, _angle * delta);
		work.Multiply(work2);
		transform->SetRotation(work);
	}
}

void ObjectRotation::RotationToObject_XZ(const GameObject* Object) {
	Vector3 MyToObject = Object->transform->GetPosition() - gameObject->transform->GetPosition();
	MyToObject.Normalize();

	// 向かせたい向きのXとZから回転角度を求める。
	// ※求まるのは絶対角度。
	float angle  = atan2f(MyToObject.x, MyToObject.z);
	// クォータニオン作成。
	Quaternion Rota = Quaternion::Identity;
	Rota.SetRotation(Vector3::axisY, angle);

	// クォータニオン設定。
	gameObject->transform->SetRotation(Rota);


	// 外積と内積を使った回転。
	// ※一応残しとく。
	{
		//D3DXMATRIX work = gameObject->transform->GetWorldMatrix();
		//Vector3 MyDirection = Vector3(work.m[2][0], work.m[2][1], work.m[2][2]);
		//MyDirection.y = 0.0f;	// Y軸成分は除く。
		//MyDirection.Normalize();

		//Vector3 MyToObject = Object.transform->GetPosition() - gameObject->transform->GetPosition();
		//MyToObject.y = 0.0f;	// Y軸成分は除く。
		//MyToObject.Normalize();

		//// 今の向きとプレイヤーまでの向きの角度を算出。

		//float rad = acosf(MyToObject.Dot(MyDirection));

		////// 1フレーム前の回転方向。
		////static float PrevAxisY = 0.0f;

		//if (rad > 0.001f) {
		//	// プレイヤーのほうを向いていない。

		//	// 回転方向算出。

		//	D3DXVECTOR3 Axis;

		//	D3DXVec3Cross(&Axis, &D3DXVECTOR3(MyDirection.x, MyDirection.y, MyDirection.z), &D3DXVECTOR3(MyToObject.x, MyToObject.y, MyToObject.z));
		//	//Vector3 Axis = EnemyToPlayer;
		//	//Axis.Cross(EnemyDir);
		//	//Axis.Normalize();


		//	//if (PrevAxisY >= 0.0001f) {
		//	//	// 初期値じゃない。

		//	//	if (Axis.y * PrevAxisY < 0.0f) {
		//	//		// 回転方向が反転したので強制的に戻す。
		//	//		OutputDebugString(_T("あああ"));
		//	//		Axis *= -1.0f;	// 再反転。
		//	//	}
		//	//}

		//	//// 更新。
		//	//PrevAxisY = Axis.y;

		//	// クォータニオンで回転。
		//	Quaternion quat = Quaternion::Identity;
		//	quat.SetRotation(Axis, rad);
		//	Quaternion Rotation = gameObject->transform->GetRotation();
		//	quat.Multiply(Rotation);
		//	gameObject->transform->SetLocalAngle(quat.GetAngle());
		//}
		////else {
		////	PrevAxisY = 0.0f;
		////}
	}
}

void ObjectRotation::RotationToObjectInterpolate_XZ(const GameObject* Object, float time){
	_interval = time;
	_counter = 0.0f;
	_isInterpolate = true;
	_isFreeRotation = true;

	Vector3 MyToObject = Object->transform->GetPosition() - gameObject->transform->GetPosition();
	//MyToObject.y = 0.0f;
	MyToObject.Normalize();

	// 向かせたい向きのXとZから回転角度を求める。
	// ※求まるのは絶対角度。
	float angle = atan2f(MyToObject.x, MyToObject.z);
	if (fabsf(angle) > D3DXToRadian(180.0f)) {
		// 大きい回転量を取得した。

		angle = (D3DXToRadian(360.0f) - angle);
		angle *= -1.0f;
	}

	// 現在の角度を求める。
	float nowAngle = atan2f(transform->GetForward().x, transform->GetForward().z);
	if (fabsf(nowAngle) > D3DXToRadian(180.0f)) {
		// 大きい回転量を取得した。

		nowAngle = (D3DXToRadian(360.0f) - nowAngle);
		nowAngle *= -1.0f;
	}

	// 現在の傾きからの回転量を計算。
	angle -= nowAngle;

	if (fabsf(angle) > D3DXToRadian(180.0f)) {
		// 大きい回転量を取得した。

		angle = (D3DXToRadian(360.0f) - angle);
		angle *= -1.0f;
	}

	_angle = angle * (1.0f / time);
	_axis = Vector3::up;
	_axis.Normalize();
}

void ObjectRotation::RotationToDirection_XZ(const Vector3& dir) {
	// 向かせたい向きのXとZから回転角度を求める。
	// ※求まるのは絶対角度。
	float angle = atan2f(dir.x, dir.z);
	// クォータニオン作成。
	Quaternion Rota = Quaternion::Identity;
	Rota.SetRotation(Vector3::axisY, angle);

	// クォータニオン設定。
	gameObject->transform->SetRotation(Rota);
}

void ObjectRotation::RotationToDirectionInterpolation_XZ(const Vector3& Dir, float time) {
	//Vector3 direction = Dir;
	//direction.Normalize();

	_interval = time;
	_counter = 0.0f;
	_isInterpolate = true;
	_isFreeRotation = true;

	_axis = Vector3::up;

	float angle = atan2f(Dir.x, Dir.z);
	if (fabsf(angle) > D3DXToRadian(180.0f)) {
		// 大きい回転量を取得した。

		angle = (D3DXToRadian(360.0f) - angle);
		angle *= -1.0f;
	}

	// 現在の角度を求める。
	float nowAngle = atan2f(transform->GetForward().x, transform->GetForward().z);
	if (fabsf(nowAngle) > D3DXToRadian(180.0f)) {
		// 大きい回転量を取得した。

		nowAngle = (D3DXToRadian(360.0f) - nowAngle);
		nowAngle *= -1.0f;
	}

	// 現在の傾きからの回転量を計算。
	angle -= nowAngle;

	if (fabsf(angle) > D3DXToRadian(180.0f)) {
		// 大きい回転量を取得した。

		angle = (D3DXToRadian(360.0f) - angle);
		angle *= -1.0f;
	}

	_angle = angle * (1.0f / time);	// 指定した時間で回転が完了するよう調整。
}

void ObjectRotation::RotationAxis(const Vector3& axis, float angle) {
	Quaternion quat;
	quat = Quaternion::Identity;
	quat.SetRotation(axis, angle);
	Quaternion rot = gameObject->transform->GetRotation();
	rot.Multiply(quat);
	gameObject->transform->SetLocalRotation(rot);
}

void ObjectRotation::RotationAxisInterpolate(const Vector3& axis, float angle, float time) {
	_interval = time;
	_counter = 0.0f;
	_isInterpolate = true;
	_isFreeRotation = true;

	_angle = angle * (1.0f / time);
	_axis = axis;
	_axis.Normalize();
}

void ObjectRotation::StartFreeRotation(float rad, const Vector3 axis) {
	_isInterpolate = false;
	_isFreeRotation = true;
	_axis = axis;
	_angle = rad;
}