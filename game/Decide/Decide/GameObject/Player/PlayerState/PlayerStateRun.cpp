#include"stdafx.h"
#include "PlayerStateRun.h"
#include "../Player.h"
#include "fbEngine\_Object\_Component\_3D\Camera.h"

PlayerStateRun::PlayerStateRun(Player* player) :
	PlayerState(player)
{

}

PlayerStateRun::~PlayerStateRun()
{

}

void PlayerStateRun::Update()
{
	if (KeyBoardInput->isPressed(DIK_0))
	{
		debug = !debug;
		if (debug == true)
		{
			Vector3 pos = Vector3(_Player->transform->GetLocalPosition().x, _Player->transform->GetLocalPosition().y + 5.0f, _Player->transform->GetLocalPosition().z);
			_Player->transform->SetLocalPosition(pos);
		}
	}

	if (debug == true)
	{
		Vector3 movespeed = Vector3::zero;
		//�Q�[���p�b�h����擾��������
		Vector3 dir = Vector3::zero;
		//�R���g���[���[�ړ�
		dir.x += (XboxInput(0)->GetAnalog(AnalogInputE::L_STICK).x / 32767.0f);
		dir.z += (XboxInput(0)->GetAnalog(AnalogInputE::L_STICK).y / 32767.0f);
#ifdef _DEBUG
		//�L�[�{�[�h(�f�o�b�O�p)
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
		if (KeyBoardInput->isPressed(DIK_1))
		{
			dir.y++;
		}
#endif
		//�ړ�������
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

									// �����x�N�g���Ɉړ��ʂ�ώZ�B
			dir = dir * _Speed;
			//�J��������݂������Ɏˉe�B
			movespeed = movespeed + cameraX * dir.x;
			movespeed.y = movespeed.y;	//������͌Œ�Ȃ̂ł��̂܂܁B
			movespeed = movespeed + cameraZ * dir.z;

			//�ړ������������̃x�N�g��
			Vector3 vec = movespeed;
			//���K��
			vec.Normalize();
			//�x�N�g������p�x�����߂�
			//��]
			_Player->_Rotation->RotationToDirection_XZ(vec);

			_Player->transform->SetLocalPosition(_Player->transform->GetLocalPosition() + movespeed);
		}
	}
	else
	{
		//�ړ����x
		Vector3 movespeed = Vector3::zero;
		movespeed.y = _Player->GetCharaCon().GetMoveSpeed().y;

		//�L�[�{�[�h��J�@or�@�p�b�h��A�{�^���ŃW�����v
		if (KeyBoardInput->isPush(DIK_J) || XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_A))
		{
			//�n�ʏ�ɂ���ꍇ
			if (_Player->GetCharaCon().IsOnGround() == true)
			{
				//�W�����v�p���[��ݒ�
				movespeed.y = _JumpSpeed;
				//�L�����N�^�[�R���g���[���[���W�����v��
				_Player->GetCharaCon().Jump();
			}
		}

		//�Q�[���p�b�h����擾��������
		Vector3 dir = Vector3::zero;
		//�R���g���[���[�ړ�
		dir.x += (XboxInput(0)->GetAnalog(AnalogInputE::L_STICK).x / 32767.0f);
		dir.z += (XboxInput(0)->GetAnalog(AnalogInputE::L_STICK).y / 32767.0f);
#ifdef _DEBUG
		//�L�[�{�[�h(�f�o�b�O�p)
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
		//�ړ�������
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

									// �����x�N�g���Ɉړ��ʂ�ώZ�B
			dir = dir * _Speed;
			//�J��������݂������Ɏˉe�B
			movespeed = movespeed + cameraX * dir.x;
			movespeed.y = movespeed.y;	//������͌Œ�Ȃ̂ł��̂܂܁B
			movespeed = movespeed + cameraZ * dir.z;

			//�ړ������������̃x�N�g��
			Vector3 vec = movespeed;
			//���K��
			vec.Normalize();
			//�x�N�g������p�x�����߂�
			//��]
			_Player->_Rotation->RotationToDirection_XZ(vec);
		}
		//�ړ����Ă��Ȃ�
		if (dir.Length() < 0.0001f)
		{
			_Player->ChangeState(Player::State::Idol);
		}

		//�L�����N�^�[�R���g���[���[�X�V
		_Player->GetCharaCon().SetMoveSpeed(movespeed);
		_Player->GetCharaCon().Execute();
		//�U���ֈړ�
		if (XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_X) || KeyBoardInput->isPush(DIK_SPACE))
		{
			if (_Player->GetCharaCon().IsOnGround() == true)
			{
				_Player->_NextAttackAnimNo = Player::AnimationNo::AnimationAttack01;
				_Player->ChangeState(Player::State::Attack);
			}
		}
	}
	
}

void PlayerStateRun::Enter()
{

}

void PlayerStateRun::Leave()
{

}