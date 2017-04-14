#include "PlayerStateRun.h"
#include "../Player.h"
#include "fbEngine\Camera.h"

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
		if (player->GetCharaCon().IsOnGround() == true)
		{
			//ジャンプパワーを設定
			movespeed.y = _JumpSpeed;
			//キャラクターコントローラーをジャンプに
			player->GetCharaCon().Jump();
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
		Camera* camera = GameObjectManager::mainCamera;
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

		//カメラからみた方向
		movespeed.x = cameraX.x * dir.x + cameraZ.x * dir.z;
		movespeed.y = movespeed.y;	//Y軸はいらない。
		movespeed.z = cameraX.z * dir.x + cameraZ.z * dir.z;
		//移動速度にスピードをかける
		movespeed.x *= _Speed;
		movespeed.z *= _Speed;
		//移動したい方向のベクトル
		Vector3 vec = movespeed;
		//正規化
		vec.Normalize();
		//ベクトルから角度を求める
		float rot = D3DXToRadian(360) - atan2f(vec.z, vec.x);
		//回転
		player->transform->SetLocalAngle(Vector3(0.0f, D3DXToDegree(rot + D3DXToRadian(-90)), 0.0f));
	}
	//移動していない
	if (dir.Length() < 0.0001f)
	{
		player->ChangeState(Player::State::Idol);
	}

	//キャラクターコントローラー更新
	player->GetCharaCon().SetMoveSpeed(movespeed);
	player->GetCharaCon().Execute();
	//攻撃へ移動
	if (XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_X) || KeyBoardInput->isPush(DIK_SPACE))
	{
		if (player->GetCharaCon().IsOnGround() == true)
		{
			player->ChangeState(Player::State::Attack);
		}
	}
}

void PlayerStateRun::Enter()
{

}

void PlayerStateRun::Leave()
{

}