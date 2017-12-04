#include "stdafx.h"
#include "FreeCamera.h"
#include "GameObject\Player\Player.h"
#include "PlayerCamera.h"

#ifdef _DEBUG
//�f�X�g���N�^�B
FreeCamera::~FreeCamera()
{
	
}

void FreeCamera::Awake()
{
	GameCamera::Awake();

	//�J�����R���|�[�l���g
	_Camera = AddComponent<Camera>();
	_Camera->SetNear(0.01f);
	_Camera->SetFar(10000.0f);

	//��_�J�����P�B
	PointCamera PointCamera1;
	PointCamera1.pos = Vector3{ 2600.0f,600.0f,-2500.0f };
	PointCamera1.rot = Vector2(-46.0f, -17.0f);
	_PointCameraInfo.push_back(PointCamera1);

	//��_�J�����Q�B
	PointCamera PointCamera2;
	PointCamera2.pos = Vector3{ 2600.0f,600.0f,2500.0f };
	PointCamera2.rot = Vector2(-134.0f, -18.2f);
	_PointCameraInfo.push_back(PointCamera2);

	//��_�J�����R�B
	PointCamera PointCamera3;
	PointCamera3.pos = Vector3{ -2600.0f,600.0f,2500.0f };
	PointCamera3.rot = Vector2(135.5f, -12.6f);
	_PointCameraInfo.push_back(PointCamera3);

	//��_�J�����S�B
	PointCamera PointCamera4;
	PointCamera4.pos = Vector3{ -2600.0f,600.0f,-2500.0f };
	PointCamera4.rot = Vector2(46.0f, -12.6f);
	_PointCameraInfo.push_back(PointCamera4);

}

void FreeCamera::Start()
{
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
	_Move();	

	//��_�J�����̐؂�ւ��B
	ChangePointCamera();
}

void FreeCamera::_Move()
{
	Vector3 movespeed = Vector3::zero;
	//�Q�[���p�b�h����擾���������B
	Vector3 dir = Vector3::zero;
	//�R���g���[���[�ړ��B
	dir.x += (XboxInput(0)->GetAnalog(AnalogE::L_STICK).x / 32767.0f);
	dir.z += (XboxInput(0)->GetAnalog(AnalogE::L_STICK).y / 32767.0f);

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
	_R_STICK_Y += (XboxInput(0)->GetAnalog(AnalogE::R_STICK).y / 32767.0f)*_CameraRotationSpeed;
	_R_STICK_X += (XboxInput(0)->GetAnalog(AnalogE::R_STICK).x / 32767.0f)*_CameraRotationSpeed;

	//�c�Ɖ��Ɉ��]���������珉�����B
	if (fabs(_R_STICK_X) >= 360)
	{
		_R_STICK_X = 0.0f;
	}
	if (fabs(_R_STICK_Y) >= 360)
	{
		_R_STICK_Y = 0.0f;
	}
	Quaternion q;
	//�I�C���[�p����N�H�[�^�j�I���ɕϊ��B
	q.SetEuler(Vector3(_R_STICK_X, -_R_STICK_Y, 0.0f));
	transform->SetRotation(Quaternion(q.y, q.x, q.z, q.w));
}

void FreeCamera::FreeCameraMoveUpDown()
{
	Vector3 pos = transform->GetPosition();
	//�E�g���K�[�������݂ŏ㏸�B
	pos.y -= XboxInput(0)->GetAnalog(AnalogE::TRIGGER).x / 255.0f*_MoveSpeed;
	//���g���K�[�������݂ŉ��~�B
	pos.y += XboxInput(0)->GetAnalog(AnalogE::TRIGGER).y / 255.0f*_MoveSpeed;
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

void FreeCamera::ChangePointCamera()
{
	//�\���L�[�E�Œ�_�J���������J��ɐ؂�ւ��B
	if ((XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_DPAD_RIGHT)))
	{

		//�Y�������z�񕪂𒴂��ĂȂ����`�F�b�N�B
		if (_InfoIndex < 4)
		{
			//��_�J�����p�̌����ƈʒu��ݒ�B
			_R_STICK_X = _PointCameraInfo[_InfoIndex].rot.x;
			_R_STICK_Y = _PointCameraInfo[_InfoIndex].rot.y;
			transform->SetPosition(_PointCameraInfo[_InfoIndex].pos);

			//�Y����������炷�B
			_InfoIndex++;
		}
		else
		{
			//�Y�������z�񕪂𒴂����̂Ŕz��̈�ԍŏ��ɖ߂��B
			_InfoIndex = 0;
			_R_STICK_X = _PointCameraInfo[_InfoIndex].rot.x;
			_R_STICK_Y = _PointCameraInfo[_InfoIndex].rot.y;
			transform->SetPosition(_PointCameraInfo[_InfoIndex].pos);
		}
	}
}

#endif // _DEBUG