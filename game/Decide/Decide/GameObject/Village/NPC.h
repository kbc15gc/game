#pragma once
#include "GameObject\Village\ContinentObject.h"

namespace npc
{
	//NPC�̃^�C�v
	enum NPCTypeE : int
	{
		VILLAGER,   //���ʂ̑��l
		SHOP,       //�X��
	};
	//�I�u�W�F�N�g�̏��
	struct NPCInfo : public ObjectInfo
	{
	public:
		NPCTypeE NPCType = NPCTypeE::VILLAGER;
		//�X��ID
		int ShopID = -1;
		int MesseageID;	//���b�Z�[�W��ID
		bool ShowTitle;	//�^�C�g���������邩�ǂ����H
	};

	//�����o�ϐ��̏��ݒ�
	const Support::DATARECORD NPCInfoData[] =
	{
		{ "filename",Support::DataTypeE::STRING, offsetof(struct NPCInfo,filename),sizeof(char) * 256 },
		{ "pos",Support::DataTypeE::VECTOR3, offsetof(struct NPCInfo,pos),sizeof(Vector3) },
		{ "ang",Support::DataTypeE::QUATERNION, offsetof(struct NPCInfo,ang),sizeof(Quaternion) },
		{ "sca",Support::DataTypeE::VECTOR3, offsetof(struct NPCInfo,sca),sizeof(Vector3) },
		{ "NPCType",Support::DataTypeE::INT, offsetof(struct NPCInfo,NPCType),sizeof(NPCTypeE) },
		{ "ShopID",Support::DataTypeE::INT, offsetof(struct NPCInfo,ShopID),sizeof(int) },
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

	enum class State
	{
		None = -1,
		Idol,
		Speak,
		Num,
	};

	NPC(const char* name);
	~NPC();
	void Awake()override;
	void Update()override;
	void LateUpdate()override;

	void CreateNPC(const npc::NPCInfo* info);

	void SetMesseage(const int& id, const bool show);

	void SetIsSpeak(bool flag)
	{
		_IsSpeak = flag;
	}

	// ��b�I�����擾�B
	bool GetIsSpeakEnd()const;

	float GetRadius()
	{
		return _Radius;
	}

	//�����o���̍�����ݒ�
	void SetHeight(float height)
	{
		_Height = height;
	}

	//�A�j���[�V�����𗬂����B
	void SetAnimation(bool flag)
	{
		_IsAnimation = flag;
	}

protected:
	//�b��
	void _Speak();
protected:
	//NPC�̐g��(���f���̃T�C�Y���v�Z���Ă�������������Ȃ��B)
	float _Height;
	//�e�L�X�g�{�b�N�X
	TextBox* _TextBox;
	//�v���C���[�̃A�h���X
	Player* _Player;
	//�v���C���[�Ɖ�b�\�ȋ���
	float _Radius;
	//�^�C�g���\�����邩�ǂ����H
	bool _ShowTitle;

	//Player�Ƙb�����ł��邩
	bool _IsSpeak;
private:
	State _State;
	bool _IsAnimation = true;

};