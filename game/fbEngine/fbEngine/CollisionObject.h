#pragma once
#include "GameObject.h"

class BoxCollider;
class GostCollision;
//�����蔻�肷��Q�[���I�u�W�F�N�g
class CollisionObject:public GameObject
{
public:
	CollisionObject(char* name) :GameObject(name) {}
	void Initialize(int ID, float Life, Vector3 size);
	void Awake()override;
	void Update()override;
private:
	BoxCollider* _Shape;
	GostCollision* _Coll;
};
