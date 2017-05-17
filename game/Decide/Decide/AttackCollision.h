#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"

class AttackCollision : public GameObject
{
public:
	enum class CollisionMaster{Player = 0,Enemy, Other};	// �N���R���W�����𔭐����������B
public:
	AttackCollision(const char* name) :GameObject(name) {}
	void Awake()override;
	void Update()override;
	void Create(const Vector3& size);

	// �U������p�̃R���W���������B
	// �����F	�T�C�Y�B
	//			�N���R���W�����𐶐��������B
	void Create(const Vector3& size,CollisionMaster master);

	// �U������p�̃R���W���������B
	// �����F	�T�C�Y�B
	//			�N���R���W�����𐶐��������B
	//			�R���W���������B
	void Create(const Vector3& size, CollisionMaster master, float lifeTime);

private:
	BoxCollider* _BoxColl;
	float time;				//�R���W�����폜�J�E���^�[�B
	float _lifeTime = 3.0f;		// �R���W���������B
	CollisionMaster _master;	// �N�������������R���W�������B
};