/**
* �A�C�e���\����ʃN���X�̒�`.
*/
#pragma once

#include"fbEngine\_Object\_GameObject\ImageObject.h"
#include"fbEngine\_Object\_GameObject\TextObject.h"
#include"GameObject\ItemManager\ItemManager.h"
#include"Item2D.h"
#include"GameObject\Player\Player.h"
#include"ParameterRender.h"
#include"HoldItem2D.h"
#include "GameObject\StatusWindow\Cursor.h"

#include"Dialog.h"

/**
* �A�C�e���\����ʃN���X.
*/
class ItemWindow : public GameObject
{
public:

	enum class IconIndex {
		LV = 0,		//!< ���x��.
		HP,			//!< �̗�.
		//MP,			//!< ����.
		ATK,		//!< �����U��.
		//MAT,		//!< ���@�U��.
		DEF,		//!< �����h��.
		MDE,		//!< ���@�h��.
		DEX,		//!< ��p�x.
		CRT,		// �N���e�B�J���З́B
		RANK,		// �����N�B
		MAX			//!< ��.
	};

	/**
	* ����A�C�e���\���X�e�[�^�X.
	*/
	enum class CIShowStatus
	{ 
		LV = 0,		//!< ���x��.
		HP,			//!< �̗�.
		//MP,			//!< ����.
		ATK,		//!< �����U��.
		//MAT,		//!< ���@�U��.
		DEF,		//!< �����h��.
		MDE,		//!< ���@�h��.
		DEX,		//!< ��p�x.
		MAX			//!< ��.
	};

	/**
	* ����A�C�e���\���X�e�[�^�X.
	*/
	enum class WIShowStatus
	{
		RANK,		//!< �����N.
		ATK,		//!< �����U��.
		//MAT,		//!< ���@�U��.
		CRT,		//!< �N���e�B�J����.
		DEX,		//!< ��p�x.
		MAX			//!< ��.
	};

	/**
	* �h��A�C�e���\���X�e�[�^�X.
	*/
	enum class AIShowStatus
	{
		RANK,		//!< �����N.
		DEF,		//!< �����h��.
		MDE,		//!< ���@�h��.
		CRT,		//!< �N���e�B�J����.
		DEX,		//!< ��p�x.
		MAX			//!< ��.
	};

public:

	/**
	* �R���X�g���N�^.
	*/
	ItemWindow(char* name): 
		GameObject(name)
	{
	}

	/**
	* �f�X�g���N�^.
	*/
	~ItemWindow()
	{
	}

	/**
	* ������.
	*/
	void Awake()override;

	/**
	* �����ŌĂԏ�����.
	*/
	void Init(Item::ItemCodeE code);

	/**
	* �X�V.
	*/
	void LateUpdate()override;

	///**
	//* �A�N�e�B�u.
	//*/
	//void OnEnable()override
	//{
	//	if (INSTANCE(Inventory)->GetInventoryListSize() > 0)
	//	{
	//		Update();
	//	}
	//}

	/**
	* ��A�N�e�B�u.
	*/
	void OnDisable()override;

	/**
	* �A�N�e�B�u.
	*/
	void OnEnable()override;

private:

	/**
	* ����A�C�e���̏�����.
	*/
	void ItemInit();

	/**
	* ����̏�����.
	*/
	void WeaponInit();
	
	/**
	* �h��̏�����.
	*/
	void ArmorInit();

	/**
	* ����.
	*/
	void Input();

	void ArrowUpdate();

private:

	/**
	* ����A�C�e���̕\���X�e�[�^�X�̍쐬.
	*/
	void _CreateCIShowStatus();

	/**
	* ����A�C�e���̕\���X�e�[�^�X�̍쐬.
	*/
	void _CreateWIShowStatus();

	/**
	* �h��A�C�e���̕\���X�e�[�^�X�̍쐬.
	*/
	void _CreateAIShowStatus();

	/**
	* �p�����[�^�\���N���X�̃C���X�^���X�ɒl��ݒ�.
	*/
	void _ConfigParamRender();
private:

	Player* _Player = nullptr;
	// �v���C���[�̃��x���B
	int _playerLevel = 0;

	/** �Z���T�C�Y. */
	static const int ItemCellSize = 8;

	ImageObject* _ItemBackWindow = nullptr;

	/** �E�B���h�E���\��. */
	TextObject* _WindowName = nullptr;

	/** �A�C�e���`�惊�X�g. */
	vector<Item2D*> _Item2DList;

	/** �A�C�e���R�[�h. */
	Item::ItemCodeE _ItemCode;

	/** �O��̃A�C�e����. */
	int _BefItemCount = 0;
	/** ���ݑI�𒆂̃A�C�e��. */
	int _NowSelectItem = 0;
	/** ���X�g�̏��߂̓Y����. */
	int _StartLoadCount = 0;
	/** �Z���N�g�J�[�\��. */
	//ImageObject* _SelectCursor = nullptr;
	/** �Z���N�g�J�[�\��. */
	Cursor* _Cursor = nullptr;

	// �����Ă���A�C�e���g�𒴂��ăA�C�e�����܂����݂��邩�������A�C�R���B
	ImageObject* _UpArrow = nullptr;
	ImageObject* _DownArrow = nullptr;

	/** E�A�C�R��. */
	ImageObject* _EIconImage = nullptr;

	// �Q�[�W�B
	ParameterBar* _ExpBar = nullptr;
	ParameterBar* _HpBar = nullptr;
	//ParameterBar* _MpBar = nullptr;

	/** �p�����[�^�[���X�g. */
	vector<ParameterRender*> _ParameterRenderList;

	/** �����A�C�e�����X�g. */
	//vector<HoldItem2D*> _HoldItem2DList;

	/** �_�C�A���O. */
	Dialog* _Dialog = nullptr;

	static const char* IconTextureNameList[static_cast<int>(IconIndex::MAX)];
};