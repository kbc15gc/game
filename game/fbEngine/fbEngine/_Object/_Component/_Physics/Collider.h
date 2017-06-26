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

	void Update()override;
	// コライダーの形状を視覚化するためのモデルを生成する関数。
	// 引数：	コリジョンのTransform情報。
	void CreateViewModel(const btTransform& collisionTr);
	// コリジョン描画用モデルのTransform情報更新。
	void UpdateTransform(const btTransform& collisionTr);

	// 描画中か。
	bool GetIsRender();
	// 描画オン。
	void RenderEnable();
	// 描画オフ。
	void RenderDisable();
private:
	// 形状に応じたモデルデータをロード。
	// ※継承先で実装。
	virtual void ColliderModelLoad() = 0;

public:
	virtual btCollisionShape* GetBody() = 0 ;
protected:
	//当たり判定を視覚化した3Dオブジェクト。
	ModelObject* _CollisionModel = nullptr;
	Vector3 _CollisionModelOffset = Vector3::zero;	// コリジョン視覚化用モデルの中心点とコリジョンの中心点の差分(継承先によって変更)。
private:
	unique_ptr<Transform> _CollisionTr;	// コリジョンのTransform情報。
};