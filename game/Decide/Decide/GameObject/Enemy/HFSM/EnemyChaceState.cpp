#include"stdafx.h"
#include "../EnemyCharacter.h"
#include "EnemyChaceState.h"
#include "EnemyTranslationState.h"
#include "EnemyWaitState.h"
#include "GameObject\Player\Player.h"

EnemyChaceState::EnemyChaceState(EnemyCharacter* Object) : EnemyState(Object)
{
	_playAnimation = EnemyCharacter::AnimationType::Dash;
	_loopNum = -1;
}


EnemyChaceState::~EnemyChaceState()
{
}

void EnemyChaceState::_EntrySubClass() {
	_Player = static_cast<Player*>(INSTANCE(GameObjectManager)->FindObject("Player"));
	_Speed = _EnemyObject->GetDashSpeed();	// �ړ����x��ݒ�B
	_isOutside = false;
}

void EnemyChaceState::_StartSubClass() {
	_EnemyObject->GetMyComponent().RotationAction->StopFreeRotation();

	// �v���C���[�̂ق���S�������Ă��Ȃ��ꍇ�͎��R��]�B
	Vector3 EnemyToPlayer = _Player->transform->GetPosition() - _EnemyObject->transform->GetPosition();
	float length = EnemyToPlayer.Length();

	EnemyToPlayer.y = 0.0f;
	EnemyToPlayer.Normalize();
	Vector3 vec = _EnemyObject->transform->GetForward();
	vec.y = 0.0f;
	vec.Normalize();

	ObjectRotation* OR = _EnemyObject->GetMyComponent().RotationAction;
	float rad = fabsf(OR->Optimization(acosf(EnemyToPlayer.Dot(vec))));
	if (rad >= D3DXToRadian(10.0f)) {
		// ���R��]�B
		_isRotationinterpolate = false;
		OR->StartFreeRotation(D3DXToRadian(10.0f), Vector3::up);
	}
	else {
		_isRotationinterpolate = true;
	}

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

			Vector3 EnemyToPlayer = _Player->transform->GetPosition() - _EnemyObject->transform->GetPosition();
			float length = EnemyToPlayer.Length();

			// �G�l�~�[���v���C���[�Ɍ�����B
			EnemyToPlayer.y = 0.0f;
			EnemyToPlayer.Normalize();
			if(_isRotationinterpolate){
				_EnemyObject->LookAtObject(_Player);
			}
			else {
				Vector3 vec = _EnemyObject->transform->GetForward();
				vec.y = 0.0f;
				vec.Normalize();
				float rad = fabsf(_EnemyObject->GetMyComponent().RotationAction->Optimization(acosf(EnemyToPlayer.Dot(vec))));
				if (rad >= D3DXToRadian(10.0f)) {
					_isRotationinterpolate = true;
					_EnemyObject->GetMyComponent().RotationAction->StopFreeRotation();
				}
			}

			if (length <= _endRange) {
				// �ǐՏI���͈͂ɓ������B

				//float nowAngle = atan2f(vec.x, vec.z);
				//float targetAngle = atan2f(EnemyToPlayer.x, EnemyToPlayer.z);

				//targetAngle -= nowAngle;

				Vector3 work = _EnemyObject->transform->GetForward();
				work.y = 0.0f;
				work.Normalize();
				float dot = EnemyToPlayer.Dot(work);
				float acos = _EnemyObject->GetMyComponent().RotationAction->Optimization(acosf(dot));
				float rad = fabsf(acos);
				if (/*fabsf(targetAngle)*/rad <= 0.1f){
					// �v���C���[�Ɍ����Ă���B

					_EnemyObject->GetMyComponent().RotationAction->StopFreeRotation();
					// �X�e�[�g�I���B
					_EndState();
					return;
				}
			}
			else {
				// �ړ��ʌv�Z�B
				Vector3 moveSpeeed = EnemyToPlayer * _Speed;

				// �Z�o�����ړ����G�l�~�[�ɉ��Z�B
				_EnemyObject->AddMoveSpeed(moveSpeeed);
			}
		}
		else {
			// �s���͈͂���O�ꂽ�B

			_isOutside = true;

			_EnemyObject->GetMyComponent().RotationAction->StopFreeRotation();

			// �Ƃ肠���������ʒu�܂Ŗ߂�B
			_ChangeLocalState(EnemyCharacter::State::Translation);
			_NowLocalState->CustamParameter(_playAnimation, _interpolate, _loopNum, _eventNo, _playSpeed);
			// �p�����[�^�ݒ�B
			Vector3 NowToInitVec = _EnemyObject->GetInitPos() - _EnemyObject->transform->GetPosition();
			NowToInitVec.y = 0.0f;
			static_cast<EnemyTranslationState*>(_NowLocalState)->SetLength(NowToInitVec.Length());
			NowToInitVec.Normalize();
			static_cast<EnemyTranslationState*>(_NowLocalState)->SetDir(NowToInitVec);
			static_cast<EnemyTranslationState*>(_NowLocalState)->SetMoveSpeed(_Speed);

			ObjectRotation* OR = _EnemyObject->GetMyComponent().RotationAction;
			Vector3 vec = _EnemyObject->transform->GetForward();
			vec.y = 0.0f;
			vec.Normalize();
			float rad = fabsf(OR->Optimization(acosf(NowToInitVec.Dot(vec))));
			if (rad >= D3DXToRadian(90.0f)) {
				// ��Ԏ��Ԃ𒷂߂ɂ���B
				_EnemyObject->LookAtDirectionInterpolate(NowToInitVec, 0.6f);
			}
			else {
				_EnemyObject->LookAtDirectionInterpolate(NowToInitVec, 0.5f);
			}
		}
	}
}

void EnemyChaceState::_ExitSubClass(EnemyCharacter::State next) {

}

void EnemyChaceState::_EndNowLocalState_CallBack(EnemyCharacter::State EndLocalStateType) {
	if (EndLocalStateType == EnemyCharacter::State::Translation) {
		// �����ʒu�ɖ߂����B

		// �����X�e�[�g�ɖ߂��B
		_EnemyObject->ChangeStateRequest(_EnemyObject->GetInitState());
	}
}

bool EnemyChaceState::IsPossibleChangeState(EnemyCharacter::State next) {
	if (next == EnemyCharacter::State::Chace) {
		return false;
	}
	return true;
}