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
	_Type = ShapeType::Sphere;
#ifdef _DEBUG
	mbstowcs_s(nullptr, name, typeid(*this).name(), strlen(typeid(*this).name()));
#endif
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
	if (radius < 0.0f) {
		// �T�C�Y��0��菬�����l���ݒ肳��Ă���B
		abort();
	}
	shape = new btSphereShape(radius);
	btVector3 work = shape->getImplicitShapeDimensions();
	_halfSize = Vector3(work.x(), work.y(), work.z());
}

void SphereCollider::Resize(float radius) {
	SAFE_DELETE(shape);
	shape = new btSphereShape(radius);
	_collision->GetCollisionObj()->setCollisionShape(shape);
	btVector3 work = shape->getImplicitShapeDimensions();
	_halfSize = Vector3(work.x(), work.y(), work.z());
#ifdef _DEBUG
	RecreateViewModel();
#endif
}

#ifdef _DEBUG
void SphereCollider::ColliderModelLoad() {

	//�����蔻������o�������I�u�W�F�N�g����
	_CollisionModel = INSTANCE(GameObjectManager)->AddNew<ModelObject>("ShowCollision", 10);
	_CollisionModel->LoadModel("SphereCollision.X");
}
#endif