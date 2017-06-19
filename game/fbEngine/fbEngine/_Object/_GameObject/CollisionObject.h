#pragma once
#include "GameObject.h"
#include "_Include\CollisionInclude.h"

//あたり判定するゲームオブジェクト
class CollisionObject:public GameObject
{
public:
	CollisionObject(char* name) :GameObject(name) {}
	void Initialize(int ID, Vector3 size);
	void Awake()override;
	void Update()override;
private:
	BoxCollider* _Shape;
	GostCollision* _Coll;
};
