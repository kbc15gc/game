#include"stdafx.h"
#include "../Player.h"
#include "PlayerStateAttack.h"
#include "fbEngine\_Object\_Component\_3D\Animation.h"
#include "../Decide/Decide/AttackCollision.h"

PlayerStateAttack::PlayerStateAttack(Player* player) :
	PlayerState(player)
{
	_SE = INSTANCE(GameObjectManager)->AddNew<SoundSource>("SE", 0);
	_SE->Init("Asset/Sound/PlayerAttack_00.wav");

	//�U���P
	{
		AttackCollisionParameter attackparam1;
		attackparam1.pos = Vector3(0.0f, 1.0f, 1.5f);
		attackparam1.rot = Quaternion::Identity;
		attackparam1.scale = Vector3(1.5f, 1.5f, 1.5f);
		attackparam1.attackframe = 10.0f;
		attackparam1.lifetime = 0.5f;
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
		_AttackPram.push_back(attackparam2);
	}
	//�U���R
	{
		AttackCollisionParameter attackparam3;
		attackparam3.pos = Vector3(0.0f, 1.0f, 1.5f);
		attackparam3.rot = Quaternion::Identity;
		attackparam3.scale = Vector3(1.5f, 1.5f, 1.5f);
		attackparam3.attackframe = 25;
		attackparam3.lifetime = 0.5f;
		_AttackPram.push_back(attackparam3);
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
	else if (XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_X)
		&& currentanimno >= (int)Player::AnimationNo::AnimationAttackStart
		&& currentanimno < (int)Player::AnimationNo::AnimationAttackEnd
		&& currentanimno == (int)_Player->_NowAttackAnimNo
		|| KeyBoardInput->isPush(DIK_SPACE)
		&& currentanimno >= (int)Player::AnimationNo::AnimationAttackStart
		&& currentanimno < (int)Player::AnimationNo::AnimationAttackEnd
		&& currentanimno == (int)_Player->_NowAttackAnimNo
		)
	{
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
		//�U���R���W�����쐬
		AttackCollision* attack = INSTANCE(GameObjectManager)->AddNew<AttackCollision>("attack01", 1);
		attack->Create(_Player->_PlayerParam->GiveDamageMass(),pram.pos, pram.rot, pram.scale, AttackCollision::CollisionMaster::Player, pram.lifetime, _Player->transform);

		_Player->_AttackValue->transform->SetLocalPosition(Vector3(pram.pos.x,pram.pos.y, pram.pos.z));
		string attackvalue = to_string(_Player->_PlayerParam->GiveDamageMass());
		_Player->_AttackValue->SetString(attackvalue.data());
	}
}