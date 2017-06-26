#include"fbstdafx.h"
#include "BoxCollider.h"
#include "_Object\_GameObject\ModelOject.h"

/*!
 * @brief	�R���X�g���N�^�B
 */
BoxCollider::BoxCollider(GameObject* g, Transform* t) :
	Collider(g,t),
	_Shape(NULL)
{
}
/*!
 * @brief	�f�X�g���N�^�B
 */
BoxCollider::~BoxCollider()
{
	SAFE_DELETE(_Shape);
}
/*!
 * @brief	�{�b�N�X�R���C�_�[���쐬�B
 */
void BoxCollider::Create( const Vector3& size )
{
	_Shape = new btBoxShape(btVector3(size.x * 0.5f, size.y * 0.5f, size.z * 0.5f));
}

void BoxCollider::ColliderModelLoad() {
	//�����蔻������o�������I�u�W�F�N�g����
	_CollisionModel = INSTANCE(GameObjectManager)->AddNew<ModelObject>("ShowCollision", 10);
	_CollisionModel->LoadModel("BoxCollision.X");
}