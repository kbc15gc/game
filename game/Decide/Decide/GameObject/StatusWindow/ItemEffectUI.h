/**
* �A�C�e������UI�N���X�̒�`.
*/
#pragma once

#include"fbEngine\_Object\_GameObject\ImageObject.h"
#include"fbEngine\_Object\_GameObject\TextObject.h"
#include"GameObject\ItemManager\HoldItem\ConsumptionItem.h"

/**
* �A�C�e������UI�N���X.
*/
class ItemEffectUI : public GameObject
{
public:

	/**
	* �A�C�e���̌��ʃp�����[�^�R�[�h.
	*/
	enum class IEParamCode
	{
		Hp,		//!< �̗�.
		Atk,	//!< �U����.
		Mat,	//!< ���@�U����.
		Def,	//!< �h���.
		Mde,	//!< ���@�h���.
		Dex,	//!< �N���e�B�J����.
		Crt,	//!< �N���e�B�J���{��.
		Count,	//!< ��.
	};

public:

	/**
	* �R���X�g���N�^.
	*/
	ItemEffectUI(const char* name) :
		GameObject(name)
	{
	}

	/**
	* �f�X�g���N�^.
	*/
	~ItemEffectUI()
	{
	}

	/**
	* ������.
	*/
	void Awake()override;

	/**
	* �A�C�e���f�[�^��ݒ�.
	*/
	void SetItemData(ConsumptionItem* itemData);

private:

	/** �p�����[�^��. */
	TextObject* _Param[(int)IEParamCode::Count] = { nullptr };

};