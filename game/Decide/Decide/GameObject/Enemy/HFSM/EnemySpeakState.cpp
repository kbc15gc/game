#include "stdafx.h"
#include "../EnemyCharacter.h"
#include "EnemySpeakState.h"

EnemySpeakState::EnemySpeakState(EnemyCharacter* Object) : EnemyState(Object)
{
	_playAnimation = EnemyCharacter::AnimationType::Idle;
	_interpolate = 0.2f;
	_loopNum = -1;
}


EnemySpeakState::~EnemySpeakState()
{
}

void EnemySpeakState::_EntrySubClass() {
}

void EnemySpeakState::_StartSubClass() {
}

void EnemySpeakState::_UpdateSubClass() {
	if (_speakObject && _speakObject->GetIsSpeakEnd()) {
		// 会話終了。

		_EndState();
	}
	else {
		// ※暫定処理
		// ※いつ歴史書が変更されてもいいようにとりあえずここに書く。

		_speakObject = static_cast<NPC*>(INSTANCE(GameObjectManager)->FindObject(const_cast<char*>(_EnemyObject->GetFileName())));
		if (_speakObject) {
			_speakObject->SetActive(true);
			const_cast<ComponentManager&>(_speakObject->GetComponentManager()).RemoveComponent<SkinModel>();
			const_cast<ComponentManager&>(_speakObject->GetComponentManager()).RemoveComponent<Animation>();
			const_cast<ComponentManager&>(_speakObject->GetComponentManager()).RemoveComponent<RigidBody>();

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
