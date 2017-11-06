#include"stdafx.h"
#include "../Player.h"
#include "PlayerStateAttack.h"
#include "fbEngine\_Object\_Component\_3D\Animation.h"
#include "../Decide/Decide/AttackCollision.h"
#include "GameObject\Enemy\EnemyCharacter.h"

namespace
{
	//プレイヤーの周り。
	float Circumference = 6.0f;
}

PlayerStateAttack::PlayerStateAttack(Player* player) :
	PlayerState(player)
{
}

PlayerStateAttack::~PlayerStateAttack()
{

}


void PlayerStateAttack::Update()
{
	//現在のアニメーションナンバー
	int currentanimno = _Player->_Anim->GetPlayAnimNo();
	//アニメーションの再生が終わる && 次のアニメーションがない場合、待機状態に変更
	if (_Player->_Anim->GetPlaying() != true && _Player->_NextAttackAnimNo == Player::AnimationNo::AnimationInvalid)
	{
		_Player->ChangeState(Player::State::Idol);
	}
	//攻撃ボタンが押される。
	//連撃。
	else if (XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_X)
		&& currentanimno >= (int)Player::AnimationNo::AnimationAttackStart
		&& currentanimno < (int)Player::AnimationNo::AnimationAttackEnd
		&& currentanimno == (int)_Player->_NowAttackAnimNo
		)
	{
		//方向を変える。
		//Dir();
		//コンボ！
		_Player->_NextAttackAnimNo = (Player::AnimationNo)(_Player->_Anim->GetPlayAnimNo() + 1);
	}

	//近くのエネミーの方向を向く。
	if (EnemyCharacter::nearEnemyInfo.length <= Circumference)
	{
		_Player->_Rotation->RotationToObject_XZ(EnemyCharacter::nearEnemyInfo.object);
	}

	//移動速度
	Vector3 movespeed = _Player->_CharacterController->GetMoveSpeed();
	movespeed.Scale(0.7f);
	//キャラクターコントローラー更新
	_Player->_CharacterController->SetMoveSpeed(movespeed);
	_Player->_CharacterController->Execute();
}

void PlayerStateAttack::Enter()
{

}

void PlayerStateAttack::Leave()
{

}

void PlayerStateAttack::Dir()
{
	//移動速度
	Vector3 movespeed = Vector3::zero;

	//ゲームパッドから取得した方向
	Vector3 dir = Vector3::zero;
	//コントローラー移動
	dir.x += (XboxInput(0)->GetAnalog(AnalogE::L_STICK).x / 32767.0f);
	dir.z += (XboxInput(0)->GetAnalog(AnalogE::L_STICK).y / 32767.0f);
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
								//ダッシュボタンの場合
		//通常のスピード
		dir = dir;
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
}