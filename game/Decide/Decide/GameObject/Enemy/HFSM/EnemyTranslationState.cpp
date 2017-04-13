#include "../EnemyCharacter.h"
#include "EnemyTranslationState.h"


EnemyTranslationState::EnemyTranslationState(EnemyCharacter* Object) : EnemyState(Object)
{
}


EnemyTranslationState::~EnemyTranslationState()
{
}

void EnemyTranslationState::Entry() {
	float InterpolateTime = 0.1f;	// ��Ԏ��ԁB
	_EnemyObject->PlayAnimation_Loop(EnemyCharacter::AnimationType::Walk, InterpolateTime);
}

void EnemyTranslationState::Exit(EnemyCharacter::State next) {

}

void EnemyTranslationState::_UpdateSubClass() {

}

void EnemyTranslationState::_EndNowLocalState_CallBack(EnemyCharacter::State now) {
	// ���݂̃��[�J���X�e�[�g�̏������I�������B

}