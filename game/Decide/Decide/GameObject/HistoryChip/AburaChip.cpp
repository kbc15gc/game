#include"stdafx.h"
#include "AburaChip.h"
#include "fbEngine\_Object\_Component\_3D\SkinModel.h"
#include "fbEngine\_Object\_Component\_3D\Light.h"
#include "GameObject\Player\Player.h"
#include "GameObject\Village\HistoryManager.h"
#include "GameObject\Village\HistoryInfo.h"
#include "GameObject\Village\HistoryMenuSelect.h"

AburaChip::AburaChip(const char * name) :
	GameObject(name)
{

}

void AburaChip::Awake()
{
	SkinModel* model = AddComponent<SkinModel>();
	SkinModelData* modeldata = new SkinModelData();
	modeldata->CloneModelData(SkinModelManager::LoadModel("AburaChip.X"));
	model->SetModelData(modeldata);
	model->SetModelEffect(ModelEffectE::CAST_SHADOW, false);

	transform->SetLocalPosition(Vector3(10.0f, 6.0f, 0.0f));
	transform->SetLocalScale(Vector3::one);
}

void AburaChip::Start()
{
	//�v���C���[������
	_Player = (Player*)INSTANCE(GameObjectManager)->FindObject("Player");
	//�q�X�g���[���j���[�Z���N�g����
	_HistoryMenuSelect = (HistoryMenuSelect*)INSTANCE(GameObjectManager)->FindObject("HistoryMenuSelect");
}

void AburaChip::Update()
{
	static Vector3 angle(0.0f, 0.0f, 0.0f);
	angle.y += 2.0f;
	transform->SetLocalAngle(angle);

	//�v���C���[�Ƃ̋���
	float toLenght = fabs(transform->GetLocalPosition().Length() - _Player->transform->GetLocalPosition().Length());
	//���̋��������ƃI�u�W�F�N�g�폜
	if (toLenght <= 0.2f)
	{
		INSTANCE(HistoryManager)->SetHistoryChip(0, 0, (int)ChipID::FIRE);
		_HistoryMenuSelect->AburaSelect();
		INSTANCE(GameObjectManager)->AddRemoveList(this);
	}

}