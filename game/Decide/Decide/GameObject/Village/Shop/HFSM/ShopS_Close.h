#pragma once
#include "ShopState.h"

//�V���b�v�̔̔���ʂ̃N���X�B
class ShopS_Close :public IShopState
{
public:
	//�R���X�g���N�^�B
	//[in] �V���b�v�̃|�C���^�B
	ShopS_Close(Shop* shop) :IShopState(shop) {}

	//�f�X�g���N�^�B
	~ShopS_Close() {}

	//���t���[���Ă΂��X�V�����B
	void Update();

	//��Ԃɐ؂�ւ����Ƃ��Ă΂�鏈���B
	void EnterState();

	//���݂̏�Ԃ��甲���鎞�ɌĂ΂�鏈���B
	void ExitState();

	//�q�ɐ���ۂɌĂяo����鏈���B
	void DiveState();
};