#include"stdafx.h"
#include "../Player.h"
#include "PlayerStateIdol.h"

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
	Vector3 movespeed = _Player->GetCharaCon().GetMoveSpeed();
	movespeed.x = 0.0f;
	movespeed.z = 0.0f;
	//キーボードのJ　or　パッドのAボタンでジャンプ
	if (KeyBoardInput->isPush(DIK_J) || XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_A))
	{
		//地面上にいる場合
		if (_Player->GetCharaCon().IsOnGround() == true)
		{
			//ジャンプパワーを設定
			movespeed.y = _JumpSpeed;
			//キャラクターコントローラーをジャンプに
			_Player->GetCharaCon().Jump();
		}
	}
	if (dir.Length() >= 0.0001f)
	{
		_Player->ChangeState(Player::State::Run);
	}

	movespeed.x = dir.x * _Speed;
	movespeed.z = dir.z * _Speed;
	
	_Player->GetCharaCon().SetMoveSpeed(movespeed);
	_Player->GetCharaCon().Execute();
	//攻撃へ移動
	if (XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_X) || KeyBoardInput->isPush(DIK_SPACE))
	{
		if (_Player->GetCharaCon().IsOnGround() == true)
		{
			_Player->_NextAttackAnimNo = Player::AnimationNo::AnimationAttack01;
			_Player->ChangeState(Player::State::Attack);
		}
	}
}

void PlayerStateIdol::Enter()
{

}

void PlayerStateIdol::Leave()
{

}