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
	SetFileName("doragon.X");
}
void BossD::_StartSubClass() {

	//ポジション
	_InitPos = Vector3(-202.0f, 60.0f, -156.0f);
	transform->SetPosition(_InitPos);

	// 視野角生成。
	_ViewAngle = 100.0f;
	_ViewRange = 30.0f;

	// 歩行速度設定。
	_walkSpeed = 2.5f;

	// 徘徊範囲設定。
	// ※暫定処理。
	_WanderingRange = 20.0f;

	// 追跡範囲設定。
	_discoveryRange = 30.0f;

	// 何回に一回くらい怯むか設定。
	_damageMotionRandNum = 30;

	//モデルにライト設定。
	_MyComponent.Model->SetLight(INSTANCE(GameObjectManager)->mainLight);
	//攻撃処理。
	
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

	// ※とりあえず暫定処理。
	int rnd = rand() % 2;
	if (rnd == 0) {
		return _singleAttack.get();
	}
	else {
		return _singleAttackSecondPattern.get();
	}

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
	// 攻撃判定用のコリジョン。
	{
		// 胴体。
		{
			RigidBody* coll = AddComponent<RigidBody>();	// キャラクターコントローラとは別に新しく作成(プレイヤーをキャラコンの形状で押し出したくないため)。

			RigidBodyInfo info;
			info.coll = AddComponent<CCapsuleColliderZ>();
			static_cast<CCapsuleColliderZ*>(info.coll)->Create(0.75f, 5.1f);
			info.id = Collision_ID::BOSS;
			info.mass = 0.0f;
			info.physicsType = Collision::PhysicsType::Kinematick;
			info.offset = Vector3(0.0f, 0.0f, 0.0f);
			info.rotation = Quaternion::Identity;
			coll->Create(info, true);

			_MyComponent.ExtrudeCollisions.push_back(coll);	// ついでに押し出しようコリジョンに追加しておく。
		}
	}

	// キャラクターコントローラ用。
	{
		// コリジョンのサイズを決定。
		// ※キャラクターコントローラーで使用するためのもの。
		_collisionInfo.radius = 1.8;
		_collisionInfo.height = 6.0f;
		_collisionInfo.offset = Vector3(0.0f, 1.0f, 0.0f);
		_collisionInfo.id = Collision_ID::CHARACTER_GHOST;

		// 重力設定。
		_Gravity = -9.8f;

		// コンポーネントにカプセルコライダーZを追加。
		_MyComponent.Collider = AddComponent<CCapsuleColliderZ>();
		// カプセルコライダーを作成。
		static_cast<CCapsuleColliderZ*>(_MyComponent.Collider)->Create(_collisionInfo.radius, _collisionInfo.height);
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
}

void BossD::_BuildAnimationSubClass(vector<double>& datas) {

	// アニメーションタイプにデータを関連づけ。
	// ※エネミーはすべて同じステートクラスを使用するため、ステートからアニメーションを再生できるよう
	//   EnemyCharacterクラスで定義されているすべてのエネミー共通の列挙子に関連付ける必要がある。
	
}

void BossD::_ConfigAnimationEvent() {
	
}

void BossD::_BuildSoundTable() {
	// 攻撃音登録。

}

