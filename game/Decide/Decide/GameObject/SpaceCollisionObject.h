#pragma once
#include "fbEngine\_Object\_GameObject\CollisionObject.h"

class Player;

class SpaceCollisionObject :public CollisionObject {
public:
	SpaceCollisionObject(char* name) :CollisionObject(name) {};
	~SpaceCollisionObject(){};

	// 空間コリジョン生成関数。
	// 引数：	位置(ローカル座標)。
	//			回転(ローカル座標)。
	//			コリジョンサイズ。
	//			コリジョン属性。
	//			親のTransform情報。
	//			衝突判定したいコリジョン属性。
	void Create(const Vector3& pos, const Quaternion& rot, const Vector3& size, int id, Transform* parent, int attr);
	void Start()override;

	// 空間の状態を更新する。
	void UpdateActiveSpace();

	// 衝突しているオブジェクトをすべて検出。 
	inline void RegistrationObject() 
	{
		_HitCollisions.clear();
		if (GetCollision()) {
			if (GetCollision()->GetCollisionObj()) {
				_HitCollisions = INSTANCE(PhysicsWorld)->AllHitsContactTest(GetCollision(), _HitCollisions, _attribute);
			}
		}
	}

	// 衝突していれば衝突オブジェクトに追加登録。
	// 引数：	衝突判定をしたいゲームオブジェクト。
	void AddObjectHitSpace(GameObject& object);

	// この空間に属するオブジェクトをアクティブ化。
	inline void EnableObjects() {
		_SetActives(true);
	}
	// 隣接する空間オブジェクトに属するオブジェクトをすべてアクティブ化。
	void _EnableObjectsAdjacent();

	// この空間に属するオブジェクトを非アクティブ化。
	inline void DisableObjects() {
		_SetActives(false);
	}

private:

	// この空間に属するオブジェクトにアクティブフラグ設定。
	void _SetActives(bool flg);

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
	vector<Collision*> _HitCollisions;	// 衝突しているコリジョン。

	vector<SpaceCollisionObject*> _adjacentSpaceObjects;		// 隣接する空間オブジェクト。
	GameObject* _player = nullptr;
	bool _isHitPlayer;	// このコリジョンオブジェクトがプレイヤーと衝突しているか。
	int _attribute;	// この空間に登録するオブジェクトのコリジョン属性。
};