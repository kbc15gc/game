#include"stdafx.h"
#include "BossGolem.h"
#include "fbEngine\_Object\_Component\_3D\SkinModel.h"
#include "HFSM\EnemyTranslationState.h"
#include "HFSM\EnemyWaitState.h"
#include "fbEngine\CharacterController.h"
#include "fbEngine\_Object\_GameObject\Particle.h"
#include "fbEngine\_Object\_Component\_Physics\CapsuleColliderZ.h"
#include "GameObject\Enemy\EnemyAttack.h"

BossGolem::BossGolem(const char* name) : EnemyCharacter(name)
{

}

BossGolem::~BossGolem()
{
}

void BossGolem::OnDestroy() {
	if (_kobusi) {
		INSTANCE(GameObjectManager)->AddRemoveList(_kobusi);
		_kobusi = nullptr;
	}
	EnemyCharacter::OnDestroy();
}

void BossGolem::_AwakeSubClass(){
	SetFileName("Boss_Golem_idle.X");
}
void BossGolem::_StartSubClass() {

	//ポジション
	/*_InitPos = Vector3(-202.0f, 60.0f, -156.0f);
	transform->SetPosition(_InitPos);*/

	// 視野角生成。
	//_ViewAngle = 100.0f;
	//_ViewRange = 30.0f;

	// 歩行速度設定。
	_walkSpeed = 2.5f;

	// 徘徊範囲設定。
	// ※暫定処理。
	//_WanderingRange = 20.0f;

	// 追跡範囲設定。
	//_discoveryRange = 30.0f;

	// 何回に一回くらい怯むか設定。
	_damageMotionRandNum = 33;

	//モデルにライト設定。
	_MyComponent.Model->SetLight(INSTANCE(GameObjectManager)->mainLight);
	//攻撃処理。
	{
		// 頭突き。
		_singleAttack.reset(new EnemySingleAttack(this));
		_singleAttack->Init(1.6f, static_cast<int>(AnimationBossGolem::Hit), 0.0f,0.5f /*0.4f*/);

		// 拳。
		_singleAttackSecondPattern.reset(new EnemySingleAttack(this));
		_singleAttackSecondPattern->Init(1.6f, static_cast<int>(AnimationBossGolem::Hit2), 0.0f,0.65f/*, 0.4f*/);
	}
	// 初期ステートに移行。
	// ※暫定処理。
	_initState = State::Wandering;
	_ChangeState(_initState);
}

void BossGolem::_UpdateSubClass() {

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

void BossGolem::_LateUpdateSubClass()
{

}

EnemyAttack* BossGolem::_AttackSelectSubClass() {
	// ※プレイヤーとエネミーの位置関係とかで遷移先決定？。

	// ※とりあえず暫定処理。
	int rnd = rand() % 2;
	if (rnd == 0) {
		return _singleAttack.get();
	}
	else {
		return _singleAttackSecondPattern.get();
	}
}

void BossGolem::AnimationEvent_KobushiSpeed() {
	_MyComponent.Animation->SetAnimeSpeed(0.05f);
}

void BossGolem::AnimationEvent_KobushiSpeed2() {
	_MyComponent.Animation->SetAnimeSpeed(0.3f);
}

void BossGolem::AnimationEvent_Kobushi() {
	Quaternion rot = Quaternion::Identity;
	rot.SetRotation(Vector3::axisY, D3DXToRadian(-45.0f));
	//_kobusi = CreateAttack(Vector3(-1.1f, 0.25f, 0.7f), rot, Vector3(1.0f, 3.0f, 2.0f), -1.0f, transform);
	//attack->RemoveParent();
	_kobusi = CreateAttack(Vector3(-1.1f, 0.25f, 0.5f), rot, Vector3(1.0f, 3.0f, 2.0f), -1.0f, transform,false,false,AttackCollision::ReactionType::Leans,60);

}

void BossGolem::AnimationEvent_Kobushi2() {
	if (_kobusi) {
		_kobusi->transform->SetLocalPosition(Vector3(-1.0f, 0.25f, 1.5f));
		Quaternion rot = _kobusi->transform->GetLocalRotation();
		Quaternion work = Quaternion::Identity;
		work.SetRotation(Vector3::axisY, D3DXToRadian(45.0f));
		rot.Multiply(work);
		_kobusi->transform->SetLocalRotation(rot);
	}
}

void BossGolem::AnimationEvent_Kobushi3(){
	if (_kobusi) {
		_kobusi->transform->SetLocalPosition(Vector3(0.0f, 0.25f, 1.5f));
		//Quaternion rot = _kobusi->transform->GetLocalRotation();
		//Quaternion work = Quaternion::Identity;
		//work.SetRotation(Vector3::axisY, D3DXToRadian(45.0f));
		//rot.Multiply(work);
		//_kobusi->transform->SetLocalRotation(rot);
	}

	//if (_kobusi) {
	//	_kobusi->transform->SetLocalPosition(Vector3());
	//}
}

void BossGolem::AnimationEvent_Kobushi4() {
	if (_kobusi) {
		INSTANCE(GameObjectManager)->AddRemoveList(_kobusi);
		_kobusi = nullptr;
	}
}


void BossGolem::AnimationEvent_ZutukiSpeed() {
	_MyComponent.Animation->SetAnimeSpeed(0.02f);
}

void BossGolem::AnimationEvent_ZutukiSpeed2() {
	_MyComponent.Animation->SetAnimeSpeed(/*0.1f*/0.5f);
}

void BossGolem::AnimationEvent_ZutukiSpeed3() {
	_MyComponent.Animation->SetAnimeSpeed(/*0.35f*/0.5f);
}

void BossGolem::AnimationEvent_Zutuki() {
	CreateAttack(Vector3(0.0f, 0.25f, 2.0f), Quaternion::Identity, Vector3(1.0f, 2.0f, 2.35f), 0.25f, transform, false, false, AttackCollision::ReactionType::Leans, 120);
	//attack->RemoveParent();
}

void BossGolem::_EndNowStateCallback(State EndStateType) {

	if (EndStateType == State::Wandering) {
		// 徘徊ステート終了。
		SetAnimationSpeed(10.0f);
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

void BossGolem::_ConfigCollision() {
	// キャラクターコントローラ用。
	{
		// コリジョンのサイズを決定。
		// ※キャラクターコントローラーで使用するためのもの。
		_collisionInfo.radius = 1.0f;
		_collisionInfo.height = 0.25f;
		_collisionInfo.offset = Vector3(0.0f, 0.375f, 0.0f);
		_collisionInfo.id = Collision_ID::BOSS;

		// 重力設定。
		_Gravity = -9.8f;

		// コンポーネントにカプセルコライダーZを追加。
		_MyComponent.Collider = AddComponent<CCapsuleCollider>();
		// カプセルコライダーを作成。
		static_cast<CCapsuleCollider*>(_MyComponent.Collider)->Create(_collisionInfo.radius, _collisionInfo.height);
	}
}

void BossGolem::_ConfigCharacterController() {
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

void BossGolem::_CreateExtrudeCollision() {
	_MyComponent.ExtrudeCollisions.push_back(_MyComponent.CharacterController->GetRigidBody());	// ついでに押し出しようコリジョンに追加しておく。
}

void BossGolem::_BuildAnimationSubClass(vector<double>& datas) {

	// アニメーションタイプにデータを関連づけ。
	// ※エネミーはすべて同じステートクラスを使用するため、ステートからアニメーションを再生できるよう
	//   EnemyCharacterクラスで定義されているすべてのエネミー共通の列挙子に関連付ける必要がある。
	{
		// 待機状態。
		_ConfigAnimationType(EnemyCharacter::AnimationType::Idle, static_cast<unsigned int>(AnimationBossGolem::Idle));
		// 歩行状態。
		_ConfigAnimationType(EnemyCharacter::AnimationType::Walk, static_cast<unsigned int>(AnimationBossGolem::Walk));
		// 走行状態。
		_ConfigAnimationType(EnemyCharacter::AnimationType::Dash, static_cast<unsigned int>(AnimationBossGolem::Walk));
		// 吠える。
		_ConfigAnimationType(EnemyCharacter::AnimationType::Threat, static_cast<unsigned int>(AnimationBossGolem::IdleAction));
		// ダメージ反応。
		_ConfigAnimationType(EnemyCharacter::AnimationType::Damage, static_cast<unsigned int>(AnimationBossGolem::Damage));
		// 落下状態。
		_ConfigAnimationType(EnemyCharacter::AnimationType::Fall, static_cast<unsigned int>(AnimationBossGolem::Fly));
		// 死亡状態。
		_ConfigAnimationType(EnemyCharacter::AnimationType::Death, static_cast<unsigned int>(AnimationBossGolem::Die));
	}
	
}

void BossGolem::_ConfigAnimationEvent() {
	//拳
	{
		float eventFrame;
		//float eventFrame = 0.1f;
		//_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationBossGolem::Hit2), eventFrame, static_cast<AnimationEvent>(&BossGolem::AnimationEvent_KobushiSpeed));

		//eventFrame = 0.15f;
		//_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationBossGolem::Hit2), eventFrame, static_cast<AnimationEvent>(&BossGolem::AnimationEvent_KobushiSpeed2));

		eventFrame = 0.17f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationBossGolem::Hit2), eventFrame, static_cast<AnimationEvent>(&BossGolem::AnimationEvent_Kobushi));
		eventFrame = 0.2f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationBossGolem::Hit2), eventFrame, static_cast<AnimationEvent>(&BossGolem::AnimationEvent_Kobushi2));
		eventFrame= 0.25f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationBossGolem::Hit2), eventFrame, static_cast<AnimationEvent>(&BossGolem::AnimationEvent_Kobushi3));
		eventFrame = 0.3f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationBossGolem::Hit2), eventFrame, static_cast<AnimationEvent>(&BossGolem::AnimationEvent_Kobushi4));
	}
	//頭突き
	{
		float eventFrame = 0.19f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationBossGolem::Hit), eventFrame, static_cast<AnimationEvent>(&BossGolem::AnimationEvent_ZutukiSpeed));
		
		eventFrame = 0.20f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationBossGolem::Hit), eventFrame, static_cast<AnimationEvent>(&BossGolem::AnimationEvent_ZutukiSpeed2));

		eventFrame = 0.24f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationBossGolem::Hit), eventFrame, static_cast<AnimationEvent>(&BossGolem::AnimationEvent_ZutukiSpeed3));

		eventFrame = 0.25f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationBossGolem::Hit), eventFrame, static_cast<AnimationEvent>(&BossGolem::AnimationEvent_Zutuki));
	}
}

