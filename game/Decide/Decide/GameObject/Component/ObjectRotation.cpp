#include"stdafx.h"
#include "ObjectRotation.h"

void ObjectRotation::RotationToObject_XZ(const GameObject& Object) {
	Vector3 MyToObject = Object.transform->GetPosition() - gameObject->transform->GetPosition();
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
