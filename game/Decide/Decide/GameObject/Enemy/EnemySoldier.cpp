#include "stdafx.h"
#include "EnemySoldier.h"
#include "fbEngine\CharacterController.h"
#include "GameObject\Enemy\EnemyAttack.h"

EnemySoldier::EnemySoldier(const char* name) : EnemyCharacter(name)
{
}


EnemySoldier::~EnemySoldier()
{
}

void EnemySoldier::_AwakeSubClass() {
	SetFileName("enemy_02.X");
}

void EnemySoldier::_StartSubClass() {

	//ポジション
	//_InitPos = Vector3(-222.0f, 60.0f, -156.0f);
	//transform->SetPosition(_InitPos);

	//視野角生成。
	//_ViewAngle = 100.0f;
	//_ViewRange = 20.0f;

	//徘徊範囲設定。
	//_WanderingRange = 10.0f;

	//追跡範囲設定。
	//_discoveryRange = 10.0f;

	//歩行速度設定。
	_walkSpeed = 1.5f;

	//モデルにライト設定。
	_MyComponent.Model->SetLight(INSTANCE(GameObjectManager)->mainLight);

	// 攻撃処理を定義。
	_SingleAttack.reset(new EnemySingleAttack(this));
	_SingleAttack->Init(1.3f, static_cast<int>(EnemySoldierAnim::Attack01), 0.2f);

	// 初期ステートに移行。
	_initState = State::Wandering;
	_ChangeState(_initState);
}

void EnemySoldier::CreateAttackCollsion()
{
	//攻撃コリジョン作成。
	unsigned int priorty = 1;
	AttackCollision* attack = INSTANCE(GameObjectManager)->AddNew<AttackCollision>("attackCollision", priorty);
	attack->Create(_MyComponent.Parameter->GiveDamageMass(false, false), Vector3(0.0f, 0.0f, 1.0f), Quaternion::Identity, Vector3(1.0f, 2.0f, 1.0f), AttackCollision::CollisionMaster::Enemy, 0.15f, AttackCollision::ReactionType::NotAction, transform);
	attack->RemoveParent();

	// 攻撃音再生。
	EnemyPlaySound(EnemyCharacter::SoundIndex::Damage);
}

void EnemySoldier::_UpdateSubClass()
{
	if (_MyComponent.CharacterController->IsOnGround() == false) {
		//落下ステートに切り替え。
		_ChangeState(State::Fall);
	}
}

EnemyAttack* EnemySoldier::_AttackSelectSubClass()
{
	// ※プレイヤーとエネミーの位置関係とかで遷移先決定？。

	// ※とりあえず暫定処理。
	return _SingleAttack.get();
}

void EnemySoldier::_ConfigCollision()
{
	// コリジョンのサイズを決定。
	// ※キャラクターコントローラーで使用するためのもの。
	_collisionInfo.radius = 0.3f;
	_collisionInfo.height = 0.1f;
	_collisionInfo.offset = Vector3(0.0f, -0.5f, 0.0f);
	_collisionInfo.id = Collision_ID::ENEMY;

	// コンポーネントにカプセルコライダーを追加。
	_MyComponent.Collider = AddComponent<CCapsuleCollider>();
	// カプセルコライダーを作成。
	static_cast<CCapsuleCollider*>(_MyComponent.Collider)->Create(_collisionInfo.radius, _collisionInfo.height);
}

void EnemySoldier::_ConfigCharacterController()
{
	// 衝突する属性を設定(横)。
	_MyComponent.CharacterController->AttributeXZ_AllOn();
	_MyComponent.CharacterController->SubAttributeXZ(Collision_ID::ATTACK);
	_MyComponent.CharacterController->SubAttributeXZ(Collision_ID::SPACE);
	_MyComponent.CharacterController->SubAttributeXZ(Collision_ID::PLAYER);	// プレイヤーは押し出すので押し戻されないようにする。
	_MyComponent.CharacterController->SubAttributeXZ(Collision_ID::DROPITEM);
	// 衝突する属性を設定(縦)。
	_MyComponent.CharacterController->AttributeY_AllOn();
	_MyComponent.CharacterController->SubAttributeY(Collision_ID::ATTACK);
	_MyComponent.CharacterController->SubAttributeY(Collision_ID::ENEMY);
	_MyComponent.CharacterController->SubAttributeY(Collision_ID::PLAYER);
	_MyComponent.CharacterController->SubAttributeY(Collision_ID::SPACE);
	_MyComponent.CharacterController->SubAttributeY(Collision_ID::DROPITEM);
}

void EnemySoldier::_CreateExtrudeCollision()
{
	_MyComponent.ExtrudeCollisions.push_back(_MyComponent.CharacterController->GetRigidBody());	// キャラクターコントローラの剛体をそのまま使用。
}

void EnemySoldier::_BuildAnimationSubClass(vector<double>& datas)
{
	
	// アニメーションタイプにデータを関連づけ。
	// ※エネミーはすべて同じステートクラスを使用するため、ステートからアニメーションを再生できるよう
	//   EnemyCharacterクラスで定義されているすべてのエネミー共通の列挙子に関連付ける必要がある。
	{
		// 待機状態。
		_ConfigAnimationType(EnemyCharacter::AnimationType::Idle, static_cast<unsigned int>(EnemySoldierAnim::Stand));
		// 歩行状態。
		_ConfigAnimationType(EnemyCharacter::AnimationType::Walk, static_cast<unsigned int>(EnemySoldierAnim::Walk));
		// 走行状態。
		datas[static_cast<int>(EnemySoldierAnim::Run)] = 0.5f;
		_ConfigAnimationType(EnemyCharacter::AnimationType::Dash, static_cast<unsigned int>(EnemySoldierAnim::Run));
		// 落下状態。
		//落ちるモーションが無いので待機で代用。
		_ConfigAnimationType(EnemyCharacter::AnimationType::Fall, static_cast<unsigned int>(EnemySoldierAnim::Stand));
		// ダメージ状態。
		_ConfigAnimationType(EnemyCharacter::AnimationType::Damage, static_cast<unsigned int>(EnemySoldierAnim::Damage));
		// 死亡状態。
		_ConfigAnimationType(EnemyCharacter::AnimationType::Death, static_cast<unsigned int>(EnemySoldierAnim::Death));
	}
	
}

void EnemySoldier::_ConfigAnimationEvent()
{
	float eventFrame = 0.2f;

	_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(EnemySoldierAnim::Attack01), eventFrame, static_cast<AnimationEvent>(&EnemySoldier::CreateAttackCollsion));
}

void EnemySoldier::_EndNowStateCallback(State EndStateType) {
	if (EndStateType == State::Wandering) {
		// 徘徊の挙動いったん終了。

		// 再度徘徊。
		_ChangeState(State::Wandering);
	}
	else if (EndStateType == State::StartAttack) {
		// 一度攻撃が終了した。

		// もう一度攻撃開始。
		_ChangeState(State::StartAttack);
	}
	else if (EndStateType == State::Fall) {
		// 落下ステート終了。

		// 直前のステートに切り替え。
		_ChangeState(State::Wandering);
	}
	else if (EndStateType == State::Damage) {
		// 攻撃を受けた。
		// 攻撃開始。
		_ChangeState(State::StartAttack);
	}
	else if (EndStateType == State::Threat) {
		//威嚇終了。
		//攻撃開始。
		_ChangeState(State::StartAttack);
	}
}