#include"stdafx.h"
#include "Enemy.h"
#include "fbEngine\_Object\_Component\_3D\SkinModel.h"
#include "HFSM\EnemyTranslationState.h"
#include "HFSM\EnemyWaitState.h"
#include "fbEngine\CharacterController.h"


//EnemySingleAttack Enemy::_singleAttack = EnemySingleAttack(_AnimationData[EnemyCharacter::AnimationType::Attack],);

Enemy::Enemy(const char* name) : EnemyCharacter(name)
{

}


Enemy::~Enemy()
{
}

void Enemy::CreateAttackCollision() {
	//攻撃コリジョン作成。
	unsigned int priorty = 1;
	AttackCollision* attack = INSTANCE(GameObjectManager)->AddNew<AttackCollision>("attackCollision", priorty);
	attack->Create(_MyComponent.Parameter->GiveDamageMass(false, false), Vector3(0.0f, 0.5f, 1.5f), Quaternion::Identity, Vector3::one, AttackCollision::CollisionMaster::Enemy, 0.15f, 0.0f, transform);
	attack->RemoveParent();

	// 攻撃音再生。
	EnemyPlaySound(EnemyCharacter::SoundIndex::Attack1);
}

void Enemy::_AwakeSubClass() {
	// 使用するモデルファイルのパスを設定。
	SetFileName("enemy_00.X");
}

void Enemy::_StartSubClass(){
	// 視野角生成。
	_ViewAngle = 90.0f;
	_ViewRange = 10.0f;

	// 攻撃可能範囲設定。
	_AttackRange = 1.3f;

	// 徘徊範囲設定。
	// ※暫定処理。
	_WanderingRange = 10.0f;

	// 追跡範囲設定。
	_discoveryRange = 10.0f;

	// 歩行速度設定。
	_walkSpeed = 1.0f;

	//モデルにライト設定。
	_MyComponent.Model->SetLight(INSTANCE(GameObjectManager)->mainLight);

	// 攻撃処理を定義。
	_singleAttack.reset(new EnemySingleAttack(this));
	_singleAttack->Init(_AnimationData[static_cast<int>(EnemyCharacter::AnimationType::Attack1)].No,0.2f);

	// 初期ステートに移行。
	// ※暫定処理。
	_ChangeState(State::Wandering);

}

void Enemy::_UpdateSubClass() {
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

void Enemy::_LateUpdateSubClass()
{
	
}


EnemyAttack* Enemy::AttackSelect() {
	// ※プレイヤーとエネミーの位置関係とかで遷移先決定？。

	// ※とりあえず暫定処理。
	return _singleAttack.get();
}

void Enemy::_EndNowStateCallback(State EndStateType) {
	if (EndStateType == State::Wandering) {
		// 徘徊の挙動いったん終了。

		// 再度徘徊。
		_ChangeState(State::Wandering);
	}
	else if (EndStateType == State::Discovery) {
		// 発見ステートの処理完了。

		_ChangeState(State::StartAttack);
	}
	else if (EndStateType == State::StartAttack) {
		// 一度攻撃が終了した。

		// プレイヤーとの位置関係再調整。
		_ChangeState(State::Discovery);
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

void Enemy::_ConfigCollision() {

	// コリジョンのサイズを決定。
	// ※キャラクターコントローラーで使用するためのもの。
	_collisionInfo.radius = 0.325f;
	_collisionInfo.height = 0.3f;
	_collisionInfo.offset = Vector3(0.0f, 0.46f, 0.0f);

	// コンポーネントにカプセルコライダーを追加。
	_MyComponent.Collider = AddComponent<CCapsuleCollider>();
	// カプセルコライダーを作成。
	static_cast<CCapsuleCollider*>(_MyComponent.Collider)->Create(_collisionInfo.radius, _collisionInfo.height);
}

void Enemy::_ConfigCharacterController() {
	// 衝突する属性を設定(横)。
	_MyComponent.CharacterController->AttributeXZ_AllOn();
	_MyComponent.CharacterController->SubAttributeXZ(Collision_ID::ATTACK);
	_MyComponent.CharacterController->SubAttributeXZ(Collision_ID::SPACE);
	_MyComponent.CharacterController->SubAttributeXZ(Collision_ID::PLAYER);
	//_MyComponent.CharacterController->SubAttributeXZ(Collision_ID::BUILDING);
	// 衝突する属性を設定(縦)。
	_MyComponent.CharacterController->AttributeY_AllOn();
	_MyComponent.CharacterController->SubAttributeY(Collision_ID::ATTACK);
	_MyComponent.CharacterController->SubAttributeY(Collision_ID::ENEMY);
	_MyComponent.CharacterController->SubAttributeY(Collision_ID::PLAYER);
	_MyComponent.CharacterController->SubAttributeY(Collision_ID::SPACE);
}

void Enemy::_BuildAnimation() {
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
		_ConfigAnimationType(EnemyCharacter::AnimationType::Idle, *Datas[static_cast<int>(AnimationProt::Stand)].get());
		// 歩行状態。
		_ConfigAnimationType(EnemyCharacter::AnimationType::Walk, *Datas[static_cast<int>(AnimationProt::Walk)].get());
		// 走行状態。
		// ※このオブジェクトにはダッシュのアニメーションがないので歩くアニメーションで代用。
		_ConfigAnimationType(EnemyCharacter::AnimationType::Dash, *Datas[static_cast<int>(AnimationProt::Walk)].get());
		// 攻撃状態。
		_ConfigAnimationType(EnemyCharacter::AnimationType::Attack1, *Datas[static_cast<int>(AnimationProt::Attack)].get());
		// 落下状態。
		// ※このオブジェクトには落下のアニメーションがないので待機アニメーションで代用。
		_ConfigAnimationType(EnemyCharacter::AnimationType::Fall, *Datas[static_cast<int>(AnimationProt::Stand)].get());
		// ダメージ状態。
		_ConfigAnimationType(EnemyCharacter::AnimationType::Damage, *Datas[static_cast<int>(AnimationProt::Damage)].get());
		// 死亡状態。
		_ConfigAnimationType(EnemyCharacter::AnimationType::Death, *Datas[static_cast<int>(AnimationProt::Death)].get());
	}
}

void Enemy::_ConfigAnimationEvent() {
	int eventFrame = 30;
	
	_MyComponent.AnimationEventPlayer->AddAnimationEvent(_AnimationData[static_cast<int>(EnemyCharacter::AnimationType::Attack1)].No, eventFrame, static_cast<AnimationEvent>(&Enemy::CreateAttackCollision));
}

void Enemy::_BuildSoundTable() {
	// 攻撃音登録。
	_ConfigSoundData(EnemyCharacter::SoundIndex::Attack1,"Damage_01.wav",false,false);
}
