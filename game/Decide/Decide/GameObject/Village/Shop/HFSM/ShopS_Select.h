#pragma once
#include "ShopState.h"

class ImageObject;
class TextObject;

//�V���b�v�̑I����ʂ̃N���X�B
class ShopS_Select:public IShopState
{
public:
	//�R���X�g���N�^�B
	//[in] �V���b�v�̃|�C���^�B
	ShopS_Select(Shop* shop);

	//�f�X�g���N�^�B
	~ShopS_Select() {}

	//���t���[���Ă΂��X�V�����B
	void Update();

	//��Ԃɐ؂�ւ����Ƃ��Ă΂�鏈���B
	void EnterState();

	//���݂̏�Ԃ��甲���鎞�ɌĂ΂�鏈���B
	void ExitState();

	//�q�ɐ���ۂɌĂяo����鏈���B
	void DiveState();
private:
	//�I�����Ă���ꏊ�B
	int select = 0;
	//�J�[�\���̉摜�B
	ImageObject* _Cursor;
	//�I���E�B���h�E�̉摜�B
	ImageObject* _SelectWindow;
};