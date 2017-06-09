#pragma once
#include "_Object\_Component\Component.h"
#include "_Include\CollisionEnum.h"

class ModelObject;
//あたり判定の基底クラス
class Collision:public Component
{
public:
	Collision(GameObject* g, Transform* t) :
		Component(g, t, typeid(this).name())
	{
	
	};
	Collision(GameObject* g, Transform* t,const char* classname) :
		Component(g, t, classname)
	{
		
	};
	virtual ~Collision();
	void Awake()override;
	virtual void Update();
	//コリジョン生成
	//第一引数 btCollisionObject* バレットフィジックスのコリジョンオブジェクト
	//第二引数 Collider* あたり判定の形状
	//第三引数 const int コリジョンに設定するID
	//第四引数 Vector3& 基点からの移動量
	void Create(btCollisionObject* collision, Collider* shape, const int& id = static_cast<int>(fbCollisionAttributeE::ALL),Vector3 offset = Vector3::zero);
	btCollisionObject* GetCollisonObj() const
	{
		return _CollisionObject.get();
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
};