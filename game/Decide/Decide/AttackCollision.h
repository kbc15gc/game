#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"

class AttackCollision : public GameObject
{
public:
	enum class CollisionMaster{Player = 0,Enemy, Other};	// 誰がコリジョンを発生させたか。
public:
	AttackCollision(const char* name) :GameObject(name) {}
	void Awake()override;
	void Update()override;
	void Create(const Vector3& size);

	// 攻撃判定用のコリジョン生成。
	// 引数：	サイズ。
	//			誰がコリジョンを生成したか。
	void Create(const Vector3& size,CollisionMaster master);

	// 攻撃判定用のコリジョン生成。
	// 引数：	サイズ。
	//			誰がコリジョンを生成したか。
	//			コリジョン寿命。
	void Create(const Vector3& size, CollisionMaster master, float lifeTime);

private:
	BoxCollider* _BoxColl;
	float time;				//コリジョン削除カウンター。
	float _lifeTime = 3.0f;		// コリジョン寿命。
	CollisionMaster _master;	// 誰が発生させたコリジョンか。
};