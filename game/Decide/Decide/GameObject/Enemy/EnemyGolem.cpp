#include "stdafx.h"
#include "EnemyGolem.h"
#include "fbEngine\CharacterController.h"
#include "GameObject\Enemy\EnemyAttack.h"

EnemyGolem::EnemyGolem(const char* name) : EnemyCharacter(name)
{
}


EnemyGolem::~EnemyGolem()
{
}

void EnemyGolem::_AwakeSubClass() {
	SetFileName("enemy_01.X");
}

void EnemyGolem::_StartSubClass() {
	//視野角生成。
	_ViewAngle = 100.0f;
	_ViewRange = 20.0f;

	//徘徊範囲設定。
	_WanderingRange = 10.0f;

	//追跡範囲設定。
	_discoveryRange = 10.0f;

	//歩行速度設定。
	_walkSpeed = 1.5f;

	//モデルにライト設定。
	_MyComponent.Model->SetLight(INSTANCE(GameObjectManager)->mainLight);

	_MyComponent.Model->SetModelEffect(ModelEffectE::SPECULAR, true);

	// 攻撃処理を定義。
	_SingleAttack.reset(new EnemySingleAttack(this));
	_SingleAttack->Init(1.5f, static_cast<int>(EnemyGolemAnim::Attack01), 0.0f,0.5f);

	// 初期ステートに移行。
	_initState = State::Wandering;
	_ChangeState(_initState);
}

void EnemyGolem::CreateAttackCollsion()
{
	//攻撃コリジョン作成。
	unsigned int priorty = 1;
	AttackCollision* attack = INSTANCE(GameObjectManager)->AddNew<AttackCollision>("attackCollision", priorty);
	attack->Create(_MyComponent.Parameter->GiveDamageMass(false, false), Vector3(0.0f, 0.5f, 1.5f), Quaternion::Identity, Vector3(2.0f,3.0f,1.0f), AttackCollision::CollisionMaster::Enemy, 0.15f, AttackCollision::ReactionType::NotAction, transform);
	attack->RemoveParent();

	//攻撃音再生。
	EnemyPlaySound(EnemyCharacter::SoundIndex::DamageGolem);
}

void EnemyGolem::_UpdateSubClass() 
{
	if (_MyComponent.CharacterController->IsOnGround()==false) {
		//落下ステートに切り替え。
		_ChangeState(State::Fall);
	}
}

EnemyAttack* EnemyGolem::_AttackSelectSubClass() 
{
	// ※プレイヤーとエネミーの位置関係とかで遷移先決定？。

	// ※とりあえず暫定処理。
	return _SingleAttack.get();
}

void EnemyGolem::_ConfigCollision()
{
	// コリジョンのサイズを決定。
	// ※キャラクターコントローラーで使用するためのもの。
	_collisionInfo.radius = 0.6f;
	_collisionInfo.height = 0.3f;
	_collisionInfo.offset = Vector3(0.0f, 0.66f, 0.0f);
	_collisionInfo.id = Collision_ID::ENEMY;

	// コンポーネントにカプセルコライダーを追加。
	_MyComponent.Collider = AddComponent<CCapsuleCollider>();
	// カプセルコライダーを作成。
	static_cast<CCapsuleCollider*>(_MyComponent.Collider)->Create(_collisionInfo.radius, _collisionInfo.height);
}

void EnemyGolem::_ConfigCharacterController() 
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

void EnemyGolem::_CreateExtrudeCollision() 
{
	_MyComponent.ExtrudeCollisions.push_back(_MyComponent.CharacterController->GetRigidBody());	// キャラクターコントローラの剛体をそのまま使用。
}

void EnemyGolem::_BuildAnimationSubClass(vector<double>& datas)
{

	// アニメーションタイプにデータを関連づけ。
	// ※エネミーはすべて同じステートクラスを使用するため、ステートからアニメーションを再生できるよう
	//   EnemyCharacterクラスで定義されているすべてのエネミー共通の列挙子に関連付ける必要がある。
	{
		// 待機状態。
		_ConfigAnimationType(EnemyCharacter::AnimationType::Idle, static_cast<unsigned int>(EnemyGolemAnim::Stand));
		// 歩行状態。
		_ConfigAnimationType(EnemyCharacter::AnimationType::Walk, static_cast<unsigned int>(EnemyGolemAnim::Walk));
		// 走行状態。
		_ConfigAnimationType(EnemyCharacter::AnimationType::Dash, static_cast<unsigned int>(EnemyGolemAnim::Run));
		// 落下状態。
		_ConfigAnimationType(EnemyCharacter::AnimationType::Fall, static_cast<unsigned int>(EnemyGolemAnim::Fall));
		// ダメージ状態。
		_ConfigAnimationType(EnemyCharacter::AnimationType::Damage, static_cast<unsigned int>(EnemyGolemAnim::Damage));
		// 死亡状態。
		_ConfigAnimationType(EnemyCharacter::AnimationType::Death, static_cast<unsigned int>(EnemyGolemAnim::Death));
	}


}

void EnemyGolem::_ConfigAnimationEvent()
{
	float eventFrame = 0.4f;

	_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(EnemyGolemAnim::Attack01), eventFrame, static_cast<AnimationEvent>(&EnemyGolem::CreateAttackCollsion));
}

void EnemyGolem::_EndNowStateCallback(State EndStateType) {
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