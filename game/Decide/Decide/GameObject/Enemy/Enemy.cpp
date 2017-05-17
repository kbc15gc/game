#include"stdafx.h"
#include "Enemy.h"
#include "fbEngine\_Object\_Component\_3D\SkinModel.h"
#include "HFSM\EnemyTranslationState.h"
#include "HFSM\EnemyWaitState.h"
#include "fbEngine\CharacterController.h"



Enemy::Enemy(const char* name) : EnemyCharacter(name)
{

}


Enemy::~Enemy()
{
}

void Enemy::_AwakeSubClass() {
	// 使用するモデルファイルのパスを設定。
	SetFileName("enemy_00.X");
}

void Enemy::_StartSubClass(){
	// 視野角生成。
	_ViewAngle = 90.0f;
	_ViewRange = 5.0f;

	// 攻撃可能範囲設定。
	_AttackRange = 1.3f;

	//モデルにライト設定
	_MyComponent.Model->SetLight(INSTANCE(GameObjectManager)->mainLight);

	// 位置情報設定。
	_InitPos = Vector3(0.0f, 10.0f, 0.0f);
	transform->SetLocalPosition(_InitPos);

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


EnemyCharacter::State Enemy::AttackSelect() {
	// ※プレイヤーとエネミーの位置関係とかで遷移先決定？。

	// ※とりあえず暫定処理。
	return State::Attack;
}

void Enemy::_EndNowStateCallback(State EndStateType) {
	if (EndStateType == State::Discovery) {
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

void Enemy::_ConfigCollision() {

	// コリジョンのサイズを決定。
	// ※キャラクターコントローラーで使用するためのもの。
	_Radius = 0.5f;
	_Height = 1.5f;

	// コンポーネントにカプセルコライダーを追加。
	_MyComponent.Collider = AddComponent<CCapsuleCollider>();
	// カプセルコライダーを作成。
	static_cast<CCapsuleCollider*>(_MyComponent.Collider)->Create(_Radius,_Height);
}

void Enemy::_BuildAnimation() {
	vector<unique_ptr<AnimationData>> Datas;
	for (int idx = 0; idx < _MyComponent.Animation->GetNumAnimationSet(); idx++) {
		// アニメーションセットの番号と再生時間をセットにしたデータを作成。
		unique_ptr<AnimationData> data(new AnimationData);
		data->No = idx;
		data->Time = -1.0f;	// すべて1秒以上のアニメーションなので、時間は設定しない。
		// 各アニメーションの終了時間を設定していく。
		//_MyComponent.Animation->SetAnimationEndTime(data->No, data->Time);

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
		_ConfigAnimationType(EnemyCharacter::AnimationType::Attack, *Datas[static_cast<int>(AnimationProt::Attack)].get());
		// 落下状態。
		// ※このオブジェクトには落下のアニメーションがないので待機アニメーションで代用。
		_ConfigAnimationType(EnemyCharacter::AnimationType::Fall, *Datas[static_cast<int>(AnimationProt::Stand)].get());
	}
}

