#include "Enemy.h"
#include "fbEngine\SkinModel.h"
#include "fbEngine\CapsuleCollider.h"

Enemy::Enemy(const char* name) : EnemyCharacter(name)
{

}


Enemy::~Enemy()
{
}

void Enemy::Awake() {
	// 使用するモデルファイルのパスを設定。
	SetFileName("enemy_00.X");
	EnemyCharacter::Awake();
}

void Enemy::Start(){
	//モデルにライト設定
	_MyComponent.Model->SetLight(GameObjectManager::mainLight);
	// 位置情報設定。
	transform->SetLocalPosition(Vector3(0.0f, 10.0f, 0.0f));
	// 初期ステートに移行。
	// ※暫定処理。
	_ChangeState(State::Translation);
	EnemyCharacter::Start();
}

void Enemy::Update() {
	EnemyCharacter::Update();
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
		Datas.push_back(move(data));
		// ここにアニメーション終了時間設定関数を記述。

	}

	// アニメーションタイプにデータを関連づけ。
	{
		// 待機状態。
		_ConfigAnimationType(EnemyCharacter::AnimationType::Idle, *Datas[static_cast<int>(AnimationProt::Stand)].get());
		// 歩行状態。
		_ConfigAnimationType(EnemyCharacter::AnimationType::Walk, *Datas[static_cast<int>(AnimationProt::Walk)].get());
	}
}