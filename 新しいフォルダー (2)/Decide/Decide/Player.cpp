#include "Player.h"
#include "fbEngine/Skinmodel.h"
#include "fbEngine/SkinmodelData.h"
#include "fbEngine/Animation.h"
#include "fbEngine/RigidBody.h"
#include "fbEngine/BoxCollider.h"
#include "fbEngine/CapsuleCollider.h"

namespace 
{
	const float SPEED = 30.0f;
	const float JUMP_POWER = 15.0f;
}

Player::Player(const char * name) :
	GameObject(name),
	//キャラクターコントローラーNULL
	_CharacterController(NULL),
	//重力設定
	_Gravity(-30.0f),
	//ジャンプフラグ設定
	_Jump(false)
{
}

void Player::Awake()
{
	//モデル
	_Model = AddComponent<SkinModel>();
	//アニメーション
	_Anim = AddComponent<Animation>();
	////リジッドボディ
	//_Rigid = AddComponent<RigidBody>();
	//カプセルコライダー
	CCapsuleCollider* coll = AddComponent<CCapsuleCollider>();
	//キャラクターコントローラー
	_CharacterController = AddComponent<CCharacterController>();
	//高さ設定
	_Height = 1.5f;
	//半径設定
	_Radius = 0.5f;
	//カプセルコライダー作成
	coll->Create(_Radius, _Height);
	////リジッドボディ作成
	//_Rigid->Create(0.0f, coll, Collision_ID::PLAYER, Vector3::zero, Vector3(0, _Height/2, 0));
	//_RB = (btRigidBody*)_Rigid->GetCollisonObj();
	////スリープさせない(必要かどうかわからない。)
	//_RB->setSleepingThresholds(0, 0);

	//スキンモデル作成
	SkinModelData* modeldata = new SkinModelData();
	//モデルデータ作成
	modeldata->CloneModelData(SkinModelManager::LoadModel("Player.X"), _Anim);
	//モデル設定
	_Model->SetModelData(modeldata);
	_Model->SetModelEffect(ModelEffectE::SPECULAR, false);
	//キャラクターコントローラー初期化
	_CharacterController->Init(this,transform,_Radius,_Height, Collision_ID::PLAYER,coll);
	//キャラクターコントローラーの重力設定
	_CharacterController->SetGravity(_Gravity);
}

void Player::Start()
{
	//モデルにカメラ設定
	_Model->SetCamera(GameObjectManager::mainCamera);
	//モデルにライト設定
	_Model->SetLight(GameObjectManager::mainLight);
	//ポジション
	transform->localPosition = Vector3(0.0f, 10.0f, 0.0f);
	//移動速度初期化
	_MoveSpeed = Vector3::zero;
	//初期プレイヤー状態（待機）
	_State = PlayerState::Wait;
	//プレイヤーのレベル初期化
	//最初は１から
	_Level = 1;
}
#include "fbEngine/Camera.h"
void Player::Update()
{
	switch (_State)
	{
	//待機状態
	case PlayerState::Wait:
		break;
	//走る状態
	case PlayerState::Run:
		break;
	//ジャンプ状態
	case PlayerState::Jump:
		break;
	//攻撃状態
	case PlayerState::Attack:
		break;
	default:
		break;
	}
	//移動
	Move();
	//ジャンプ
	Jump();
	//キャラクターコントローラー更新
	_CharacterController->SetMoveSpeed(_MoveSpeed);
	_CharacterController->Execute();
	//トランスフォーム更新
	transform->UpdateTransform();
}

void Player::Move()
{
	//初期化
	//移動速度
	_MoveSpeed = _CharacterController->GetMoveSpeed();
	_MoveSpeed.x = 0.0f;
	_MoveSpeed.z = 0.0f;
	//方向
	_Dir = Vector3::zero;

	//キーボードのJ　or　パッドのAボタンでジャンプ
	if (KeyBoardInput->isPush(DIK_J) || XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_A))
	{
		//地面上にいる場合
		if (_CharacterController->IsOnGround())
		{
			//ジャンプパワーを設定
			_MoveSpeed.y = JUMP_POWER;
			//キャラクターコントローラーをジャンプに
			_CharacterController->Jump();
		}
	}

	//コントローラー移動
	_Dir.x += (XboxInput(0)->GetAnalog(AnalogInputE::L_STICK).x / 32767.0f);
	_Dir.z += (XboxInput(0)->GetAnalog(AnalogInputE::L_STICK).y / 32767.0f);
#ifdef _DEBUG
	//キーボード(デバッグ用)
	if (KeyBoardInput->isPressed(DIK_W))
	{
		_Dir.z += SPEED;
	}
	if (KeyBoardInput->isPressed(DIK_S))
	{
		_Dir.z -= SPEED;
	}
	if (KeyBoardInput->isPressed(DIK_A))
	{
		_Dir.x -= SPEED;
	}
	if (KeyBoardInput->isPressed(DIK_D))
	{
		_Dir.x += SPEED;
	}
#endif

	//移動したか
	if (_Dir.Length() != 0)
	{
		//カメラからみた向きに変換
		Camera* c = GameObjectManager::mainCamera;
		//_Dir = c->transform->Direction(_Dir)*Time::DeltaTime();
		//Yの移動量を消す
		_MoveSpeed = Vector3(_Dir.x, 0.0f, _Dir.z);

		Vector3 vec = _MoveSpeed;
		//正規化
		vec.Normalize();
		//ベクトルから角度を求める
		float rot = D3DXToRadian(360) - atan2f(vec.z, vec.x);
		//回転
		transform->localAngle.y = D3DXToDegree(rot + D3DXToRadian(-90));
	}
}

void Player::Jump()
{
	
}

void Player::Attack()
{

}