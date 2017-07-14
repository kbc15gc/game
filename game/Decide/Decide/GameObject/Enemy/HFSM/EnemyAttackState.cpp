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
	if (_attack) {
		// 攻撃処理が設定されている。

		_attack->Start();	// 初期化。
		if (_attack->GetAnimationType() >= 0) {
			// 再生するアニメーション番号が設定されている。
			if (_attack->GetAnimationLoopNum() < 0) {
				// 無限ループ再生。
			}
			else if(_attack->GetAnimationLoopNum() > 0){
				// 指定回数ループ再生。
				_EnemyObject->PlayAnimation_OriginIndex(_attack->GetAnimationType(), _attack->GetInterpolate(), _attack->GetAnimationLoopNum());
			}
			else {
				// 再生回数に0が設定された。
				abort();
			}
		}
	}
	else {
		// 攻撃処理設定されてない。
		abort();
	}
}

void EnemyAttackState::_UpdateSubClass() {
	_attack->SetIsPlaying(_EnemyObject->GetIsPlaying());	// アニメーションの更新状況を通知。

	// 更新処理(戻り値は終了したか)。
	if (_attack->Update()) {
		// 更新処理終了。
		_EndState();
	}
}

void EnemyAttackState::Exit(EnemyCharacter::State next) {
	_attack->Exit();
}

void EnemyAttackState::_EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType) {
}
