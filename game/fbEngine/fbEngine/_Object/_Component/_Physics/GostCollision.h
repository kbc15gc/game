#pragma once
#include "_Object\_Component\_Physics\Collision.h"

//���̃R���W�����ƏՓ˂��Ȃ��R���W�����B
//���g�Əd�Ȃ����R���W������f�����擾�ł���(�͂�)�B
class GostCollision :public Collision
{
public:
	GostCollision(GameObject* g, Transform* t) :Collision(g, t, typeid(this).name())
	{
		
	};
	GostCollision(GameObject* g, Transform* t,const char* classname) :Collision(g, t, classname)
	{

	};
	virtual ~GostCollision();
	void Awake()override;
	void Create(Collider* shape, int id);
	void Update()override;
	//�S�[�X�g�I�u�W�F�N�g�Əd�Ȃ��Ă���R���W�������擾����B
	//�d�Ȃ��Ă��邩�ǂ����̔����Update�ōs����̂ŁB
	//������g���̂�Update��(LateUpdate)�ɂ��ĂˁB
	const btAlignedObjectArray<btCollisionObject*>& GetPairCollisions();
	//���̃R���W�������ڐG�������ɌĂ΂��
	virtual void OnCollisionEnter(Collision* coll) {};
	//�G��Ă���R���W���������ꂽ���ɌĂяo�����B
	virtual void OnCollisionExit(Collision* coll) {};
protected:
	//�S�[�X�g�ւ̃|�C���^(_CollisionObject���L���X�g���������B)
	btGhostObject* _GostObject;
};