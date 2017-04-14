#include "../Player.h"
#include "PlayerStateAttack.h"
#include "fbEngine\Animation.h"
#include "../Decide/Decide/AttackCollision.h"

PlayerStateAttack::PlayerStateAttack(Player* player) :
	PlayerState(player)
{

}

PlayerStateAttack::~PlayerStateAttack()
{

}


void PlayerStateAttack::Update()
{
	//�ړ����x�ݒ�
	Vector3 movespeed = player->GetCharaCon().GetMoveSpeed();
	//�}�Ɏ~�܂�Ȃ��悤��
	movespeed.Scale(0.3f);
	player->GetCharaCon().SetMoveSpeed(movespeed);
	player->GetCharaCon().Execute();

	//�U�����̂����蔻��쐬
	//���݂̃t���[���擾
	const int nowFrame = player->_Anim->NowFrame();
	//�t���[����10�̎������蔻��쐬
	if (nowFrame == 10)
	{
		//�U���R���W�����쐬
		AttackCollision* attack = GameObjectManager::AddNew<AttackCollision>("attack01", 1);
		Transform* trans = attack->GetComponent<Transform>();
		trans->SetLocalPosition(player->transform->Local(Vector3(0.0f, 0.0f, 1.0f)));
		trans->SetLocalAngle(player->transform->GetLocalAngle());
		trans->UpdateTransform();
		attack->Create(Vector3(1.0f,1.0f,1.0f));
	}

	if (player->GetAnimIsPlay() != true)
	{
		player->ChangeState(Player::State::Idol);
	}
}

void PlayerStateAttack::Enter()
{

}

void PlayerStateAttack::Leave()
{

}