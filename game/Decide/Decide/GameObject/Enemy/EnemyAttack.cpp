#include "stdafx.h"
#include "GameObject\Enemy\EnemyAttack.h"
#include "GameObject\Enemy\EnemyCharacter.h"
#include "fbEngine\_Object\_GameObject\ParticleEmitter.h"
#include "GameObject\Enemy\LaserBreath.h"

// EnemyAttack。

void EnemyAttack::Init(float attackRange, int animType, float interpolate, int animLoopNum,int playEventNo) {
	_AttackRange = attackRange;
	_animType = animType;
	_interpolate = interpolate;
	_animLoopNum = animLoopNum;
	_playEventNo = playEventNo;
}

void EnemyAttack::Entry() {
	if (_animType >= 0) {
		// 再生するアニメーション番号が設定されている。
		_enemyObject->PlayAnimation_OriginIndex(_animType, _interpolate, _animLoopNum, _playEventNo);
	}
}


// EnemySingleAttack。

EnemySingleAttack::EnemySingleAttack(EnemyCharacter* object) :EnemyAttack(object) {
	_player = INSTANCE(GameObjectManager)->FindObject("Player");
}
void EnemySingleAttack::Entry(){
	EnemyAttack::Entry();
	_enemyObject->LookAtObject(_player);
};

bool EnemySingleAttack::Update() {
	return !_enemyObject->GetIsPlaying();
}

// EnemyBreathAttack。
EnemyBreathAttack::EnemyBreathAttack(EnemyCharacter* object) :EnemyAttack(object) {
	_player = INSTANCE(GameObjectManager)->FindObject("Player");
}

void EnemyBreathAttack::Entry(){
	EnemyAttack::Entry();
	_enemyObject->LookAtObject(_player);
}

bool EnemyBreathAttack::Update() {
	return !_enemyObject->GetIsPlaying();
}


void GhostComboAttack::Entry() {
	_oneCombo->Init(0.0f, _animType, _interpolate, 1, _playEventNo);
	_comboCount = 0;
	_nowWarpState = WarpState::Through;
}

bool GhostComboAttack::Update() {

	float alpha = _enemyObject->GetAlpha();

	if (_nowWarpState == WarpState::Through) {
		// 透過処理中。

		alpha -= 3.0f * Time::DeltaTime();
		if (alpha <= 0.0f) {
			// 完全に透明になった。

			_nowWarpState = WarpState::Materialization;		// 消えたので次は実体化。
			Transform* parent = _enemyObject->transform->GetParent();	// 現在のエネミーの親を保存。

			// プレイヤーの近くに移動。
			_enemyObject->transform->SetParent(_player->transform);
			_enemyObject->transform->SetLocalPosition(Vector3(0.0f, 1.0f, 0.5f));
			_enemyObject->transform->SetParent(parent);

			_oneCombo->Entry();	// 攻撃開始。

			alpha = 0.0f;
		}
	}
	else {
		// 実体化処理中。

		alpha += 3.0f * Time::DeltaTime();
		if (alpha >= 1.0f) {
			// 完全に実体化した。

			alpha = 1.0f;
		}

		if (_oneCombo->Update()) {
			// 攻撃一回終了。

			_comboCount++;

			if (_comboCount >= _attackNum) {
				// 攻撃回数が指定回数に達した。

				return true;
			}
			_nowWarpState = WarpState::Through;		// また消える。
		}
	}

	_enemyObject->SetAlpha(alpha);

	return false;
}
