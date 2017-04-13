#include "../EnemyCharacter.h"
#include "EnemyTranslationState.h"


EnemyTranslationState::EnemyTranslationState(EnemyCharacter* Object) : EnemyState(Object)
{
}


EnemyTranslationState::~EnemyTranslationState()
{
}

void EnemyTranslationState::_EntrySubClass() {
}

void EnemyTranslationState::_Start() {
	float InterpolateTime = 0.1f;	// ��Ԏ��ԁB
	_EnemyObject->PlayAnimation_Loop(EnemyCharacter::AnimationType::Walk, InterpolateTime);
}

void EnemyTranslationState::_UpdateSubClass() {
	OutputDebugString(_T("�g�����X�X�e�[�g�Ȃ��B\n"));
}

void EnemyTranslationState::Exit(EnemyCharacter::State next) {

}

void EnemyTranslationState::_EndNowLocalState_CallBack(EnemyCharacter::State now) {
	// ���݂̃��[�J���X�e�[�g�̏������I�������B

}