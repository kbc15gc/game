#include"stdafx.h"
#include "../EnemyCharacter.h"
#include "EnemyAttackState.h"

EnemyAttackState::EnemyAttackState(EnemyCharacter* Object) : EnemyState(Object)
{
}


EnemyAttackState::~EnemyAttackState()
{
}

void EnemyAttackState::_EntrySubClass() {

}

void EnemyAttackState::_Start() {
	_EnemyObject->PlayAnimation(EnemyCharacter::AnimationType::Attack,0.2f,1);
}

void EnemyAttackState::_UpdateSubClass() {
	// 攻撃判定用のコリジョン生成。
	// ※暫定処理。
	// ※エネミーは複数種類いる可能性があるため、モーションによってコリジョン発生位置やタイミングを変える必要がある。
	const int eventFrame = 30;
	Vector3 pos = _EnemyObject->transform->GetPosition();
	pos += _EnemyObject->transform->GetForward() * 1.5f;
	pos.y += 0.5f;
	_EnemyObject->CreateAttackCollision(
		eventFrame,
		pos,
		Vector3::zero,
		Vector3::one,
		0.25f);

	if (!_EnemyObject->GetIsPlaying()) {
		// 攻撃モーション一度終了。
		_EndState();
	}
}

void EnemyAttackState::Exit(EnemyCharacter::State next) {
}

void EnemyAttackState::_EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType) {
}
