#pragma once
#include "_Object\_Component\Component.h"
#include "_Include\CollisionEnum.h"

class ModelObject;
//あたり判定の基底クラス
class Collision:public Component
{
public:
	enum class CollisionObjectType{Rigid = 0,Ghost};
	enum class PhysicsType{Dynamic = 0,Kinematick,Static};
#ifdef _DEBUG
	static const wchar_t* TypeName[];
#endif
public:
	Collision(GameObject* g, Transform* t) :
		Component(g, t, typeid(this).name())
	{
#ifdef _DEBUG
		mbstowcs_s(nullptr, name, typeid(*this).name(), strlen(typeid(*this).name()));
#endif
	};
	Collision(GameObject* g, Transform* t,const char* classname) :
		Component(g, t, classname)
	{
#ifdef _DEBUG
		mbstowcs_s(nullptr, name, typeid(*this).name(), strlen(typeid(*this).name()));
#endif
	};
	virtual ~Collision();
	void Awake()override;
	virtual void Update()override;

	virtual void OnDestroy()override;
	//コリジョン生成
	//第一引数 btCollisionObject* バレットフィジックスのコリジョンオブジェクト
	//第二引数 Collider* あたり判定の形状
	//第三引数 const int コリジョンに設定するID
	//第四引数 Vector3& 基点からの移動量
	//第五引数 bool 生成時にワールドに追加するか。
	void Create(btCollisionObject* collision, Collider* shape, const int& id = static_cast<int>(fbCollisionAttributeE::ALL),Vector3 offset = Vector3::zero,bool isAddWorld = true);
	btCollisionObject* GetCollisionObj() const
	{
		return _CollisionObject.get();
	}
	// シェアードポインタで取得。
	// コリジョンが削除されたときにNullチェックしたいときなどにシェアードポインタでコリジョンオブジェクトを保管しておくといい。
	// ※Nullチェックの仕方は下記。
	// if(static_cast<Collision*>(_CollisionObject->getUserPointer())->gameObject) : コリジョンがアタッチされたゲームオブジェクトのNullチェック。
	// if(_CollisionObject->getUserPointer()) : コリジョンのNullチェック。
	shared_ptr<btCollisionObject> GetCollisionObj_shared()const {
		return _CollisionObject;
	}
	//オフセットした先のポジション取得
	const Vector3 GetOffsetPos()
	{
		return transform->GetPosition() + _Offset;
	}

	//Createより前に設定しといて。
	// 全レイヤーマスクオフ。
	// すべての衝突を無視。
	inline void FilterMask_AllOff() {
		SetFilterMask(0x00000000);
	}

	// 全レイヤーマスクオン。
	// すべてのコリジョンと当たり判定を行う。
	inline void FilterMask_AllOn() {
		SetFilterMask(static_cast<int>(fbCollisionAttributeE::ALL));
	}

	// フィルターマスクに加算。
	inline void AddFilterMask(short bit) {
		SetFilterMask(_FilterMask | bit);
	}

	// フィルターマスクから減算。
	inline void SubFilterMask(short bit) {
		// すべてのbitを反転し、目的のビットのみ0、他は1にする。
		bit = ~bit;
		SetFilterMask(_FilterMask & bit);
	}

	void SetFilter(short group, short mask);
	void SetFilterGroup(short group);
	void SetFilterMask(short mask);
	inline short GetFilterGroup()const {
		return _FilterGroup;
	}
	inline short GetFilterMask()const {
		return _FilterMask;
	}
	// このコリジョン生成時に設定した属性を返却。
	inline int GetID()const {
		return _CollisionObject->getUserIndex();
	}

	// 剛体をスリープ状態にする。
	// ※物理挙動や力を加える処理などが無効化される。
	inline void Sleep() {
		if (_MyObjectType == CollisionObjectType::Rigid) {
			static_cast<btRigidBody*>(_CollisionObject.get())->wantsSleeping();
		}
	}
	// コリジョンをアクティブ状態にする。
	// ※物理挙動や力を加える処理などが有効化される。
	inline void Activate() {
		_CollisionObject->activate();
	}

	inline bool GetIsActive()const {
		return _CollisionObject->getActivationState();
	}

	// 継承先によって異なる処理。
	virtual void _AddWorldSubClass() = 0;
	// ワールドに登録。
	void AddWorld(); 
	// 継承先によって異なる処理。
	virtual void _RemoveWorldSubClass() = 0;
	// ワールドから削除。
	void RemoveWorld();
	// 動的剛体にする。
	inline void OnDynamic()
	{
		_physicsType = PhysicsType::Dynamic;
	}
	// キネマティック剛体にする。
	inline void OnKinematick()
	{
		_physicsType = PhysicsType::Kinematick;
	}
	// 静的剛体にする。
	inline void OnStatic()
	{
		_physicsType = PhysicsType::Static;
	}
	inline PhysicsType GetPhysicsType()const {
		return _physicsType;
	}
	inline CollisionObjectType GetCollisionType()const {
		return _MyObjectType;
	}
	inline const Vector3& GetOffset()const {
		return _Offset;
	}

	inline const Collider& GetShape()const {
		return *_Shape;
	}

	inline bool GetIsAddWorld()const {
		return _isAddWorld;
	}

protected:
	//コリジョンの位置や回転を更新
	void _UpdateCollisionTrans();
protected:
	//ポジションからのオフセット量
	Vector3 _Offset;
	//自分の属するグループ
	short _FilterGroup;
	//当たり判定をとるグループ
	short _FilterMask;
	//コリジョンの形状。
	Collider *_Shape;
	//コリジョンオブジェクト。
	std::shared_ptr<btCollisionObject>	_CollisionObject;
	bool _isAddWorld = false;	// ワールドに追加したか。
	CollisionObjectType _MyObjectType;	// 剛体かゴーストか。
	PhysicsType _physicsType;
};