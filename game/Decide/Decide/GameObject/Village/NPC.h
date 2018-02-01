#pragma once
#include "GameObject\Village\ContinentObject.h"
#include"GameObject\Component\ObjectRotation.h"

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
		int EventNo = -1;//�C�x���g�̔ԍ��B
		int MesseageID;	//���b�Z�[�W��ID�B
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
		{ "EventNo",Support::DataTypeE::INT, offsetof(struct NPCInfo,EventNo),sizeof(int) },
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

	/**
	* �A�j���[�V�����R�[�h.
	*/
	enum class AnimationCodeE
	{
		Idol,	//!< �ҋ@.
		Speak,	//!< �b��.
		AnimationCount,
	};

	NPC(const char* name);
	~NPC();
	void Awake()override;

	/**
	* ������.
	*/
	void Start()override;

	void Update()override;
	void LateUpdate()override;

	void CreateNPC(const npc::NPCInfo* info);

	void SetMesseage(const int& id, const bool show);

	void SetIsSpeak(bool flag)
	{
		_IsSpeak = flag;
	}

	// ��b�I�����擾(���f�����ꍇ���܂�)�B
	bool GetIsSpeakEnd()const;

	// ��b�I�����擾(�b���Ō�܂ŕ����I������ꍇ�̂�)�B
	bool GetisSpeakEndLastMessage()const;

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

	//������]�ݒ�
	void SetRotation(Quaternion q)
	{
		_Rot = q;
	}

	void SetActive(const bool act, const bool children = false)override;

private:

	/**
	* ������n�ʂɍ��킹��.
	*/
	void _FitGround();

protected:
	//�b��
	void _Speak();
	//�A�j���[�V�����Đ�
	void PlayAnimation(AnimationCodeE code, const float interpolateTime, const int lnum = -1)
	{
		_Anim->PlayAnimation((int)code, interpolateTime, lnum);
	}
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

	//��]
	ObjectRotation* _Rotation = nullptr;
	Quaternion _Rot;

private:
	//�A�j���[�V����
	State _State;
	bool _IsAnimation = true;
};