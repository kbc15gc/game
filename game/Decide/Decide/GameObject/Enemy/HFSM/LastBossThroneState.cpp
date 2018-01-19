#include"stdafx.h"
#include "../EnemyCharacter.h"
#include "LastBossThroneState.h"
#include "EnemyTranslationState.h"
#include "EnemyWaitState.h"
#include "GameObject\Enemy\BossGhost.h"
#include "GameObject\Enemy\HFSM\GhostPairAttackState.h"
#include "GameObject\Enemy\HFSM\EnemyChaceState.h"
#include "GameObject\Player\Player.h"
#include "GameObject\Enemy\HFSM\EnemyBackStepState.h"
#include "GameObject\Enemy\LastBoss.h"
#include "GameObject\Enemy\HFSM\EnemyAttackState.h"

//const int LastBossThroneState::_entourageNum = 2;

// �e�X�g�B
const int LastBossThroneState::_entourageNum = 2;

LastBossThroneState::LastBossThroneState(EnemyCharacter* Object) : EnemyState(Object)
{
	_player = static_cast<Player*>(INSTANCE(GameObjectManager)->FindObject("Player"));
	_playAnimation = EnemyCharacter::AnimationType::Idle;
	_interpolate = 0.3f;
	_loopNum = -1;
}


LastBossThroneState::~LastBossThroneState()
{
}

bool LastBossThroneState::EntourageCommand() {
	if (_entourageEnemys.size() == _entourageNum) {
		// ���߂���̂������Ă��Ȃ��B
		for (auto enemy : _entourageEnemys) {
			enemy->SetIsCommand(true);
		}
		BossGhost::SelectPairAttack();
		return true;
	}
	return false;
}

void LastBossThroneState::EncourageBuff() {
	int value[CharacterParameter::Param::MAX];
	for (int idx = 0; idx < static_cast<int>(CharacterParameter::Param::MAX); idx++) {
		value[idx] = 0;
	}
	if (_entourageEnemys.size() == _entourageNum) {
		// ���߂���̂������Ă��Ȃ��B

		value[static_cast<int>(CharacterParameter::Param::ATK)] = 10;
		value[static_cast<int>(CharacterParameter::Param::MAT)] = 10;
		value[static_cast<int>(CharacterParameter::Param::DEF)] = 30;
		value[static_cast<int>(CharacterParameter::Param::MDE)] = 30;
	}
	else {
		value[static_cast<int>(CharacterParameter::Param::ATK)] = 50;
		value[static_cast<int>(CharacterParameter::Param::MAT)] = 50;
		value[static_cast<int>(CharacterParameter::Param::DEF)] = 50;
		value[static_cast<int>(CharacterParameter::Param::MDE)] = 50;
	}

	for (auto enemy : _entourageEnemys) {
		enemy->BuffAndDebuff(value,15.0f);
	}
}

void LastBossThroneState::_EntrySubClass() {

	// ���ߏ����B
	float dir = 1.0f;
	float startAttackOffset = 1.0f;	// �����U���J�n�^�C�~���O�����炷�����B
	for (int idx = 0; idx < _entourageNum; idx++) {
		_entourageEnemys.push_back(INSTANCE(GameObjectManager)->AddNew<BossGhost>("_entourageEnemy", 1));
		vector<BarColor> color;
		color.push_back(BarColor::Red);
		vector<int> param = vector<int>(CharacterParameter::Param::MAX,0);

		param[CharacterParameter::Param::HP] = 2500;
		param[CharacterParameter::Param::ATK] = 800;
		param[CharacterParameter::Param::DEF] = 200;
		param[CharacterParameter::Param::MAT] = 800;
		param[CharacterParameter::Param::MDE] = 200;
		if (idx == 0) {
			//_entourageEnemys[idx]->SetColor(Color::blue);
			param[CharacterParameter::Param::HP] += 7500;
			param[CharacterParameter::Param::DEF] += 200;
			param[CharacterParameter::Param::MDE] += 200;

		}
		else {
			_entourageEnemys[idx]->SetColor(Color::red);
			param[CharacterParameter::Param::ATK] += 350;
			param[CharacterParameter::Param::MAT] += 350;
		}
		param[CharacterParameter::Param::DEX] = 10;
		param[CharacterParameter::Param::CRT] = 10;

		_entourageEnemys[idx]->SetParamAll(color, param);
		Vector3 pos = _EnemyObject->transform->GetPosition() + (_EnemyObject->transform->GetForward() * 3.0f * dir);
		_entourageEnemys[idx]->transform->SetPosition(pos - Vector3(0.0f,0.5f,0.0f));
		_entourageEnemys[idx]->SetIntervalStartPairAttack(startAttackOffset);

		// ����̋����ݒ�B
		_entourageEnemys[idx]->SetViewRange(0.0f);

		// �p�j�͈͐ݒ�B
		// ���b�菈���B
		_entourageEnemys[idx]->SetWanderingRange(20.0f);

		// �ǐՔ͈͐ݒ�B
		_entourageEnemys[idx]->SetDiscoveryRange(30.0f);


		// ���߂ɐݒ�B
		_entourageEnemys[idx]->SetIsEntourage(true);

		startAttackOffset += 0.97f;
		if (idx >= 1) {
			// ���߂��̂Ƃ����������B

			// ���݂����y�A�Ƃ��ēo�^�B
			_entourageEnemys[idx]->SetPairGhost(_entourageEnemys[idx - 1]);
			_entourageEnemys[idx - 1]->SetPairGhost(_entourageEnemys[idx]);
		}
		dir *= -1.0f;
	}

	_isFirstDestroyEntourage = true;
	_isDeathEntourage = false;
	_timeCounter = 0.0f;

	_ChangeLocalState(EnemyCharacter::State::Attack);
	static_cast<EnemyAttackState*>(_NowLocalState)->SetAttack(static_cast<LastBoss*>(_EnemyObject)->GetEncourageBuffAttack());

	_EnemyObject->EnemyPlaySound(static_cast<EnemyCharacter::SoundIndex>(LastBoss::LastBossSoundIndex::Battle1));
}

void LastBossThroneState::_StartSubClass() {
	// �̂�����ݒ�B
	_EnemyObject->ConfigDamageReaction(false);
}

void LastBossThroneState::_UpdateSubClass() {

	// ��Ƀv���C���[�Ƌ������肵�A�o�g���͈͊O�ɏo���珉���X�e�[�g�ɖ߂��B
	if (_EnemyObject->IsOutsideDiscovery()) {
		_EnemyObject->ChangeStateRequest(_EnemyObject->GetInitState());
	}

	if (_isDeathEntourage) {
		if (!_EnemyObject->GetIsPlaying()) {
			// �ʍ�����������X�e�[�g�I���B

			_EnemyObject->EnemyStopSound(static_cast<EnemyCharacter::SoundIndex>(LastBoss::LastBossSoundIndex::Battle1));
			_EnemyObject->EnemyPlaySound(static_cast<EnemyCharacter::SoundIndex>(LastBoss::LastBossSoundIndex::Battle2));

			_EndState();
		}
	}
	else {
		_isDeathEntourage = true;
		// ���߂���̂Ƃ��|�����΃X�e�[�g�I���B
		int endWarpNum = 0;	// �U���J�n�̏������������Ă��鑤�߂̐��B
		for (auto itr = _entourageEnemys.begin(); itr != _entourageEnemys.end();) {
			if ((*itr)->GetNowState() && (*itr)->GetNowStateIndex() == EnemyCharacter::State::Death) {
				for (auto entourage : _entourageEnemys) {
					// ���݂��̃y�A�֌W�������B
					entourage->SetPairGhost(nullptr);
				}
				itr = _entourageEnemys.erase(itr);
			}
			else {
				_isDeathEntourage = false;
				itr++;
			}
		}

		if (_isDeathEntourage) {
			_EnemyObject->PlayAnimation_OriginIndex(static_cast<int>(LastBoss::AnimationLastBoss::ThroneEnd), 0.3f, 1, 0);
			return;
		}
		else {
			if (_isFirstDestroyEntourage) {
				if(_entourageEnemys.size() < _entourageNum){
					// ���߂đ��߂��������B
					_ChangeLocalState(EnemyCharacter::State::Attack);
					static_cast<EnemyAttackState*>(_NowLocalState)->SetAttack(static_cast<LastBoss*>(_EnemyObject)->GetSpecialAttack());

					_timeCounter = 0.0f;
					_isFirstDestroyEntourage = false;
				}
			}
		}

		_timeCounter += Time::DeltaTime();

		_EnemyObject->LookAtObject(_player);


		// ��莞�ԂƊm���Œ����ƃv���C���[�ւ̃f�o�t�A���߂ւ̃o�t�ƍU�����߂��s���B
		if (_timeCounter >= _interval) {
			_ChangeLocalState(EnemyCharacter::State::StartAttack);
			_timeCounter = 0.0f;
		}

		if (_NowLocalStateIdx == EnemyCharacter::State::None) {
			// �����A�N�V���������Ă��Ȃ��B

			// ��Ƀv���C���[�ƈ��̋��������B
			float minRange = 6.0f;	// 6���[�g���͍Œ�ł������B
			float maxRange = 7.0f;	// 7���[�g����艓����΋ߊ��B

			Vector3 enemyToPlayer = _player->transform->GetPosition() - _EnemyObject->transform->GetPosition();
			float length = enemyToPlayer.Length();
			if (length >= minRange && length <= maxRange) {
				// �v���C���[�Ƃ̋��������͈͓��B

				if (_NowLocalStateIdx == EnemyCharacter::State::BackStep) {
					_ChangeLocalState(EnemyCharacter::State::None);
					_PlayAnimation();
				}

			}
			else {
				// �v���C���[�Ƃ̋��������e�͈͊O�ɂȂ����B

				if (_NowLocalStateIdx != EnemyCharacter::State::BackStep) {
					_ChangeLocalState(EnemyCharacter::State::BackStep);
					static_cast<EnemyBackStepState*>(_NowLocalState)->SetAddPower(10.0f);
					static_cast<EnemyBackStepState*>(_NowLocalState)->SetMaxPower(5.0f);
				}

				if (_NowLocalStateIdx == EnemyCharacter::State::BackStep) {
					if (length >= maxRange) {
						// ���ꂷ���Ă���B

						// ���e�͈͂܂ŒǐՁB
						static_cast<EnemyBackStepState*>(_NowLocalState)->SetDirection(_EnemyObject->transform->GetForward());
					}
					else {
						// �������߂�����B

						// ���e�͈͂܂Ō�ށB
						static_cast<EnemyBackStepState*>(_NowLocalState)->SetDirection(_EnemyObject->transform->GetForward() * -1.0f);
					}
				}
			}
		}
	}
}

void LastBossThroneState::_ExitSubClass(EnemyCharacter::State next) {
	// ���߂��폜�B
	if (_entourageEnemys.size() > 0) {
		// �v���C���[�����X�{�X�Ƃ̐퓬�͈͊O�ɏo���B

		for (auto& enemy : _entourageEnemys) {
			INSTANCE(GameObjectManager)->AddRemoveList(enemy);
		}
	}
	_entourageEnemys.clear();

	// �ʍ��������B

}

void LastBossThroneState::_EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType) {
}
