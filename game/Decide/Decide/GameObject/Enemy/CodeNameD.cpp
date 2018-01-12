#include"stdafx.h"
#include "CodeNameD.h"
#include "fbEngine\_Object\_Component\_3D\SkinModel.h"
#include "HFSM\EnemyTranslationState.h"
#include "HFSM\EnemyWaitState.h"
#include "fbEngine\CharacterController.h"
#include "fbEngine\_Object\_GameObject\Particle.h"
#include "fbEngine\_Object\_Component\_Physics\CapsuleColliderZ.h"
#include "GameObject\Enemy\EnemyAttack.h"

BossD::BossD(const char* name) : EnemyCharacter(name)
{

}

BossD::~BossD()
{
}

void BossD::_AwakeSubClass() {
	SetFileName("BossD.X");
}
void BossD::_StartSubClass() {

	//ポジション
	//_InitPos = Vector3(-202.0f, 120.0f, -156.0f);
	//transform->SetPosition(_InitPos);

	// 視野角生成。
	_ViewAngle = 100.0f;
	//_ViewRange = 30.0f;

	// 歩行速度設定。
	_walkSpeed = 2.5f;

	// 徘徊範囲設定。
	// ※暫定処理。
	//_WanderingRange = 20.0f;

	// 追跡範囲設定。
	//_discoveryRange = 30.0f;

	// 何回に一回くらい怯むか設定。
	_damageMotionRandNum = 30;

	//モデルにライト設定。
	_MyComponent.Model->SetLight(INSTANCE(GameObjectManager)->mainLight);
	//攻撃処理。
	_singleAttack.reset(new EnemySingleAttack(this));
	_singleAttack->Init(6.5f, static_cast<int>(AnimationBossD::Attack), 0.2f);

	_kick.reset(new EnemySingleAttack(this));
	_kick->Init(6.5f, static_cast<int>(AnimationBossD::Kick), 0.2f);

	_kiriage.reset(new EnemySingleAttack(this));
	_kiriage->Init(6.5f, static_cast<int>(AnimationBossD::Kiriage), 0.2f);

	_360Attack.reset(new EnemySingleAttack(this));
	_360Attack->Init(6.5f, static_cast<int>(AnimationBossD::Attack360), 0.2f);

	// 初期ステートに移行。
	// ※暫定処理。
	_initState = State::Wandering;
	_ChangeState(_initState);
}

void BossD::_UpdateSubClass() {

	if (!(_MyComponent.CharacterController->IsOnGround())) {
		// エネミーが地面から離れている。
		if (_NowStateIdx != State::Fall) {
			// 現在のステートタイプを保存。
			_saveState = _NowStateIdx;
			// 落下ステートに切り替え。
			_ChangeState(State::Fall);
		}
	}
}

void BossD::_LateUpdateSubClass()
{

}

EnemyAttack* BossD::_AttackSelectSubClass() {
	// ※プレイヤーとエネミーの位置関係とかで遷移先決定？。

	 //※とりあえず暫定処理。
	int rnd = rand() % 4;
	if (rnd == 0) {
		return _singleAttack.get();
	}
	else if(rnd == 1)
	{
		return _kiriage.get();
	}
	else if (rnd == 2)
	{
		return _360Attack.get();
	}
	else {
		return _kick.get();
	}
}
void BossD::AnimationEvent_Attack(){
	AttackCollision* attack = CreateAttack(Vector3(0.0f, 0.25f, 1.5f), Quaternion::Identity, Vector3(2.5f, 5.0f, 2.5f), 0.25f, transform);
	attack->RemoveParent();
}
void BossD::AnimationEvent_Kick(){
	AttackCollision* attack = CreateAttack(Vector3(0.0f, 0.25f, 2.0f), Quaternion::Identity, Vector3(2.5f, 3.0f, 3.0f), 0.25f, transform);
	attack->RemoveParent();
}
void BossD::AnimationEvent_Kiriage(){
	AttackCollision* attack = CreateAttack(Vector3(0.0f, 0.25f, 1.5f), Quaternion::Identity, Vector3(3.0f, 5.0f, 2.0f), 0.25f, transform);
	attack->RemoveParent();
}
void BossD::AnimationEvent_360Attack(){
	AttackCollision* attack = CreateAttack(Vector3(0.0f, 0.25f, 2.0f), Quaternion::Identity, Vector3(5.0f, 3.0f, 2.5f), 0.25f, transform);
	attack->RemoveParent();
}
void BossD::_EndNowStateCallback(State EndStateType) {

	if (EndStateType == State::Wandering) {
		// 徘徊ステート終了。
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
		_ChangeState(_saveState);
	}
	else if (EndStateType == State::Damage) {
		// 攻撃を受けた。
		// 攻撃開始。
		_ChangeState(State::StartAttack);
	}
	else if (EndStateType == State::Threat) {
		// 威嚇終了。
		// 攻撃開始。
		_ChangeState(State::StartAttack);
	}
}

void BossD::_ConfigCollision() {
	
	// キャラクターコントローラ用。
	{
		// コリジョンのサイズを決定。
		// ※キャラクターコントローラーで使用するためのもの。
		_collisionInfo.radius = 0.5f;
		_collisionInfo.height = 1.1f;
		_collisionInfo.offset = Vector3(0.0f, 1.0f, 0.0f);
		_collisionInfo.id = Collision_ID::BOSS;

		// 重力設定。
		_Gravity = -9.8f;

		// コンポーネントにカプセルコライダーを追加。
		_MyComponent.Collider = AddComponent<CCapsuleCollider>();
		// カプセルコライダーを作成。
		static_cast<CCapsuleCollider*>(_MyComponent.Collider)->Create(_collisionInfo.radius, _collisionInfo.height);

	}
}

void BossD::_ConfigCharacterController() {
	// 衝突する属性を設定(横)。
	_MyComponent.CharacterController->AttributeXZ_AllOn();
	_MyComponent.CharacterController->SubAttributeXZ(Collision_ID::ATTACK);
	_MyComponent.CharacterController->SubAttributeXZ(Collision_ID::SPACE);
	_MyComponent.CharacterController->SubAttributeXZ(Collision_ID::PLAYER);
	_MyComponent.CharacterController->SubAttributeXZ(Collision_ID::ENEMY);
	_MyComponent.CharacterController->SubAttributeXZ(Collision_ID::DROPITEM);
	// 衝突する属性を設定(縦)。
	_MyComponent.CharacterController->AttributeY_AllOn();
	_MyComponent.CharacterController->SubAttributeY(Collision_ID::ATTACK);
	_MyComponent.CharacterController->SubAttributeY(Collision_ID::ENEMY);
	_MyComponent.CharacterController->SubAttributeY(Collision_ID::PLAYER);
	_MyComponent.CharacterController->SubAttributeY(Collision_ID::SPACE);
	_MyComponent.CharacterController->SubAttributeY(Collision_ID::DROPITEM);

}

void BossD::_CreateExtrudeCollision() {
	_MyComponent.ExtrudeCollisions.push_back(_MyComponent.CharacterController->GetRigidBody());	// ついでに押し出しようコリジョンに追加しておく。
}

void BossD::_BuildAnimationSubClass(vector<double>& datas) {

	// アニメーションタイプにデータを関連づけ。
	// ※エネミーはすべて同じステートクラスを使用するため、ステートからアニメーションを再生できるよう
	//   EnemyCharacterクラスで定義されているすべてのエネミー共通の列挙子に関連付ける必要がある。
	{
		// 待機状態。
		_ConfigAnimationType(EnemyCharacter::AnimationType::Idle, static_cast<unsigned int>(AnimationBossD::Idle));
		// 歩行状態。
		_ConfigAnimationType(EnemyCharacter::AnimationType::Walk, static_cast<unsigned int>(AnimationBossD::Walk));
		// 走行状態。
		_ConfigAnimationType(EnemyCharacter::AnimationType::Dash, static_cast<unsigned int>(AnimationBossD::Run));
		// 吠える。
		_ConfigAnimationType(EnemyCharacter::AnimationType::Threat, static_cast<unsigned int>(AnimationBossD::DrawingaSword));
		// ダメージ反応。
		_ConfigAnimationType(EnemyCharacter::AnimationType::Damage, static_cast<unsigned int>(AnimationBossD::Damage));
		// 落下状態。
		_ConfigAnimationType(EnemyCharacter::AnimationType::Fall, static_cast<unsigned int>(AnimationBossD::Idle));
		// 死亡状態。
		_ConfigAnimationType(EnemyCharacter::AnimationType::Death, static_cast<unsigned int>(AnimationBossD::Damage));
	}
	datas[static_cast<int>(AnimationBossD::Run)] = 44.0f / 60.0f;
}

void BossD::_ConfigAnimationEvent() {
	//斬撃
	{
		float eventFrame = 0.9f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationBossD::Attack), eventFrame, static_cast<AnimationEvent>(&BossD::AnimationEvent_Attack));
	}
	//キック
	{
		float eventFrame = 0.9f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationBossD::Kick), eventFrame, static_cast<AnimationEvent>(&BossD::AnimationEvent_Kick));
	}
	//斬り上げ
	{
		float eventFrame = 0.9f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationBossD::Kiriage), eventFrame, static_cast<AnimationEvent>(&BossD::AnimationEvent_Kiriage));
	}
	//回転斬り
	{
		float eventFrame = 0.9f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationBossD::Attack360), eventFrame, static_cast<AnimationEvent>(&BossD::AnimationEvent_360Attack));
	}
}

