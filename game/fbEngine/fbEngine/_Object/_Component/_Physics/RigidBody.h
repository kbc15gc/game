#pragma once
#include "_Object\_Component\_Physics\Collision.h"

//���̃N���X�B
class RigidBody:public Collision
{
public:
	RigidBody(GameObject* g, Transform* t);
	~RigidBody();
	void Awake()override;
	void Update()override;
	void LateUpdate()override;
	void Release();
	//void Create(RigidBodyInfo& rbInfo);
	// �R���W���������֐��B
	// �����F	���ʁB
	//			�`��B
	//			�R���W���������B
	//			�������ۈȊO�̊���(��{0)�B
	//			�����B
	//			�������Ƀ��[���h�ɓo�^���邩(�o�^�����u�Ԃ�Transform�̒l��AABB�̃o�E���f�B���O�{�b�N�X�����������)�B
	void Create(float mass,Collider* coll, int id, Vector3 inertia = Vector3::zero,Vector3 off = Vector3::zero, bool isAddWorld = true);
	void SetGravity(Vector3 set);
	void SetGravity(btVector3& set);
	void SetGravity(float x, float y, float z);

	// ���[���h�ɓo�^�B
	void _AddWorldSubClass()override;
	// ���[���h����폜�B
	void _RemoveWorldSubClass()override;
private:
	btDefaultMotionState*	myMotionState;	//���[�V�����X�e�[�g�B
};

