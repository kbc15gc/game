#include"stdafx.h"
#include "PlayerStateRun.h"
#include "../Player.h"
#include "fbEngine\_Object\_Component\_3D\Camera.h"

PlayerStateRun::PlayerStateRun(Player* player) :
	PlayerState(player)
{

}

PlayerStateRun::~PlayerStateRun()
{

}

void PlayerStateRun::Update()
{
	if (KeyBoardInput->isPressed(DIK_0))
	{
		debug = !debug;
		if (debug == true)
		{
			Vector3 pos = Vector3(_Player->transform->GetLocalPosition().x, _Player->transform->GetLocalPosition().y + 5.0f, _Player->transform->GetLocalPosition().z);
			_Player->transform->SetLocalPosition(pos);
		}
	}

	if (debug == true)
	{
		Vector3 movespeed = Vector3::zero;
		//ゲームパッドから取得した方向
		Vector3 dir = Vector3::zero;
		//コントローラー移動
		dir.x += (XboxInput(0)->GetAnalog(AnalogInputE::L_STICK).x / 32767.0f);
		dir.z += (XboxInput(0)->GetAnalog(AnalogInputE::L_STICK).y / 32767.0f);
#ifdef _DEBUG
		//キーボード(デバッグ用)
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
		if (KeyBoardInput->isPressed(DIK_1))
		{
			dir.y++;
		}
#endif
		//移動したか
		if (dir.Length() != 0)
		{
			//カメラからみた向きに変換
			Camera* camera = INSTANCE(GameObjectManager)->mainCamera;
			//カメラのビュー行列をゲット
			D3DXMATRIX view = camera->GetViewMat();
			//ビュー行列の逆行列
			D3DXMATRIX viewinv;
			D3DXMatrixInverse(&viewinv, NULL, &view);
			//カメラ空間から見た奥方向のベクトルを取得。
			Vector3 cameraZ;
			cameraZ.x = viewinv.m[2][0];
			cameraZ.y = 0.0f;		//Y軸いらない。
			cameraZ.z = viewinv.m[2][2];
			cameraZ.Normalize();	//Y軸を打ち消しているので正規化する。
									//カメラから見た横方向のベクトルを取得。
			Vector3 cameraX;
			cameraX.x = viewinv.m[0][0];
			cameraX.y = 0.0f;		//Y軸はいらない。
			cameraX.z = viewinv.m[0][2];
			cameraX.Normalize();	//Y軸を打ち消しているので正規化する。

									// 向きベクトルに移動量を積算。
			dir = dir * _Speed;
			//カメラからみた方向に射影。
			movespeed = movespeed + cameraX * dir.x;
			movespeed.y = movespeed.y;	//上方向は固定なのでそのまま。
			movespeed = movespeed + cameraZ * dir.z;

			//移動したい方向のベクトル
			Vector3 vec = movespeed;
			//正規化
			vec.Normalize();
			//ベクトルから角度を求める
			//回転
			_Player->_Rotation->RotationToDirection_XZ(vec);

			_Player->transform->SetLocalPosition(_Player->transform->GetLocalPosition() + movespeed);
		}
	}
	else
	{
		//移動速度
		Vector3 movespeed = Vector3::zero;
		movespeed.y = _Player->GetCharaCon().GetMoveSpeed().y;

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

		//ゲームパッドから取得した方向
		Vector3 dir = Vector3::zero;
		//コントローラー移動
		dir.x += (XboxInput(0)->GetAnalog(AnalogInputE::L_STICK).x / 32767.0f);
		dir.z += (XboxInput(0)->GetAnalog(AnalogInputE::L_STICK).y / 32767.0f);
#ifdef _DEBUG
		//キーボード(デバッグ用)
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
		//移動したか
		if (dir.Length() != 0)
		{
			//カメラからみた向きに変換
			Camera* camera = INSTANCE(GameObjectManager)->mainCamera;
			//カメラのビュー行列をゲット
			D3DXMATRIX view = camera->GetViewMat();
			//ビュー行列の逆行列
			D3DXMATRIX viewinv;
			D3DXMatrixInverse(&viewinv, NULL, &view);
			//カメラ空間から見た奥方向のベクトルを取得。
			Vector3 cameraZ;
			cameraZ.x = viewinv.m[2][0];
			cameraZ.y = 0.0f;		//Y軸いらない。
			cameraZ.z = viewinv.m[2][2];
			cameraZ.Normalize();	//Y軸を打ち消しているので正規化する。
									//カメラから見た横方向のベクトルを取得。
			Vector3 cameraX;
			cameraX.x = viewinv.m[0][0];
			cameraX.y = 0.0f;		//Y軸はいらない。
			cameraX.z = viewinv.m[0][2];
			cameraX.Normalize();	//Y軸を打ち消しているので正規化する。

									// 向きベクトルに移動量を積算。
			dir = dir * _Speed;
			//カメラからみた方向に射影。
			movespeed = movespeed + cameraX * dir.x;
			movespeed.y = movespeed.y;	//上方向は固定なのでそのまま。
			movespeed = movespeed + cameraZ * dir.z;

			//移動したい方向のベクトル
			Vector3 vec = movespeed;
			//正規化
			vec.Normalize();
			//ベクトルから角度を求める
			//回転
			_Player->_Rotation->RotationToDirection_XZ(vec);
		}
		//移動していない
		if (dir.Length() < 0.0001f)
		{
			_Player->ChangeState(Player::State::Idol);
		}

		//キャラクターコントローラー更新
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
	
}

void PlayerStateRun::Enter()
{

}

void PlayerStateRun::Leave()
{

}