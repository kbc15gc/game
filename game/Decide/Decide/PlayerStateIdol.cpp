#include "Player.h"
#include "PlayerStateIdol.h"

namespace
{
	const float SPEED = 30.0f;
	const float JUMP_POWER = 15.0f;
}

PlayerStateIdol::PlayerStateIdol(Player* player) :
	PlayerState(player)
{

}

PlayerStateIdol::~PlayerStateIdol()
{

}


void PlayerStateIdol::Update()
{
	//ゲームパッドから取得した方向
	Vector3 dir = Vector3::zero;
	//コントローラー移動
	dir.x += (XboxInput(0)->GetAnalog(AnalogInputE::L_STICK).x / 32767.0f);
	dir.z += (XboxInput(0)->GetAnalog(AnalogInputE::L_STICK).y / 32767.0f);
#ifdef _DEBUG
	//キーボード(デバッグ用)
	if (KeyBoardInput->isPressed(DIK_W))
	{
		dir.z += 1.0f;
	}
	if (KeyBoardInput->isPressed(DIK_S))
	{
		dir.z -= 1.0f;
	}
	if (KeyBoardInput->isPressed(DIK_A))
	{
		dir.x -= 1.0f;
	}
	if (KeyBoardInput->isPressed(DIK_D))
	{
		dir.x += 1.0f;
	}
#endif
	if (dir.Length() >= 0.0001f)
	{
		player->ChangeState(Player::State::Run);
	}
	Vector3 movespeed = player->GetCharaCon().GetMoveSpeed();
	movespeed.x = 0.0f;
	movespeed.z = 0.0f;
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

	movespeed = Vector3(dir.x, movespeed.y, dir.z);
	
	player->GetCharaCon().SetMoveSpeed(movespeed);
	player->GetCharaCon().Execute();
}

void PlayerStateIdol::Enter()
{

}

void PlayerStateIdol::Leave()
{

}