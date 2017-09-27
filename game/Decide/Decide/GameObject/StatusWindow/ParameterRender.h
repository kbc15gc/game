/**
* �p�����[�^�\���N���X�̒�`.
*/
#pragma once

#include"fbEngine\_Object\_GameObject\TextObject.h"
#include"fbEngine\_Object\_GameObject\ImageObject.h"
#include"GameObject\ItemManager\HoldItem\HoldEquipment.h"

/**
* �p�����[�^�\���N���X.
*/
class ParameterRender : public GameObject
{
public:

	/**
	* �\���^�C�v.
	*/
	enum ShowType
	{
		Normal,		//!< �ʏ�.
		Max,		//!< �ő�l�g�p.
		Buff,		//!< �o�t�g�p.
		Equip,		//!< �����g�p.
		Rank,		//!< �����N�g�p.
	};

public:

	/**
	* �R���X�g���N�^.
	*/
	ParameterRender(const char* name) :
		GameObject(name)
	{
	}

	/**
	* �f�X�g���N�^.
	*/
	~ParameterRender()
	{
	}

	/**
	* ������.
	*/
	void Awake()override;

	/**
	* �X�V.
	*/
	void Update()override;

	/**
	* �p�����[�^�ݒ�.
	*/
	void SetParam(char* name, char* iconName, int param)
	{
		_IconImage->SetTexture(LOADTEXTURE(iconName));
		_IconImage->SetSize(Vector2(30.0f, 30.0f));
		_ParamName = name;
		_Param = param;

		_ShowType = ShowType::Normal;
	}
	/**
	* �p�����[�^�ݒ�. 
	*/
	void SetParamMax(char* name,char* iconName, int param,int maxParam)
	{
		_IconImage->SetTexture(LOADTEXTURE(iconName));
		_IconImage->SetSize(Vector2(30.0f, 30.0f));
		_ParamName = name;
		_Param = param;
		_MaxParam = maxParam;

		_ShowType = ShowType::Max;
	}
	/**
	* �p�����[�^�ݒ�.
	*/
	void SetParamBuff(char* name, char* iconName, int param, int buff)
	{
		_IconImage->SetTexture(LOADTEXTURE(iconName));
		_IconImage->SetSize(Vector2(30.0f, 30.0f));
		_ParamName = name;
		_Param = param;
		_ParamBuff = buff;

		_ShowType = ShowType::Buff;
	}
	/**
	* �p�����[�^�ݒ�.
	*/
	void SetParamEquip(char* name, char* iconName, int param, int equip, int newEquip)
	{
		_IconImage->SetTexture(LOADTEXTURE(iconName));
		_IconImage->SetSize(Vector2(30.0f, 30.0f));
		_ParamName = name;
		_Param = param;

		_ParamEquip = equip;
		_ParamNewEquip = newEquip;

		_ShowType = ShowType::Equip;
	}
	/**
	* �p�����[�^�ݒ�.
	*/
	void SetParamRank(char* name, char* iconName, HoldEquipment::Rank rank, HoldEquipment::Rank newRank)
	{
		_IconImage->SetTexture(LOADTEXTURE(iconName));
		_IconImage->SetSize(Vector2(30.0f, 30.0f));
		_ParamName = name;

		_ParamRank = rank;
		_ParamNewRank = newRank;

		_ShowType = ShowType::Rank;
	}


	// �p�����[�^�e�L�X�g�̈ʒu�ݒ�(���[�J�����W)�B
	inline void SetParamTextPos(const Vector3& localPos) {
		_ParamText->transform->SetLocalPosition(localPos);
	}
	// �p�����[�^�e�L�X�g�̈ʒu�擾�B
	inline const Vector3& GetParamTextPos()const {
		return _ParamText->transform->GetLocalPosition();
	}
private:

	/** �p�����[�^���\��. */
	TextObject* _ParamNameText = nullptr;
	/** �p�����[�^��. */
	char* _ParamName = "���ݒ�";

	/** �p�����[�^�\��. */
	TextObject* _ParamText = nullptr;
	// �ő�p�����[�^�\���B
	TextObject* _MaxParamText = nullptr;
	/** �p�����[�^. */
	int _Param;
	/** �ő�p�����[�^. */
	int _MaxParam = INT_MIN;

	TextObject* _BuffText = nullptr;

	/** �p�����[�^�o�t�l(�}�C�i�X�Ȃ�f�o�t). */
	int _ParamBuff;

	/** ���ݑ���. */
	int _ParamEquip;
	/** ��⑕��. */
	int _ParamNewEquip;

	HoldEquipment::Rank _ParamRank;
	HoldEquipment::Rank _ParamNewRank;

	/** �A�C�R���摜. */
	ImageObject* _IconImage = nullptr;

	ShowType _ShowType = ShowType::Normal;

};