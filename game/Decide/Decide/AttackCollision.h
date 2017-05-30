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
	// �����F	�ʒu�B
	//			��](�I�C���[�A�P�ʂ̓��W�A��)�B
	//			�T�C�Y�B
	//			�N���R���W�����𐶐��������B
	//			�R���W��������(0.0f��菬�����l�Ŗ���)�B
	//			�e�ɂ�����Transform���(�������Ȃǂ̏�ŃR���W���������������ꍇ�Ɏg�p)�B
	// �߂�l�F	���������R���W�����B
	GostCollision* Create(const Vector3& pos, const Vector3& angle, const Vector3& size, CollisionMaster master = CollisionMaster::Other, float lifeTime = -1.0f, Transform* Parent = nullptr);
private:	
	// �Փˌ��o�B
	void DetectionCollision();

private:
	Collider* _Colider = nullptr;	// �R���W�����`��B
	GostCollision* _Gost = nullptr;	// �S�[�X�g�B
	float time;				//�R���W�����폜�J�E���^�[�B
	float _lifeTime = -1.0f;		// �R���W��������(0.0f��菬�����l�Ŗ���)�B
	CollisionMaster _master;	// �N�������������R���W�������B
};