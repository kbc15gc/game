#pragma once
#include "EnemyCharacter.h"
#include "fbEngine\_Object\_GameObject\ParticleEmitter.h"
#include "GameObject\Enemy\LaserBreath.h"
#include "GameObject\History\Chip.h"

// �p���N���X�B
// �{�X�G�l�~�[(���߃S�[�X�g)�B
class BossGhost :
	public EnemyCharacter
{
private:
	// �{�X(���s�^�h���S��)�̃A�j���[�V�����ԍ��B
	enum class AnimationBossGhost {
		Wait = 0,
		Max
	};
public:
	BossGhost(const char* name);
	~BossGhost();

	// �A�j���[�V�����C�x���g�֘A�B



protected:
	void _EndNowStateCallback(State EndStateType)override;

private:
	void _AwakeSubClass()override;
	void _StartSubClass()override;
	void _UpdateSubClass()override;
	void _LateUpdateSubClass()override;


	EnemyAttack* _AttackSelectSubClass()override;

	// �R���W������`�֐��B
	// �R���W�����̌`���p�����[�^��ݒ肷��֐��B
	void _ConfigCollision()override;

	// �L�����N�^�[�R���g���[���[�̃p�����[�^�[��ݒ肷��֐��B
	// �Փ˂���R���W����������d�͂̒l�Ȃǂ������Őݒ肷��B
	void _ConfigCharacterController()override;

	// �L�����N�^�[�R���g���[�������o���R���|�[�l���g�p�̍��̍쐬�֐��B
	// �R���W����������`��Ȃǂ�ݒ肵�A�쐬����B
	void _CreateExtrudeCollision()override;

	// �L�����N�^�[�R���g���[�������o���R���|�[�l���g�̃p�����[�^�[��ݒ肷��֐��B
	// �Փ˂���R���W����������d�͂̒l�Ȃǂ������Őݒ肷��B
	// ���������̂͌p����ɈϏ��B
	inline void _ConfigCharacterExtrude()override {
		_MyComponent.CharacterExtrude->Attribute_AllOff();
		_MyComponent.CharacterExtrude->AddAttribute(Collision_ID::PLAYER);
		_MyComponent.CharacterExtrude->AddAttribute(Collision_ID::ENEMY);
	}

	// �p����ŃA�j���[�V�����ԍ��̃e�[�u�����쐬�B
	// �����F	�A�j���[�V�����I�����Ԃ̊i�[�p�z��(���̔z��ɏI�����Ԃ�ݒ肷��A�Y�����̓��f���ɐݒ肳��Ă���A�j���[�V�����ԍ�)�B
	// �󂯎��z����̒l�̓f�t�H���g��-1�ƂȂ��Ă���̂ŁA�A�j���[�V�����̏I�����Ԃ�1�b�ȏ�̂��̂͐ݒ肵�Ȃ��Ă悢�B
	void _BuildAnimationSubClass(vector<double>& datas)override;

	// �A�j���[�V�����C�x���g��ݒ肷��֐��B
	void _ConfigAnimationEvent()override;

	// ���ʉ��̃e�[�u���쐬�֐��B
	void _BuildSoundTable()override;

	inline void _DropSubClass()override {
		Chip* chip = INSTANCE(GameObjectManager)->AddNew<Chip>("Chip", 8);
		chip->SetDropChipID(ChipID::Oil, transform->GetPosition() + Vector3(0.0f, -1.5f, 0.0f));
		for (int idx = 0; idx < static_cast<int>(Item::ItemCodeE::Max); idx++)
		{
			for (int i = 0; i < 5; i++)
			{
				//���Ƃ��A�C�e�������`�F�b�N�B
				if (_Type[idx][i] != -1)
				{
					DropItem* item = INSTANCE(GameObjectManager)->AddNew<DropItem>("DropItem", 9);
					//���Ƃ��A�C�e����id�ƃR�[�h���w��B
					item->Create(_Type[idx][i], idx, transform->GetPosition(), 2);
				}
			}
		}
	}

private:
	State _saveState;
	unique_ptr<EnemySingleAttack> _singleAttack;	// �P�U������(1�̃N���X���G�l�~�[�̎�ʂȂ̂ŁA�ÓI�����o�ŃI�b�P�[�����ǃG���[�͂���������)�B
};
