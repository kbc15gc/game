#pragma once
#include "_Object\_Component\_Physics\Collision.h"

//剛体クラス。
class RigidBody:public Collision
{
public:
	RigidBody(GameObject* g, Transform* t);
	~RigidBody();
	void Awake()override;
	void Update()override;
	void LateUpdate()override;
	void Release();
	//void Create(RigidBodyInfo& rbInfo);
	// コリジョン生成関数。
	// 引数：	質量。
	//			形状。
	//			コリジョン属性。
	//			物理現象以外の慣性(基本0)。
	//			差分。
	//			生成時にワールドに登録するか(登録した瞬間のTransformの値でAABBのバウンディングボックスが生成される)。
	void Create(float mass,Collider* coll, int id, Vector3 inertia = Vector3::zero,Vector3 off = Vector3::zero, bool isAddWorld = true);
	void SetGravity(Vector3 set);
	void SetGravity(btVector3& set);
	void SetGravity(float x, float y, float z);

	// ワールドに登録。
	void _AddWorldSubClass()override;
	// ワールドから削除。
	void _RemoveWorldSubClass()override;
private:
	btDefaultMotionState*	myMotionState;	//モーションステート。
};

