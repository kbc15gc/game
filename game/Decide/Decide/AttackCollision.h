#pragma once
#include "fbEngine\GameObject.h"

class BoxCollider;

class AttackCollision : public GameObject
{
public:
	AttackCollision(const char* name) :GameObject(name) {}
	void Awake()override;
	void Update()override;
	void Create(Vector3& size);
private:
	BoxCollider* _BoxColl;
	float time;				//ボックスコリジョン削除用時間（仮）
};