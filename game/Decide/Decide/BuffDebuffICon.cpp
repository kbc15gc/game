#include "stdafx.h"
#include "BuffDebuffICon.h"

//初期化。
void BuffDebuffICon::Awake() {

	//矢印アイコン。
	_ArrowIconImage = INSTANCE(GameObjectManager)->AddNew<ImageObject>("ArrowIconImage", 9);
	
	//武器か防具のアイコン。
	_BuffDebuffTypeIconImage = INSTANCE(GameObjectManager)->AddNew<ImageObject>("BuffDebuffTypeIconImage", 9);
}
