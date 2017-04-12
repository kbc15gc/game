#pragma once
#include "Collision.h"
class Collider;

//���̏��B
struct RigidBodyInfo {
	RigidBodyInfo() :
		collider(NULL),
		mass(0.0f)
	{
		pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					//������
		rot = D3DXQUATERNION{ 0.0f, 0.0f, 0.0f, 1.0f };		//������
	}
	D3DXVECTOR3 pos;				//���W�B
	D3DXQUATERNION rot;			//��]�B
	Collider* collider;		//�R���C�_�[�B
	float mass;					//���ʁB

};

//���̃N���X�B
class RigidBody:public Collision
{
public:
	RigidBody(GameObject* g, Transform* t);
	~RigidBody();
	void Update()override;
	void LateUpdate()override;
	void Release();
	//void Create(RigidBodyInfo& rbInfo);
	void Create(float mass,Collider* coll, int id, Vector3 inertia = Vector3::zero,Vector3 off = Vector3::zero);
	void SetGravity(Vector3 set);
	void SetGravity(btVector3& set);
	void SetGravity(float x, float y, float z);
private:
	btDefaultMotionState*	myMotionState;	//���[�V�����X�e�[�g�B
};

