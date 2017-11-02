#include"stdafx.h"
#include "../EnemyCharacter.h"
#include "EnemyChaceState.h"
#include "EnemyTranslationState.h"
#include "EnemyWaitState.h"
#include "GameObject\Player\Player.h"

EnemyChaceState::EnemyChaceState(EnemyCharacter* Object) : EnemyState(Object)
{
}


EnemyChaceState::~EnemyChaceState()
{
}

void EnemyChaceState::_EntrySubClass() {
	_Player = static_cast<Player*>(INSTANCE(GameObjectManager)->FindObject("Player"));
	_Speed = _EnemyObject->GetDashSpeed();	// �ړ����x��ݒ�B
	_isOutside = false;
}

void EnemyChaceState::_Start() {
	// �A�j���[�V�����Đ��B
	_EnemyObject->PlayAnimation_Loop(EnemyCharacter::AnimationType::Dash, 0.2f);
	// �̂�����ݒ�B
	_EnemyObject->ConfigDamageReaction(true, _EnemyObject->GetDamageMotionRandNum());
}

void EnemyChaceState::_UpdateSubClass() {
	// �b�菈��
	if (_Player->GetState() == Player::State::Death) {
		// �v���C���[������ł���B

		_EnemyObject->ChangeStateRequest(_EnemyObject->GetInitState());
	}
	else if (!_isOutside) {
		if (!_EnemyObject->IsOutsideDiscovery()) {
			// �s���͈͓��B

			// �ǐՏ����B
			// �G�l�~�[���v���C���[�Ɍ�����B
			_EnemyObject->LookAtObject(_Player);

			Vector3 EnemyToPlayer = _Player->transform->GetPosition() - _EnemyObject->transform->GetPosition();
			if (EnemyToPlayer.Length() <= _EnemyObject->GetNowSelectAttackRange()) {
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
}

void EnemyChaceState::_ExitSubClass(EnemyCharacter::State next) {

}

void EnemyChaceState::_EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType) {
	if (EndLocalStateType == EnemyCharacter::State::Translation) {
		// �����ʒu�ɖ߂����B

		_EnemyObject->ChangeStateRequest(_EnemyObject->GetInitState());
	}
}

bool EnemyChaceState::IsPossibleChangeState(EnemyCharacter::State next) {
	if (next == EnemyCharacter::State::Chace) {
		return false;
	}
	return true;
}