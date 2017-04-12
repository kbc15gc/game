#include "PlayerStateRun.h"
#include "Player.h"

namespace
{
	const float SPEED = 30.0f;
	const float JUMP_POWER = 15.0f;
}

PlayerStateRun::PlayerStateRun(Player* player) :
	PlayerState(player)
{

}

PlayerStateRun::~PlayerStateRun()
{

}

void PlayerStateRun::Update()
{
	//移動速度
	Vector3 movespeed = Vector3::zero;
	movespeed.y = player->GetCharaCon().GetMoveSpeed().y;

	//キーボードのJ　or　パッドのAボタンでジャンプ
	if (KeyBoardInput->isPush(DIK_J) || XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_A))
	{
		//地面上にいる場合
		if (player->GetCharaCon().IsOnGround())
		{
			//ジャンプパワーを設定
			movespeed.y = JUMP_POWER;
			//キャラクターコントローラーをジャンプに
			player->GetCharaCon().Jump();
		}
	}

	//ゲームパッドから取得した方向
	Vector3 dir = Vector3::zero;
	//コントローラー移動
	dir.x += (XboxInput(0)->GetAnalog(AnalogInputE::L_STICK).x / 32767.0f) * SPEED;
	dir.z += (XboxInput(0)->GetAnalog(AnalogInputE::L_STICK).y / 32767.0f) * SPEED;
#ifdef _DEBUG
	//キーボード(デバッグ用)
	if (KeyBoardInput->isPressed(DIK_W))
	{
		dir.z += SPEED;
	}
	if (KeyBoardInput->isPressed(DIK_S))
	{
		dir.z -= SPEED;
	}
	if (KeyBoardInput->isPressed(DIK_A))
	{
		dir.x -= SPEED;
	}
	if (KeyBoardInput->isPressed(DIK_D))
	{
		dir.x += SPEED;
	}
#endif
	//移動したか
	if (dir.Length() != 0)
	{
		//カメラからみた向きに変換
		Camera* c = GameObjectManager::mainCamera;
		//dir = c->transform->Direction(Vector3::front);
		//dir = dir * SPEED;
		//dir = c->transform->Direction(dir)*Time::DeltaTime();
		//Yの移動量を消す
		movespeed = Vector3(dir.x, movespeed.y, dir.z);

		Vector3 vec = movespeed;
		//正規化
		vec.Normalize();
		//ベクトルから角度を求める
		float rot = D3DXToRadian(360) - atan2f(vec.z, vec.x);
		//回転
		player->transform->SetLocalAngle(Vector3(0.0f, D3DXToDegree(rot + D3DXToRadian(-90)), 0.0f));
	}
	//移動していない
	if (player->GetCharaCon().GetMoveSpeed().Length() < 0.0001f)
	{
		player->ChangeState(Player::State::Idol);
	}

	//キャラクターコントローラー更新
	player->GetCharaCon().SetMoveSpeed(movespeed);
	player->GetCharaCon().Execute();
}

void PlayerStateRun::Enter()
{

}

void PlayerStateRun::Leave()
{

}