#include "../EnemyCharacter.h"
#include "EnemyDiscoveryState.h"
#include "EnemyTranslationState.h"
#include "EnemyWaitState.h"

EnemyDiscoveryState::EnemyDiscoveryState(EnemyCharacter* Object) : EnemyState(Object)
{
}


EnemyDiscoveryState::~EnemyDiscoveryState()
{
}

void EnemyDiscoveryState::_EntrySubClass() {
	_Player = GameObjectManager::FindObject("Player");
	_Speed = 5.0f;	// �ړ����x��ݒ�B
}

void EnemyDiscoveryState::_Start() {
	// �A�j���[�V�����Đ��B
	_EnemyObject->PlayAnimation_Loop(EnemyCharacter::AnimationType::Dash,0.2f);
}

void EnemyDiscoveryState::_UpdateSubClass() {
	// �b�菈��
	// �����L��if���������ɂ̓G�l�~�[�̍s���͈͊O�ɍs�������̔��������B
	if (true) {
		// �s���͈͓��B

		Vector3 EnemyToPlayer = _Player->transform->GetPosition() - _EnemyObject->transform->GetPosition();
		if (EnemyToPlayer.Length() <= _EnemyObject->GetAttackRange()) {
			// �U���͈͂ɓ������B

			// �X�e�[�g�I�������B
			_EndState();
			return;
		}
		EnemyToPlayer.Normalize();
		Vector3 moveSpeeed = EnemyToPlayer * _Speed;

		// �Z�o�����ړ����G�l�~�[�ɉ��Z�B
		_EnemyObject->AddMoveSpeed(moveSpeeed);
	}
	else {
		// �s���͈͂���O�ꂽ�B

		// ���b�菈���B
		// �Ƃ肠���������ʒu�܂Ŗ߂�B
		_EnemyObject->ChangeStateRequest(EnemyCharacter::State::Translation);
		// �p�����[�^�ݒ�B
		Vector3 NowToInitVec = _EnemyObject->GetInitPos() - _EnemyObject->transform->GetPosition();
		static_cast<EnemyTranslationState*>(_EnemyObject->GetNowState())->SetLength(NowToInitVec.Length());
		NowToInitVec.Normalize();
		static_cast<EnemyTranslationState*>(_EnemyObject->GetNowState())->SetDir(NowToInitVec);
		static_cast<EnemyTranslationState*>(_EnemyObject->GetNowState())->SetMoveSpeed(_Speed);
	}
}

void EnemyDiscoveryState::Exit(EnemyCharacter::State next) {

}

void EnemyDiscoveryState::_EndNowLocalState_CallBack(EnemyCharacter::State now) {
}