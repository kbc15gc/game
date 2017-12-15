#include"stdafx.h"
#include "ObjectRotation.h"


void ObjectRotation::Update() {
	if (_isFreeRotation) {
		float delta = Time::DeltaTime();

		if (_isInterpolate) {
			_counter += delta;

			if (_counter > _interval) {
				// ��]�I���B

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

	// ����������������X��Z�����]�p�x�����߂�B
	// �����܂�̂͐�Ίp�x�B
	float angle  = atan2f(MyToObject.x, MyToObject.z);
	// �N�H�[�^�j�I���쐬�B
	Quaternion Rota = Quaternion::Identity;
	Rota.SetRotation(Vector3::axisY, angle);

	// �N�H�[�^�j�I���ݒ�B
	gameObject->transform->SetRotation(Rota);


	// �O�ςƓ��ς��g������]�B
	// ���ꉞ�c���Ƃ��B
	{
		//D3DXMATRIX work = gameObject->transform->GetWorldMatrix();
		//Vector3 MyDirection = Vector3(work.m[2][0], work.m[2][1], work.m[2][2]);
		//MyDirection.y = 0.0f;	// Y�������͏����B
		//MyDirection.Normalize();

		//Vector3 MyToObject = Object.transform->GetPosition() - gameObject->transform->GetPosition();
		//MyToObject.y = 0.0f;	// Y�������͏����B
		//MyToObject.Normalize();

		//// ���̌����ƃv���C���[�܂ł̌����̊p�x���Z�o�B

		//float rad = acosf(MyToObject.Dot(MyDirection));

		////// 1�t���[���O�̉�]�����B
		////static float PrevAxisY = 0.0f;

		//if (rad > 0.001f) {
		//	// �v���C���[�̂ق��������Ă��Ȃ��B

		//	// ��]�����Z�o�B

		//	D3DXVECTOR3 Axis;

		//	D3DXVec3Cross(&Axis, &D3DXVECTOR3(MyDirection.x, MyDirection.y, MyDirection.z), &D3DXVECTOR3(MyToObject.x, MyToObject.y, MyToObject.z));
		//	//Vector3 Axis = EnemyToPlayer;
		//	//Axis.Cross(EnemyDir);
		//	//Axis.Normalize();


		//	//if (PrevAxisY >= 0.0001f) {
		//	//	// �����l����Ȃ��B

		//	//	if (Axis.y * PrevAxisY < 0.0f) {
		//	//		// ��]���������]�����̂ŋ����I�ɖ߂��B
		//	//		OutputDebugString(_T("������"));
		//	//		Axis *= -1.0f;	// �Ĕ��]�B
		//	//	}
		//	//}

		//	//// �X�V�B
		//	//PrevAxisY = Axis.y;

		//	// �N�H�[�^�j�I���ŉ�]�B
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

	// ����������������X��Z�����]�p�x�����߂�B
	// �����܂�̂͐�Ίp�x�B
	float angle = atan2f(MyToObject.x, MyToObject.z);
	if (fabsf(angle) > D3DXToRadian(180.0f)) {
		// �傫����]�ʂ��擾�����B

		angle = (D3DXToRadian(360.0f) - angle);
		angle *= -1.0f;
	}

	// ���݂̊p�x�����߂�B
	float nowAngle = atan2f(transform->GetForward().x, transform->GetForward().z);
	if (fabsf(nowAngle) > D3DXToRadian(180.0f)) {
		// �傫����]�ʂ��擾�����B

		nowAngle = (D3DXToRadian(360.0f) - nowAngle);
		nowAngle *= -1.0f;
	}

	// ���݂̌X������̉�]�ʂ��v�Z�B
	angle -= nowAngle;

	if (fabsf(angle) > D3DXToRadian(180.0f)) {
		// �傫����]�ʂ��擾�����B

		angle = (D3DXToRadian(360.0f) - angle);
		angle *= -1.0f;
	}

	_angle = angle * (1.0f / time);
	_axis = Vector3::up;
	_axis.Normalize();
}

void ObjectRotation::RotationToDirection_XZ(const Vector3& dir) {
	// ����������������X��Z�����]�p�x�����߂�B
	// �����܂�̂͐�Ίp�x�B
	float angle = atan2f(dir.x, dir.z);
	// �N�H�[�^�j�I���쐬�B
	Quaternion Rota = Quaternion::Identity;
	Rota.SetRotation(Vector3::axisY, angle);

	// �N�H�[�^�j�I���ݒ�B
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
		// �傫����]�ʂ��擾�����B

		angle = (D3DXToRadian(360.0f) - angle);
		angle *= -1.0f;
	}

	// ���݂̊p�x�����߂�B
	float nowAngle = atan2f(transform->GetForward().x, transform->GetForward().z);
	if (fabsf(nowAngle) > D3DXToRadian(180.0f)) {
		// �傫����]�ʂ��擾�����B

		nowAngle = (D3DXToRadian(360.0f) - nowAngle);
		nowAngle *= -1.0f;
	}

	// ���݂̌X������̉�]�ʂ��v�Z�B
	angle -= nowAngle;

	if (fabsf(angle) > D3DXToRadian(180.0f)) {
		// �傫����]�ʂ��擾�����B

		angle = (D3DXToRadian(360.0f) - angle);
		angle *= -1.0f;
	}

	_angle = angle * (1.0f / time);	// �w�肵�����Ԃŉ�]����������悤�����B
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