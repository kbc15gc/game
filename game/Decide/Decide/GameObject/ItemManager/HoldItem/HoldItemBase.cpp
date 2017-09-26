#include "stdafx.h"
#include "HoldItemBase.h"

HoldItemBase::HoldItemBase(char* name):ImageObject(name)
{
}

//デストラクタ。
HoldItemBase::~HoldItemBase()
{

}

void HoldItemBase::Start() {
	LOADTEXTURE(_Info->filePath);
}