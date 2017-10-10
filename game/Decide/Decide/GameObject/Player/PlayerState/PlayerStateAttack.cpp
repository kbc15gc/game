#include"stdafx.h"
#include "../Player.h"
#include "PlayerStateAttack.h"
#include "fbEngine\_Object\_Component\_3D\Animation.h"
#include "../Decide/Decide/AttackCollision.h"

PlayerStateAttack::PlayerStateAttack(Player* player) :
	PlayerState(player)
{
	_SE = INSTANCE(GameObjectManager)->AddNew<SoundSource>("SE", 0);
	_SE->Init("Asset/Sound/Player/PlayerAttack_00.wav");
	//�U���P
	{
		AttackCollisionParameter attackparam1;
		attackparam1.pos = Vector3(0.0f, 1.0f, 1.5f);
		attackparam1.rot = Quaternion::Identity;
		attackparam1.scale = Vector3(1.5f, 1.5f, 1.5f);
		attackparam1.attackframe = 10.0f;
		attackparam1.lifetime = 0.5f;
		attackparam1.atk = 120;
		attackparam1.attackboice = (int)Player::AttackBoice::Attack1;
		_AttackPram.push_back(attackparam1);
	}
	//�U���Q
	{
		AttackCollisionParameter attackparam2;
		attackparam2.pos = Vector3(0.0f, 1.0f, 1.5f);
		attackparam2.rot = Quaternion::Identity;
		attackparam2.scale = Vector3(1.5f, 1.5f, 1.5f);
		attackparam2.attackframe = 10;
		attackparam2.lifetime = 0.5f;
		attackparam2.atk = 100;
		attackparam2.attackboice = (int)Player::AttackBoice::Attack2;
		_AttackPram.push_back(attackparam2);
	}
	//�U���R
	{
		AttackCollisionParameter attackparam3;
		attackparam3.pos = Vector3(0.0f, 1.0f, 1.5f);
		attackparam3.rot = Quaternion::Identity;
		attackparam3.scale = Vector3(1.5f, 1.5f, 1.5f);
		attackparam3.attackframe = 10.0f;
		attackparam3.lifetime = 0.5f;
		attackparam3.atk = 120;
		attackparam3.attackboice = (int)Player::AttackBoice::Attack1;
		_AttackPram.push_back(attackparam3);
	}
	//�U���S
	{
		AttackCollisionParameter attackparam4;
		attackparam4.pos = Vector3(0.0f, 1.0f, 1.5f);
		attackparam4.rot = Quaternion::Identity;
		attackparam4.scale = Vector3(1.5f, 1.5f, 1.5f);
		attackparam4.attackframe = 10;
		attackparam4.lifetime = 0.5f;
		attackparam4.atk = 100;
		attackparam4.attackboice = (int)Player::AttackBoice::Attack2;
		_AttackPram.push_back(attackparam4);
	}
	//�U���T
	{
		AttackCollisionParameter attackparam5;
		attackparam5.pos = Vector3(0.0f, 1.0f, 1.5f);
		attackparam5.rot = Quaternion::Identity;
		attackparam5.scale = Vector3(1.5f, 1.5f, 1.5f);
		attackparam5.attackframe = 30;
		attackparam5.lifetime = 0.5f;
		attackparam5.atk = 200;
		attackparam5.attackboice = (int)Player::AttackBoice::Attack3;
		_AttackPram.push_back(attackparam5);
	}
	
}

PlayerStateAttack::~PlayerStateAttack()
{

}


void PlayerStateAttack::Update()
{
	//�ړ����x�ݒ�
	Vector3 movespeed = _Player->GetCharaCon().GetMoveSpeed();
	//�}�Ɏ~�܂�Ȃ��悤��
	movespeed.Scale(0.3f);
	_Player->GetCharaCon().SetMoveSpeed(movespeed);
	_Player->GetCharaCon().Execute();

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
		Dir();
		//�R���{�I
		_Player->_NextAttackAnimNo = (Player::AnimationNo)(_Player->_Anim->GetPlayAnimNo() + 1);
	}
	//�����蔻��쐬
 	switch (currentanimno)
	{
		//�U���P�̎�
	case (int)Player::AnimationNo::AnimationAttack01:
		Attack(_AttackPram[0]);
		break;
		//�U���Q�̎�
	case (int)Player::AnimationNo::AnimationAttack02:
		Attack(_AttackPram[1]);
		break;
		//�U���R�̎�
	case (int)Player::AnimationNo::AnimationAttack03:
		Attack(_AttackPram[2]);
		break;
	case (int)Player::AnimationNo::AnimationAttack04:
		Attack(_AttackPram[3]);
		break;
	case (int)Player::AnimationNo::AnimationAttack05:
		Attack(_AttackPram[4]);
		break;
	}
	
}

void PlayerStateAttack::Enter()
{

}

void PlayerStateAttack::Leave()
{

}

void PlayerStateAttack::Attack(AttackCollisionParameter pram)
{
	//�U�����̂����蔻��쐬
	//���݂̃t���[���擾
	const int nowFrame = _Player->_Anim->NowFrame();
	if (nowFrame == pram.attackframe)
	{
		//�U�����̃T�E���h�Đ��B
		_SE->Play(false);
		//�U���{�C�X�Đ�
		_Player->_AttackBoiceSound[pram.attackboice]->Play(false);
		//�U���R���W�����쐬
		AttackCollision* attack = INSTANCE(GameObjectManager)->AddNew<AttackCollision>("attack01", 1);
		if (_Player->GetEquipment()) {
			attack->Create(move(_Player->_PlayerParam->GiveDamageMass(false,false, _Player->GetEquipment()->weapon, pram.atk)),  pram.pos, pram.rot, pram.scale, AttackCollision::CollisionMaster::Player, pram.lifetime, 0.0f, _Player->transform);
		}
	}
}

void PlayerStateAttack::Dir()
{
	//�ړ����x
	Vector3 movespeed = Vector3::zero;
	movespeed.y = _Player->_CharacterController->GetMoveSpeed().y;

	//�L�[�{�[�h��J�@or�@�p�b�h��A�{�^���ŃW�����v
	if (KeyBoardInput->isPush(DIK_J) || XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_A))
	{
		//�n�ʏ�ɂ���ꍇ
		if (_Player->_CharacterController->IsOnGround() == true && !_Player->_Speak)
		{
			//�W�����v�p���[��ݒ�
			movespeed.y = _JumpSpeed;
			//�L�����N�^�[�R���g���[���[���W�����v��
			_Player->_CharacterController->Jump();
		}
	}

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

	//�L�����N�^�[�R���g���[���[�X�V
	_Player->_CharacterController->SetMoveSpeed(movespeed);
	_Player->_CharacterController->Execute();
}
