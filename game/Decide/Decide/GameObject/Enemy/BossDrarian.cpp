#include"stdafx.h"
#include "BossDrarian.h"
#include "fbEngine\_Object\_Component\_3D\SkinModel.h"
#include "HFSM\EnemyTranslationState.h"
#include "HFSM\EnemyWaitState.h"
#include "fbEngine\CharacterController.h"
#include "fbEngine\_Object\_GameObject\Particle.h"
#include "fbEngine\_Object\_Component\_Physics\CapsuleColliderZ.h"


BossDrarian::BossDrarian(const char* name) : EnemyCharacter(name)
{

}


BossDrarian::~BossDrarian()
{
}

void BossDrarian::_AwakeSubClass() {
	// 使用するモデルファイルのパスを設定。
	SetFileName("DRARIAN.X");
}

void BossDrarian::_StartSubClass() {

	// 視野角生成。
	_ViewAngle = 100.0f;
	_ViewRange = 30.0f;

	// 攻撃可能範囲設定。
	_AttackRange = 5.5f;

	// 歩行速度設定。
	_walkSpeed = 2.5f;

	// 徘徊範囲設定。
	// ※暫定処理。
	_WanderingRange = 20.0f;

	// 追跡範囲設定。
	_discoveryRange = 30.0f;

	//モデルにライト設定。
	_MyComponent.Model->SetLight(INSTANCE(GameObjectManager)->mainLight);

	// 攻撃処理を定義。
	_singleAttack.reset(new EnemySingleAttack(this));
	_singleAttack->Init(_AnimationData[static_cast<int>(EnemyCharacter::AnimationType::Attack1)].No, 0.2f);
	_tailAttack.reset(new EnemySingleAttack(this));
	_tailAttack->Init(static_cast<int>(AnimationBossDrarian::TailAttackRight), 0.2f);

	// 攻撃処理に使用するパーティクル設定。
	ParticleParameter param;
	param.Init();
	param.texturePath = "t1.png";
	param.alphaBlendMode = 1;
	param.addVelocityRandomMargih = Vector3::zero;
	param.brightness = 7.0f;
	param.fadeTime = 0.2f;
	param.gravity = 0.0f;
	param.initAlpha = 1.0f;
	param.initPositionRandomMargin = Vector3::zero;
	param.initVelocity = Vector3::front * 10.0f;
	param.initVelocityVelocityRandomMargin = Vector3::zero;
	param.intervalTime = 0.001f;
	param.isBillboard = true;
	param.isFade = true;
	param.life = -1.0f;
	param.size = Vector2(0.5f, 0.5f);

	// ブレス攻撃生成。
	_breathAttack.reset(new EnemyBreathAttack(this, param, Vector3(0.0f, 0.0f, 5.0f)));
	_breathAttack->Init(static_cast<int>(AnimationBossDrarian::Breath), 0.2f);

	// 初期ステートに移行。
	// ※暫定処理。
	_ChangeState(State::Wandering);
}

void BossDrarian::_UpdateSubClass() {

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

void BossDrarian::_LateUpdateSubClass()
{

}


EnemyAttack* BossDrarian::AttackSelect() {
	// ※プレイヤーとエネミーの位置関係とかで遷移先決定？。

	// ※とりあえず暫定処理。
	int rnd = rand() % 3;
	if (rnd == 0) {
		return _singleAttack.get();
	}
	else if (rnd == 1) {
		return _breathAttack.get();
	}
	else {
		return _tailAttack.get();
	}
}

void BossDrarian::AnimationEvent_Kamituki() {
	//攻撃コリジョン作成。
	AttackCollision* attack = CreateAttack(Vector3(0.0f, 0.25f, 5.0f), Quaternion::Identity, Vector3(1.0f, 2.0f, 2.0f), 0.25f, transform);
	attack->RemoveParent();

	// 攻撃音再生。
	EnemyPlaySound(EnemyCharacter::SoundIndex::Attack1);
}

void BossDrarian::CreateAttackCollision_TailAttackSub1() 
{
	AttackCollision* attack = CreateAttack(Vector3(1.0f, -1.5f, 2.0f), Quaternion::Identity, Vector3(1.0f, 1.0f, 1.0f), 0.15f, transform,false,true,20);
	attack->RemoveParent();
}

void BossDrarian::CreateAttackCollision_TailAttackSub2() 
{
	AttackCollision* attack = CreateAttack(Vector3(-1.0f, -1.5f, 2.5f), Quaternion::Identity, Vector3(1.0f, 1.0f, 1.0f), 0.15f, transform, false, true, 30);
	attack->RemoveParent();
}

void BossDrarian::CreateAttackCollision_TailAttackSub3()
{
	AttackCollision* attack = CreateAttack(Vector3(1.0f, -1.5f, 1.5f), Quaternion::Identity, Vector3(1.0f, 1.0f, 1.0f), 0.15f, transform, false, true, 35);
	attack->RemoveParent();
}

void BossDrarian::CreateAttackCollision_TailAttackSub4()
{
	AttackCollision* attack = CreateAttack(Vector3(0.0f, -1.5f, 2.5f), Quaternion::Identity, Vector3(3.0f, 1.0f, 1.0f), 0.15f, transform, false, true, 40);
	attack->RemoveParent();
}


void BossDrarian::CreateAttackCollision_TailAttack1() {
	//攻撃コリジョン作成。
	Quaternion rot = Quaternion::Identity;
	rot.SetRotation(Vector3::axisY, D3DXToRadian(-40.0f));
	AttackCollision* attack = CreateAttack(Vector3(2.0f, 0.0f, -2.0f), rot, Vector3(2.0f, 2.0f, 5.0f), 0.15f, transform);
	attack->RemoveParent();

	// 攻撃音再生。
	EnemyPlaySound(EnemyCharacter::SoundIndex::Attack2);
}

void BossDrarian::CreateAttackCollision_TailAttack2() {
	//攻撃コリジョン作成。
	Quaternion rot = Quaternion::Identity;
	rot.SetRotation(Vector3::axisY, D3DXToRadian(-60.0f));
	AttackCollision* attack = CreateAttack(Vector3(3.0f, 0.0f, 0.0f), rot, Vector3(2.0f, 2.0f, 5.0f), 0.15f, transform);
	attack->RemoveParent();

	// 攻撃音再生。
	EnemyPlaySound(EnemyCharacter::SoundIndex::Attack2);
}

void BossDrarian::CreateAttackCollision_TailAttack3() {
	//攻撃コリジョン作成。
	AttackCollision* attack = CreateAttack(Vector3(4.0f, 0.0f, 2.0f), Quaternion::Identity, Vector3(4.0f, 2.0f, 2.0f), 0.15f, transform);
	attack->RemoveParent();

	// 攻撃音再生。
	EnemyPlaySound(EnemyCharacter::SoundIndex::Attack2);
}

void BossDrarian::CreateAttackCollision_TailAttack4() {
	//攻撃コリジョン作成。
	AttackCollision* attack = CreateAttack(Vector3(3.0f, 0.0f, 4.5f), Quaternion::Identity, Vector3(2.0f, 2.0f, 3.0f), 0.15f, transform);
	attack->RemoveParent();

	// 攻撃音再生。
	EnemyPlaySound(EnemyCharacter::SoundIndex::Attack2);
}

void BossDrarian::AnimationEvent_BreathStart() {
	_breathAttack->BreathStart();
	// 攻撃音再生。
	EnemyPlaySound(EnemyCharacter::SoundIndex::Attack3);
}

void BossDrarian::AnimationEvent_BreathEnd() {
	_breathAttack->BreathEnd();
}

void BossDrarian::CreateAttackCollision_BreathAttackSub1()
{
	AttackCollision* attack = CreateAttack(Vector3(1.5f, -1.5f, 2.5f), Quaternion::Identity, Vector3(1.0f, 1.0f, 1.0f), 0.25f, transform, false, true, 30);
	attack->RemoveParent();
}
void BossDrarian::CreateAttackCollision_BreathAttackSub2() 
{
	AttackCollision* attack = CreateAttack(Vector3(-1.5f, -1.5f, 2.5f), Quaternion::Identity, Vector3(1.0f, 1.0f, 1.0f), 0.25f, transform, false, true, 30);
	attack->RemoveParent();
}

void BossDrarian::_EndNowStateCallback(State EndStateType) {
	if (EndStateType == State::Wandering) {
		// 徘徊ステート終了。
		_ChangeState(State::Wandering);
	}
	else if (EndStateType == State::Discovery) {
		// 発見ステートの処理完了。

		_ChangeState(State::StartAttack);
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
		// 発見状態に移行。
		_ChangeState(State::Discovery);
	}
	else if (EndStateType == State::Threat) {
		// 威嚇終了。
		// 発見状態に移行。
		_ChangeState(State::Discovery);
	}
}

void BossDrarian::_ConfigCollision() {
	// 攻撃判定用のコリジョン。
	{
		// 足元。
		{
			RigidBody* coll = AddComponent<RigidBody>();	// キャラクターコントローラとは別に新しく作成(プレイヤーをキャラコンの形状で押し出したくないため)。

			RigidBodyInfo info;
			info.coll = AddComponent<BoxCollider>();
			static_cast<BoxCollider*>(info.coll)->Create(Vector3(3.0f, 3.0f,1.0f));
			info.id = Collision_ID::ENEMY;
			info.mass = 0.0f;
			info.physicsType = Collision::PhysicsType::Kinematick;
			info.offset = Vector3(0.0f,0.0f,2.0f);
			info.rotation = Quaternion::Identity;
			coll->Create(info, true);

			_MyComponent.ExtrudeCollisions.push_back(coll);	// ついでに押し出しようコリジョンに追加しておく。
		}
		// 胴体。
		{
			RigidBody* coll = AddComponent<RigidBody>();	// キャラクターコントローラとは別に新しく作成(プレイヤーをキャラコンの形状で押し出したくないため)。

			RigidBodyInfo info;
			info.coll = AddComponent<CCapsuleColliderZ>();
			static_cast<CCapsuleColliderZ*>(info.coll)->Create(0.75f, 8.1f);
			info.id = Collision_ID::ENEMY;
			info.mass = 0.0f;
			info.physicsType = Collision::PhysicsType::Kinematick;
			info.offset = Vector3(0.0f,1.0f,0.0f);
			info.rotation = Quaternion::Identity;
			coll->Create(info, true);

			_MyComponent.ExtrudeCollisions.push_back(coll);	// ついでに押し出しようコリジョンに追加しておく。
		}
	}

	// キャラクターコントローラ用。
	{
		// コリジョンのサイズを決定。
		// ※キャラクターコントローラーで使用するためのもの。
		_collisionInfo.radius = 1.8f;
		_collisionInfo.height = 6.0f;
		_collisionInfo.offset = Vector3(0.0f, 0.125f, 0.0f);
		_collisionInfo.id = Collision_ID::CHARACTER_GHOST;

		// 重力設定。
		_Gravity = -9.8f;

		// コンポーネントにカプセルコライダーZを追加。
		_MyComponent.Collider = AddComponent<CCapsuleColliderZ>();
		// カプセルコライダーを作成。
		static_cast<CCapsuleColliderZ*>(_MyComponent.Collider)->Create(_collisionInfo.radius, _collisionInfo.height); 
	}
}

void BossDrarian::_ConfigCharacterController() {
	// 衝突する属性を設定(横)。
	_MyComponent.CharacterController->AttributeXZ_AllOn();
	_MyComponent.CharacterController->SubAttributeXZ(Collision_ID::ATTACK);
	_MyComponent.CharacterController->SubAttributeXZ(Collision_ID::SPACE);
	_MyComponent.CharacterController->SubAttributeXZ(Collision_ID::PLAYER);
	// 衝突する属性を設定(縦)。
	_MyComponent.CharacterController->AttributeY_AllOn();
	_MyComponent.CharacterController->SubAttributeY(Collision_ID::ATTACK);
	_MyComponent.CharacterController->SubAttributeY(Collision_ID::ENEMY);
	_MyComponent.CharacterController->SubAttributeY(Collision_ID::PLAYER);
	_MyComponent.CharacterController->SubAttributeY(Collision_ID::SPACE);

}

void BossDrarian::_CreateExtrudeCollision() {
}

void BossDrarian::_BuildAnimation() {
	vector<unique_ptr<AnimationData>> Datas;
	for (int idx = 0; idx < _MyComponent.Animation->GetNumAnimationSet(); idx++) {
		// アニメーションセットの番号と再生時間をセットにしたデータを作成。
		unique_ptr<AnimationData> data(new AnimationData);
		data->No = idx;
		data->Time = -1.0f;	// すべて1秒以上のアニメーションなので、時間は設定しない。
		// 配列に追加。
		Datas.push_back(move(data));
	}

	// アニメーションタイプにデータを関連づけ。
	// ※エネミーはすべて同じステートクラスを使用するため、ステートからアニメーションを再生できるよう
	//   EnemyCharacterクラスで定義されているすべてのエネミー共通の列挙子に関連付ける必要がある。
	{
		// 待機状態。
		_ConfigAnimationType(EnemyCharacter::AnimationType::Idle, *Datas[static_cast<int>(AnimationBossDrarian::Wait)].get());
		// 歩行状態。
		_ConfigAnimationType(EnemyCharacter::AnimationType::Walk, *Datas[static_cast<int>(AnimationBossDrarian::Walk)].get());
		// 走行状態。
		_ConfigAnimationType(EnemyCharacter::AnimationType::Dash, *Datas[static_cast<int>(AnimationBossDrarian::Dash)].get());
		// 吠える。
		_ConfigAnimationType(EnemyCharacter::AnimationType::Threat, *Datas[static_cast<int>(AnimationBossDrarian::Barking)].get());
		// 攻撃状態(噛みつき)。
		_ConfigAnimationType(EnemyCharacter::AnimationType::Attack1, *Datas[static_cast<int>(AnimationBossDrarian::Attack)].get());
		// 攻撃状態(しっぽ)。
		_ConfigAnimationType(EnemyCharacter::AnimationType::Attack2, *Datas[static_cast<int>(AnimationBossDrarian::TailAttackRight)].get());
		// 攻撃状態(ブレス)。
		_ConfigAnimationType(EnemyCharacter::AnimationType::Attack3, *Datas[static_cast<int>(AnimationBossDrarian::Breath)].get());
		// ダメージ反応。
		Datas[static_cast<int>(AnimationBossDrarian::Damage)]->Time = 8.0f / 30.0f;
		_ConfigAnimationType(EnemyCharacter::AnimationType::Damage, *Datas[static_cast<int>(AnimationBossDrarian::Damage)].get());
		//// 落下状態。
		//// ※このオブジェクトには落下のアニメーションがないので待機アニメーションで代用。
		//_ConfigAnimationType(EnemyCharacter::AnimationType::Fall, *Datas[static_cast<int>(AnimationProt::Stand)].get());
		// 死亡状態。
		_ConfigAnimationType(EnemyCharacter::AnimationType::Death, *Datas[static_cast<int>(AnimationBossDrarian::Death)].get());
	}
}

void BossDrarian::_ConfigAnimationEvent() {
	// かみつき攻撃。
	{
		float eventFrame = 0.3f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationBossDrarian::Attack), eventFrame, static_cast<AnimationEvent>(&BossDrarian::AnimationEvent_Kamituki));
	}
	// しっぽ攻撃。
	{
		float eventFrame = 0.3f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationBossDrarian::TailAttackRight), eventFrame, static_cast<AnimationEvent>(&BossDrarian::CreateAttackCollision_TailAttackSub1));

		eventFrame = 0.6f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationBossDrarian::TailAttackRight), eventFrame, static_cast<AnimationEvent>(&BossDrarian::CreateAttackCollision_TailAttackSub2));

		eventFrame = 1.1f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationBossDrarian::TailAttackRight), eventFrame, static_cast<AnimationEvent>(&BossDrarian::CreateAttackCollision_TailAttackSub3));

		eventFrame = 2.0f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationBossDrarian::TailAttackRight), eventFrame, static_cast<AnimationEvent>(&BossDrarian::CreateAttackCollision_TailAttack1));

		eventFrame = 2.1f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationBossDrarian::TailAttackRight), eventFrame, static_cast<AnimationEvent>(&BossDrarian::CreateAttackCollision_TailAttack2));

		eventFrame = 2.2f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationBossDrarian::TailAttackRight), eventFrame, static_cast<AnimationEvent>(&BossDrarian::CreateAttackCollision_TailAttack3));

		eventFrame = 2.3f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationBossDrarian::TailAttackRight), eventFrame, static_cast<AnimationEvent>(&BossDrarian::CreateAttackCollision_TailAttack4));

		eventFrame = 4.0f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationBossDrarian::TailAttackRight), eventFrame, static_cast<AnimationEvent>(&BossDrarian::CreateAttackCollision_TailAttackSub4));

	}
	// ブレス攻撃。
	{
		float eventFrame = 0.6f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationBossDrarian::Breath), eventFrame, static_cast<AnimationEvent>(&BossDrarian::CreateAttackCollision_BreathAttackSub1));

		eventFrame = 1.3f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationBossDrarian::Breath), eventFrame, static_cast<AnimationEvent>(&BossDrarian::CreateAttackCollision_BreathAttackSub2));

		eventFrame = 2.7f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationBossDrarian::Breath), eventFrame, static_cast<AnimationEvent>(&BossDrarian::AnimationEvent_BreathStart));

		eventFrame = 4.0f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationBossDrarian::Breath), eventFrame, static_cast<AnimationEvent>(&BossDrarian::AnimationEvent_BreathEnd));
	
		eventFrame = 5.0f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationBossDrarian::Breath), eventFrame, static_cast<AnimationEvent>(&BossDrarian::CreateAttackCollision_TailAttackSub1));

		eventFrame = 5.5f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationBossDrarian::Breath), eventFrame, static_cast<AnimationEvent>(&BossDrarian::CreateAttackCollision_TailAttackSub2));
	}
}

void BossDrarian::_BuildSoundTable() {
	// 攻撃音登録。
	_ConfigSoundData(EnemyCharacter::SoundIndex::Attack1, "Damage_01.wav", false, false);
	_ConfigSoundData(EnemyCharacter::SoundIndex::Attack2, "Buoonn.wav", false, false);
	_ConfigSoundData(EnemyCharacter::SoundIndex::Attack3, "Buoonn.wav", false, false);
}


// EnemyBreathAttack。
EnemyBreathAttack::EnemyBreathAttack(EnemyCharacter* object, ParticleParameter& param, const Vector3& emitPos):EnemyAttack(object) {
	_player = INSTANCE(GameObjectManager)->FindObject("Player");
	_initParticleParam = param;
	AddParticleEmitter(param, emitPos,_enemyObject->transform);
}

bool EnemyBreathAttack::Update() {
	ParticleParameter param = _particleEmitter[0]->GetParam();
	param.size.y -= 0.1f * Time::DeltaTime();
	_particleEmitter[0]->SetParam(param);

	if (!_isPlaying) {
		// モーション再生終了。
		return true;
	}
	return false;
}
