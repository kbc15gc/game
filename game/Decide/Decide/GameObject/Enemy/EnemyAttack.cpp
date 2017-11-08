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
	_oneCombo->Init(1.0f, _animType, _interpolate, 1, _playEventNo);
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
			//Transform* parent = _enemyObject->transform->GetParent();	// ���݂̃G�l�~�[�̐e��ۑ��B

			// �v���C���[�̋߂��Ɉړ��B
			_enemyObject->transform->SetParent(_player->transform);
			_enemyObject->transform->SetLocalPosition(Vector3(0.0f, 1.0f, -0.7f));
			_enemyObject->transform->SetParent(/*parent*/nullptr);

			_oneCombo->Entry();	// �U���J�n�B

			_counter = 0.0f;
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

		if(_counter < _chaceTime) {
			// �ǐՎ��Ԃ𒴂��Ă��Ȃ��B

			// �v���C���[�ɃX�e�b�v��������������Ɠ���ł��肬��܂Œǔ��B
			// �������������Ȃ������炱�̉��̈ʒu�s�͏����ĂˁB
			_enemyObject->LookAtObject(_player);

			Vector3 moveDir, work;
			moveDir = work = _player->GetOffsetPos() - _enemyObject->transform->GetPosition();
			work = Vector3(0.0f, moveDir.y, 0.0f);	// Y���̈ړ���XZ���̋����Ɋ֌W�Ȃ��s���B

			moveDir.y = 0.0f;

			float speed = _player->GetSpeed();

			if (moveDir.Length() > _oneCombo->GetAttackRange()) {
				// �U���͈͓��ɓ����Ă��Ȃ��B
				moveDir.Normalize();
				_enemyObject->AddMoveSpeed(moveDir * speed);
			}
			if (work.Length() > 0.0001f) {
				// �ړI�ʒu�ɂ��Ȃ��B
				work.Normalize();
				speed = 10.0f;
				_enemyObject->AddMoveSpeed(work * speed);
			}

			_counter += Time::DeltaTime();
		}

		if (_oneCombo->Update()) {
			// �U�����I���B

			_comboCount++;

			if (_comboCount >= _attackNum) {
				// �U���񐔂��w��񐔂ɒB�����B

				_nowWarpState = WarpState::Through;
				return true;
			}
			_nowWarpState = WarpState::Through;		// �܂�������B
		}
	}
	_enemyObject->SetAlpha(alpha);

	return false;
}
