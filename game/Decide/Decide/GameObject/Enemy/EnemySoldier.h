#pragma once
#include "GameObject\Enemy\EnemyCharacter.h"
class EnemySoldier :public EnemyCharacter
{
private:
	//�S�[�����̃A�j���[�V�����ԍ�
	enum class EnemySoldierAnim
	{

		Death = 0,
		Damage,
		Attack01,
		Run,
		Walk,
		Stand,
		Max
	};

public:
	EnemySoldier(const char* name);
	~EnemySoldier();
private:
	void _AwakeSubClass()override;
	void _StartSubClass()override;
	void _UpdateSubClass()override;

	EnemyAttack* _AttackSelectSubClass()override;

	void CreateAttackCollsion();

	//�R���W������`�֐��B
	//�R���W�����̌`���p�����[�^��ݒ肷��֐��B
	void _ConfigCollision()override;

	//�L�����N�^�[�R���g���[���[�̃p�����[�^�[��ݒ肷��֐��B
	//�Փ˂���R���W����������d�͂̒l�Ȃǂ������Őݒ肷��B
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
	}

	// �A�j���[�V�����ԍ��̃e�[�u�����쐬�B
	// �����F	�A�j���[�V�����I�����Ԃ̊i�[�p�z��(���̔z��ɏI�����Ԃ�ݒ肷��A�Y�����̓��f���ɐݒ肳��Ă���A�j���[�V�����ԍ�)�B
	// �󂯎��z����̒l�̓f�t�H���g��-1�ƂȂ��Ă���̂ŁA�A�j���[�V�����̏I�����Ԃ�1�b�ȏ�̂��̂͐ݒ肵�Ȃ��Ă悢�B
	void _BuildAnimationSubClass(vector<double>& datas)override;

	// �A�j���[�V�����C�x���g��ݒ肷��֐��B
	void _ConfigAnimationEvent()override;

	// ���ʉ��̃e�[�u���쐬�֐��B
	void _BuildSoundTable()override;

	inline void _DropSubClass()override {
		//�A�C�e��ID�i�A�C�e���E�h��E����j�B
		for (int idx = 0; idx < static_cast<int>(Item::ItemCodeE::Max); idx++)
		{
			//�ǂ̔z��̂���B
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
protected:
	void _EndNowStateCallback(State EndStateType)override;
private:
	unique_ptr<EnemySingleAttack> _SingleAttack;	// �P�U�������B
};

