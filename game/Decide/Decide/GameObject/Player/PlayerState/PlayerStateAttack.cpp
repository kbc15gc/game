#include"stdafx.h"
#include "../Player.h"
#include "PlayerStateAttack.h"
#include "fbEngine\_Object\_Component\_3D\Animation.h"
#include "../Decide/Decide/AttackCollision.h"

namespace
{
	const int Attack = 10;
}

PlayerStateAttack::PlayerStateAttack(Player* player) :
	PlayerState(player)
{
	_SE = INSTANCE(GameObjectManager)->AddNew<SoundSource>("SE", 0);
	_SE->Init("Asset/Sound/PlayerAttack_00.wav");

	//�U���P
	{
		attackpram1.pos = Vector3(0.0f, 0.0f, 2.0f);
		attackpram1.rot = Quaternion::Identity;
		attackpram1.scale = Vector3::one;
		attackpram1.attackframe = 10.0f;
		attackpram1.lifetime = 0.5f;
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
	else if (XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_X) || KeyBoardInput->isPush(DIK_SPACE)
		&& currentanimno <= (int)Player::AnimationNo::AnimationAttackStart
		&& currentanimno > (int)Player::AnimationNo::AnimationAttackEnd
		&& currentanimno == (int)_Player->_NowAttackAnimNo
		)
	{
		//�R���{�I
		_Player->_NextAttackAnimNo = (Player::AnimationNo)(_Player->_Anim->GetPlayAnimNo() - 1);
	}

	//�t���[����10�̎������蔻��쐬
	Attack(attackpram1);

	
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
		attack->Create(pram.pos, pram.rot, pram.scale, AttackCollision::CollisionMaster::Player, pram.lifetime, _Player->transform);
	}
}