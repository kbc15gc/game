#include "stdafx.h"
#include "GameObject\Enemy\EnemyAttack.h"
#include "GameObject\Enemy\EnemyCharacter.h"
#include "fbEngine\_Object\_GameObject\ParticleEmitter.h"
#include "GameObject\Enemy\LaserBreath.h"

// EnemyAttack�B

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
		// �Đ�����A�j���[�V�����ԍ����ݒ肳��Ă���B
		float saveSpeed = _enemyObject->GetAnimationSpeed();
		_enemyObject->SetAnimationSpeed(_playSpeed);
		_enemyObject->PlayAnimation_OriginIndex(_animType, _interpolate, _animLoopNum, _playEventNo);
		_enemyObject->SetAnimationSpeed(saveSpeed);
	}
}


// EnemySingleAttack�B

EnemySingleAttack::EnemySingleAttack(EnemyCharacter* object) :EnemyAttack(object) {
	_player = INSTANCE(GameObjectManager)->FindObject("Player");
}
void EnemySingleAttack::Entry(){
	EnemyAttack::Entry();
	//_enemyObject->LookAtObject(_player);
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
	//_enemyObject->LookAtObject(_player);
}

bool EnemyBreathAttack::Update() {
	return !_enemyObject->GetIsPlaying();
}

void EnemyBreathAttack::BreathStart(BreathObject* obj) {
	if (warptest) {
		OutputDebugString("warptest\n");
	}
	_breath = obj;
	_breath->SetActive(true);
	_breath->BreathStart();
}

void EnemyWarpAttack::Entry() {
	//_oneCombo->Init(1.0f, _animType, _interpolate, 1, _playEventNo);
	_nowWarpState = WarpState::Through;
	_isWarpEnd = false;
	_isAttackEnd = false;
}

bool EnemyWarpAttack::Update() {

	bool ret = false;

	float alpha = _enemyObject->GetAlpha();

	if (_nowWarpState == WarpState::Through) {
		// ���ߏ������B

		alpha -= 1.0f * Time::DeltaTime();
		if (alpha <= 0.0f) {
			// ���S�ɓ����ɂȂ����B

			_nowWarpState = WarpState::Materialization;		// �������̂Ŏ��͎��̉��B
															//Transform* parent = _enemyObject->transform->GetParent();	// ���݂̃G�l�~�[�̐e��ۑ��B

			// �v���C���[�̋߂��Ɉړ��B
			_enemyObject->transform->SetParent(_player->transform);
			_enemyObject->transform->SetLocalPosition(Vector3(0.0f, 1.0f, -0.7f));
			_enemyObject->transform->SetParent(/*parent*/nullptr);

			_enemyObject->LookAtObject(_player);

			// �S�[�X�g���m���d�Ȃ�Ȃ��悤�ɂ���B
			_enemyObject->AddMoveSpeed(_enemyObject->transform->GetForward() * -0.1f);

			_attack->Entry();	// �U���J�n�B

			_counter = 0.0f;
			alpha = 0.0f;
		}
	}
	else {
		// ���̉��������B

		alpha += 1.0f * Time::DeltaTime();
		if (alpha >= 1.0f) {
			// ���S�Ɏ��̉������B

			alpha = 1.0f;
			_isWarpEnd = true;
		}

		if (_counter < _chaceTime) {
			// �ǐՎ��Ԃ𒴂��Ă��Ȃ��B

			// �v���C���[�ɃX�e�b�v��������������Ɠ���ł��肬��܂Œǔ��B
			// �������������Ȃ������炱�̉��̈ʒu�s�͏����ĂˁB
			_enemyObject->LookAtObject(_player);

			Vector3 moveDir, work;
			moveDir = work = _player->GetOffsetPos() - _enemyObject->transform->GetPosition();
			work = Vector3(0.0f, moveDir.y, 0.0f);	// Y���̈ړ���XZ���̋����Ɋ֌W�Ȃ��s���B

			moveDir.y = 0.0f;

			float speed = _player->GetSpeed();

			if (moveDir.Length() > _attack->GetAttackRange()) {
				// �U���͈͓��ɓ����Ă��Ȃ��B
				moveDir.Normalize();
				_enemyObject->AddMoveSpeed(moveDir * speed);
			}
			if (work.Length() > 0.0001f) {
				// �ړI�ʒu�ɂ��Ȃ��B
				work.Normalize();
				speed = 5.0f;
				_enemyObject->AddMoveSpeed(work * speed);
			}

			_counter += Time::DeltaTime();
		}

		if (!_isAttackEnd) {
			if (_attack->Update()) {
				// �U���I���B

				if (_attack->warptest) {
					OutputDebugString("warpLaser\n");
				}
				_attack->Exit();
				_isAttackEnd = true;
			}
		}

		if (_isAttackEnd && _isWarpEnd) {
			// �U���Ǝ��̉��I���B

			ret = true;
		}
	}
	_enemyObject->SetAlpha(alpha);

	return ret;
}

void EnemyComboAttack::Entry() {
	//_oneCombo->Init(1.0f, _animType, _interpolate, 1, _playEventNo);
	_comboCount = 0;
	_isStartAttack = false;
}

bool EnemyComboAttack::Update() {

	if (!_isStartAttack) {
		_oneCombo->Entry();
		_isStartAttack = true;
	}
	if (_isStartAttack) {
		if (_oneCombo->Update()) {
			// �U�����I���B

			_oneCombo->Exit();

			_comboCount++;
			if (_comboCount >= _attackNum) {
				// �U���񐔂��w��񐔂ɒB�����B

				return true;
			}
			else {
				// ������x�U������B
				_isStartAttack = false;
			}
		}
	}
	return false;
}
