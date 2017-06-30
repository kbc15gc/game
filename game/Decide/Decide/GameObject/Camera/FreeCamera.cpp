#include "stdafx.h"
#include "FreeCamera.h"
#include "GameObject\Player\Player.h"
#include "PlayerCamera.h"
//�f�X�g���N�^�B
FreeCamera::~FreeCamera()
{
	
}

void FreeCamera::Awake()
{
	//�J�����R���|�[�l���g
	_Camera = AddComponent<Camera>();
	_Camera->SetNear(0.01f);
	_Camera->SetFar(10000.0f);

}

void FreeCamera::Start()
{
	_Player = (Player*)INSTANCE(GameObjectManager)->FindObject("Player");

	transform->SetPosition(Vector3(0, 70, 0));

	transform->SetRotation(Quaternion(0.0f, 0.0f, 0.0f, 1.0f));

	_Camera->SetTarget(transform->GetPosition());

	// �ŏ��͍X�V���Ȃ��B
	UnActivateFlg();
}

void FreeCamera::UpdateSubClass()
{

	//�_�b�V�����ʏ펞�������ăX�s�[�h�����߂�B
	DeicideCameraSpeed(_DashSpeed, _NormalSpeed);

	//�؂�ւ���O�̃v���C���[�J�����̈ʒu�ɖ߂��B
	Return();

	//��]�B
	FreeCameraRotation();

	//�㉺�ړ��B
	FreeCameraMoveUpDown();

	//�O�㍶�E�̈ړ��B
	Move();	
}

void FreeCamera::Move()
{
	Vector3 movespeed = Vector3::zero;
	//�Q�[���p�b�h����擾���������B
	Vector3 dir = Vector3::zero;
	//�R���g���[���[�ړ��B
	dir.x += (XboxInput(0)->GetAnalog(AnalogInputE::L_STICK).x / 32767.0f);
	dir.z += (XboxInput(0)->GetAnalog(AnalogInputE::L_STICK).y / 32767.0f);
#ifdef _DEBUG
	//�L�[�{�[�h(�f�o�b�O�p)�B
	if (KeyBoardInput->isPressed(DIK_W))
	{
		dir.z++;
	}
	if (KeyBoardInput->isPressed(DIK_S))
	{
		dir.z--;
	}
	if (KeyBoardInput->isPressed(DIK_A))
	{
		dir.x--;
	}
	if (KeyBoardInput->isPressed(DIK_D))
	{
		dir.x++;
	}
#endif
	//�ړ��������B
	if (dir.Length() != 0)
	{
		//�J��������݂������ɕϊ�
		Camera* camera = INSTANCE(GameObjectManager)->mainCamera;
		//�J�����̃r���[�s����Q�b�g
		D3DXMATRIX view = camera->GetViewMat();
		//�r���[�s��̋t�s��
		D3DXMATRIX viewinv;
		D3DXMatrixInverse(&viewinv, NULL, &view);
		//�J������Ԃ��猩���������̃x�N�g�����擾�B
		Vector3 cameraZ;
		cameraZ.x = viewinv.m[2][0];
		cameraZ.y = 0.0f;		//Y������Ȃ��B
		cameraZ.z = viewinv.m[2][2];
		cameraZ.Normalize();	//Y����ł������Ă���̂Ő��K������B
								//�J�������猩���������̃x�N�g�����擾�B
		Vector3 cameraX;
		cameraX.x = viewinv.m[0][0];
		cameraX.y = 0.0f;		//Y���͂���Ȃ��B
		cameraX.z = viewinv.m[0][2];
		cameraX.Normalize();	//Y����ł������Ă���̂Ő��K������B

		dir = dir * _MoveSpeed;

		Vector3 pos;
		pos = transform->GetPosition();

		//�J��������݂������Ɏˉe�B
		movespeed = movespeed + cameraX * dir.x;
		movespeed.y = dir.y;
		movespeed = movespeed + cameraZ * dir.z;

		transform->SetPosition(pos + movespeed);

		_Camera->SetTarget(Vector3(pos.x, pos.y, pos.z + 1.0f));
	}
}

void FreeCamera::FreeCameraRotation()
{
	//�Q�[���p�b�h������͂��ꂽ�l��_CameraRotationSpeed���|���ĉ�]���x�𒲐��B
	_R_STICK_Y += (XboxInput(0)->GetAnalog(AnalogInputE::R_STICK).y / 32767.0f)*_CameraRotationSpeed;
	_R_STICK_X += (XboxInput(0)->GetAnalog(AnalogInputE::R_STICK).x / 32767.0f)*_CameraRotationSpeed;
	Quaternion q;
	//�I�C���[�p����N�H�[�^�j�I���ɕϊ��B
	q.SetEuler(Vector3(_R_STICK_X, -_R_STICK_Y, 0.0f));
	transform->SetRotation(Quaternion(q.y, q.x, q.z, q.w));
}

void FreeCamera::FreeCameraMoveUpDown()
{
	Vector3 pos = transform->GetPosition();
	//�E�g���K�[�������݂ŏ㏸�B
	pos.y -= XboxInput(0)->GetAnalog(AnalogInputE::TRIGGER).x / 255.0f*_MoveSpeed;
	//���g���K�[�������݂ŉ��~�B
	pos.y += XboxInput(0)->GetAnalog(AnalogInputE::TRIGGER).y / 255.0f*_MoveSpeed;
	transform->SetPosition(pos);
}

void FreeCamera::Return()
{
	//P����������J�����̈ʒu��ύX�B
	if (KeyBoardInput->isPush(DIK_P))
	{
		PlayerCamera* playercamera = (PlayerCamera*)INSTANCE(GameObjectManager)->FindObject("PlayerCamera");
		//�؂�ւ���O�̃v���C���[�J�����̈ʒu�ɖ߂��B
		transform->SetPosition(playercamera->transform->GetPosition());
	}
}