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
	// 待機中のアニメーション再生。
	_EnemyObject->PlayAnimation_Loop(EnemyCharacter::AnimationType::Idle, 0.1f);
}

void EnemySpeakState::_Start() {
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
