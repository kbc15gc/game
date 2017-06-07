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
	//移動速度設定
	Vector3 movespeed = _Player->GetCharaCon().GetMoveSpeed();
	//急に止まらないように
	movespeed.Scale(0.3f);
	_Player->GetCharaCon().SetMoveSpeed(movespeed);
	_Player->GetCharaCon().Execute();

	//フレームが10の時あたり判定作成
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
	//攻撃時のあたり判定作成
	//現在のフレーム取得
	const int nowFrame = _Player->_Anim->NowFrame();
	if (nowFrame == attackframe)
	{
		//攻撃時のサウンド再生。
		_SE->Play(false);
		//攻撃コリジョン作成
		AttackCollision* attack = INSTANCE(GameObjectManager)->AddNew<AttackCollision>("attack01", 1);
		// ※テスト用(後で直してね)。
		{
			Quaternion rot;
			rot = Quaternion::Identity;
			attack->Create(Vector3(0.0f, 0.0f, 2.0f), rot/*player->transform->GetLocalAngle()*/, Vector3(1.0f, 1.0f, 1.0f)/*Vector3::one*/, AttackCollision::CollisionMaster::Player, 0.5f, _Player->transform);
		}
	}
}