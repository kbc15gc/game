#include"stdafx.h"
#include "Chip.h"
#include "fbEngine\_Object\_Component\_3D\SkinModel.h"
#include "fbEngine\_Object\_Component\_3D\Light.h"
#include "GameObject\Player\Player.h"
#include "HistoryManager.h"

namespace
{
	//�e�R�C���̃|�W�V����
	Vector3 pos[] =
	{
		Vector3(-481.0f, 66.0f, 266.0f),		//��
		Vector3(-453.0f, 58.0f, -295.0f),		//��
		Vector3(-1060.0f, 69.0f, -1950.0f),				//��
		Vector3(-125.0f,54.2f,443.0f),//!< ���̃`�b�v.
		Vector3(-351.0f,65.0f,649.0f),//!< �_�Ƃ̃`�b�v
		Vector3(275.0f,54.0f,621.0f),//!< ���̃`�b�v.
		Vector3(249.36f,65.0f,121.88f),//!< �S�̃`�b�v.
		Vector3(410.43f,90.0f,214.52),//!< �Ζ��̃`�b�v.
		//!< ��̃`�b�v.
		//!< �`�b�v�̐�.
	};

	//�����蔻��̋����B
	const float atari = 1.5f;
	//��]�̃X�s�[�h
	const float rotation_speed = 2.0f;

}

/**
* �R���X�g���N�^��̏�����.
*/
void Chip::Awake()
{
	//�`�b�v���擾����SE��������
	_SE = INSTANCE(GameObjectManager)->AddNew<SoundSource>("SE", 0);
	_SE->Init("Asset/Sound/coin.wav");
	//�v���C���[������
	_Player = (Player*)INSTANCE(GameObjectManager)->FindObject("Player");
}

/**
* ������.
*/
void Chip::Start()
{
	
}

/**
* �X�V.
*/
void Chip::Update()
{
	static Vector3 rotation = Vector3::zero;
	rotation.y += rotation_speed;
	transform->SetLocalAngle(rotation);

	//�v���C���[�Ƃ̋���
	float toLenght = (transform->GetLocalPosition() - _Player->transform->GetLocalPosition()).Length();
	//���̋��������ƃI�u�W�F�N�g�폜
	if (toLenght <= atari)
	{
		//�`�b�v�擾SE
		_SE->Play(false);

		//�������X�g�ɒǉ�.
		INSTANCE(HistoryManager)->AddPossessionChip(_ChipID);

		//�擾�������������Ȃ��̂ō폜
		INSTANCE(GameObjectManager)->AddRemoveList(this);
	}
}

void Chip::Render()
{
	TEXTURE* texture = LOADTEXTURE((char*)ChipFileName[(int)_ChipID].c_str());
	_Material->SetTexture(Material::TextureHandleE::DiffuseMap, texture->pTexture);
}

/**
* �`�b�vID��ݒ�.
*/
void Chip::SetChipID(ChipID chipID)
{ 
	//�O������Z�b�g����ID��ݒ�B
	_ChipID = chipID;

	//�ݒ肳�ꂽID�̃��f�������[�h�B
	_Model = AddComponent<SkinModel>();
	SkinModelData* modelData = new SkinModelData();
	modelData->CloneModelData(SkinModelManager::LoadModel("Page.x"));
	_Material = modelData->FindMaterial("HuntingPage.png");
	_Model->SetModelData(modelData);
	_Model->SetModelEffect(ModelEffectE::CAST_SHADOW, true);
	//model->SetModelEffect(ModelEffectE::SPECULAR, true);
	//model->SetAllBlend(Color::white * 13);
	//�ݒ肳�ꂽID�̃��f���̈ʒu�Ƒ傫����ݒ�B
	transform->SetLocalPosition(pos[(int)_ChipID]);
	transform->SetLocalScale(Vector3::one);

}

void Chip::SetDropChipID(ChipID chipID,const Vector3& pos)

{
	//�O������Z�b�g����ID��ݒ�B
	_ChipID = chipID;

	//�ݒ肳�ꂽID�̃��f�������[�h�B
	_Model = AddComponent<SkinModel>();
	SkinModelData* modelData = new SkinModelData();
	modelData->CloneModelData(SkinModelManager::LoadModel("Page.x"));
	_Material = modelData->FindMaterial("HuntingPage.png");
	_Model->SetModelData(modelData);
	_Model->SetModelEffect(ModelEffectE::CAST_SHADOW, true);
	//model->SetModelEffect(ModelEffectE::SPECULAR, true);
	//model->SetAllBlend(Color::white * 13);
	//�ݒ肳�ꂽID�̃��f���̈ʒu�Ƒ傫����ݒ�B
	transform->SetLocalPosition(pos);
	transform->SetLocalScale(Vector3::one);
}
