/*!
 * @brief	�J�v�Z���R���C�_�[�B
 */
#include"fbstdafx.h"
#include "CapsuleCollider.h"


 /*!
 * @brief	�R���X�g���N�^�B
 */
CCapsuleCollider::CCapsuleCollider(GameObject* g, Transform* t) :
	Collider(g,t),
	shape(nullptr)
{
	_Type = ShapeType::Capsule;
#ifdef _DEBUG
	mbstowcs_s(nullptr, name, typeid(*this).name(), strlen(typeid(*this).name()));
#endif
}
/*!
* @brief	�f�X�g���N�^�B
*/
CCapsuleCollider::~CCapsuleCollider()
{
	delete shape;
}

void CCapsuleCollider::Create(float radius, float height)
{
	if (radius < 0.0f || height < 0.0f) {
		// �R���C�_�[�T�C�Y��0��菬�����l���ݒ肳��Ă��B
		abort();
	}
	shape = new btCapsuleShape(radius, height);
	btVector3 work = shape->getImplicitShapeDimensions();
	_halfSize = Vector3(work.x(), work.y() + work.x(), work.z());
}

void CCapsuleCollider::Resize(float radius, float height) {
	shape->setImplicitShapeDimensions(btVector3(radius, height * 0.5f, radius));
	_halfSize = Vector3(radius, height * 0.5f + radius, radius);
	CreateViewModel(_collision->GetCollisionObj()->getWorldTransform());
}
