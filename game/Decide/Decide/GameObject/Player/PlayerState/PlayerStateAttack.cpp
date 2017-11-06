#include"stdafx.h"
#include "../Player.h"
#include "PlayerStateAttack.h"
#include "fbEngine\_Object\_Component\_3D\Animation.h"
#include "../Decide/Decide/AttackCollision.h"
#include "GameObject\Enemy\EnemyCharacter.h"

namespace
{
	//�v���C���[�̎���B
	float Circumference = 6.0f;
}

PlayerStateAttack::PlayerStateAttack(Player* player) :
	PlayerState(player)
{
}

PlayerStateAttack::~PlayerStateAttack()
{

}


void PlayerStateAttack::Update()
{
	//���݂̃A�j���[�V�����i���o�[
	int currentanimno = _Player->_Anim->GetPlayAnimNo();
	//�A�j���[�V�����̍Đ����I��� && ���̃A�j���[�V�������Ȃ��ꍇ�A�ҋ@��ԂɕύX
	if (_Player->_Anim->GetPlaying() != true && _Player->_NextAttackAnimNo == Player::AnimationNo::AnimationInvalid)
	{
		_Player->ChangeState(Player::State::Idol);
	}
	//�U���{�^�����������B
	//�A���B
	else if (XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_X)
		&& currentanimno >= (int)Player::AnimationNo::AnimationAttackStart
		&& currentanimno < (int)Player::AnimationNo::AnimationAttackEnd
		&& currentanimno == (int)_Player->_NowAttackAnimNo
		)
	{
		//������ς���B
		//Dir();
		//�R���{�I
		_Player->_NextAttackAnimNo = (Player::AnimationNo)(_Player->_Anim->GetPlayAnimNo() + 1);
	}

	//�߂��̃G�l�~�[�̕����������B
	if (EnemyCharacter::nearEnemyInfo.length <= Circumference)
	{
		_Player->_Rotation->RotationToObject_XZ(EnemyCharacter::nearEnemyInfo.object);
	}

	//�ړ����x
	Vector3 movespeed = _Player->_CharacterController->GetMoveSpeed();
	movespeed.Scale(0.7f);
	//�L�����N�^�[�R���g���[���[�X�V
	_Player->_CharacterController->SetMoveSpeed(movespeed);
	_Player->_CharacterController->Execute();
}

void PlayerStateAttack::Enter()
{

}

void PlayerStateAttack::Leave()
{

}

void PlayerStateAttack::Dir()
{
	//�ړ����x
	Vector3 movespeed = Vector3::zero;

	//�Q�[���p�b�h����擾��������
	Vector3 dir = Vector3::zero;
	//�R���g���[���[�ړ�
	dir.x += (XboxInput(0)->GetAnalog(AnalogE::L_STICK).x / 32767.0f);
	dir.z += (XboxInput(0)->GetAnalog(AnalogE::L_STICK).y / 32767.0f);
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
								//�_�b�V���{�^���̏ꍇ
		//�ʏ�̃X�s�[�h
		dir = dir;
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
}