#include "Enemy.h"
#include "fbEngine\SkinModel.h"
#include "fbEngine\CapsuleCollider.h"
#include "HFSM\EnemyTranslationState.h"
#include "HFSM\EnemyWaitState.h"

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
	//モデルにライト設定
	_MyComponent.Model->SetLight(GameObjectManager::mainLight);
	// 位置情報設定。
	transform->SetLocalPosition(Vector3(0.0f, 10.0f, 0.0f));
	// 初期ステートに移行。
	// ※暫定処理。
	_ChangeState(State::Translation);
	// パラメータ設定。
	static_cast<EnemyTranslationState*>(_NowState)->SetDir(transform->GetForward());
	static_cast<EnemyTranslationState*>(_NowState)->SetLength(5.0f);
	static_cast<EnemyTranslationState*>(_NowState)->SetMoveSpeed(1.0f);
}

void Enemy::_UpdateSubClass() {
}

void Enemy::_EndNowStateCallback(State EndStateType) {
	if (EndStateType == State::Translation) {
		// 移動ステート終了。

		_ChangeState(State::Wait);		// 待機ステートに移行。
		// パラメータ設定。
		static_cast<EnemyWaitState*>(_NowState)->SetInterval(4.5f);
	}
	else if (EndStateType == State::Wait) {
		// 待機ステート終了。

		_ChangeState(State::Translation);	// 移動ステートに移行。
		// パラメータ設定。
		static_cast<EnemyTranslationState*>(_NowState)->SetDir(transform->GetForward());
		static_cast<EnemyTranslationState*>(_NowState)->SetLength(5.0f);
		static_cast<EnemyTranslationState*>(_NowState)->SetMoveSpeed(1.0f);
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
	}
}

