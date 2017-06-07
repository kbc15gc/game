#include"stdafx.h"
#include "../Player.h"
#include "PlayerStateAttack.h"
#include "fbEngine\_Object\_Component\_3D\Animation.h"
#include "../Decide/Decide/AttackCollision.h"

namespace
{
	const int Attack1 = 10;
}

PlayerStateAttack::PlayerStateAttack(Player* player) :
	PlayerState(player)
{
	_SE = INSTANCE(GameObjectManager)->AddNew<SoundSource>("SE", 0);
	_SE->Init("Asset/Sound/PlayerAttack_00.wav");
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

	//�t���[����10�̎������蔻��쐬
	Attack(Attack1);

	if (_Player->GetAnimIsPlay() != true)
	{
		_Player->ChangeState(Player::State::Idol);
	}
}

void PlayerStateAttack::Enter()
{

}

void PlayerStateAttack::Leave()
{

}

void PlayerStateAttack::Attack(int attackframe)
{
	//�U�����̂����蔻��쐬
	//���݂̃t���[���擾
	const int nowFrame = _Player->_Anim->NowFrame();
	if (nowFrame == attackframe)
	{
		//�U�����̃T�E���h�Đ��B
		_SE->Play(false);
		//�U���R���W�����쐬
		AttackCollision* attack = INSTANCE(GameObjectManager)->AddNew<AttackCollision>("attack01", 1);
		// ���e�X�g�p(��Œ����Ă�)�B
		{
			Quaternion rot;
			rot = Quaternion::Identity;
			attack->Create(Vector3(0.0f, 0.0f, 2.0f), rot/*player->transform->GetLocalAngle()*/, Vector3(1.0f, 1.0f, 1.0f)/*Vector3::one*/, AttackCollision::CollisionMaster::Player, 0.5f, _Player->transform);
		}
	}
}