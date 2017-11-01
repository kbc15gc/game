#pragma once
#include "fbEngine\_Object\_Component\_Physics\GostCollision.h"
#include "GameObject\Component\CharacterParameter.h"

class AttackCollision : public GameObject
{
public:
	enum class CollisionMaster{Player = 0,Enemy, Other};	// 誰がコリジョンを発生させたか。

	enum class ReactionType{ NotAction = 0,Leans, Blown};	// この攻撃で怯まないか怯むか吹っ飛ぶか。

	struct HitObjectInfo {
		GameObject* object = nullptr;		// 衝突したオブジェクト。
		shared_ptr<btCollisionObject> coll;
		bool isCallStay = false;
	};
public:
	AttackCollision(const char * name) :GameObject(name)
	{

	};
	~AttackCollision() {
		_hitInfos.clear();
	}

	void Awake()override;
	void Update()override;
	void LateUpdate()override;

	// 攻撃判定用のコリジョン生成。
	// 引数：	コリジョン製作者によるダメージ情報。
	//			位置(ローカル座標)。
	//			回転(ローカル座標、クォータニオン)。
	//			サイズ。
	//			誰がコリジョンを生成したか。
	//			コリジョン寿命(0.0fより小さい値で無限)。
	//			命中時のキャラの反応の種類。
	//			親にしたいTransform情報(動く床などの上でコリジョンが発生した場合に使用)。
	//			寿命が過ぎたら内部で自動的に削除するか(falseにすると非アクティブになるだけで削除されない)。
	void Create(unique_ptr<CharacterParameter::DamageInfo> info, const Vector3& pos, const Quaternion& rotation, const Vector3& size, CollisionMaster master, float lifeTime, ReactionType reactionType, Transform* Parent = nullptr,bool isLifeOverDelete = true);

	void ReSize(const Vector3 size) {
		_Colider->Resize(size);
	}

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

	CharacterParameter::DamageInfo* GetDamageInfo()
	{
		return _DamageInfo.get();
	}

	inline GostCollision* GetGostCollision()const {
		return _Gost;
	}

	inline void SetIsHit(bool flg) {
		_isHit = flg;
	}
	inline bool GetIsHit()const {
		return _isHit;
	}

	// 生成されているかつ攻撃コリジョンの寿命が尽きている。
	inline bool IsDeath()const {
		return !_isAlive;
	}

	// 攻撃命中時のリアクションの種類を取得。
	inline ReactionType GetReactionType()const {
		return _reactionType;
	}
private:	
	// 攻撃コリジョン生成関数。
	void CreateCollision();
	// 衝突検出。
	void DetectionCollision();
	// 衝突した瞬間呼ぶコールバック処理。
	void _CallBackEnter(btCollisionObject* coll);
	// 衝突している間呼び続けるコールバック処理。
	void _CallBackStay(GameObject* coll);
	// 衝突した瞬間呼ぶコールバック処理。
	void _CallBackExit(GameObject* coll);
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
	BoxCollider* _Colider = nullptr;	// コリジョン形状。
	GostCollision* _Gost = nullptr;	// ゴースト。
	float _time;				//作業用カウンター。
	float _lifeTime = -1.0f;		// コリジョン寿命(0.0fより小さい値で無限)。
	CollisionMaster _master;	// 誰が発生させたコリジョンか。
	vector<unique_ptr<HitObjectInfo>> _hitInfos;	// 当たっているオブジェクト情報。
	unique_ptr<CharacterParameter::DamageInfo> _DamageInfo;

	bool _isHit = false;	// 衝突する設定にしている何らかのオブジェクトに衝突した。
	bool _isLifeOverDelete = true;	// 寿命を過ぎたら削除される。
	bool _isAlive = false;	// 寿命を過ぎていないか。
	ReactionType _reactionType;	// 攻撃命中時のキャラのリアクションの種類。
};