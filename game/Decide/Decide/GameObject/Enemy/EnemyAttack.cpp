#include "stdafx.h"
#include "GameObject\Enemy\EnemyAttack.h"
#include "GameObject\Enemy\EnemyCharacter.h"
#include "fbEngine\_Object\_GameObject\ParticleEmitter.h"
#include "GameObject\Enemy\LaserBreath.h"

// EnemyAttack�B

void EnemyAttack::Init(float attackRange, int animType, float interpolate, int animLoopNum,int playEventNo) {
	_AttackRange = attackRange;
	_animType = animType;
	_interpolate = interpolate;
	_animLoopNum = animLoopNum;
	_playEventNo = playEventNo;
}

void EnemyAttack::Entry() {
	if (_animType >= 0) {
		// �Đ�����A�j���[�V�����ԍ����ݒ肳��Ă���B
		_enemyObject->PlayAnimation_OriginIndex(_animType, _interpolate, _animLoopNum, _playEventNo);
	}
}


// EnemySingleAttack�B

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

// EnemyBreathAttack�B
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
		// ���ߏ������B

		alpha -= 3.0f * Time::DeltaTime();
		if (alpha <= 0.0f) {
			// ���S�ɓ����ɂȂ����B

			_nowWarpState = WarpState::Materialization;		// �������̂Ŏ��͎��̉��B
			Transform* parent = _enemyObject->transform->GetParent();	// ���݂̃G�l�~�[�̐e��ۑ��B

			// �v���C���[�̋߂��Ɉړ��B
			_enemyObject->transform->SetParent(_player->transform);
			_enemyObject->transform->SetLocalPosition(Vector3(0.0f, 1.0f, 0.5f));
			_enemyObject->transform->SetParent(parent);

			_oneCombo->Entry();	// �U���J�n�B

			alpha = 0.0f;
		}
	}
	else {
		// ���̉��������B

		alpha += 3.0f * Time::DeltaTime();
		if (alpha >= 1.0f) {
			// ���S�Ɏ��̉������B

			alpha = 1.0f;
		}

		if (_oneCombo->Update()) {
			// �U�����I���B

			_comboCount++;

			if (_comboCount >= _attackNum) {
				// �U���񐔂��w��񐔂ɒB�����B

				return true;
			}
			_nowWarpState = WarpState::Through;		// �܂�������B
		}
	}

	_enemyObject->SetAlpha(alpha);

	return false;
}
