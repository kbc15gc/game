#include"stdafx.h"
#include "LastBoss.h"
#include "fbEngine\_Object\_Component\_3D\SkinModel.h"
#include "HFSM\EnemyTranslationState.h"
#include "HFSM\EnemyWaitState.h"
#include "fbEngine\CharacterController.h"
#include "HFSM\LastBossThroneState.h"
#include "HFSM\LastBossMagicianState.h"
#include "HFSM\LastBossHistoryTamperingState.h"
#include "HFSM\LastBossDownState.h"
#include "fbEngine\_Object\_GameObject\Particle.h"
#include "GameObject\Enemy\EnemyAttack.h"
#include "GameObject\Enemy\LastBossMagic.h"

//EnemySingleAttack Enemy::_singleAttack = EnemySingleAttack(_AnimationData[EnemyCharacter::AnimationType::Attack],);

LastBoss::LastBoss(const char* name) : EnemyCharacter(name)
{

}


LastBoss::~LastBoss()
{
}

void LastBoss::SordAttackEvent() {
	//攻撃コリジョン作成。
	unsigned int priorty = 1;
	AttackCollision* attack = INSTANCE(GameObjectManager)->AddNew<AttackCollision>("attackCollision", priorty);
	attack->Create(_MyComponent.Parameter->GiveDamageMass(false, false), Vector3(0.0f, 0.0f, 2.0f), Quaternion::Identity, Vector3(1.0f,3.0f,2.0f), AttackCollision::CollisionMaster::Enemy, 0.25f, 0.0f, transform);
	attack->RemoveParent();

	// 攻撃音再生。
	EnemyPlaySound(EnemyCharacter::SoundIndex::Attack1);
}

void LastBoss::MagicAttackStart() {
	_magicAttack->BreathStart<LastBossMagic>(this,Vector3::zero);
}

void LastBoss::MagicAttackEnd() {
	_magicAttack->BreathEnd();
}

void LastBoss::_AwakeSubClass() {
	// 使用するモデルファイルのパスを設定。
	SetFileName("LastBoss.X");
}

void LastBoss::_StartSubClass() {

	//ポジション
	_InitPos = Vector3(-202.0f, 60.0f, -156.0f);
	transform->SetPosition(_InitPos);

	// 視野角生成。
	_ViewAngle = 360.0f;
	_ViewRange = 50.0f;

	// 徘徊範囲設定。
	// ※暫定処理。
	_WanderingRange = 0.0f;

	// 追跡範囲設定。
	_discoveryRange = 100.0f;

	// 歩行速度設定。
	_walkSpeed = 5.0f;

	//モデルにライト設定。
	_MyComponent.Model->SetLight(INSTANCE(GameObjectManager)->mainLight);

	// 攻撃処理を定義。
	_sordAttack.reset(new EnemySingleAttack(this));
	_sordAttack->Init(3.0f,static_cast<int>(AnimationLastBoss::SordAttack), 0.2f);
	_magicAttack.reset(new EnemyBreathAttack(this));
	_magicAttack->Init(7.0f, static_cast<int>(AnimationLastBoss::Magic), 0.2f);

	// 初期ステートに移行。
	// ※暫定処理。
	//_initState = static_cast<State>(LastBossState::LastBossMagician);
	_initState = State::Speak;
	_ChangeState(_initState);
}

void LastBoss::_UpdateSubClass() {
	//if (!(_MyComponent.CharacterController->IsOnGround())) {
	//	// エネミーが地面から離れている。
	//	if (_NowStateIdx != State::Fall) {
	//		// 現在のステートタイプを保存。
	//		_saveState = _NowStateIdx;
	//		// 落下ステートに切り替え。
	//		_ChangeState(State::Fall);
	//	}
	//}
#ifdef _DEBUG
	Debug();
#endif // _DEBUG

}

void LastBoss::_LateUpdateSubClass()
{

}


EnemyAttack* LastBoss::_AttackSelectSubClass() {
	// ※プレイヤーとエネミーの位置関係とかで遷移先決定？。
	EnemyAttack* attack = nullptr;

	int rnd;

	if (_NowStateIdx == static_cast<State>(LastBossState::LastBossMagician)) {
		// 魔術師ステート。
		// 確率で攻撃と魔王へのバフを行う。

		rnd = rand() % 2;
		//rnd = rand() % 4;

		if (rnd == 0) {
			// 剣攻撃。

			attack = _sordAttack.get();
		}
		else if (rnd == 1) {
			// 魔法攻撃。

			attack = _magicAttack.get();
		}
		//else if(rnd == 2){
		//	// バフ。

		//	attack = _tailAttack.get();
		//}
		//else {
		//	// 鎌攻撃。

		//	attack = 
		//}
	}

	return attack;
}

void LastBoss::_EndNowStateCallback(State EndStateType) {
	if (EndStateType == State::Speak) {
		// 会話終了。
		
		// 玉座ステートに移行。
		_ChangeState(static_cast<State>(LastBossState::LastBossThrone));
	}
	else if (static_cast<LastBossState>(EndStateType) == LastBossState::LastBossThrone) {
		// 玉座ステート終了。

		// 魔術師ステートに移行。
		_ChangeState(static_cast<State>(LastBossState::LastBossMagician));
	}
	else if (static_cast<LastBossState>(EndStateType) == LastBossState::LastBossMagician) {
		// 魔術師ステート終了。

		// 歴史改竄ステートに移行。
		_ChangeState(static_cast<State>(LastBossState::LastBossHistory));
	}
	else if (static_cast<LastBossState>(EndStateType) == LastBossState::LastBossHistory) {
		// 歴史改竄ステート終了。

		// 魔術師ステートに移行。
		_ChangeState(static_cast<State>(LastBossState::LastBossMagician));
	}
}

void LastBoss::_ConfigCollision() {

	// コリジョンのサイズを決定。
	// ※キャラクターコントローラーで使用するためのもの。
	_collisionInfo.radius = 0.5f;
	_collisionInfo.height = 3.6f;
	_collisionInfo.offset = Vector3(0.0f, 0.46f, 0.0f);
	_collisionInfo.id = Collision_ID::BOSS;

	// コンポーネントにカプセルコライダーを追加。
	_MyComponent.Collider = AddComponent<CCapsuleCollider>();
	// カプセルコライダーを作成。
	static_cast<CCapsuleCollider*>(_MyComponent.Collider)->Create(_collisionInfo.radius, _collisionInfo.height);

	_Gravity = 0.0f;
}

void LastBoss::_ConfigCharacterController() {
	// 衝突する属性を設定(横)。
	_MyComponent.CharacterController->AttributeXZ_AllOn();
	_MyComponent.CharacterController->SubAttributeXZ(Collision_ID::ATTACK);
	_MyComponent.CharacterController->SubAttributeXZ(Collision_ID::SPACE);
	_MyComponent.CharacterController->SubAttributeXZ(Collision_ID::ENEMY);
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

void LastBoss::_CreateExtrudeCollision() {
	_MyComponent.ExtrudeCollisions.push_back(_MyComponent.CharacterController->GetRigidBody());	// キャラクターコントローラの剛体をそのまま使用。
}

void LastBoss::_BuildStateSubClass() {
	// ラスボス玉座ステート追加。
	_MyState.push_back(unique_ptr<LastBossThroneState>(new LastBossThroneState(this)));
	// ラスボス魔術師ステート追加。
	_MyState.push_back(unique_ptr<LastBossMagicianState>(new LastBossMagicianState(this)));
	// ラスボス歴史改竄ステート追加。
	_MyState.push_back(unique_ptr<LastBossHistoryTamperingState>(new LastBossHistoryTamperingState(this)));
	// ラスボスダウンステート追加。
	_MyState.push_back(unique_ptr<LastBossDownState>(new LastBossDownState(this)));
}

void LastBoss::_BuildAnimation() {
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
		_ConfigAnimationType(EnemyCharacter::AnimationType::Idle, *Datas[static_cast<int>(AnimationLastBoss::Move)].get());
		// 歩行状態。
		_ConfigAnimationType(EnemyCharacter::AnimationType::Walk, *Datas[static_cast<int>(AnimationLastBoss::Move)].get());
		 //走行状態。
		 //※このオブジェクトにはダッシュのアニメーションがないので歩くアニメーションで代用。
		_ConfigAnimationType(EnemyCharacter::AnimationType::Dash, *Datas[static_cast<int>(AnimationLastBoss::Move)].get());
		//// 攻撃状態。
		//_ConfigAnimationType(EnemyCharacter::AnimationType::Attack1, *Datas[static_cast<int>(AnimationProt::Attack)].get());
	//	// 落下状態。
	//	// ※このオブジェクトには落下のアニメーションがないので待機アニメーションで代用。
	//	_ConfigAnimationType(EnemyCharacter::AnimationType::Fall, *Datas[static_cast<int>(AnimationProt::Stand)].get());
	//	// ダメージ状態。
	//	_ConfigAnimationType(EnemyCharacter::AnimationType::Damage, *Datas[static_cast<int>(AnimationProt::Damage)].get());
	//	// 死亡状態。
	//	_ConfigAnimationType(EnemyCharacter::AnimationType::Death, *Datas[static_cast<int>(AnimationProt::Death)].get());
	}
}

void LastBoss::_ConfigAnimationEvent() {

	float eventFrame;

	// 剣攻撃。
	{
		eventFrame = 1.6f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationLastBoss::SordAttack), eventFrame, static_cast<AnimationEvent>(&LastBoss::SordAttackEvent));
	}

	// 魔法攻撃。
	{
		eventFrame = 2.3f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationLastBoss::Magic), eventFrame, static_cast<AnimationEvent>(&LastBoss::MagicAttackStart));
		eventFrame = 2.4f;
		_MyComponent.AnimationEventPlayer->AddAnimationEvent(static_cast<int>(AnimationLastBoss::Magic), eventFrame, static_cast<AnimationEvent>(&LastBoss::MagicAttackEnd));
	}
}

void LastBoss::_BuildSoundTable() {
	// 攻撃音登録。
	_ConfigSoundData(EnemyCharacter::SoundIndex::Attack1, "Damage_01.wav", false, false);
}

#ifdef _DEBUG
void LastBoss::Debug() {
	if(KeyBoardInput->isPressed(DIK_X)&&KeyBoardInput->isPush(DIK_B))
	{
		this->SetActive(false, true);
	}

	if (KeyBoardInput->isPressed(DIK_X) && KeyBoardInput->isPush(DIK_V))
	{
		this->SetActive(true, true);
	}
}
#endif // _DEBUG

