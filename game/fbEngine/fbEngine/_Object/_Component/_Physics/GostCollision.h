#pragma once
#include "_Object\_Component\_Physics\Collision.h"

//他のコリジョンと衝突しないコリジョン。
//自身と重なったコリジョンを素早く取得できる(はず)。
class GostCollision :public Collision
{
public:
	GostCollision(GameObject* g, Transform* t) :Collision(g, t, typeid(this).name())
	{
#ifdef _DEBUG
		mbstowcs_s(nullptr, name, typeid(*this).name(), strlen(typeid(*this).name()));
#endif

	};
	GostCollision(GameObject* g, Transform* t, const char* classname) :Collision(g, t, classname)
	{
#ifdef _DEBUG
		mbstowcs_s(nullptr, name, typeid(*this).name(), strlen(typeid(*this).name()));
#endif

	};
	virtual ~GostCollision();
	void Awake()override;
	// コリジョン生成関数。
	// 引数：	形状。
	//			コリジョン属性。
	//			生成時にワールドに登録するか(登録した瞬間のTransformの値でAABBのバウンディングボックスが生成される)。
	void Create(Collider* shape, int id, bool isAddWorld = true);
	void Update()override;
	//ゴーストオブジェクトと重なっているコリジョンを取得する。
	//重なっているかどうかの判定はUpdateで行われるので。
	//これを使うのはUpdate後(LateUpdate)にしてね。
	const btAlignedObjectArray<btCollisionObject*>& GetPairCollisions();
	//他のコリジョンが接触した時に呼ばれる
	virtual void OnCollisionEnter(Collision* coll) {};
	//触れているコリジョンが離れた時に呼び出される。
	virtual void OnCollisionExit(Collision* coll) {};

	void _AddWorldSubClass()override;

	void _RemoveWorldSubClass()override;
protected:
	//ゴーストへのポインタ(_CollisionObjectをキャストしただけ。)
	btGhostObject* _GostObject;
};