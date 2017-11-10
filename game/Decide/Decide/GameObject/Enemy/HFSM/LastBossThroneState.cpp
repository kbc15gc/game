#include"stdafx.h"
#include "../EnemyCharacter.h"
#include "LastBossThroneState.h"
#include "EnemyTranslationState.h"
#include "EnemyWaitState.h"
#include "GameObject\Enemy\BossGhost.h"
#include "GameObject\Enemy\HFSM\GhostPairAttackState.h"

//const int LastBossThroneState::_entourageNum = 2;

// �e�X�g�B
const int LastBossThroneState::_entourageNum = 2;

LastBossThroneState::LastBossThroneState(EnemyCharacter* Object) : EnemyState(Object)
{
}


LastBossThroneState::~LastBossThroneState()
{
}

void LastBossThroneState::_EntrySubClass() {
	_EnemyObject->PlayAnimation_Loop(EnemyCharacter::AnimationType::Idle, 0.1f);

	// ���ߏ����B
	float dir = 1.0f;
	float startAttackOffset = 1.0f;	// �����U���J�n�^�C�~���O�����炷�����B
	for (int idx = 0; idx < _entourageNum; idx++) {
		_entourageEnemys.push_back(INSTANCE(GameObjectManager)->AddNew<BossGhost>("_entourageEnemy", 1));
		vector<BarColor> color;
		color.push_back(BarColor::Red);
		vector<int> param = vector<int>(CharacterParameter::Param::MAX,0);
		param[static_cast<int>(CharacterParameter::Param::HP)] = _EnemyObject->GetMaxHP() / 4;
		param[static_cast<int>(CharacterParameter::Param::MP)] = _EnemyObject->GetMaxMP()/ 4;

		for (int i = CharacterParameter::Param::ATK; i < CharacterParameter::Param::MAX;i++) {
			param[i] = _EnemyObject->GetPigmentParam(static_cast<CharacterParameter::Param>(i)) / 4;
		}

		// �e�X�g�B
		//param[static_cast<int>(CharacterParameter::Param::HP)] = 10;

		_entourageEnemys[idx]->SetParamAll(color, param);
		_entourageEnemys[idx]->transform->SetPosition(_EnemyObject->transform->GetPosition() + (_EnemyObject->transform->GetForward() * 3.0f * dir));
		_entourageEnemys[idx]->SetIntervalStartPairAttack(startAttackOffset);
		startAttackOffset += 0.97f;
		if (idx >= 1) {
			// ���߂��̂Ƃ����������B

			// ���݂����y�A�Ƃ��ēo�^�B
			_entourageEnemys[idx]->SetPairGhost(_entourageEnemys[idx - 1]);
			_entourageEnemys[idx - 1]->SetPairGhost(_entourageEnemys[idx]);
		}
		dir *= -1.0f;
	}

	_isCommand = false;
	_timeCounter = 0.0f;
}

void LastBossThroneState::_Start() {
	// �̂�����ݒ�B
	_EnemyObject->ConfigDamageReaction(false);
}

void LastBossThroneState::_UpdateSubClass() {
	// ��Ƀv���C���[�Ƌ������肵�A�o�g���͈͊O�ɏo���珉���X�e�[�g�ɖ߂��B
	if (_EnemyObject->IsOutsideDiscovery()) {
		_EnemyObject->ChangeStateRequest(_EnemyObject->GetInitState());
	}

	// ���߂���̂Ƃ��|�����΃X�e�[�g�I���B
	bool isEnd = true;	
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
			isEnd = false;
			itr++;
		}
	}

	_timeCounter += Time::DeltaTime();

	// ��莞�ԂƊm���Œ����ƃv���C���[�ւ̃f�o�t�A���߂ւ̃o�t�ƍU�����߂��s���B
	if (_timeCounter >= _interval) {
		//int rnd = rand() % 2;
		//if (rnd == 0) {
		_isCommand = true;
		//}
		_timeCounter = 0.0f;
	}

	if (_isCommand) {
		if (_entourageEnemys.size() == _entourageNum) {
			// ���߂���̂������Ă��Ȃ��B
			for (auto enemy : _entourageEnemys) {
				enemy->SetIsCommand(true);
			}
			BossGhost::SelectPairAttack();
			_isCommand = false;
		}
	}

	if (isEnd) {
		_EndState();
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
