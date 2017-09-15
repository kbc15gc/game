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
	// 引数：	コリジョン製作者によるダメージ量。
	//			魔法攻撃か。
	//			位置(ローカル座標)。
	//			回転(ローカル座標、クォータニオン)。
	//			サイズ。
	//			誰がコリジョンを生成したか。
	//			コリジョン寿命(0.0fより小さい値で無限)。
	//			コリジョン生成待ち時間(この関数が呼ばれてから何秒後にコリジョン生成するか)。
	//			親にしたいTransform情報(動く床などの上でコリジョンが発生した場合に使用)。
	void Create(int damage,bool isMagic,const Vector3& pos, const Quaternion& rotation, const Vector3& size, CollisionMaster master = CollisionMaster::Other, float lifeTime = -1.0f,float waitTime = 0.0f, Transform* Parent = nullptr);

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
	inline int GetDamage() const
	{
		return _Damage;
	}

	inline bool GetIsMagic()const {
		return _isMagic;
	}

	inline GostCollision* GetGostCollision()const {
		return _Gost;
	}
private:	
	// 攻撃コリジョン生成関数。
	void CreateCollision();
	// 衝突検出。
	void DetectionCollision();
	// 衝突した瞬間呼ぶコールバック処理。
	void _CallBackEnter(btCollisionObject* coll);
	// 衝突している間呼び続けるコールバック処理。
	void _CallBackStay(btCollisionObject* coll);
	// 衝突した瞬間呼ぶコールバック処理。
	void _CallBackExit(btCollisionObject* coll);
	// コリジョンオブジェクトからゲームオブジェクトを取得する。
	GameObject* _CollisionObjectToGameObject(btCollisionObject* coll) {
		if (!coll) {
			return nullptr;
		}
		Collision* Coll = static_cast<Collision*>(coll->getUserPointer());
		if (Coll) {
			return Coll->gameObject;
		}
		else {
			// コリジョンコンポーネントがない。
			return nullptr;
		}
	}
private:
	Collider* _Colider = nullptr;	// コリジョン形状。
	GostCollision* _Gost = nullptr;	// ゴースト。
	float _time;				//作業用カウンター。
	float _waitTime = 0.0f;		// コリジョン生成待ち時間。
	float _lifeTime = -1.0f;		// コリジョン寿命(0.0fより小さい値で無限)。
	CollisionMaster _master;	// 誰が発生させたコリジョンか。
	int _Damage = 0;
	bool _isMagic;	// 魔法か。
	vector<shared_ptr<btCollisionObject>> _HitCollisions;	// 当たっているコリジョン。
	bool _isCreateCollision = false;	// コリジョンを生成したか。
};