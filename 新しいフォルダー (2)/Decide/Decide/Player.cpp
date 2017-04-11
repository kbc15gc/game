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
	const float JUMP_POWER = 5.0f;
}

Player::Player(const char * name) :
	GameObject(name),
	_Gravity(-30.0f),
	_Jump(false)
{

}

void Player::Awake()
{
	_Model = AddComponent<SkinModel>();
	_Anim = AddComponent<Animation>();
	_Rigid = AddComponent<RigidBody>();
	_CharacterController = AddComponent<CCharacterController>();
	CCapsuleCollider* coll = AddComponent<CCapsuleCollider>();
	_Height = 1.5f;
	_Radius = 0.5f;
	coll->Create(_Radius, _Height);
	_Rigid->Create(0.0f, coll, Collision_ID::PLAYER, Vector3::zero, Vector3(0, _Height/2, 0));
	_RB = (btRigidBody*)_Rigid->GetCollisonObj();
	//RigidBodyの上下の移動量を消す
	//_Rigid->SetGravity(btVector3(0.0f, 0.0f, 0.0f));
	//スリープさせない(必要かどうかわからない。)
	_RB->setSleepingThresholds(0, 0);

	SkinModelData* modeldata = new SkinModelData();
	modeldata->CloneModelData(SkinModelManager::LoadModel("Unity.X"), _Anim);
	_Model->SetModelData(modeldata);
	_Model->SetModelEffect(ModelEffectE::SPECULAR, false);

	_CharacterController->Init(_Radius,_Height,_Rigid,coll);
	_CharacterController->SetGravity(_Gravity);
}

void Player::Start()
{
	_Model->SetCamera(GameObjectManager::mainCamera);
	_Model->SetLight(GameObjectManager::mainLight);

	transform->SetLocalPosition(Vector3(0.0f, 10.0f, 0.0f));
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
	case PlayerState::Wait:
		break;
	case PlayerState::Run:
		break;
	case PlayerState::Jump:
		break;
	case PlayerState::Attack:
		break;
	default:
		break;
	}
	Move();
	Jump();
	//Y軸の移動量保存
	//Vector3 move = _MoveSpeed;
	//キャラクターコントローラー更新
	_CharacterController->SetMoveSpeed(_MoveSpeed);
	_CharacterController->Execute();
	/*transform->position = _CharacterController->GetPosition();	//座標代入
	_CharacterController->SetPosition(transform->position);*/
	//XZ軸の移動
//	transform->localPosition.Add(_MoveSpeed);
	transform->UpdateTransform();
	//if (_MoveSpeed.Length() > 0)
	//{
	//	
	//}
	//Y軸移動はしない
	//_MoveSpeed.y = 0.0f;
	//壁チェック
	//_WallCheck(move);
	//重力チェック
	//_GravityCheck(move.y);
}

void Player::Move()
{
	//初期化
	_MoveSpeed = _CharacterController->GetMoveSpeed();
	_MoveSpeed.x = 0.0f;
	_MoveSpeed.z = 0.0f;
	//方向
	_Dir = Vector3::zero;
	static Vector3 dirX = Vector3::axisX;
	static Vector3 dirZ = Vector3::axisZ;
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
#endif // _DEBUG

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
		Vector3 ang = transform->GetLocalAngle();
		ang.y = D3DXToDegree(rot + D3DXToRadian(90));
		//回転
		transform->SetLocalAngle(ang);
	}
}

void Player::Jump()
{
	if (KeyBoardInput->isPush(DIK_J) || XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_A))
	{
		_MoveSpeed.y = JUMP_POWER;
		_CharacterController->Jump();
	}
}

void Player::Attack()
{

}