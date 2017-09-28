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
	void SetParam(char* name, char* iconName, int param, fbText::TextAnchorE paramAnchor = fbText::TextAnchorE::MiddleRight, float nameTextSize = 40.0f, const Vector2& iconSize = defaultIconSize)
	{
		_SetRenderStyle(ShowType::Normal,iconName, iconSize, name,nameTextSize,param,paramAnchor);
	}
	/**
	* �p�����[�^�ݒ�. 
	*/
	void SetParamMax(char* name,char* iconName, int param,int maxParam, fbText::TextAnchorE paramAnchor = fbText::TextAnchorE::MiddleRight, float nameTextSize = 40.0f, const Vector2& iconSize = defaultIconSize)
	{
		_SetRenderStyle(ShowType::Max, iconName, iconSize, name, nameTextSize, param, paramAnchor);
		_MaxParam = maxParam;
	}

	/**
	* �p�����[�^�ݒ�.
	*/
	void SetParamBuff(char* name, char* iconName, int param, int buff, fbText::TextAnchorE paramAnchor = fbText::TextAnchorE::MiddleRight, float nameTextSize = 40.0f, const Vector2& iconSize = defaultIconSize)
	{
		_SetRenderStyle(ShowType::Buff, iconName, iconSize, name, nameTextSize, param, paramAnchor);
		_ParamBuff = buff;
	}
	/**
	* �p�����[�^�ݒ�.
	*/
	void SetParamEquip(char* name, char* iconName, int param, int equip, int newEquip)
	{
		_SetRenderStyle(ShowType::Equip, iconName, defaultIconSize, name, defaultNameTextSize, param, fbText::TextAnchorE::MiddleRight);
		_ParamEquip = equip;
		_ParamNewEquip = newEquip;
	}
	/**
	* �p�����[�^�ݒ�.
	*/
	void SetParamRank(char* name, char* iconName, HoldEquipment::Rank rank, HoldEquipment::Rank newRank)
	{
		_SetRenderStyle(ShowType::Rank, iconName, defaultIconSize, name, defaultNameTextSize, 0, fbText::TextAnchorE::MiddleRight);
		_ParamRank = rank;
		_ParamNewRank = newRank;
	}


	// �p�����[�^�e�L�X�g�̈ʒu�ݒ�(���[�J�����W)�B
	inline void SetParamTextPos(const Vector3& localPos) {
		_ParamText->transform->SetLocalPosition(localPos);
	}
	// �p�����[�^�e�L�X�g�̈ʒu�擾�B
	inline const Vector3& GetParamTextPos()const {
		return _ParamText->transform->GetLocalPosition();
	}

	inline ImageObject* GetIconObject()const {
		return _IconImage;
	}

private:
	inline void _SetRenderStyle(ShowType type,char* iconName,  const Vector2& iconSize,char* nameText, float nameTextSize, int param, fbText::TextAnchorE paramAnchor) {
		_ShowType = type;
		if (_IconImage->GetTexture() == nullptr) {
			_IconImage->SetTexture(LOADTEXTURE(iconName));
		}
		_IconImage->SetSize(iconSize);
		_ParamName = nameText;
		_Param = param;
		_ParamText->SetAnchor(paramAnchor);
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

	const static float defaultNameTextSize;
	const static Vector2 defaultIconSize;
};