#pragma once
#include "_Object\_Component\_Physics\Collision.h"

struct RigidBodyInfo
{
	// 剛体ダイナミクス(物理挙動するか、ゲーム内での移動などを反映するか)。
	Collision::PhysicsType physicsType = Collision::PhysicsType::Dynamic;
	//質量。
	float mass = 1.0f;
	//コリジョンの形状。
	Collider* coll;
	//コリジョンの属性。
	int id;
	//物理現象以外の慣性(基本0)。
	Vector3 inertia = Vector3::zero;
	//ポジションからどれだけ移動させるか。
	Vector3 offset = Vector3::zero;
	//回転
	Quaternion rotation = Quaternion::Identity;
};

//剛体クラス。
class RigidBody:public Collision
{
public:
	RigidBody(GameObject* g, Transform* t);
	~RigidBody();
	void Awake()override;
	void Update()override;
	void LateUpdate()override;
	void OnEnable()override;
	void OnDisable()override;
	void Release();
	void Create(RigidBodyInfo& rbInfo, bool isAddWorld = true);
	// コリジョン生成関数。
	// 引数：	質量。
	//			形状。
	//			コリジョン属性。
	//			物理現象以外の慣性(基本0)。
	//			差分。
	//			剛体ダイナミクス(物理挙動するか、ゲーム内での移動などを反映するか)。
	//			生成時にワールドに登録するか(登録した瞬間のTransformの値でAABBのバウンディングボックスが生成される)。
	void Create(float mass,Collider* coll, int id, Vector3 inertia = Vector3::zero,Vector3 off = Vector3::zero, PhysicsType physicsType = PhysicsType::Dynamic, bool isAddWorld = true);
	void SetGravity(Vector3 set);
	void SetGravity(btVector3& set);
	void SetGravity(float x, float y, float z);

	//眠るんじゃねぇぞ・・・
	void NonSleep();

	// ワールドに登録。
	void _AddWorldSubClass()override;
	// ワールドから削除。
	void _RemoveWorldSubClass()override;
private:
	btDefaultMotionState*	myMotionState;	//モーションステート。
};

