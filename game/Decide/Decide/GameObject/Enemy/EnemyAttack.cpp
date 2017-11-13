#include "stdafx.h"
#include "GameObject\Enemy\EnemyAttack.h"
#include "GameObject\Enemy\EnemyCharacter.h"
#include "fbEngine\_Object\_GameObject\ParticleEmitter.h"
#include "GameObject\Enemy\LaserBreath.h"

// EnemyAttack。

void EnemyAttack::Init(float attackRange, int animType, float interpolate, float playSpeed, int animLoopNum,int playEventNo) {
	_AttackRange = attackRange;
	_animType = animType;
	_interpolate = interpolate;
	_playSpeed = playSpeed;
	_animLoopNum = animLoopNum;
	_playEventNo = playEventNo;
}

void EnemyAttack::Entry() {
	if (_animType >= 0) {
		// 再生するアニメーション番号が設定されている。
		float saveSpeed = _enemyObject->GetAnimationSpeed();
		_enemyObject->SetAnimationSpeed(_playSpeed);
		_enemyObject->PlayAnimation_OriginIndex(_animType, _interpolate, _animLoopNum, _playEventNo);
		_enemyObject->SetAnimationSpeed(saveSpeed);
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
	//_oneCombo->Init(1.0f, _animType, _interpolate, 1, _playEventNo);
	_comboCount = 0;
	_nowWarpState = WarpState::Through;
	_isWarpEnd = false;
	_isAttackEnd = false;
}

bool GhostComboAttack::Update() {

	bool ret = false;

	float alpha = _enemyObject->GetAlpha();

	if (_nowWarpState == WarpState::Through) {
		// 透過処理中。

		alpha -= 1.0f * Time::DeltaTime();
		if (alpha <= 0.0f) {
			// 完全に透明になった。

			_nowWarpState = WarpState::Materialization;		// 消えたので次は実体化。
			//Transform* parent = _enemyObject->transform->GetParent();	// 現在のエネミーの親を保存。

			// プレイヤーの近くに移動。
			_enemyObject->transform->SetParent(_player->transform);
			_enemyObject->transform->SetLocalPosition(Vector3(0.0f, 1.0f, -0.7f));
			_enemyObject->transform->SetParent(/*parent*/nullptr);

			_oneCombo->Entry();	// 攻撃開始。

			_counter = 0.0f;
			alpha = 0.0f;
		}
	}
	else {
		// 実体化処理中。

		alpha += 1.0f * Time::DeltaTime();
		if (alpha >= 1.0f) {
			// 完全に実体化した。

			alpha = 1.0f;
			_isWarpEnd = true;
		}

		if (_counter < _chaceTime) {
			// 追跡時間を超えていない。

			// プレイヤーにステップ回避が実装されると踏んでぎりぎりまで追尾。
			// ※回避実装されなかったらこの下の位置行は消してね。
			_enemyObject->LookAtObject(_player);

			Vector3 moveDir, work;
			moveDir = work = _player->GetOffsetPos() - _enemyObject->transform->GetPosition();
			work = Vector3(0.0f, moveDir.y, 0.0f);	// Y軸の移動はXZ軸の距離に関係なく行う。

			moveDir.y = 0.0f;

			float speed = _player->GetSpeed();

			if (moveDir.Length() > _oneCombo->GetAttackRange()) {
				// 攻撃範囲内に入っていない。
				moveDir.Normalize();
				_enemyObject->AddMoveSpeed(moveDir * speed);
			}
			if (work.Length() > 0.0001f) {
				// 目的位置にいない。
				work.Normalize();
				speed = 5.0f;
				_enemyObject->AddMoveSpeed(work * speed);
			}

			_counter += Time::DeltaTime();
		}

		if (!_isAttackEnd) {
			if (_oneCombo->Update()) {
				// 攻撃一回終了。

				_isAttackEnd = true;
			}
		}

		if(_isAttackEnd && _isWarpEnd) {
			// 一回攻撃終了。

			_comboCount++;
			_nowWarpState = WarpState::Through;		// また消える。
			_isWarpEnd = false;
			if (_comboCount >= _attackNum) {
				// 攻撃回数が指定回数に達した。

				ret = true;
			}
		}
	}
	_enemyObject->SetAlpha(alpha);

	return ret;
}
