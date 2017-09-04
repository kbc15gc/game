#pragma once
#include "_Object\_Component\Component.h"

class ModelObject;
class Collision;

//あたり判定の形状
class Collider :public Component
{
public:
	enum class ShapeType { Box = 0, Sphere, Capsule, CapsuleX, CapsuleZ, Mesh };
#ifdef _DEBUG
	static const wchar_t* ShapeName[];
#endif
public:
	Collider(GameObject* g, Transform* t) :Component(g, t, typeid(this).name())
	{
#ifdef _DEBUG
		this->_CollisionModel = nullptr;
		mbstowcs_s(nullptr, name, typeid(*this).name(), strlen(typeid(*this).name()));
#endif
	};
	Collider(GameObject* g, Transform* t,const char* classname) :Component(g, t, classname)
	{
#ifdef _DEBUG
		this->_CollisionModel = nullptr;
		mbstowcs_s(nullptr, name, typeid(*this).name(), strlen(typeid(*this).name()));
#endif
	};
	~Collider();

#ifdef _DEBUG
	void Debug()override;
#endif
	// コライダーの形状を視覚化するためのモデルを生成する関数。
	// 引数：	コリジョンのTransform情報。
	void CreateViewModel(const btTransform& collisionTr);
	// コリジョン描画用モデルのTransform情報更新。
	void UpdateTransform(const btTransform& collisionTr);

#ifdef _DEBUG
	// 描画中か。
	bool GetIsRender();
	// 描画オン。
	void RenderEnable();
	// 描画オフ。
	void RenderDisable();
#endif

	inline ShapeType GetType()const {
		return _Type;
	}
	inline const Vector3& GetHalfSize()const {
		return _halfSize;
	}
	inline void SetCollision(Collision* collision) {
		_collision = collision;
	}
private:
	// 形状に応じたモデルデータをロード。
	// ※継承先で実装。
	virtual void ColliderModelLoad() = 0;

public:
	virtual btCollisionShape* GetBody() = 0 ;
protected:
	ShapeType _Type;
	Vector3 _halfSize;
	Collision* _collision = nullptr;	// このコライダーを保持するコリジョン。
#ifdef _DEBUG
protected:
	//当たり判定を視覚化した3Dオブジェクト。
	ModelObject* _CollisionModel = nullptr;
	Vector3 _CollisionModelOffset = Vector3::zero;	// コリジョン視覚化用モデルの中心点とコリジョンの中心点の差分(継承先によって変更)。
private:
	unique_ptr<Transform> _CollisionTr;	// コリジョンのTransform情報。
#endif
};