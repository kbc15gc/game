#include"stdafx.h"
#include "ObjectRotation.h"

void ObjectRotation::Update() {
	if (_isFreeRotation) {
		_angle += _power * Time::DeltaTime();

		if (_isInterpolate) {
			char text[256];

			if (_power > 0.0f){
				// 回転方向がプラス。
				//if (_targetAngle < _angle) {
				//	// 現在の角度から目標角度までの間にプラスとマイナスの境目がある。
				//	_targetAngle = ConvertAngle_From0Through360(_targetAngle);
				//}

				//float angle = ConvertAngle_From0Through360(_angle);
				//float target = ConvertAngle_From0Through360(_targetAngle);


				//if(angle >= target){
				if (_angle >= _targetAngle) {

					// 目標角度を超えた。
					
					//if (_test) {
					//	sprintf(text, "power = %f, nowAngle = %f, targetAngle = %f\n", D3DXToDegree(_power), D3DXToDegree(angle), D3DXToDegree(target));
					//	OutputDebugString(text);
					//}

					// 回転終了。
					_EndRotation();
				}
			}
			else if (_power < 0.0f) {

				//float angle = ConvertAngle_FromNegative179Through180(_angle);
				//float target = ConvertAngle_FromNegative179Through180(_targetAngle);


				//if (angle <= target) {
				if (_angle <= _targetAngle) {

					// 目標角度を超えた。

					//if (_test) {
					//	sprintf(text, "power = %f, nowAngle = %f, targetAngle = %f\n", D3DXToDegree(_power), D3DXToDegree(angle), D3DXToDegree(target));
					//	OutputDebugString(text);
					//}

					// 回転終了。
					_EndRotation();
				}
			}
		}

		Quaternion rot = Quaternion::Identity;
		rot.SetRotation(_axis, _angle);
		transform->SetRotation(rot);
	}
}

void ObjectRotation::RotationToObject_XZ(const GameObject* Object) {
	Vector3 MyToObject = Object->transform->GetPosition() - gameObject->transform->GetPosition();
	MyToObject.y = 0.0f;	// 必要？。
	MyToObject.Normalize();

	// 向かせたい向きのXとZから回転角度を求める。
	// ※求まるのは絶対角度。
	float angle  = atan2f(MyToObject.x, MyToObject.z);
	// クォータニオン作成。
	Quaternion Rota = Quaternion::Identity;
	Rota.SetRotation(Vector3::axisY, angle);

	// クォータニオン設定。
	gameObject->transform->SetRotation(Rota);

}

void ObjectRotation::RotationToObjectInterpolate_XZ(const GameObject* Object, float time){
	_isInterpolate = true;
	_isFreeRotation = true;

	Vector3 MyToObject = Object->transform->GetPosition() - gameObject->transform->GetPosition();
	MyToObject.y = 0.0f;	// 必要？。
	MyToObject.Normalize();

	// 目標角度を算出。
	// 向かせたい向きのXとZから回転角度を求める。
	// ※求まるのは絶対角度。
	_targetAngle = atan2f(MyToObject.x, MyToObject.z);
	_targetAngle = Optimization(_targetAngle);

	float angle = _targetAngle;

	// 現在の角度を求める。
	_angle = atan2f(transform->GetForward().x, transform->GetForward().z);
	_angle = Optimization(_angle);

	// 現在の傾きからの回転量を計算。
	angle -= _angle;
	angle = Optimization(angle);

	_power = angle * (1.0f / time);	// 一秒間での回転量を算出。
	_axis = Vector3::up;
	_axis.Normalize();
}

void ObjectRotation::RotationToDirection_XZ(const Vector3& dir) {
	Vector3 direction = dir;
	direction.y = 0.0f;	// 必要？。
	direction.Normalize();
	// 向かせたい向きのXとZから回転角度を求める。
	// ※求まるのは絶対角度。
	float angle = atan2f(direction.x, direction.z);
	// クォータニオン作成。
	Quaternion Rota = Quaternion::Identity;
	Rota.SetRotation(Vector3::axisY, angle);

	// クォータニオン設定。
	gameObject->transform->SetRotation(Rota);
}

void ObjectRotation::RotationToDirectionInterpolation_XZ(const Vector3& Dir, float time) {
	//Vector3 direction = Dir;
	//direction.y = 0.0f;	// 必要？。
	//direction.Normalize();

	_test = true;


	_isInterpolate = true;
	_isFreeRotation = true;

	_axis = Vector3::up;

	// 目標角度を算出。
	// 向かせたい向きのXとZから回転角度を求める。
	// ※求まるのは絶対角度。
	_targetAngle = atan2f(Dir.x, Dir.z);
	_targetAngle = Optimization(_targetAngle);

	float angle = _targetAngle;

	// 現在の角度を求める。
	_angle = atan2f(transform->GetForward().x, transform->GetForward().z);
	_angle = Optimization(_angle);

	// 現在の傾きからの回転量を計算。
	angle -= _angle;
	angle = Optimization(angle);

	_power = angle * (1.0f / time);	// 一秒間での回転量を算出。

	if (_power > 0.0f) {
		// 回転方向がプラス。
		if (_targetAngle < _angle) {
			// 現在の角度から目標角度までの間にプラスとマイナスの境目がある。
			_targetAngle = ConvertAngle_From0Through360(_targetAngle);
		}
	}
	else if (_power < 0.0f) {
		// 回転方向がマイナス。
		if (_targetAngle > _angle) {
			// 現在の角度から目標角度までの間にプラスとマイナスの境目がある。
			_targetAngle = ConvertAngle_From0Through360(_targetAngle);
			_angle = ConvertAngle_From0Through360(_angle);
		}
	}
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
	_isInterpolate = true;
	_isFreeRotation = true;

	// 現在の角度を求める。
	_angle = atan2f(transform->GetForward().x, transform->GetForward().z);
	_angle = Optimization(_angle);
	
	// 回転量を最適化。
	angle = Optimization(angle);

	// 目標角度を算出。
	// 向かせたい向きのXとZから回転角度を求める。
	// ※求まるのは絶対角度。
	_targetAngle = Optimization(_angle + angle);

	_power = angle * (1.0f / time);	// 一秒間での回転量を算出。

	_axis = axis;
	_axis.Normalize();
}

void ObjectRotation::StartFreeRotation(float rad, const Vector3 axis) {
	_isInterpolate = false;
	_isFreeRotation = true;
	_axis = axis;

	// 現在の角度を求める。
	_angle = atan2f(transform->GetForward().x, transform->GetForward().z);
	_angle = Optimization(_angle);

	_power = Optimization(rad);
}

float ObjectRotation::Optimization(float angle) {
	if (fabsf(angle) > D3DXToRadian(180.0f)) {
		// 大きい回転量を取得した。

		float wark = min(1.0f, max(-1.0f, angle / fabsf(angle)));	// 符号保存。
		angle = D3DXToRadian(360.0f) - fabsf(angle);	// 小さい値に変換。
		angle = angle * wark * -1.0f;	// もとの符号にマイナスを掛けて回転方向を反転。
	}
	return angle;
}

float ObjectRotation::ConvertAngle_From0Through360(float angle) {
	if (angle < 0.0f) {
		// マイナスの値が入力された。
		while (angle < D3DXToRadian(-360.0f)) {
			angle += D3DXToRadian(360.0f);
		}

		// 0～360の値に変換する。
		angle = D3DXToRadian(360.0f) + angle;
	}
	else if(angle > 0.0f){
		while (angle > D3DXToRadian(360.0f)) {
			angle -= D3DXToRadian(360.0f);
		}
	}
	return angle;
}

float ObjectRotation::ConvertAngle_FromNegative179Through180(float angle) {
	if (fabsf(angle) > D3DXToRadian(180.0f)) {
		float wark = min(1.0f, max(-1.0f, angle / fabsf(angle)));	// 符号保存。

		float angleWork = fabsf(angle);
		while (angleWork > D3DXToRadian(360.0f)) {
			angleWork -= D3DXToRadian(360.0f);
		}

		// -179～180の値に変換する。
		angle = -1.0f * wark * (D3DXToRadian(180.0f) - (angleWork - D3DXToRadian(180.0f)));
	}
	return angle;
}

void ObjectRotation::_EndRotation() {
	_test = false;


	_angle = _targetAngle;

	_isInterpolate = false;
	_isFreeRotation = false;
}
