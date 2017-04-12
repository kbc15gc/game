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
	_Jump(false),
	_CurrentState(NULL),
	_RunState(this)
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
	_CharacterController->Init(this,transform,_Radius,_Height, Vector3(0.0f,_Height / 2, 0.0f) , Collision_ID::PLAYER,coll, _Gravity);
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
	transform->SetLocalPosition(Vector3(0.0f, 10.0f, 0.0f));
	//移動速度初期化
	_MoveSpeed = Vector3::zero;
	//初期プレイヤー状態（待機）
	_State = State::Wait;
	//プレイヤーのレベル初期化
	//最初は１から
	_Level = 1;
}
#include "fbEngine/Camera.h"
void Player::Update()
{
	
	_CurrentState = &_RunState;
	_CurrentState->Update();
	
	//トランスフォーム更新
	transform->UpdateTransform();
}

void Player::ChangeState(State nextstate)
{
	if (_CurrentState != NULL) {
		//現在のステートを抜ける。
		_CurrentState->Leave();
	}
	switch (_State)
	{
		//待機状態
	case State::Wait:
		break;
		//走る状態
	case State::Run:
		
		break;
		//ジャンプ状態
	case State::Jump:
		break;
		//攻撃状態
	case State::Attack:
		break;
	default:
		break;
	}
	_State = nextstate;
	_CurrentState->Enter();
}

void Player::Attack()
{

}