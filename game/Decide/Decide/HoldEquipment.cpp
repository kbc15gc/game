#include "stdafx.h"
#include "HoldEquipment.h"

//�R���X�g���N�^�B
HoldEquipment::HoldEquipment(Item::BaseInfo* info) :HoldItemBase(info)
{
	//���l�ŕ���A�h��̃p�����[�^�[���쐬�B
	CreateOriginParam();
}


//�f�X�g���N�^�B
HoldEquipment::~HoldEquipment()
{

}
