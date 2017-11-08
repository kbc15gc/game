#include"stdafx.h"
#include "BossGhost.h"
#include "fbEngine\_Object\_Component\_3D\SkinModel.h"
#include "HFSM\EnemyTranslationState.h"
#include "HFSM\EnemyWaitState.h"
#include "fbEngine\CharacterController.h"
#include "fbEngine\_Object\_GameObject\Particle.h"
#include "fbEngine\_Object\_Component\_Physics\CapsuleColliderZ.h"
#include "GameObject\Enemy\EnemyAttack.h"
#include "GameObject\Enemy\HFSM\GhostPairAttackState.h"

BossGhost::BossGhost(const char* name) : EnemyCharacter(name)
{

}


BossGhost::~BossGhost()
{
}

void BossGhost::CreateCollision() {
	//攻撃コリジョン作成。
	AttackCollision* attack = CreateAttack(Vector3(0.0f, 0.25f, 1.0f), Quaternion::Identity, Vector3(0.5f, 1.0f, 1.5f), 0.25f, transform);
	attack->RemoveParent();

	// 攻撃音再生。
	EnemyPlaySound(EnemyCharacter::SoundIndex::Attack1);
}

void BossGhost::_AwakeSubClass() {
	// 使用するモデルファイルのパスを設定。
	SetFileName("Ghost.X");
}

void BossGhost::_StartSubClass() {

	// 視野角生成。
	_ViewAngle = 160.0f;
	_ViewRange = 30.0f;

	// 歩行速度設定。
	_walkSpeed = 2.5f;

	// 徘徊範囲設定。
	// ※暫定処理。
	_WanderingRange = 20.0f;

	// 追跡範囲設定。
	_discoveryRange = 30.0f;

	// 何回に一回くらい怯むか設定。
	_damageMotionRandNum = 1;

	//モデルにライト設定。
	_MyComponent.Model->SetLight(INSTANCE(GameObjectManager)->mainLight);
	_MyComponent.Model->SetModelEffect(ModelEffectE::ALPHA);

	// 攻撃処理を定義。
	_comboAttack.reset(new GhostComboAttack(this));
	_comboAttack->Init(13.0f, static_cast<int>(AnimationBossGhost::Attack), 0.2f);
	_breathAttack.reset(new EnemyBreathAttack(this));
	_breathAttack->Init(13.0f, static_cast<int>(AnimationBossGhost::Attack), 0.2f);

	// 初期ステートに移行。
	// ※暫定処理。
	_initState = State::StartAttack;
	_ChangeState(_initState);
}

void BossGhost::_UpdateSubClass() {

	//if (!(_MyComponent.CharacterController->IsOnGround())) {
	//	// エネミーが地面から離れている。
	//	if (_NowStateIdx != State::Fall) {
	//		// 現在のステートタイプを保存。
	//		_saveState = _NowStateIdx;
	//		// 落下ステートに切り替え。
	//		_ChangeState(State::Fall);
	//	}
	//}
}

void BossGhost::_LateUpdateSubClass()
{

}


EnemyAttack* BossGhost::_AttackSelectSubClass() {
	// ※プレイヤーとエネミーの位置関係とかで遷移先決定？。

	// ※とりあえず暫定処理。
	//int rnd = rand() % 3;
	//if (rnd == 0) {
		return _comboAttack.get();
	//}
}

void BossGhost::_EndNowStateCallback(State EndStateType) {

	// とりあえず連続で共同攻撃しないようにする。
	if (EndStateType == static_cast<EnemyCharacter::State>(BossGhostState::BossGhostPairAttack)) {
		_ChangeState(State::StartAttack);
	}
	else if (_isCommand) {
		// ラスボスから命令を受けた。

		// 共同攻撃開始。
		_ChangeState(static_cast<EnemyCharacter::State>(BossGhostState::BossGhostPairAttack));
		_isCommand = false;
	}
	else if (EndStateType == State::Damage) {
		// 攻撃を受けた。
		// 攻撃開始。
		_ChangeState(State::StartAttack);
	}
	else if (EndStateType == State::StartAttack) {
		// 一度攻撃が終了した。

		// もう一度攻撃開始。
		_ChangeState(State::StartAttack);
	}
}

void BossGhost::_ConfigCollision() {
	// キャラクターコントローラ用。
	{
		// コリジョンのサイズを決定。
		// ※キャラクターコントローラーで使用するためのもの。
		_collisionInfo.radius = 0.3f;
		_collisionInfo.height = 0.5f;
		_collisionInfo.offset = Vector3(0.0f, 0.0f, 0.0f);
		_collisionInfo.id = Collision_ID::BOSS;

		// 重力設定。
		_Gravity = 0.0f;

		// コンポーネントにカプセルコライダーZを追加。
		_MyComponent.Collider = AddComponent</*CCapsuleCollider*/BoxCollider>();
		// カプセルコライダーを作成。
		//static_cast<CCapsuleCollider*>(_MyComponent.Collider)->Create(_collisionInfo.radius, _collisionInfo.height);
		static_cast<BoxCollider*>(_MyComponent.Collider)->Create(Vector3(0.6f, 1.7f,0.6f));
	}
}

void BossGhost::_ConfigCharacterController() {
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

void BossGhost::_CreateExtrudeCollision() {
	_MyComponent.ExtrudeCollisions.push_back(_MyComponent.CharacterController->GetRigidBody());	// 押し出しようコリジョンに追加しておく。
}

void BossGhost::_BuildStateSubClass() {
	// ゴースト共同攻撃ステート追加。
	_MyState.push_back(unique_ptr<GhostPairAttackState>(new GhostPairAttackState(this)));
}


void BossGhost::_BuildAnimationSubClass(vector<double>& datas) {

	// アニメーションタイプにデータを関連づけ。
	// ※エネミーはすべて同じステートクラスを使用するため、ステートからアニメーションを再生できるよう
	//   EnemyCharacterクラスで定義されているすべてのエネミー共通の列挙子に関連付ける必要がある。
	{
		// 待機状態。
		_ConfigAnimationType(EnemyCharacter::AnimationType::Idle, static_cast<unsigned int>(AnimationBossGhost::Wait));
		// 歩行状態。
		_ConfigAnimationType(EnemyCharacter::AnimationType::Walk, static_cast<unsigned int>(AnimationBossGhost::Walk));
		// 走行状態。
		_ConfigAnimationType(EnemyCharacter::AnimationType::Dash, static_cast<unsigned int>(AnimationBossGhost::Walk));
		// ダメージ反応。
		_ConfigAnimationType(EnemyCharacter::AnimationType::Damage, static_cast<unsigned int>(AnimationBossGhost::Damage));
		// 死亡状態。
		_ConfigAnimationType(EnemyCharacter::AnimationType::Death, static_cast<unsigned int>(AnimationBossGhost::Death));
	}

	datas[static_cast<int>(AnimationBossGhost::Damage)] = 10.0f / 30.0f;
	datas[static_cast<int>(AnimationBossGhost::Attack)] = 20.0f / 30.0f;
}

void BossGhost::_ConfigAnimationEvent() {
	float eventFrame = 0.1f;
	
	// コンボ攻撃。
	{
		eventFrame = 0.5f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationBossGhost::Attack), eventFrame, static_cast<AnimationEvent>(&BossGhost::CreateCollision));
	}
}

void BossGhost::_BuildSoundTable() {
	// 攻撃音登録。
	_ConfigSoundData(EnemyCharacter::SoundIndex::Attack1, "Damage_01.wav", false, false);
	_ConfigSoundData(EnemyCharacter::SoundIndex::Attack2, "Buoonn.wav", false, false);
	_ConfigSoundData(EnemyCharacter::SoundIndex::Attack3, "Buoonn.wav", false, false);
}

