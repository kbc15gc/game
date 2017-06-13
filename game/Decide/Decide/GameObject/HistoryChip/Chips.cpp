#include"stdafx.h"
#include "Chips.h"
#include "fbEngine\_Object\_Component\_3D\SkinModel.h"
#include "fbEngine\_Object\_Component\_3D\Light.h"
#include "GameObject\Player\Player.h"
#include "GameObject\Village\HistoryManager.h"

#include "GameObject\Village\HistoryMenuSelect.h"


namespace
{
	//�e�R�C���̃t�@�C���l�[��
	char* filename[] = {
		"",//0�Ԗڂ͂Ȃ����ߋ�̏���
		"FireChip.X",
		"TetuChip.X",
		"AburaChip.X"
	};

	//�e�R�C���̃|�W�V����
	Vector3 pos[] =
	{
		Vector3(0,0,0),
		Vector3(344, 69, -1255),
		Vector3(385, 69, -1275),
		Vector3(356, 69, -1245),
	};

	//�����蔻��̋����B
	const float atari = 1.2f;
	//��]�̃X�s�[�h
	const float rotation_speed = 2.0f;
}

Chips::Chips(const char * name) :
	GameObject(name)
{

}

void Chips::Awake()
{
	//�`�b�v���擾����SE��������
	_SE = INSTANCE(GameObjectManager)->AddNew<SoundSource>("SE", 0);
	_SE->Init("Asset/Sound/coin.wav");
}

void Chips::Start()
{
	//�v���C���[������
	_Player = (Player*)INSTANCE(GameObjectManager)->FindObject("Player");
	//�q�X�g���[���j���[�Z���N�g����
	_HistoryMenuSelect = (HistoryMenuSelect*)INSTANCE(GameObjectManager)->FindObject("HistoryMenuSelect");
}

void Chips::Update()
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

		//�擾�����`�b�v�����j���[�ɃZ�b�g
		INSTANCE(HistoryManager)->SetHistoryChip(0, 0, (int)_ChipID);
		_HistoryMenuSelect->ChipSelect(_ChipID);
		//�擾�������������Ȃ��̂ō폜
		INSTANCE(GameObjectManager)->AddRemoveList(this);
	}
}

void Chips::SetChipID(const ChipID & id)
{ 
	//�O������Z�b�g����ID��ݒ�B
	_ChipID = id;

	//�ݒ肳�ꂽID�̃��f�������[�h�B
	SkinModel* model = AddComponent<SkinModel>();
	SkinModelData* modeldata = new SkinModelData();
	modeldata->CloneModelData(SkinModelManager::LoadModel(filename[(int)_ChipID]));
	model->SetModelData(modeldata);
	model->SetModelEffect(ModelEffectE::CAST_SHADOW, true);

	//�ݒ肳�ꂽID�̃��f���̈ʒu�Ƒ傫����ݒ�B
	transform->SetLocalPosition(pos[(int)_ChipID]);
	transform->SetLocalScale(Vector3::one);
}
