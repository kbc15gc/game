#pragma once
#include "EnemyCharacter.h"
#include "fbEngine\_Object\_GameObject\ParticleEmitter.h"
#include "GameObject\Enemy\LaserBreath.h"
#include "GameObject\History\Chip.h"

class EnemyBreathAttack;

// �p���N���X�B
// �{�X�G�l�~�[(���s�^�h���S��)�B
class BossDrarian :
	public EnemyCharacter
{
private:
	// �{�X(���s�^�h���S��)�̃A�j���[�V�����ԍ��B
	enum class AnimationDrarian {
		Wait = 0,
		Walk,
		Dash,
		Attack,
		Barking,
		TailAttackRight,
		Breath,
		Damage,
		Death,
		Max
	};
public:
	BossDrarian(const char* name);
	~BossDrarian();

	// �A�j���[�V�����C�x���g�֘A�B
	void AnimationEvent_Kamituki();
	void CreateAttackCollision_TailAttack1();
	void CreateAttackCollision_TailAttack2();
	void CreateAttackCollision_TailAttack3();
	void CreateAttackCollision_TailAttack4();
	void AnimationEvent_BreathStart();
	void AnimationEvent_BreathEnd();


protected:
	void _EndNowStateCallback(State EndStateType)override;

private:
	void _AwakeSubClass()override;
	void _StartSubClass()override;
	void _UpdateSubClass()override;
	void _LateUpdateSubClass()override;


	EnemyAttack* AttackSelect()override;

	// �R���W������`�֐��B
	// �R���W�����̌`���p�����[�^��ݒ肷��֐��B
	void _ConfigCollision()override;

	// �L�����N�^�[�R���g���[���[�̃p�����[�^�[��ݒ肷��֐��B
	// �Փ˂���R���W����������d�͂̒l�Ȃǂ������Őݒ肷��B
	void _ConfigCharacterController()override;

	// �p����ŃA�j���[�V�����ԍ��̃e�[�u�����쐬�B
	// ���Y�����ɂ͂��̃N���X��`����AnimationType�񋓑̂��g�p�B
	void _BuildAnimation()override;

	// �A�j���[�V�����C�x���g��ݒ肷��֐��B
	void _ConfigAnimationEvent()override;

	// ���ʉ��̃e�[�u���쐬�֐��B
	void _BuildSoundTable()override;

	inline void _DropSubClass()override {
		Chip* chip = INSTANCE(GameObjectManager)->AddNew<Chip>("Chip", 8);
		chip->SetDropChipID(ChipID::Oil, transform->GetPosition());
	}

private:
	State _saveState;
	unique_ptr<EnemySingleAttack> _singleAttack;	// �P�U������(1�̃N���X���G�l�~�[�̎�ʂȂ̂ŁA�ÓI�����o�ŃI�b�P�[�����ǃG���[�͂���������)�B
	unique_ptr<EnemySingleAttack> _tailAttack;
	unique_ptr<EnemyBreathAttack> _breathAttack;
};


class EnemyBreathAttack : public EnemyAttack {
private:
	EnemyBreathAttack(EnemyCharacter* object):EnemyAttack(object) {};
public:
	// �����F	���̃u���X�U�����s���G�l�~�[�B
	//			��������p�[�e�B�N���̃p�����[�^�B
	//			�����ʒu(���[�J�����W�A�e�̓u���X�𔭎˂���G�l�~�[)�B	
	EnemyBreathAttack(EnemyCharacter* object, ParticleParameter& param, const Vector3& emitPos);

	~EnemyBreathAttack() {
		ReleaceEmitterAll();
	}

	void Entry()override {
		_breath = INSTANCE(GameObjectManager)->AddNew<LaserBreath>("breath", 8);
		_breath->Init(_enemyObject);
		_enemyObject->LookAtObject(*_player);
		// �p�[�e�B�N���̔�ԕ������G�l�~�[�̌����ɍĐݒ�B
		Vector3 initVelocity = _enemyObject->transform->GetForward();
		initVelocity.Normalize();
		_initParticleParam.initVelocity = initVelocity * _particleEmitter[0]->GetInitVelocity().Length();
		_particleEmitter[0]->SetParam(_initParticleParam);
	}

	bool Update()override;

	void Exit()override {
		BreathEnd();
		_breath = nullptr;	// �u���X�I�u�W�F�N�g�͎����I�ɏ��ł��邽�ߕ��u�B
	};

	// �p�[�e�B�N���G�~�b�^�[�ǉ��B
	// ��������p�[�e�B�N���̃p�����[�^�B
	// �����ʒu�B
	// �e(�f�t�H���g��null)�B
	// �����̊֐����Ă񂾂����ł̓G�~�b�g�͊J�n����Ȃ��B
	// ���G�~�b�g����ۂ̓G�~�b�^�[��SetEmitFlg�֐����ĂԁB
	void AddParticleEmitter(const ParticleParameter& param, const Vector3& emitPos, Transform* parent = nullptr) {
		ParticleEmitter* p = INSTANCE(GameObjectManager)->AddNew<ParticleEmitter>("BreathEmitter", 8);
		p->transform->SetParent(parent);
		p->transform->SetLocalPosition(emitPos);
		p->Init(param);
		p->SetEmitFlg(false);
		_particleEmitter.push_back(p);
	}

	// �u���X�J�n�B
	inline void BreathStart() {
		unique_ptr<vector<Particle*>> list;
		list.reset(new vector<Particle*>);
		_particleEmitter[0]->AchievedCreateParticleStart(list.get());
		_breath->SetParticleList(move(list));
		_particleEmitter[0]->SetEmitFlg(true);
		_breath->BreathStart();
	}

	// �u���X�I���B
	inline void BreathEnd() {
		_particleEmitter[0]->SetEmitFlg(false);
		_particleEmitter[0]->AchievedCreateParticleEnd();
	}

	void ReleaceEmitterAll() {
		for (auto emitter : _particleEmitter) {
			INSTANCE(GameObjectManager)->AddRemoveList(emitter);
		}
		_particleEmitter.clear();
	}

private:
	GameObject* _player = nullptr;
	BreathObject* _breath = nullptr;	// �u���X�I�u�W�F�N�g(�u���X���ˏ������I���������u���X�̋������Ǘ��ł���悤�ɂ��邽�߂ɃN���X������)�B
	vector<ParticleEmitter*> _particleEmitter;
	ParticleParameter _initParticleParam;
};