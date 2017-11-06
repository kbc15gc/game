#include"stdafx.h"
#include "../EnemyCharacter.h"
#include "LastBossThroneState.h"
#include "EnemyTranslationState.h"
#include "EnemyWaitState.h"
#include "GameObject\Enemy\BossGhost.h"

//const int LastBossThroneState::_entourageNum = 2;

// �e�X�g�B
const int LastBossThroneState::_entourageNum = 1;

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
		dir *= -1.0f;
	}
}

void LastBossThroneState::_Start() {
	// �̂�����ݒ�B
	_EnemyObject->ConfigDamageReaction(false);
}

void LastBossThroneState::_UpdateSubClass() {
	// ��Ƀv���C���[�Ƌ������肵�A�o�g���͈͊O�ɏo���珉���X�e�[�g�ɖ߂��B

	// ��莞�ԂƊm���Œ����ƃv���C���[�ւ̃f�o�t���s���B

	// ���߂���̂Ƃ��|�����΃X�e�[�g�I���B
	bool isEnd = true;

	for (auto itr = _entourageEnemys.begin(); itr != _entourageEnemys.end();) {
		if ((*itr)->GetNowState() && (*itr)->GetNowStateIndex() == EnemyCharacter::State::Death) {
			itr = _entourageEnemys.erase(itr);
		}
		else {
			isEnd = false;
			itr++;
		}
	}

	if (isEnd) {
		_EndState();
	}
}

void LastBossThroneState::_ExitSubClass(EnemyCharacter::State next) {
	// ���߂��폜�B
	_entourageEnemys.clear();

	// �ʍ��������B

}

void LastBossThroneState::_EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType) {
}
