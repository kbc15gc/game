/**
* ���j���̃y�[�W�N���X�̎���.
*/
#include"stdafx.h"
#include"HistoryPage.h"

/**
* �R���X�g���N�^��̏�����.
*/
void HistoryPage::Awake()
{
	_Model = AddComponent<SkinModel>();

	SkinModelData* modelData = new SkinModelData();
	modelData->CloneModelData(SkinModelManager::LoadModel("Page.x"));

	_Model->SetModelData(modelData);
	_Model->SetModelEffect(ModelEffectE::CAST_SHADOW, false);

	//transform->SetLocalScale(Vector3(0.4f, 0.4f, 0.4f));

}

/**
* ������.
*/
void HistoryPage::Start()
{
}

/**
* �X�V.
*/
void HistoryPage::Update()
{
}
