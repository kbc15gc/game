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

BossGhost::PairAttackType BossGhost::_selectNowPairAttack;

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
	EnemyPlaySound(EnemyCharacter::SoundIndex::Damage);
}

void BossGhost::CreateCollision2() {
	//攻撃コリジョン作成。
	AttackCollision* attack = CreateAttack(Vector3(0.0f, 0.25f, 1.0f), Quaternion::Identity, Vector3(0.5f, 1.0f, 1.5f), 0.25f, transform, false, false, AttackCollision::ReactionType::NotAction,85);
	attack->RemoveParent();

	// 攻撃音再生。
	EnemyPlaySound(EnemyCharacter::SoundIndex::Damage);
}

void BossGhost::LaserStartSingle()
{
	LaserBreath* laser = INSTANCE(GameObjectManager)->AddNew<LaserBreath>("laser", 3);
	laser->Create(this, Vector3::zero, 20.0f, 0.0025f, Vector3::axisY, 0.0f, "t1.png", Vector2(0.25f, 0.15f), 3.0f, Color::red,120);
	_singleLaser->BreathStart(laser);

	_MyComponent.Animation->SetAnimeSpeed(0.2f);
}

void BossGhost::LaserEndSingle() {
	_singleLaser->BreathEnd();
}

void BossGhost::LaserStart() {
	LaserBreath* laser = INSTANCE(GameObjectManager)->AddNew<LaserBreath>("laser", 3);
	laser->Create(this, Vector3::zero, 20.0f,0.0025f, Vector3::axisY,0.0f,"t1.png",Vector2(0.25f,0.15f),3.0f,Color::red,110);
	static_cast<EnemyBreathAttack*>(static_cast<EnemyWarpAttack*>(_laserComboAttack->GetOneAttack())->GetAttack())->BreathStart(laser);

	_MyComponent.Animation->SetAnimeSpeed(0.2f);
}

void BossGhost::LaserEnd() {
	static_cast<EnemyBreathAttack*>(static_cast<EnemyWarpAttack*>(_laserComboAttack->GetOneAttack())->GetAttack())->BreathEnd();
}

void BossGhost::LaserSpeed()
{
	_MyComponent.Animation->SetAnimeSpeed(0.2f);
}


void BossGhost::_AwakeSubClass() {
	// 使用するモデルファイルのパスを設定。
	SetFileName("Ghost.X");
}

void BossGhost::_StartSubClass() {

	// 視野角生成。
	_ViewAngle = 120.0f;
	//_ViewRange = 10.0f;


	// 徘徊範囲設定。
	// ※暫定処理。
	//_WanderingRange = 20.0f;

	// 追跡範囲設定。
	//_discoveryRange = 30.0f;

	// 歩行速度設定。
	_walkSpeed = 3.0f;

	// 何回に一回くらい怯むか設定。
	_damageMotionRandNum = 7;

	//モデルにライト設定。
	_MyComponent.Model->SetLight(INSTANCE(GameObjectManager)->mainLight);
	_MyComponent.Model->SetModelEffect(ModelEffectE::ALPHA);

	// 攻撃処理を定義。
	{
		_singleAttack.reset(new EnemySingleAttack(this));
		_singleAttack->Init(1.25f, static_cast<int>(AnimationBossGhost::Attack), 0.2f,0.7f);

		_singleLaser.reset(new EnemyBreathAttack(this));
		_singleLaser->Init(3.0f, static_cast<int>(AnimationBossGhost::Attack), 0.2f, 0.7f, 1, 2);

		_comboAttack.reset(new EnemyComboAttack(this));
		EnemyAttack* singleAttack = new EnemySingleAttack(this);
		singleAttack->Init(1.0f, static_cast<int>(AnimationBossGhost::Attack), 0.2f,0.7f,1,3);
		EnemyAttack* warpAttack = new EnemyWarpAttack(this);
		static_cast<EnemyWarpAttack*>(warpAttack)->Init(13.0f, singleAttack);
		_comboAttack->Init(13.0f, warpAttack);

		_laserComboAttack.reset(new EnemyComboAttack(this));
		EnemyAttack* singleAttack2 = new EnemyBreathAttack(this);
		singleAttack2->Init(7.0f, static_cast<int>(AnimationBossGhost::Attack), 0.2f, 0.7f, 1, 1);
		EnemyAttack* warpAttack2 = new EnemyWarpAttack(this);
		static_cast<EnemyWarpAttack*>(warpAttack2)->Init(13.0f, singleAttack2);
		_laserComboAttack->Init(13.0f, warpAttack2);
	}

	_pairAttackArray.push_back(_comboAttack.get());
	_pairAttackArray.push_back(_laserComboAttack.get());

	_isPairAttackReady = false;

	// 初期ステートに移行。
	// ※暫定処理。
	_initState = State::Wait;
	_ChangeState(_initState);
	static_cast<EnemyWaitState*>(_NowState)->CustamParameter(EnemyCharacter::AnimationType::Idle,0.2f,2,0,1.5f)/*->SetInterval(5.0f)*/;
}

void BossGhost::_UpdateSubClass() {
	if (_pairGhost) {
		if (_NowStateIdx == static_cast<EnemyCharacter::State>(BossGhostState::BossGhostPairAttack)) {
			// 共同攻撃状態。

			if (!_isPairAttackReady) {
				_isPairAttackReady = static_cast<GhostPairAttackState*>(_NowState)->GetIsEndWarp();
			}

			if (_isPairAttackReady) {
				// 自分の準備が完了している。
				if (static_cast<BossGhostState>(_pairGhost->GetNowStateIndex()) == BossGhostState::BossGhostPairAttack) {
					if (_pairGhost->GetIsPairAttackReady()) {
						// ペアの準備も完了している。

						static_cast<GhostPairAttackState*>(_NowState)->SetIsStartAttack(true);	// 攻撃を開始する。
					}
				}
			}
		}
	}
	else if(_NowStateIdx == static_cast<EnemyCharacter::State>(BossGhostState::BossGhostPairAttack)){
		// 共同攻撃中にペア関係が解除された。

		static_cast<GhostPairAttackState*>(_NowState)->SetIsStartAttack(true);	// 攻撃を開始する。
	}
}

void BossGhost::_LateUpdateSubClass()
{

}


EnemyAttack* BossGhost::_AttackSelectSubClass() {
	// ※プレイヤーとエネミーの位置関係とかで遷移先決定？。

	EnemyAttack* attack = nullptr;

	if (static_cast<BossGhostState>(_NowStateIdx) == BossGhostState::BossGhostPairAttack) {
		attack = _pairAttackArray[_selectNowPairAttack];
	}
	else {
		// ※とりあえず暫定処理。
		int rnd = rand() % 4;
		if (rnd == 0) {
			attack = _singleAttack.get();
		}
		else if (rnd == 1) {
			attack = _singleLaser.get();
		}
		else if (rnd == 2) {
			attack = _comboAttack.get();
		}
		else {
			attack = _laserComboAttack.get();
		}
	}
	return attack;
}

void BossGhost::_EndNowStateCallback(State EndStateType) {

	// とりあえず連続で共同攻撃しないようにする。
	if (EndStateType == static_cast<EnemyCharacter::State>(BossGhostState::BossGhostPairAttack)) {
		_isPairAttackReady = false;
		_ChangeState(State::Wait);
		static_cast<EnemyWaitState*>(_NowState)->SetInterval(3.0f);
	}
	else if (_pairGhost && _isCommand) {
		// ラスボスから命令を受けた。

		// 共同攻撃開始。
		_ChangeState(static_cast<EnemyCharacter::State>(BossGhostState::BossGhostPairAttack));
		static_cast<GhostPairAttackState*>(_NowState)->SetStartAttackInterval(_intervalStartPairAttack);
		_isCommand = false;
	}
	else if (EndStateType == State::Damage) {
		// 攻撃を受けた。
		// 攻撃開始。
		_ChangeState(State::StartAttack);
	}
	else if (EndStateType == State::StartAttack) {
		// 一度攻撃が終了した。

		if (!_isEntourage) {
			// もう一度攻撃開始。
			_ChangeState(State::StartAttack);
		}
		else {
			_ChangeState(State::Wait);
			static_cast<EnemyWaitState*>(_NowState)->SetInterval(3.0f);
		}
	}
	else if (EndStateType == State::Wait) {
		if (_isEntourage) {
			_ChangeState(State::StartAttack);
		}
		else {
			_ChangeState(State::Wait);
		}
	}
	else if (EndStateType == State::Threat && !_isEntourage) {
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
	
	// パンチ攻撃。
	{
		eventFrame = 0.5f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationBossGhost::Attack), eventFrame, static_cast<AnimationEvent>(&BossGhost::CreateCollision));
	}

	// 弱パンチ攻撃。
	{
		eventFrame = 0.5f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationBossGhost::Attack), eventFrame, static_cast<AnimationEvent>(&BossGhost::CreateCollision2),3);
	}

	// レーザー攻撃(単発)。
	{
		eventFrame = 0.5f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationBossGhost::Attack), eventFrame, static_cast<AnimationEvent>(&BossGhost::LaserStartSingle), 2);
		eventFrame = 1.5f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationBossGhost::Attack), eventFrame, static_cast<AnimationEvent>(&BossGhost::LaserEndSingle), 2);
	}

	// レーザー攻撃(コンボ)。
	{
		eventFrame = 0.5f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationBossGhost::Attack), eventFrame, static_cast<AnimationEvent>(&BossGhost::LaserStart),1);
		eventFrame = 1.5f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationBossGhost::Attack), eventFrame, static_cast<AnimationEvent>(&BossGhost::LaserEnd),1);
	}
}

