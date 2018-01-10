#include"stdafx.h"
#include "ObjectRotation.h"

void ObjectRotation::Update() {
	if (_isFreeRotation) {
		_angle += _power * Time::DeltaTime();

		if (_isInterpolate) {
			char text[256];

			if (_power > 0.0f){
				// ��]�������v���X�B
				//if (_targetAngle < _angle) {
				//	// ���݂̊p�x����ڕW�p�x�܂ł̊ԂɃv���X�ƃ}�C�i�X�̋��ڂ�����B
				//	_targetAngle = ConvertAngle_From0Through360(_targetAngle);
				//}

				//float angle = ConvertAngle_From0Through360(_angle);
				//float target = ConvertAngle_From0Through360(_targetAngle);


				//if(angle >= target){
				if (_angle >= _targetAngle) {

					// �ڕW�p�x�𒴂����B
					
					//if (_test) {
					//	sprintf(text, "power = %f, nowAngle = %f, targetAngle = %f\n", D3DXToDegree(_power), D3DXToDegree(angle), D3DXToDegree(target));
					//	OutputDebugString(text);
					//}

					// ��]�I���B
					_EndRotation();
				}
			}
			else if (_power < 0.0f) {

				//float angle = ConvertAngle_FromNegative179Through180(_angle);
				//float target = ConvertAngle_FromNegative179Through180(_targetAngle);


				//if (angle <= target) {
				if (_angle <= _targetAngle) {

					// �ڕW�p�x�𒴂����B

					//if (_test) {
					//	sprintf(text, "power = %f, nowAngle = %f, targetAngle = %f\n", D3DXToDegree(_power), D3DXToDegree(angle), D3DXToDegree(target));
					//	OutputDebugString(text);
					//}

					// ��]�I���B
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
	MyToObject.y = 0.0f;	// �K�v�H�B
	MyToObject.Normalize();

	// ����������������X��Z�����]�p�x�����߂�B
	// �����܂�̂͐�Ίp�x�B
	float angle  = atan2f(MyToObject.x, MyToObject.z);
	// �N�H�[�^�j�I���쐬�B
	Quaternion Rota = Quaternion::Identity;
	Rota.SetRotation(Vector3::axisY, angle);

	// �N�H�[�^�j�I���ݒ�B
	gameObject->transform->SetRotation(Rota);

}

void ObjectRotation::RotationToObjectInterpolate_XZ(const GameObject* Object, float time){
	_isInterpolate = true;
	_isFreeRotation = true;

	Vector3 MyToObject = Object->transform->GetPosition() - gameObject->transform->GetPosition();
	MyToObject.y = 0.0f;	// �K�v�H�B
	MyToObject.Normalize();

	// �ڕW�p�x���Z�o�B
	// ����������������X��Z�����]�p�x�����߂�B
	// �����܂�̂͐�Ίp�x�B
	_targetAngle = atan2f(MyToObject.x, MyToObject.z);
	_targetAngle = Optimization(_targetAngle);

	float angle = _targetAngle;

	// ���݂̊p�x�����߂�B
	_angle = atan2f(transform->GetForward().x, transform->GetForward().z);
	_angle = Optimization(_angle);

	// ���݂̌X������̉�]�ʂ��v�Z�B
	angle -= _angle;
	angle = Optimization(angle);

	_power = angle * (1.0f / time);	// ��b�Ԃł̉�]�ʂ��Z�o�B
	_axis = Vector3::up;
	_axis.Normalize();
}

void ObjectRotation::RotationToDirection_XZ(const Vector3& dir) {
	Vector3 direction = dir;
	direction.y = 0.0f;	// �K�v�H�B
	direction.Normalize();
	// ����������������X��Z�����]�p�x�����߂�B
	// �����܂�̂͐�Ίp�x�B
	float angle = atan2f(direction.x, direction.z);
	// �N�H�[�^�j�I���쐬�B
	Quaternion Rota = Quaternion::Identity;
	Rota.SetRotation(Vector3::axisY, angle);

	// �N�H�[�^�j�I���ݒ�B
	gameObject->transform->SetRotation(Rota);
}

void ObjectRotation::RotationToDirectionInterpolation_XZ(const Vector3& Dir, float time) {
	//Vector3 direction = Dir;
	//direction.y = 0.0f;	// �K�v�H�B
	//direction.Normalize();

	_test = true;


	_isInterpolate = true;
	_isFreeRotation = true;

	_axis = Vector3::up;

	// �ڕW�p�x���Z�o�B
	// ����������������X��Z�����]�p�x�����߂�B
	// �����܂�̂͐�Ίp�x�B
	_targetAngle = atan2f(Dir.x, Dir.z);
	_targetAngle = Optimization(_targetAngle);

	float angle = _targetAngle;

	// ���݂̊p�x�����߂�B
	_angle = atan2f(transform->GetForward().x, transform->GetForward().z);
	_angle = Optimization(_angle);

	// ���݂̌X������̉�]�ʂ��v�Z�B
	angle -= _angle;
	angle = Optimization(angle);

	_power = angle * (1.0f / time);	// ��b�Ԃł̉�]�ʂ��Z�o�B

	if (_power > 0.0f) {
		// ��]�������v���X�B
		if (_targetAngle < _angle) {
			// ���݂̊p�x����ڕW�p�x�܂ł̊ԂɃv���X�ƃ}�C�i�X�̋��ڂ�����B
			_targetAngle = ConvertAngle_From0Through360(_targetAngle);
		}
	}
	else if (_power < 0.0f) {
		// ��]�������}�C�i�X�B
		if (_targetAngle > _angle) {
			// ���݂̊p�x����ڕW�p�x�܂ł̊ԂɃv���X�ƃ}�C�i�X�̋��ڂ�����B
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

	// ���݂̊p�x�����߂�B
	_angle = atan2f(transform->GetForward().x, transform->GetForward().z);
	_angle = Optimization(_angle);
	
	// ��]�ʂ��œK���B
	angle = Optimization(angle);

	// �ڕW�p�x���Z�o�B
	// ����������������X��Z�����]�p�x�����߂�B
	// �����܂�̂͐�Ίp�x�B
	_targetAngle = Optimization(_angle + angle);

	_power = angle * (1.0f / time);	// ��b�Ԃł̉�]�ʂ��Z�o�B

	_axis = axis;
	_axis.Normalize();
}

void ObjectRotation::StartFreeRotation(float rad, const Vector3 axis) {
	_isInterpolate = false;
	_isFreeRotation = true;
	_axis = axis;

	// ���݂̊p�x�����߂�B
	_angle = atan2f(transform->GetForward().x, transform->GetForward().z);
	_angle = Optimization(_angle);

	_power = Optimization(rad);
}

float ObjectRotation::Optimization(float angle) {
	if (fabsf(angle) > D3DXToRadian(180.0f)) {
		// �傫����]�ʂ��擾�����B

		float wark = min(1.0f, max(-1.0f, angle / fabsf(angle)));	// �����ۑ��B
		angle = D3DXToRadian(360.0f) - fabsf(angle);	// �������l�ɕϊ��B
		angle = angle * wark * -1.0f;	// ���Ƃ̕����Ƀ}�C�i�X���|���ĉ�]�����𔽓]�B
	}
	return angle;
}

float ObjectRotation::ConvertAngle_From0Through360(float angle) {
	if (angle < 0.0f) {
		// �}�C�i�X�̒l�����͂��ꂽ�B
		while (angle < D3DXToRadian(-360.0f)) {
			angle += D3DXToRadian(360.0f);
		}

		// 0�`360�̒l�ɕϊ�����B
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
		float wark = min(1.0f, max(-1.0f, angle / fabsf(angle)));	// �����ۑ��B

		float angleWork = fabsf(angle);
		while (angleWork > D3DXToRadian(360.0f)) {
			angleWork -= D3DXToRadian(360.0f);
		}

		// -179�`180�̒l�ɕϊ�����B
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
