#pragma once
#include "ShopState.h"

class ImageObject;
class TextObject;

//�V���b�v�̊m�F��ʂ̃N���X�B
class ShopS_Confirmation :public IShopState
{
public:
	//�R���X�g���N�^�B
	//[in] �V���b�v�̃|�C���^�B
	ShopS_Confirmation(Shop* shop);

	//�f�X�g���N�^�B
	~ShopS_Confirmation() {}

	//���t���[���Ă΂��X�V�����B
	void Update();

	//��Ԃɐ؂�ւ����Ƃ��Ă΂�鏈���B
	void EnterState();

	//���݂̏�Ԃ��甲���鎞�ɌĂ΂�鏈���B
	void ExitState();

	//�q�ɐ���ۂɌĂяo����鏈���B
	void DiveState();
private:
	//�J�[�\���̉摜�B
	ImageObject* _Cursor;
	//�m�F�E�B���h�E�̉摜�B
	ImageObject* _ConfirmationWindow;

	//�����̑O�̃X�e�[�g��ێ��B
	Shop::ShopStateE _BeforeState;
};