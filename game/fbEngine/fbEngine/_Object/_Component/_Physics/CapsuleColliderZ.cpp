/*!
* @brief	�J�v�Z���R���C�_�[Z�B
*/
#include"fbstdafx.h"
#include "CapsuleColliderZ.h"


/*!
* @brief	�R���X�g���N�^�B
*/
CCapsuleColliderZ::CCapsuleColliderZ(GameObject* g, Transform* t) :
	Collider(g, t),
	shape(nullptr)
{
	_Type = ShapeType::CapsuleZ;
#ifdef _DEBUG
	mbstowcs_s(nullptr, name, typeid(*this).name(), strlen(typeid(*this).name()));
#endif
}
/*!
* @brief	�f�X�g���N�^�B
*/
CCapsuleColliderZ::~CCapsuleColliderZ()
{
	delete shape;
}

void CCapsuleColliderZ::Create(float radius, float height)
{
	if (radius < 0.0f || height < 0.0f) {
		// �R���C�_�[�T�C�Y��0��菬�����l���ݒ肳��Ă��B
		abort();
	}
	shape = new btCapsuleShapeZ(radius, height);
	btVector3 work = shape->getImplicitShapeDimensions();
	_halfSize = Vector3(work.x(), work.y(),work.z() + work.x());
}

void CCapsuleColliderZ::Resize(float radius, float height) {
	shape->setImplicitShapeDimensions(btVector3(radius, radius, height * 0.5f));
	_halfSize = Vector3(radius, radius, height * 0.5f + radius);
	CreateViewModel(_collision->GetCollisionObj()->getWorldTransform());
}
