#pragma once
#include "_Object\_Component\_Physics\Collision.h"

struct RigidBodyInfo
{
	// ���̃_�C�i�~�N�X(�����������邩�A�Q�[�����ł̈ړ��Ȃǂ𔽉f���邩)�B
	Collision::PhysicsType physicsType = Collision::PhysicsType::Dynamic;
	//���ʁB
	float mass = 1.0f;
	//�R���W�����̌`��B
	Collider* coll;
	//�R���W�����̑����B
	int id;
	//�������ۈȊO�̊���(��{0)�B
	Vector3 inertia = Vector3::zero;
	//�|�W�V��������ǂꂾ���ړ������邩�B
	Vector3 offset = Vector3::zero;
	//��]
	Quaternion rotation = Quaternion::Identity;
};

//���̃N���X�B
class RigidBody:public Collision
{
public:
	RigidBody(GameObject* g, Transform* t);
	~RigidBody();
	void Awake()override;
	void Update()override;
	void LateUpdate()override;
	void OnEnable()override;
	void OnDisable()override;
	void Release();
	void Create(RigidBodyInfo& rbInfo, bool isAddWorld = true);
	// �R���W���������֐��B
	// �����F	���ʁB
	//			�`��B
	//			�R���W���������B
	//			�������ۈȊO�̊���(��{0)�B
	//			�����B
	//			���̃_�C�i�~�N�X(�����������邩�A�Q�[�����ł̈ړ��Ȃǂ𔽉f���邩)�B
	//			�������Ƀ��[���h�ɓo�^���邩(�o�^�����u�Ԃ�Transform�̒l��AABB�̃o�E���f�B���O�{�b�N�X�����������)�B
	void Create(float mass,Collider* coll, int id, Vector3 inertia = Vector3::zero,Vector3 off = Vector3::zero, PhysicsType physicsType = PhysicsType::Dynamic, bool isAddWorld = true);
	void SetGravity(Vector3 set);
	void SetGravity(btVector3& set);
	void SetGravity(float x, float y, float z);

	//����񂶂�˂����E�E�E
	void NonSleep();

	// ���[���h�ɓo�^�B
	void _AddWorldSubClass()override;
	// ���[���h����폜�B
	void _RemoveWorldSubClass()override;
private:
	btDefaultMotionState*	myMotionState;	//���[�V�����X�e�[�g�B
};

