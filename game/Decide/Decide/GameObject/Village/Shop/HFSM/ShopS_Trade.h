#pragma once
#include "ShopState.h"

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
	~ShopS_Trade() {}

	//���t���[���Ă΂��X�V�����B
	void Update();

	//��Ԃɐ؂�ւ����Ƃ��Ă΂�鏈���B
	void EnterState();

	//���݂̏�Ԃ��甲���鎞�ɌĂ΂�鏈���B
	void ExitState();

	//�q�ɐ���ۂɌĂяo����鏈���B
	void DiveState();
private:
	void _CreateMenu();
	void _CloseMenu();
	//�I�����Ă���Y������ݒ�B
	void SetIndex(int idx);
	//
	void SetMinIndex(int min);
	//�A�C�e�����X�g�̕\���X�V�B
	void UpdateDisplayItem();
	
	//�����̊֐��B
	function<void()> TradeFunc;

	//���菈���B
	void Decision();
	//�A�C�e���̍w�������B
	//[in] �A�C�e���̏��B
	void BuyItem(Item::BaseInfo*);
	//�A�C�e���̔̔������B
	//[in] �A�C�e���̏��B
	void SellItem(Item::BaseInfo*);
private:
	//���ݑI�����Ă��鍀��
	int _Select = 0;
	//�\������͈́B
	int _MinIdx = 0;
	//�V���b�v�ɕ\�����鐔�B
	const int DISPLAY_ITEM_NUM = 7;

	//�\������A�C�e���̈ꗗ
	vector<Item::BaseInfo*> _ItemList;

	//�J�[�\���̉摜�B
	ImageObject* _Cursor;
	//�w���E�B���h�E�̉摜�B
	ImageObject* _BuyWindow;
	//�A�C�e���ꗗ�����o�������e�L�X�g�̃��X�g�B
	vector<TextObject*> _MenuTexts;
	//���j���[�̈ꍀ�ڂ̏c���B
	float _MenuListHeight;
};