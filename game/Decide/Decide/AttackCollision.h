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
	// 引数：	サイズ。
	inline void Create(const Vector3& size) {
		static_cast<BoxCollider*>(_Colider)->Create(size);		// コライダー生成。
		_Gost->Create(_Colider,Collision_ID::ATTACK);	// ゴーストコリジョン生成。
	}


	// 攻撃判定用のコリジョン生成。
	// 引数：	サイズ。
	//			誰がコリジョンを生成したか。
	inline void Create(const Vector3& size, CollisionMaster master) {
		_master = master;
		Create(size);
	}

	// 攻撃判定用のコリジョン生成。
	// 引数：	サイズ。
	//			誰がコリジョンを生成したか。
	//			コリジョン寿命(0.0fより小さい値で無限)。
	inline void Create(const Vector3& size, CollisionMaster master, float lifeTime) {
		_lifeTime = lifeTime;
		Create(size, master);
	}

private:
	Collider* _Colider = nullptr;	// コリジョン形状。
	GostCollision* _Gost = nullptr;	// ゴースト。
	float time;				//コリジョン削除カウンター。
	float _lifeTime = -1.0f;		// コリジョン寿命(0.0fより小さい値で無限)。
	CollisionMaster _master;	// 誰が発生させたコリジョンか。
};