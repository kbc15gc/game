#pragma once
#include "stdafx.h"
#include "GameObject\Village\Shop\Shop.h"

//�V���b�v�̏�Ԃ������N���X�̃C���^�[�t�F�[�X�B
class IShopState
{
public:	
	//�R���X�g���N�^�B
	//[in] �V���b�v�̃|�C���^�B
	//[in] �e�̃X�e�[�g�B
	IShopState(Shop* shop) :
		_Shop(shop),
		_Caller(Shop::ShopStateE::None){}
	
	//�f�X�g���N�^�B
	virtual ~IShopState(){}
	
	//���t���[���Ă΂��X�V�����B
	virtual void Update() = 0;

	//��Ԃɐ؂�ւ����Ƃ��Ă΂�鏈���B
	void Enter(Shop::ShopStateE call)
	{
		if (_Caller == Shop::ShopStateE::None)
			_Caller = call;
		EnterState();
	}

	virtual void EnterState() = 0;
	
	//��Ԃ��甲���鎞�ɌĂ΂�鏈���B
	virtual void Exit(Shop::ShopStateE next)
	{
		if (next == _Caller)
		{
			ExitState();
			_Caller = Shop::ShopStateE::None;
		}
		else
		{
			DiveState();
		}
	}

	//
	virtual void ExitState() = 0;

	//
	virtual void DiveState() = 0;
protected:
	//�V���b�v�ւ̃|�C���^�B
	Shop* _Shop;
	//�Ăяo�����B
	Shop::ShopStateE _Caller;
};