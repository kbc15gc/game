#pragma once
#include "fbEngine\_Object\_Component\_Physics\GostCollision.h"
class AttackCollision : public GameObject
{
public:
	enum class CollisionMaster{Player = 0,Enemy, Other};	// 誰がコリジョンを発生させたか。
public:
	AttackCollision(const char * name) :GameObject(name)
	{

	};

	void Awake()override;
	void Update()override;

	// 攻撃判定用のコリジョン生成。
	// 引数：	位置(ローカル座標)。
	//			回転(ローカル座標、クォータニオン)。
	//			サイズ。
	//			誰がコリジョンを生成したか。
	//			コリジョン寿命(0.0fより小さい値で無限)。
	//			親にしたいTransform情報(動く床などの上でコリジョンが発生した場合に使用)。
	// 戻り値：	生成したコリジョン。
	GostCollision* Create(const Vector3& pos, const Quaternion& rotation, const Vector3& size, CollisionMaster master = CollisionMaster::Other, float lifeTime = -1.0f, Transform* Parent = nullptr);

	inline void SetParent(Transform* Parent) {
		transform->SetParent(Parent);
	}
	inline void RemoveParent() {
		transform->SetParent(nullptr);
	}
private:	
	// 衝突検出。
	void DetectionCollision();

private:
	Collider* _Colider = nullptr;	// コリジョン形状。
	GostCollision* _Gost = nullptr;	// ゴースト。
	float time;				//コリジョン削除カウンター。
	float _lifeTime = -1.0f;		// コリジョン寿命(0.0fより小さい値で無限)。
	CollisionMaster _master;	// 誰が発生させたコリジョンか。
};