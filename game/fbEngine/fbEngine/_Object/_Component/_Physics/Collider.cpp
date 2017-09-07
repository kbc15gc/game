#include "fbstdafx.h"
#include "_Object\_Component\_Physics\Collider.h"
#include "_Object\_GameObject\ModelOject.h"

#ifdef _DEBUG
const wchar_t* Collider::ShapeName[] = {
	L"Box",
	L"Sphere",
	L"Capsule",
	L"Mesh",
};
#endif


Collider::~Collider(){
#ifdef _DEBUG
	if (_CollisionModel) {
		_CollisionModel->transform->SetParent(nullptr);
		INSTANCE(GameObjectManager)->AddRemoveList(_CollisionModel);
	}
#endif //_DEBUG
};

#ifdef _DEBUG
void Collider::Debug(){
	if ((KeyBoardInput->isPush(DIK_M))) {
		if (GetIsRender()) {
			// �R���W�����`��I�t�B
			RenderDisable();
		}
		else {
			// �R���W�����`��I���B
			RenderEnable();
		}
	}
}
#endif

#ifdef _DEBUG
void Collider::RecreateViewModel() {
	bool isEnable = _CollisionModel->GetSkinModel()->enable;
	CreateViewModel(_collision->GetCollisionObj()->getWorldTransform());
	_CollisionModel->GetSkinModel()->enable = isEnable;	// ���O�̕`���Ԃ�ݒ�B
}

void Collider::CreateViewModel(const btTransform& collisionTr){

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
		//�����蔻��̑傫���𒲂ׂ�B
		btVector3 min, max;
		btTransform tr;
		tr.setIdentity();
		GetBody()->getAabb(tr, min, max);
		Vector3 size = Vector3(max.x() - min.x(), max.y() - min.y(), max.z() - min.z());
		//�����蔻��̃T�C�Y���w��
		_CollisionModel->transform->SetLocalScale(size);
		//���ۂ̃R���W������e�ɐݒ�(�`��p���f�����R���W�����̍��W�n�ɂ���B)

		// �R���W������Transform��񐶐��B
		_CollisionTr.reset(new Transform(nullptr,nullptr));
		
		UpdateTransform(collisionTr);

		//�R���W�����`��p���f���̒��S�_�ƃR���W�����̒��S�_�̍�����ݒ�B
		_CollisionModel->transform->SetLocalPosition(_CollisionModelOffset);
	}
}


void Collider::UpdateTransform(const btTransform& collisionTr) {

	if (_CollisionTr.get()) {
		// �ʒu���ݒ�B
		btVector3 pos = collisionTr.getOrigin();
		_CollisionTr->SetPosition(pos.getX(), pos.getY(), pos.getZ());
		// ��]���ݒ�B
		btQuaternion rot = collisionTr.getRotation();
		_CollisionTr->SetRotation(rot.getX(), rot.getY(), rot.getZ(), rot.getW());
		// �R���W������Transform����e�ɐݒ�B
		_CollisionModel->transform->SetParent(_CollisionTr.get());
	}
}


bool Collider::GetIsRender() {
	if (_CollisionModel) {
		return _CollisionModel->GetSkinModel()->enable;
	}
	return false;
}

void Collider::RenderEnable() {
	if (_CollisionModel) {
		_CollisionModel->GetSkinModel()->enable = true;
	}
}

void Collider::RenderDisable() {
	if (_CollisionModel) {
		_CollisionModel->GetSkinModel()->enable = false;
	}
}
#endif