#pragma once
#include "fbEngine\_Object\_GameObject\CollisionObject.h"

class Player;

class SpaceCollisionObject :public CollisionObject {
public:
	SpaceCollisionObject(char* name) :CollisionObject(name) {};
	~SpaceCollisionObject(){};
	void Create(const Vector3& pos, const Quaternion& rot, const Vector3& scale, int id);
	void Start()override;
	void Update()override;

	// 衝突しているオブジェクトをすべて検出。 
	inline void RegistrationObject() {
		_HitCollisions = GetCollision()->GetPairCollisions();
	}

	// 衝突していれば衝突オブジェクトに追加登録。
	// 引数：	衝突判定をしたいゲームオブジェクト。
	void AddObjectHitSpace(GameObject& object);

	// この空間に属するオブジェクトをアクティブ化。
	inline void EnableObjects() {
		_SetActives(true);
	}

	// この空間に属するオブジェクトを非アクティブ化。
	inline void DisableObjects() {
		_SetActives(false);
	}

private:

	// この空間に属するオブジェクトにアクティブフラグ設定。
	void _SetActives(bool flg);

	// 隣接する空間オブジェクトに属するオブジェクトをすべてアクティブ化。
	void _EnableObjectsAdjacent();

	// 引数に指定したオブジェクトからアタッチされたコリジョンコンポーネントを取得。
	Collision* _GetAttachCollision(GameObject& object);
public:
	// プレイヤーと衝突しているか取得。
	inline bool GetisHitPlayer()const {
		return _isHitPlayer;
	}

	// 隣接する空間オブジェクトを追加。
	inline void AddAdjacentSpaceObject(SpaceCollisionObject* obj) {
		_adjacentSpaceObjects.push_back(obj);
	}
private:
	btAlignedObjectArray<btCollisionObject*> _HitCollisions;	// 衝突しているコリジョン。
	vector<SpaceCollisionObject*> _adjacentSpaceObjects;		// 隣接する空間オブジェクト。
	GameObject* _player = nullptr;
	bool _isHitPlayer;	// このコリジョンオブジェクトがプレイヤーと衝突しているか。
};