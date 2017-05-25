/*!
 * @brief	���̃R���C�_�[�B
 */
#include"fbstdafx.h"
#include "_Object\_Component\_Physics\SphereCollider.h"
#include "_Object\_GameObject\ModelOject.h"

/*!
 * @brief	�R���X�g���N�^�B
 */
SphereCollider::SphereCollider(GameObject* g, Transform* t) :
	Collider(g,t, typeid(this).name()),
	shape(NULL)
{

}
/*!
 * @brief	�f�X�g���N�^�B
 */
SphereCollider::~SphereCollider()
{
	delete shape;
}
/*!
 * @brief	���̃R���C�_�[���쐬�B
 */
void SphereCollider::Create( const float radius )
{
	shape = new btSphereShape(radius);
}

void SphereCollider::ColliderModelLoad() {
	//�����蔻������o�������I�u�W�F�N�g����
	_CollisionModel = INSTANCE(GameObjectManager)->AddNew<ModelObject>("ShowCollision", 10);
	_CollisionModel->LoadModel("SphereCollision.X");
}