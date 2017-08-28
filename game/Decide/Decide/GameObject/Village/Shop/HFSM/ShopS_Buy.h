#pragma once
#include "ShopState.h"

class ImageObject;
class TextObject;

//�V���b�v�̍w����ʂ̃N���X�B
class ShopS_Buy :public IShopState
{
public:
	//�R���X�g���N�^�B
	//[in] �V���b�v�̃|�C���^�B
	ShopS_Buy(Shop* shop);

	//�f�X�g���N�^�B
	~ShopS_Buy() {}

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
	//�J�[�\�����ړ�������
	//[in] ���j���[�̉��ԖڂɈړ����邩�H
	void MoveMenuCursor(int idx);
private:
	//���ݑI�����Ă��鍀��
	int idx = 0;
	//�J�[�\���̉摜�B
	ImageObject* _Cursor;
	//�w���E�B���h�E�̉摜�B
	ImageObject* _BuyWindow;
	//���i�����o�������e�L�X�g�̃��X�g�B
	vector<TextObject*> _MenuTexts;
	//���j���[�̈ꍀ�ڂ̏c���B
	float _MenuListHeight;
};