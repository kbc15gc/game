#include "stdafx.h"
#include "HoldItemBase.h"

HoldItemBase::HoldItemBase(char* name):ImageObject(name)
{
}

//�f�X�g���N�^�B
HoldItemBase::~HoldItemBase()
{

}

void HoldItemBase::Start() {
	LOADTEXTURE(_Info->filePath);
}