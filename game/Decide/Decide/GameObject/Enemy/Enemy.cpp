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
