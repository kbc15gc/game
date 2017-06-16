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
	~AttackCollision() {
		_HitCollisions.clear();
	}

	void Awake()override;
	void Update()override;
	void LateUpdate()override;

	// 攻撃判定用のコリジョン生成。
	// 引数：	コリジョン製作者の攻撃力
	//			位置(ローカル座標)。
	//			回転(ローカル座標、クォータニオン)。
	//			サイズ。
	//			誰がコリジョンを生成したか。
	//			コリジョン寿命(0.0fより小さい値で無限)。
	//			親にしたいTransform情報(動く床などの上でコリジョンが発生した場合に使用)。
	// 戻り値：	生成したコリジョン。
	GostCollision* Create(int damage,const Vector3& pos, const Quaternion& rotation, const Vector3& size, CollisionMaster master = CollisionMaster::Other, float lifeTime = -1.0f, Transform* Parent = nullptr);

	inline void SetParent(Transform* Parent) {
		transform->SetParent(Parent);
	}
	inline void RemoveParent() {
		transform->SetParent(nullptr);
	}
	//誰が発生させたコリジョンかをゲット。
	inline CollisionMaster GetMaster()  const
	{
		return _master;
	}

	//設定されたダメージをゲット。
	inline int GetDamage()
	{
		return _Damage;
	}
private:	
	// 衝突検出。
	void DetectionCollision();
	// 衝突した瞬間呼ぶコールバック処理。
	void _CallBackEnter(Collision* coll);
	// 衝突している間呼び続けるコールバック処理。
	void _CallBackStay(Collision* coll);
	// 衝突した瞬間呼ぶコールバック処理。
	void _CallBackExit(Collision* coll);
private:
	Collider* _Colider = nullptr;	// コリジョン形状。
	GostCollision* _Gost = nullptr;	// ゴースト。
	float time;				//コリジョン削除カウンター。
	float _lifeTime = -1.0f;		// コリジョン寿命(0.0fより小さい値で無限)。
	CollisionMaster _master;	// 誰が発生させたコリジョンか。
	int _Damage = 0;
	vector<Collision*> _HitCollisions;	// 当たっているコリジョン。
};