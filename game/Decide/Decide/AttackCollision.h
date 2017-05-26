#pragma once
#include "fbEngine\_Object\_Component\_Physics\GostCollision.h"

class AttackCollision : public GameObject
{
public:
	enum class CollisionMaster{Player = 0,Enemy, Other};	// �N���R���W�����𔭐����������B
public:
	AttackCollision(const char * name) :GameObject(name)
	{

	};

	void Awake()override;
	void Update()override;

	// �U������p�̃R���W���������B
	// �����F	�T�C�Y�B
	inline void Create(const Vector3& size) {
		static_cast<BoxCollider*>(_Colider)->Create(size);		// �R���C�_�[�����B
		_Gost->Create(_Colider,Collision_ID::ATTACK);	// �S�[�X�g�R���W���������B
	}


	// �U������p�̃R���W���������B
	// �����F	�T�C�Y�B
	//			�N���R���W�����𐶐��������B
	inline void Create(const Vector3& size, CollisionMaster master) {
		_master = master;
		Create(size);
	}

	// �U������p�̃R���W���������B
	// �����F	�T�C�Y�B
	//			�N���R���W�����𐶐��������B
	//			�R���W��������(0.0f��菬�����l�Ŗ���)�B
	inline void Create(const Vector3& size, CollisionMaster master, float lifeTime) {
		_lifeTime = lifeTime;
		Create(size, master);
	}

private:
	Collider* _Colider = nullptr;	// �R���W�����`��B
	GostCollision* _Gost = nullptr;	// �S�[�X�g�B
	float time;				//�R���W�����폜�J�E���^�[�B
	float _lifeTime = -1.0f;		// �R���W��������(0.0f��菬�����l�Ŗ���)�B
	CollisionMaster _master;	// �N�������������R���W�������B
};