#include"stdafx.h"
#include "../EnemyCharacter.h"
#include "LastBossHistoryTamperingState.h"
#include "EnemyTranslationState.h"
#include "EnemyWaitState.h"
#include "GameObject\Enemy\LastBoss.h"
#include "GameObject\History\HistoryManager.h"
#include "GameObject\Village\ContinentObject.h"

LastBossHistoryTamperingState::LastBossHistoryTamperingState(EnemyCharacter* Object) : EnemyState(Object)
{
	_buildingsParent.reset(new Transform(nullptr,nullptr));
	_buildingsParent->SetPosition(_EnemyObject->transform->GetPosition() + Vector3(0.0f,0.0f,-10.0f));
	Quaternion rot;
	rot = Quaternion::Identity;
	rot.SetRotation(Vector3::axisX, D3DXToRadian(180.0f));
	_buildingsParent->SetRotation(Quaternion::Identity);
	 INSTANCE(HistoryManager)->CreateBuilding("Asset/Data/LastBossStageEquipment/LastDungeonObj.csv", _historyBuildings);
	 for (auto obj : _historyBuildings) {
		 // �f���o�������W���o�g���t�B�[���h�̍��W�n�ɕϊ��B
		 obj->transform->SetParent(_buildingsParent.get());
		 obj->transform->SetLocalPosition(obj->transform->GetPosition());
		 Quaternion work = obj->transform->GetRotation();
		 work.Multiply(rot);
		 obj->transform->SetLocalRotation(work);
	 }
}


LastBossHistoryTamperingState::~LastBossHistoryTamperingState()
{
}

void LastBossHistoryTamperingState::_EntrySubClass() {
	// ���G�����B
	
	// ���j�������B
}

void LastBossHistoryTamperingState::_StartSubClass() {
	// �̂�����ݒ�B
	_EnemyObject->ConfigDamageReaction(false);
}

void LastBossHistoryTamperingState::_UpdateSubClass() {
	// ��Ƀv���C���[�Ƌ������肵�A�o�g���͈͊O�ɏo���珉���X�e�[�g�ɖ߂��B

	// ���䑕�u�𔭓��������̂��A�ɑ�U�������B

	// �喂�@���������ƃX�e�[�g�I���B

	// �喂�@���L�����Z�������ƃ_�E���Ɉڍs�B
	_EnemyObject->ChangeStateRequest(static_cast<EnemyCharacter::State>(LastBoss::LastBossState::LastBossDown));
	
	// ���b�菈���B
	//_EndState();
}

void LastBossHistoryTamperingState::_ExitSubClass(EnemyCharacter::State next) {
	// ���j���������B

}

void LastBossHistoryTamperingState::_EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType) {
}
