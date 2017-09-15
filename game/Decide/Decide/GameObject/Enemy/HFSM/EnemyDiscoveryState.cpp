#include"stdafx.h"
#include "../EnemyCharacter.h"
#include "EnemyDiscoveryState.h"
#include "EnemyTranslationState.h"
#include "EnemyWaitState.h"
#include "GameObject\Player\Player.h"

EnemyDiscoveryState::EnemyDiscoveryState(EnemyCharacter* Object) : EnemyState(Object)
{
}


EnemyDiscoveryState::~EnemyDiscoveryState()
{
}

void EnemyDiscoveryState::_EntrySubClass() {
	_Player = static_cast<Player*>(INSTANCE(GameObjectManager)->FindObject("Player"));
	_Speed = _EnemyObject->GetDashSpeed();	// �ړ����x��ݒ�B
	_isOutside = false;
}

void EnemyDiscoveryState::_Start() {
	// �A�j���[�V�����Đ��B
	_EnemyObject->PlayAnimation_Loop(EnemyCharacter::AnimationType::Dash,0.2f);
	// �̂�����ݒ�B
	_EnemyObject->ConfigDamageReaction(true, 3);
}

void EnemyDiscoveryState::_UpdateSubClass() {
	// �b�菈��
	if (_Player->GetState() == Player::State::Death) {
		// �v���C���[������ł���B

		_EnemyObject->ChangeStateRequest(EnemyCharacter::State::Wandering);
	}
	else if (!_isOutside) {
		if (!_EnemyObject->IsOutsideWandering()) {
			// �s���͈͓��B

			// �ǐՏ����B
			// �G�l�~�[���v���C���[�Ɍ�����B
			_EnemyObject->LookAtObject(*_Player);

			Vector3 EnemyToPlayer = _Player->transform->GetPosition() - _EnemyObject->transform->GetPosition();
			if (EnemyToPlayer.Length() <= _EnemyObject->GetAttackRange()) {
				// �U���͈͂ɓ������B

				// �X�e�[�g�I���B
				_EndState();
				return;
			}

			EnemyToPlayer.Normalize();
			// �ړ��ʌv�Z�B
			Vector3 moveSpeeed = EnemyToPlayer * _Speed;

			// �Z�o�����ړ����G�l�~�[�ɉ��Z�B
			_EnemyObject->AddMoveSpeed(moveSpeeed);
		}
		else {
			// �s���͈͂���O�ꂽ�B

			_isOutside = true;

			// �Ƃ肠���������ʒu�܂Ŗ߂�B
			_ChangeLocalState(EnemyCharacter::State::Translation);
			// �p�����[�^�ݒ�B
			Vector3 NowToInitVec = _EnemyObject->GetInitPos() - _EnemyObject->transform->GetPosition();
			static_cast<EnemyTranslationState*>(_NowLocalState)->SetLength(NowToInitVec.Length());
			NowToInitVec.Normalize();
			static_cast<EnemyTranslationState*>(_NowLocalState)->SetDir(NowToInitVec);
			static_cast<EnemyTranslationState*>(_NowLocalState)->SetMoveSpeed(_Speed);

			// �G�l�~�[��i�ޕ����Ɍ�����B
			_EnemyObject->LookAtDirection(NowToInitVec);
		}
	}
	//else {
	//	// ���b�菈���B
	//}
}

void EnemyDiscoveryState::Exit(EnemyCharacter::State next) {

}

void EnemyDiscoveryState::_EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType) {
	if (EndLocalStateType == EnemyCharacter::State::Translation) {
		// �����ʒu�ɖ߂����B

		_EnemyObject->ChangeStateRequest(EnemyCharacter::State::Wandering);
	}
}

bool EnemyDiscoveryState::IsPossibleChangeState(EnemyCharacter::State next) {
	if (next == EnemyCharacter::State::Discovery) {
		return false;
	}
	return true;
}