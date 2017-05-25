#include "fbstdafx.h"
#include "_Object\_Component\_Physics\Collider.h"
#include "_Object\_GameObject\ModelOject.h"

Collider::~Collider(){
	if (_CollisionModel) {
		INSTANCE(GameObjectManager)->AddRemoveList(_CollisionModel);
	}
};

void Collider::CreateViewModel(GameObject* Parent, btCollisionObject* CollisionObject, const Vector3& Offset){
	CreateViewModel(Parent,CollisionObject->getWorldTransform(), Offset);
}

void Collider::CreateViewModel(GameObject* Parent, const btTransform& Transform, const Vector3& Offset){
//�O�ɐݒ肳��Ă����A�h���X���폜
	if (_CollisionModel)
		INSTANCE(GameObjectManager)->AddRemoveList(_CollisionModel);

	// �`��ɉ��������f�������[�h�B
	ColliderModelLoad();

	//���f��������̂Ȃ�
	if (_CollisionModel)
	{
		_CollisionModel->GetSkinModel()->SetModelEffect(ModelEffectE::NONE);
		//�������ȐԂɐݒ�B
		_CollisionModel->GetSkinModel()->SetAllBlend(Color(1.0f, 0.0f, 0.0f, 0.5f));
		//�q�ɐݒ�
		if (Parent) {
			_CollisionModel->transform->SetParent(Parent->transform);
		}
		//�����蔻��̑傫���𒲂ׂ�B
		btVector3 min, max;
		GetBody()->getAabb(Transform, min, max);
		Vector3 size = Vector3(max.x() - min.x(), max.y() - min.y(), max.z() - min.z());
		//�����蔻��̃T�C�Y���w��
		_CollisionModel->transform->SetLocalScale(size);
		//�����蔻������炷
		_CollisionModel->transform->SetLocalPosition(Offset);
	}
}
