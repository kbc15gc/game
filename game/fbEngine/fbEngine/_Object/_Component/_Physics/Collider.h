#pragma once
#include "_Object\_Component\Component.h"

class ModelObject;

//あたり判定の形状
class Collider :public Component
{
public:
	Collider(GameObject* g, Transform* t) :Component(g, t, typeid(this).name())
	{
		this->_CollisionModel = nullptr;
	};
	Collider(GameObject* g, Transform* t,const char* classname) :Component(g, t, classname)
	{
		this->_CollisionModel = nullptr;
	};
	~Collider();

	// コライダーの形状を視覚化するためのモデルを生成する関数。
	// 引数：	コライダーをアタッチするオブジェクト(自動的にこのオブジェクトと親子関係を形成する,nullだと親子関係を持たないコライダーとなる)。
	//			コリジョンオブジェクト。
	//			モデルの中心点とコリジョンの中心点の差分。
	void CreateViewModel(GameObject* Parent,btCollisionObject* CollisionObject, const Vector3& Offset);
	// コライダーの形状を視覚化するためのモデルを生成する関数。
	// 引数：	コライダーをアタッチするオブジェクト(自動的にこのオブジェクトと親子関係を形成する,nullだと親子関係を持たないコライダーとなる)。
	//			Transform情報。
	//			モデルの中心点とコリジョンの中心点の差分。
	void CreateViewModel(GameObject* Parent, const btTransform& Transform, const Vector3& Offset);

private:
	// 形状に応じたモデルデータをロード。
	// ※継承先で実装。
	virtual void ColliderModelLoad() {}/* = 0*/;

public:
	virtual btCollisionShape* GetBody() = 0 ;
protected:
	//当たり判定を視覚化した3Dオブジェクト。
	ModelObject* _CollisionModel = nullptr;
};