#pragma once
#include "GameObject\Village\ContinentObject.h"

namespace
{
	//�I�u�W�F�N�g�̏��
	struct NPCInfo : public ObjectInfo
	{
		int MesseageID;	//���b�Z�[�W��ID
		bool ShowTitle;	//�^�C�g���������邩�ǂ����H
	};

	//�����o�ϐ��̏��ݒ�
	const Support::DATARECORD NPCInfoData[6] =
	{
		{ "filename",Support::DataTypeE::STRING, offsetof(struct NPCInfo,filename),sizeof(char) * 256 },
		{ "pos",Support::DataTypeE::VECTOR3, offsetof(struct NPCInfo,pos),sizeof(Vector3) },
		{ "ang",Support::DataTypeE::VECTOR3, offsetof(struct NPCInfo,ang),sizeof(Vector3) },
		{ "sca",Support::DataTypeE::VECTOR3, offsetof(struct NPCInfo,sca),sizeof(Vector3) },
		{ "MesseageID",Support::DataTypeE::INT, offsetof(struct NPCInfo,MesseageID),sizeof(int) },
		{ "ShowTitle",Support::DataTypeE::INT, offsetof(struct NPCInfo,ShowTitle),sizeof(bool) },
	};
}

class TextBox;
class Player;

//��b�̂ł���NPC
class NPC :public ContinentObject
{
public:
	NPC(const char* name);
	~NPC();
	void Awake()override;
	void Update()override;
	void SetMesseage(const int& id,const bool show);
	void LateUpdate()override;
private:
	//NPC�̐g��(���f���̃T�C�Y���v�Z���Ă�������������Ȃ��B)
	float _Height;
	//�e�L�X�g�{�b�N�X
	TextBox* _TextBox;
	//�v���C���[�̃A�h���X
	Player* _Player;
	//�v���C���[�Ɖ�b�\�ȋ���
	float _Radius;
};