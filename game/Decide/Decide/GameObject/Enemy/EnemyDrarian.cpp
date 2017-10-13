#include"stdafx.h"
#include "EnemyDrarian.h"
#include "fbEngine\_Object\_Component\_3D\SkinModel.h"
#include "HFSM\EnemyTranslationState.h"
#include "HFSM\EnemyWaitState.h"
#include "fbEngine\CharacterController.h"
#include "fbEngine\_Object\_GameObject\Particle.h"
#include "fbEngine\_Object\_Component\_Physics\CapsuleColliderZ.h"
#include "GameObject\Enemy\EnemyAttack.h"

EnemyDrarian::EnemyDrarian(const char* name) : EnemyCharacter(name)
{

}


EnemyDrarian::~EnemyDrarian()
{
}

void EnemyDrarian::_AwakeSubClass() {
	// 使用するモデルファイルのパスを設定。
	SetFileName("DRARIAN.X");
}

void EnemyDrarian::_StartSubClass() {

	// 視野角生成。
	_ViewAngle = 100.0f;
	_ViewRange = 30.0f;

	// 歩行速度設定。
	_walkSpeed = 2.5f;

	// 徘徊範囲設定。
	// ※暫定処理。
	_WanderingRange = 130.0f;

	//モデルにライト設定。
	_MyComponent.Model->SetLight(INSTANCE(GameObjectManager)->mainLight);

	// 攻撃処理を定義。
	_singleAttack.reset(new EnemySingleAttack(this));
	_singleAttack->Init(_AnimationData[static_cast<int>(EnemyCharacter::AnimationType::Attack1)].No, 0.2f);

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

	// 初期ステートに移行。
	// ※暫定処理。
	_ChangeState(State::Wandering);
}

void EnemyDrarian::_UpdateSubClass() {

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

void EnemyDrarian::_LateUpdateSubClass()
{

}


EnemyAttack* EnemyDrarian::_AttackSelectSubClass() {
	// ※プレイヤーとエネミーの位置関係とかで遷移先決定？。

	// ※とりあえず暫定処理。
	return _singleAttack.get();
}

void EnemyDrarian::AnimationEvent_Kamituki() {
	//攻撃コリジョン作成。
	AttackCollision* attack = CreateAttack(Vector3(0.0f, 0.25f, 5.0f), Quaternion::Identity, Vector3(1.0f, 2.0f, 2.0f), 0.25f, transform);
	attack->RemoveParent();

	// 攻撃音再生。
	EnemyPlaySound(EnemyCharacter::SoundIndex::Attack1);
}


void EnemyDrarian::_EndNowStateCallback(State EndStateType) {
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

void EnemyDrarian::_ConfigCollision() {

	// コリジョンのサイズを決定。
	// ※キャラクターコントローラーで使用するためのもの。
	_collisionInfo.radius = 1.8f;
	_collisionInfo.height = 6.0f;
	_collisionInfo.offset = 0.125f;

	//// 重力設定。
	//_Gravity = -9.8f;

	// コンポーネントにカプセルコライダーを追加。

	_MyComponent.Collider = AddComponent<CCapsuleColliderZ>();
	// カプセルコライダーを作成。
	static_cast<CCapsuleColliderZ*>(_MyComponent.Collider)->Create(_collisionInfo.radius, _collisionInfo.height);
}

void EnemyDrarian::_ConfigCharacterController() {
	// 衝突する属性を設定(横)。
	_MyComponent.CharacterController->AttributeXZ_AllOn();
	_MyComponent.CharacterController->SubAttributeXZ(Collision_ID::ATTACK);
	_MyComponent.CharacterController->SubAttributeXZ(Collision_ID::SPACE);
	// 衝突する属性を設定(縦)。
	_MyComponent.CharacterController->AttributeY_AllOn();
	_MyComponent.CharacterController->SubAttributeY(Collision_ID::ATTACK);
	_MyComponent.CharacterController->SubAttributeY(Collision_ID::ENEMY);
	_MyComponent.CharacterController->SubAttributeY(Collision_ID::PLAYER);
	_MyComponent.CharacterController->SubAttributeY(Collision_ID::SPACE);
}

void EnemyDrarian::_BuildAnimation() {
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
		//// 待機状態。
		//_ConfigAnimationType(EnemyCharacter::AnimationType::Idle, *Datas[static_cast<int>(AnimationBossDrarian::Wait)].get());
		//// 歩行状態。
		//_ConfigAnimationType(EnemyCharacter::AnimationType::Walk, *Datas[static_cast<int>(AnimationBossDrarian::Walk)].get());
		//// 走行状態。
		//_ConfigAnimationType(EnemyCharacter::AnimationType::Dash, *Datas[static_cast<int>(AnimationBossDrarian::Dash)].get());
		//// 吠える。
		//_ConfigAnimationType(EnemyCharacter::AnimationType::Threat, *Datas[static_cast<int>(AnimationBossDrarian::Barking)].get());
		//// 攻撃状態(噛みつき)。
		//_ConfigAnimationType(EnemyCharacter::AnimationType::Attack1, *Datas[static_cast<int>(AnimationBossDrarian::Attack)].get());
		//// 攻撃状態(しっぽ)。
		//_ConfigAnimationType(EnemyCharacter::AnimationType::Attack2, *Datas[static_cast<int>(AnimationBossDrarian::TailAttackRight)].get());
		//// 攻撃状態(ブレス)。
		//_ConfigAnimationType(EnemyCharacter::AnimationType::Attack3, *Datas[static_cast<int>(AnimationBossDrarian::Breath)].get());
		//// ダメージ反応。
		//Datas[static_cast<int>(AnimationBossDrarian::Damage)]->Time = 8.0f / 30.0f;
		//_ConfigAnimationType(EnemyCharacter::AnimationType::Damage, *Datas[static_cast<int>(AnimationBossDrarian::Damage)].get());
		////// 落下状態。
		////// ※このオブジェクトには落下のアニメーションがないので待機アニメーションで代用。
		////_ConfigAnimationType(EnemyCharacter::AnimationType::Fall, *Datas[static_cast<int>(AnimationProt::Stand)].get());
		//// 死亡状態。
		//_ConfigAnimationType(EnemyCharacter::AnimationType::Death, *Datas[static_cast<int>(AnimationBossDrarian::Death)].get());
	}
}

void EnemyDrarian::_ConfigAnimationEvent() {
	// かみつき攻撃。
	{
		float eventFrame = 15;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(EnemyCharacter::AnimationType::Attack1), eventFrame, static_cast<AnimationEvent>(&EnemyDrarian::AnimationEvent_Kamituki));
	}
}

void EnemyDrarian::_BuildSoundTable() {
	// 攻撃音登録。
	_ConfigSoundData(EnemyCharacter::SoundIndex::Attack1, "Damage_01.wav", false, false);
	_ConfigSoundData(EnemyCharacter::SoundIndex::Attack2, "Buoonn.wav", false, false);
	_ConfigSoundData(EnemyCharacter::SoundIndex::Attack3, "Buoonn.wav", false, false);
}
