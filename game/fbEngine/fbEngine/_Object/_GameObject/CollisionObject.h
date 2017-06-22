#pragma once
#include "GameObject.h"
#include "_Include\CollisionInclude.h"

//あたり判定するゲームオブジェクト
class CollisionObject:public GameObject
{
public:
	CollisionObject(char* name) :GameObject(name) {}
	void Create(int ID, Vector3 size, bool isAddWorld = true);
	void Awake()override;
	void Update()override;

	inline GostCollision* GetCollision() const{
		return _Coll;
	}
private:
	BoxCollider* _Shape;
	GostCollision* _Coll;
};
