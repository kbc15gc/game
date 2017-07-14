#include"stdafx.h"
#include "BossDrarian.h"
#include "fbEngine\_Object\_Component\_3D\SkinModel.h"
#include "HFSM\EnemyTranslationState.h"
#include "HFSM\EnemyWaitState.h"
#include "fbEngine\CharacterController.h"



BossDrarian::BossDrarian(const char* name) : EnemyCharacter(name)
{

}


BossDrarian::~BossDrarian()
{
}

void BossDrarian::_AwakeSubClass() {
	// 使用するモデルファイルのパスを設定。
	SetFileName("DRARIAN.X");

	//パラメーター初期化。
	_MyComponent.Parameter->ParamInit(0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0);
}

void BossDrarian::_StartSubClass() {

	transform->SetPosition(INSTANCE(GameObjectManager)->FindObject("Player")->transform->GetPosition());
	//パラメーター初期化。
	_MyComponent.Parameter->ParamInit(500, 500, 500, 500, 75, 50, 100, 20,1,0,500);

	// パラメーター設定。
	vector<BarColor> Color;
	Color.push_back(BarColor::Blue);
	Color.push_back(BarColor::Green);
	Color.push_back(BarColor::Yellow);
	Color.push_back(BarColor::Red);
	SetParamAll(Color, _MyComponent.Parameter->GetParams());

	// 視野角生成。
	_ViewAngle = 140.0f;
	_ViewRange = 20.0f;

	// 攻撃可能範囲設定。
	_AttackRange = 1.3f;

	// 徘徊範囲設定。
	// ※暫定処理。
	_WanderingRange = 130.0f;

	//モデルにライト設定。
	_MyComponent.Model->SetLight(INSTANCE(GameObjectManager)->mainLight);


	// 初期ステートに移行。
	// ※暫定処理。
	_ChangeState(State::Wait);

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


EnemyCharacter::State BossDrarian::AttackSelect() {
	// ※プレイヤーとエネミーの位置関係とかで遷移先決定？。

	// ※とりあえず暫定処理。
	return State::Attack;
}

void BossDrarian::_EndNowStateCallback(State EndStateType) {
	if (EndStateType == State::Wait) {
		// 待ちの挙動いったん終了。

		// 再度待ち。
		_ChangeState(State::Wait);
		static_cast<EnemyWaitState*>(_NowState)->SetInterval(5.0f);
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
}

void BossDrarian::_ConfigCollision() {

	// コリジョンのサイズを決定。
	// ※キャラクターコントローラーで使用するためのもの。
	_Radius = 0.5f;
	_Height = 7.5f;

	// コンポーネントにカプセルコライダーを追加。
	_MyComponent.Collider = AddComponent<CCapsuleCollider>();
	// カプセルコライダーを作成。
	static_cast<CCapsuleCollider*>(_MyComponent.Collider)->Create(_Radius, _Height);
}

void BossDrarian::_ConfigCharacterController() {
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
		_ConfigAnimationType(EnemyCharacter::AnimationType::Idle, *Datas[static_cast<int>(AnimationDrarian::Wait)].get());
	//	// 歩行状態。
	//	_ConfigAnimationType(EnemyCharacter::AnimationType::Walk, *Datas[static_cast<int>(AnimationProt::Walk)].get());
	//	// 走行状態。
	//	// ※このオブジェクトにはダッシュのアニメーションがないので歩くアニメーションで代用。
	//	_ConfigAnimationType(EnemyCharacter::AnimationType::Dash, *Datas[static_cast<int>(AnimationProt::Walk)].get());
	//	// 攻撃状態。
	//	_ConfigAnimationType(EnemyCharacter::AnimationType::Attack, *Datas[static_cast<int>(AnimationProt::Attack)].get());
	//	// 落下状態。
	//	// ※このオブジェクトには落下のアニメーションがないので待機アニメーションで代用。
	//	_ConfigAnimationType(EnemyCharacter::AnimationType::Fall, *Datas[static_cast<int>(AnimationProt::Stand)].get());
	//	// 死亡状態。
	//	_ConfigAnimationType(EnemyCharacter::AnimationType::Death, *Datas[static_cast<int>(AnimationProt::Death)].get());
	}
}

void BossDrarian::_ConfigAnimationEvent() {
	//int eventFrame = 30;

	//// 攻撃アニメーションにコリジョン生成イベント追加。
	//AnimationEvent::AttackEventInfo info(transform, true);
	//info.damage = _MyComponent.Parameter->GiveDamageMass();
	//info.master = AttackCollision::CollisionMaster::Enemy;
	//info.pos = Vector3(1.5f, 0.5f, 0.0f);
	//info.rot = Quaternion::Identity;
	//info.size = Vector3::one;
	//info.life = 0.25f;
	//_MyComponent.AnimationEvent->AddAnimationEvent(static_cast<int>(AnimationDrarian::Attack), eventFrame, info);
}
