#include "stdafx.h"
#include "../EnemyCharacter.h"
#include "EnemySpeakState.h"

EnemySpeakState::EnemySpeakState(EnemyCharacter* Object) : EnemyState(Object)
{
}


EnemySpeakState::~EnemySpeakState()
{
}

void EnemySpeakState::_EntrySubClass() {
	// �ҋ@���̃A�j���[�V�����Đ��B
	_EnemyObject->PlayAnimation_Loop(EnemyCharacter::AnimationType::Idle, 0.1f);
}

void EnemySpeakState::_Start() {
}

void EnemySpeakState::_UpdateSubClass() {
	if (_speakObject && _speakObject->GetIsSpeakEnd()) {
		// ��b�I���B

		_EndState();
	}
	else {
		// ���b�菈��
		// �������j�����ύX����Ă������悤�ɂƂ肠���������ɏ����B

		_speakObject = static_cast<NPC*>(INSTANCE(GameObjectManager)->FindObject(const_cast<char*>(_EnemyObject->GetFileName())));
		if (_speakObject) {
			const_cast<ComponentManager&>(_speakObject->GetComponentManager()).RemoveComponent<SkinModel>();
			const_cast<ComponentManager&>(_speakObject->GetComponentManager()).RemoveComponent<Animation>();
			_speakObject->transform->SetPosition(_EnemyObject->transform->GetPosition());
			_speakObject->transform->SetRotation(_EnemyObject->transform->GetRotation());
			_speakObject->transform->SetScale(_EnemyObject->transform->GetScale());
		}
	}
}

void EnemySpeakState::_ExitSubClass(EnemyCharacter::State next) {
	_speakObject->SetActive(false);
}

void EnemySpeakState::_EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType) {
}
