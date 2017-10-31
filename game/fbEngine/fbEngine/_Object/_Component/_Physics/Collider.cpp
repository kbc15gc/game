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
	bool isEnable = _CollisionModel->GetSkinModel()->GetEnable();
	CreateViewModel();
	_CollisionModel->GetSkinModel()->SetEnable(isEnable);	// ���O�̕`���Ԃ�ݒ�B
}

void Collider::CreateViewModel(){

//�O�ɐݒ肳��Ă����A�h���X���폜
	if (_CollisionModel)
		INSTANCE(GameObjectManager)->AddRemoveList(_CollisionModel);

	_CollisionModel = nullptr;

	// �`��ɉ��������f�������[�h�B
	ColliderModelLoad();

	//���f��������̂Ȃ�
	if (_CollisionModel)
	{
		_CollisionModel->GetSkinModel()->SetModelEffect(ModelEffectE::NONE);
		//�������ȐԂɐݒ�B
		_CollisionModel->GetSkinModel()->SetAllBlend(Color(1.0f, 0.0f, 0.0f, 0.5f));
		//�����蔻��̑傫���𒲂ׂ�B

		//btVector3 min, max;
		//btTransform tr;
		//tr.setIdentity();
		//GetBody()->getAabb(tr, min, max);
		//Vector3 size = Vector3(max.x() - min.x(), max.y() - min.y(), max.z() - min.z());

		Vector3 size = _halfSize * 2.0f;


		//�����蔻��̃T�C�Y���w��
		//_CollisionModel->transform->SetLocalScale(size);
		_CollisionModel->transform->SetScale(size);

		//���ۂ̃R���W������e�ɐݒ�(�`��p���f�����R���W�����̍��W�n�ɂ���B)

		// �R���W������Transform��񐶐��B
		//_CollisionTr.reset(new Transform(nullptr,nullptr));
		
		UpdateTransform();

		////�R���W�����`��p���f���̒��S�_�ƃR���W�����̒��S�_�̍�����ݒ�B
		//_CollisionModel->transform->SetLocalPosition(_CollisionModelOffset);
	}
}


void Collider::UpdateTransform() {

	if (_CollisionModel) {
		//_CollisionModel->transform->SetParent(transform);

		// �ʒu���ݒ�B
		_CollisionModel->transform->SetLocalPosition(_collision->GetOffset());
	}
}


bool Collider::GetIsRender() {
	if (_CollisionModel) {
		return _CollisionModel->GetSkinModel()->GetEnable();
	}
	return false;
}

void Collider::RenderEnable() {
	if (_CollisionModel) {
		_CollisionModel->GetSkinModel()->SetEnable(true);
	}
}

void Collider::RenderDisable() {
	if (_CollisionModel) {
		_CollisionModel->GetSkinModel()->SetEnable(false);
	}
}
#endif