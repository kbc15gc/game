#pragma once
#include "ShopState.h"
#include "GameObject\ItemManager\HoldItem\HoldItemBase.h"
class ImageObject;
class TextObject;

//�V���b�v�̍w����ʂ̃N���X�B
class ShopS_Trade :public IShopState
{
public:
	//�R���X�g���N�^�B
	//[in] �V���b�v�̃|�C���^�B
	ShopS_Trade(Shop* shop);

	//�f�X�g���N�^�B
	~ShopS_Trade();

	//���t���[���Ă΂��X�V�����B
	void Update();

	//��Ԃɐ؂�ւ����Ƃ��Ă΂�鏈���B
	void EnterState();

	//���݂̏�Ԃ��甲���鎞�ɌĂ΂�鏈���B
	void ExitState();

	//�q�ɐ���ۂɌĂяo����鏈���B
	void DiveState();
private:
	//�^�u�̐؂�ւ��B
	void _SwitchTab();
	//�������ύX�B
	void _UpdateTradeNum();

	//���j���[�쐬�B
	void _CreateMenu();
	//���j���[�����B
	void _CloseMenu();

	//�\�����鍀�ڂ��X�V�B
	void _UpdateList();
	//�I�����Ă���Y������ݒ�B
	void _SetIndex(int idx);
	//
	void _SetMinIndex(int min);
	//�e�L�X�g�X�V�B
	void _UpdateText();
	//�A�C�e�����X�g���ړ�������B
	void _ScrollDisplayItem();

	//�A�C�e���̏����e�L�X�g�ɑ��M�B
	void _SendItemInfo(HoldItemBase* item);
	//�J���[�R�[�h���Z�o�B
	char* _CalcColorCode(int diff);
	
	//���菈���B
	void _Decision();
	//�A�C�e���̍w�������B
	void BuyItem();
	//�A�C�e���̔̔������B
	void SellItem();
private:
	//�v���C���[�ւ̎Q�ƁB
	Player* player;
	Player* GetPlayer()
	{
		if(player == nullptr)
		{
			player = (Player*)INSTANCE(GameObjectManager)->FindObject("Player");
		}
		return player;
	}
private:
	//�V���b�v�̃X�e�[�g�ێ��B
	Shop::ShopStateE _SaveState;

	//�\������A�C�e���̎�ށB
	int _DisplayType = static_cast<int>(Item::ItemCodeE::Item);
	//�\������A�C�e���̈ꗗ�B
	vector<HoldItemBase*> _DisplayList;
	//�A�C�e���̐��B
	int _DisplayItemNum = 0;

	//���ݑI�����Ă��鍀��
	int _Select = 0;
	//�\������͈́B
	int _MinIdx = 0;
	//�V���b�v�ɕ\�����鐔�B
	const int DISPLAY_ITEM_NUM = 7;
	//����������B
	vector<int> _TradeNum;
	//�I�����Ă���A�C�e���B
	Item::BaseInfo* _SelectItem;

	//�J�[�\���̉摜�B
	ImageObject* _Cursor;
	//�w���E�B���h�E�̉摜�B
	ImageObject* _TradeWindow;
	//�A�C�e���ꗗ�����o�������e�L�X�g�̃��X�g�B
	vector<TextObject*> _MenuTexts,_MoneyTexts;
	//���j���[�̈ꍀ�ڂ̏c���B
	float _MenuListHeight;
	//�p�����[�^�E�B���h�E�B
	ImageObject* _ParmWindow;
	//�p�����[�^
	TextObject* _ParmText;
};