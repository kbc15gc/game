/**
* �e�X�g�I�u�W�F�N�g�N���X�̎���.
*/
#include"stdafx.h"
#include"TestObject.h"

/**
* ������.
*/
void TestObject::Awake()
{
	SkinModel* model = AddComponent<SkinModel>();

	//�X�L�����f���쐬
	SkinModelData* modeldata = new SkinModelData();
	//���f���f�[�^�쐬
	modeldata->CloneModelData(SkinModelManager::LoadModel("maru.X"));
	//���f���ݒ�
	model->SetModelData(modeldata);
	model->SetModelEffect(ModelEffectE::SPECULAR, true);

	transform->SetLocalPosition(Vector3(-202.0f, 60.0f, -156.0f));
}
