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
		EnemyToPlayer.y = 0.0f;	// Y�������͏����B
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

		// �G�l�~�[���v���C���[�Ɍ�����B
		//_EnemyObject->transform->LockAt(_Player);

		// ���̌����ƃv���C���[�܂ł̌����̊p�x���Z�o�B
		Vector3 EnemyDir = _EnemyObject->transform->GetForward();
		EnemyDir.y = 0.0f;	// Y�������͏����B
		EnemyDir.Normalize();
		float dot = EnemyToPlayer.Dot(EnemyDir);
		float rad = acosf(dot);	

		if (rad > 0.001f) {
			// �v���C���[�̂ق��������Ă��Ȃ��B

			// ��]�����Z�o�B
			D3DXVECTOR3 cross;
			D3DXVec3Cross(&cross,&D3DXVECTOR3(EnemyDir.x, EnemyDir.y, EnemyDir.z),&D3DXVECTOR3(EnemyToPlayer.x, EnemyToPlayer.y, EnemyToPlayer.z));
			//Vector3 cross = EnemyToPlayer;
			//cross.Cross(EnemyDir);
			//cross.Normalize();

			// �e�X�g�B
			static float time = 0.0f;
			time += Time::DeltaTime();
			float kakudo = D3DXToDegree(rad);

			if (time >= 2.0f) {
				time = 0.0f;
				OutputDebugString(_T("��"));
			}
			if (rad > D3DXToRadian(180.0f)) {
				// ����]���傫���p�x���擾�����B

				// �������ق��̊p�x���Z�o�B
				rad = D3DXToRadian(360.0f) - rad;
				cross *= -1.0f;
			}

			// �N�H�[�^�j�I���ŉ�]�B
			Quaternion quat = Quaternion::Identity;
			quat.SetRotation(cross, rad);
			Quaternion Rotation = _EnemyObject->transform->GetRotation();
			quat.Multiply(Rotation);
			_EnemyObject->transform->SetLocalAngle(quat.GetAngle());
			//_EnemyObject->transform->SetRotation(quat);
		}

		//if (rad > 0.001f) {
		//	Vector3 Euler = _EnemyObject->transform->GetAngle();	// �I�C���[�p�Ŏ擾�B
		//	if (/*AngleDir*/dot < 0.0f) {
		//		OutputDebugString(_T("�}�C�i�X\n"));
		//	}
		//	Euler.y = Euler.y + (rad * AngleDir);
		//	_EnemyObject->transform->SetAngle(Euler);
		//}
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

bool EnemyDiscoveryState::IsPossibleChangeState(EnemyCharacter::State next) {
	if (next == EnemyCharacter::State::Discovery) {
		return false;
	}
	return true;
}