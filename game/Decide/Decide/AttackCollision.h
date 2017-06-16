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
	~AttackCollision() {
		_HitCollisions.clear();
	}

	void Awake()override;
	void Update()override;
	void LateUpdate()override;

	// �U������p�̃R���W���������B
	// �����F	�R���W��������҂̍U����
	//			�ʒu(���[�J�����W)�B
	//			��](���[�J�����W�A�N�H�[�^�j�I��)�B
	//			�T�C�Y�B
	//			�N���R���W�����𐶐��������B
	//			�R���W��������(0.0f��菬�����l�Ŗ���)�B
	//			�e�ɂ�����Transform���(�������Ȃǂ̏�ŃR���W���������������ꍇ�Ɏg�p)�B
	// �߂�l�F	���������R���W�����B
	GostCollision* Create(int damage,const Vector3& pos, const Quaternion& rotation, const Vector3& size, CollisionMaster master = CollisionMaster::Other, float lifeTime = -1.0f, Transform* Parent = nullptr);

	inline void SetParent(Transform* Parent) {
		transform->SetParent(Parent);
	}
	inline void RemoveParent() {
		transform->SetParent(nullptr);
	}
	//�N�������������R���W���������Q�b�g�B
	inline CollisionMaster GetMaster()  const
	{
		return _master;
	}

	//�ݒ肳�ꂽ�_���[�W���Q�b�g�B
	inline int GetDamage()
	{
		return _Damage;
	}
private:	
	// �Փˌ��o�B
	void DetectionCollision();
	// �Փ˂����u�ԌĂԃR�[���o�b�N�����B
	void _CallBackEnter(Collision* coll);
	// �Փ˂��Ă���ԌĂё�����R�[���o�b�N�����B
	void _CallBackStay(Collision* coll);
	// �Փ˂����u�ԌĂԃR�[���o�b�N�����B
	void _CallBackExit(Collision* coll);
private:
	Collider* _Colider = nullptr;	// �R���W�����`��B
	GostCollision* _Gost = nullptr;	// �S�[�X�g�B
	float time;				//�R���W�����폜�J�E���^�[�B
	float _lifeTime = -1.0f;		// �R���W��������(0.0f��菬�����l�Ŗ���)�B
	CollisionMaster _master;	// �N�������������R���W�������B
	int _Damage = 0;
	vector<Collision*> _HitCollisions;	// �������Ă���R���W�����B
};