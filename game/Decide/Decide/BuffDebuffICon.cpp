#include "stdafx.h"
#include "BuffDebuffICon.h"

//�������B
void BuffDebuffICon::Awake() {

	//���A�C�R���B
	_ArrowIconImage = INSTANCE(GameObjectManager)->AddNew<ImageObject>("ArrowIconImage", 9);
	
	//���킩�h��̃A�C�R���B
	_BuffDebuffTypeIconImage = INSTANCE(GameObjectManager)->AddNew<ImageObject>("BuffDebuffTypeIconImage", 9);
}
