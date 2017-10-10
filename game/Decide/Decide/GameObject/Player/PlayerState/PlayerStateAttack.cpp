#include"stdafx.h"
#include "../Player.h"
#include "PlayerStateAttack.h"
#include "fbEngine\_Object\_Component\_3D\Animation.h"
#include "../Decide/Decide/AttackCollision.h"

PlayerStateAttack::PlayerStateAttack(Player* player) :
	PlayerState(player)
{
	_SE = INSTANCE(GameObjectManager)->AddNew<SoundSource>("SE", 0);
	_SE->Init("Asset/Sound/Player/PlayerAttack_00.wav");
	//攻撃１
	{
		AttackCollisionParameter attackparam1;
		attackparam1.pos = Vector3(0.0f, 1.0f, 1.5f);
		attackparam1.rot = Quaternion::Identity;
		attackparam1.scale = Vector3(1.5f, 1.5f, 1.5f);
		attackparam1.attackframe = 10.0f;
		attackparam1.lifetime = 0.5f;
		attackparam1.atk = 120;
		attackparam1.attackboice = (int)Player::AttackBoice::Attack1;
		_AttackPram.push_back(attackparam1);
	}
	//攻撃２
	{
		AttackCollisionParameter attackparam2;
		attackparam2.pos = Vector3(0.0f, 1.0f, 1.5f);
		attackparam2.rot = Quaternion::Identity;
		attackparam2.scale = Vector3(1.5f, 1.5f, 1.5f);
		attackparam2.attackframe = 10;
		attackparam2.lifetime = 0.5f;
		attackparam2.atk = 100;
		attackparam2.attackboice = (int)Player::AttackBoice::Attack2;
		_AttackPram.push_back(attackparam2);
	}
	//攻撃３
	{
		AttackCollisionParameter attackparam3;
		attackparam3.pos = Vector3(0.0f, 1.0f, 1.5f);
		attackparam3.rot = Quaternion::Identity;
		attackparam3.scale = Vector3(1.5f, 1.5f, 1.5f);
		attackparam3.attackframe = 10.0f;
		attackparam3.lifetime = 0.5f;
		attackparam3.atk = 120;
		attackparam3.attackboice = (int)Player::AttackBoice::Attack1;
		_AttackPram.push_back(attackparam3);
	}
	//攻撃４
	{
		AttackCollisionParameter attackparam4;
		attackparam4.pos = Vector3(0.0f, 1.0f, 1.5f);
		attackparam4.rot = Quaternion::Identity;
		attackparam4.scale = Vector3(1.5f, 1.5f, 1.5f);
		attackparam4.attackframe = 10;
		attackparam4.lifetime = 0.5f;
		attackparam4.atk = 100;
		attackparam4.attackboice = (int)Player::AttackBoice::Attack2;
		_AttackPram.push_back(attackparam4);
	}
	//攻撃５
	{
		AttackCollisionParameter attackparam5;
		attackparam5.pos = Vector3(0.0f, 1.0f, 1.5f);
		attackparam5.rot = Quaternion::Identity;
		attackparam5.scale = Vector3(1.5f, 1.5f, 1.5f);
		attackparam5.attackframe = 30;
		attackparam5.lifetime = 0.5f;
		attackparam5.atk = 200;
		attackparam5.attackboice = (int)Player::AttackBoice::Attack3;
		_AttackPram.push_back(attackparam5);
	}
	
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
		Dir();
		//コンボ！
		_Player->_NextAttackAnimNo = (Player::AnimationNo)(_Player->_Anim->GetPlayAnimNo() + 1);
	}
	//あたり判定作成
 	switch (currentanimno)
	{
		//攻撃１の時
	case (int)Player::AnimationNo::AnimationAttack01:
		Attack(_AttackPram[0]);
		break;
		//攻撃２の時
	case (int)Player::AnimationNo::AnimationAttack02:
		Attack(_AttackPram[1]);
		break;
		//攻撃３の時
	case (int)Player::AnimationNo::AnimationAttack03:
		Attack(_AttackPram[2]);
		break;
	case (int)Player::AnimationNo::AnimationAttack04:
		Attack(_AttackPram[3]);
		break;
	case (int)Player::AnimationNo::AnimationAttack05:
		Attack(_AttackPram[4]);
		break;
	}
	
}

void PlayerStateAttack::Enter()
{

}

void PlayerStateAttack::Leave()
{

}

void PlayerStateAttack::Attack(AttackCollisionParameter pram)
{
	//攻撃時のあたり判定作成
	//現在のフレーム取得
	const int nowFrame = _Player->_Anim->NowFrame();
	if (nowFrame == pram.attackframe)
	{
		//攻撃時のサウンド再生。
		_SE->Play(false);
		//攻撃ボイス再生
		_Player->_AttackBoiceSound[pram.attackboice]->Play(false);
		//攻撃コリジョン作成
		AttackCollision* attack = INSTANCE(GameObjectManager)->AddNew<AttackCollision>("attack01", 1);
		if (_Player->GetEquipment()) {
			attack->Create(move(_Player->_PlayerParam->GiveDamageMass(false,false, _Player->GetEquipment()->weapon, pram.atk)),  pram.pos, pram.rot, pram.scale, AttackCollision::CollisionMaster::Player, pram.lifetime, 0.0f, _Player->transform);
		}
	}
}

void PlayerStateAttack::Dir()
{
	//移動速度
	Vector3 movespeed = Vector3::zero;
	movespeed.y = _Player->_CharacterController->GetMoveSpeed().y;

	//キーボードのJ　or　パッドのAボタンでジャンプ
	if (KeyBoardInput->isPush(DIK_J) || XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_A))
	{
		//地面上にいる場合
		if (_Player->_CharacterController->IsOnGround() == true && !_Player->_Speak)
		{
			//ジャンプパワーを設定
			movespeed.y = _JumpSpeed;
			//キャラクターコントローラーをジャンプに
			_Player->_CharacterController->Jump();
		}
	}

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

	//キャラクターコントローラー更新
	_Player->_CharacterController->SetMoveSpeed(movespeed);
	_Player->_CharacterController->Execute();
}
