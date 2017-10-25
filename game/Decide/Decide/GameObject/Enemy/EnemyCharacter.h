#pragma once

#include "fbEngine\_Object\_Component\_3D\Animation.h"
#include "SearchViewAngle.h"
#include "../Component/ObjectRotation.h"
#include "fbEngine\CharacterController.h"
#include "GameObject\Component\CharacterParameter.h"
#include "GameObject\Component\ParameterBar.h"
#include "GameObject\Component\ObjectSpawn.h"
#include "GameObject\TextImage\AttackValue2D.h"
#include "GameObject\Component\AnimationEvent.h"
#include "fbEngine\_Object\_GameObject\SoundSource.h"
#include "GameObject\Player\Player.h"
#include "fbEngine\_Object\_Component\_Physics\CharacterExtrude.h"
#include "GameObject\ItemManager\DropItem\DropItem.h"

class SkinModel;
class Animation;
class EnemyState;
class ParticleEffect;
class BuffDebuffICon;

class EnemyAttack;
class EnemySingleAttack;
class EnemyBreathAttack;

// ���N���X�B
// �G�l�~�[�̃L�����N�^�[�B
class EnemyCharacter :
	public GameObject
{
public:
	// �v���C���[�ƍł��߂��G�l�~�[�̏��B
	struct NearEnemyInfo {
		NearEnemyInfo(float len, EnemyCharacter* obj) {
			length = len;
			object = obj;
		}
		float length = FLT_MAX;
		EnemyCharacter* object = nullptr;
	};

	// �������ǂ̎�ނ̃G�l�~�[���B
	// �����̃N���X���p�����ĐV��G�l�~�[���쐬�����炱���Ɏ�ʂ�ǉ����邱�ƁB
	enum class EnemyType{Born = 0,BossDrarian,Drarian, Golem};

	// �X�e�[�g�z��̓Y������񋓁B
	// ���p����Ŏg�p������̂��܂߂Ă��ׂẴX�e�[�g�������ɗ񋓂���B
	// ���ǉ�����ۂ͂��̃N���X��_BuildState�֐��ɋL�q�������ԂɂȂ��Ă��邩����������m�F���邱�ƁB
	// ���X�e�[�g��ǉ������ۂ͂��������łȂ����̃N���X����_BuildState�֐����X�V���邱�ƁB
	enum class State {None = -1, Wandering = 0,Discovery, Chace,Threat, Speak,StartAttack, Attack ,Wait ,Translation, Fall,Damage,Death};

	// �A�j���[�V�����f�[�^�e�[�u���̓Y�����B
	// ��0�ԂȂ�ҋ@�A�j���[�V�����A1�ԂȂ�����A�j���[�V�����B
	// �����̗񋓎q��Y�����Ƃ��āA�p����̃N���X�ŃA�j���[�V�����ԍ��̃e�[�u�����쐬����B
	// ���U�����[�V�����̓��f���ɂ���Đ����قȂ�̂ł����ł͈���Ȃ��B
	enum class AnimationType { None = -1,Idle = 0, Walk, Dash, Threat, Fall,Damage, Death,Max };

	// �T�E���h�f�[�^�e�[�u���̓Y�����B
	enum class SoundIndex{None = -1, Attack1 = 0, Attack2, Attack3, Threat,Damage,Death, Max};

	// �T�E���h�f�[�^�\���́B
	struct SoundData {
		char Path[FILENAME_MAX];	// �T�E���h��(.wav������Ă�)�B
		bool Is3D = false;		// 3D�T�E���h���B
		bool IsLoop = false;	// ���[�v�Đ����B
		SoundSource* Source = nullptr;	// �T�E���h�Đ��I�u�W�F�N�g�B
	};
private:
	// ���̃N���X�Ŏg�p����R���|�[�l���g�B
	// ���R���|�[�l���g�͏���ɍ폜����邽�߁A���̃N���X�ł͍폜���Ȃ��B
	// �����̃N���X�Ǝ��̃����o�ϐ��ƏZ�ݕ������s�����߂ɍ\���̉�����B
	// �������œo�^����Ă�����̂��G�l�~�[�ɃA�^�b�`����Ă���R���|�[�l���g�̂��ׂĂł͂Ȃ��̂Œ��ӁB
	struct Components {
		SkinModel* Model = nullptr;	// ���̃I�u�W�F�N�g�̃��f���B
		Animation* Animation = nullptr;	// ���̃I�u�W�F�N�g�̃A�j���[�V�����B
		Collider* Collider = nullptr;	// �R���W�����`��B
		CCharacterController* CharacterController = nullptr;		// �L�����N�^�[�R���g���[���B
		ObjectRotation* RotationAction = nullptr;	// �I�u�W�F�N�g����]������N���X�B
		CharacterParameter* Parameter = nullptr;//�G�l�~�[�̃p�����[�^�[�B
		ParameterBar* HPBar = nullptr;			// �Q�[�WHP�p�B
		ObjectSpawn* Spawner = nullptr;		// ���X�|�[���ݒ�ł���B
		AnimationEventPlayer* AnimationEventPlayer = nullptr;	// �A�j���[�V�����ɃC�x���g��ݒ�ł���B
		ParticleEffect* ParticleEffect = nullptr;	// �p�[�e�B�N���G�t�F�N�g�B
		BuffDebuffICon*	BuffDebuffICon = nullptr;	// �o�t�f�o�t�A�C�R���B
		CharacterExtrude* CharacterExtrude = nullptr;	// ���̃G�l�~�[�����̃I�u�W�F�N�g�������o���B
		vector<RigidBody*> ExtrudeCollisions;	// �L�����N�^�[�R���g���[���̉����߂��Ɏg�p���鍄�́B
	};

	struct CollisionInfo {
		float radius = 0.0f;	// �R���W�����T�C�Y(��)�B
		float height = 0.0f;	// �R���W�����T�C�Y(����)�B
		Vector3 offset = Vector3::zero;	// �����B
		Collision_ID id = Collision_ID::ENEMY;	// �R���W���������B
	};

public:
	// �����̓I�u�W�F�N�g�̖��O�B
	EnemyCharacter(const char* name);
	~EnemyCharacter();

	// ������͊�{�I�Ɍp�����Ȃ��B
	// ���p����Ǝ��̎����́Aprivate�ɂ���_AwakeSubClass�֐����p�����Ď������邱�ƁB
	void Awake()override;

	// ������͊�{�I�Ɍp�����Ȃ��B
	// ���p����Ǝ��̎����́Aprivate�ɂ���_StartSubClass�֐����p�����Ď������邱�ƁB
	void Start()override;

	// ������͊�{�I�Ɍp�����Ȃ��B
	// ���p����Ǝ��̎����́Aprivate�ɂ���_UpdateSubClass�֐����p�����Ď������邱�ƁB
	void Update()override;

	// ������͊�{�I�Ɍp�����Ȃ��B
	// ���p����Ǝ��̎����́Aprivate�ɂ���_LateUpdateSubClass�֐����p�����Ď������邱�ƁB
	void LateUpdate()override;
	

	// �G�l�~�[���w�肵���I�u�W�F�N�g�Ɍ������鏈��(��ԂȂ�)�B
	// �����F	�������I�u�W�F�N�g�B
	inline void LookAtObject(const GameObject& Object) {
		_MyComponent.RotationAction->RotationToObject_XZ(Object);
	}

	// �G�l�~�[���w�肵�������Ɍ������鏈��(��ԂȂ�)�B
	// �����F	�����x�N�g���B
	inline void LookAtDirection(const Vector3& dir) {
		_MyComponent.RotationAction->RotationToDirection_XZ(dir);
	}

	// �G�l�~�[���w�肵�����Ɗp�x��p���ĉ�]����֐��B
	// �����F	��(�P�ʃx�N�g��)�B
	//			��]�p�x(���W�A��)�B
	inline void RotationAxis(const Vector3& axis, float angle) {
		_MyComponent.RotationAction->RotationAxis(axis, angle);
	}

	// �U�����������肷��֐��B
	// �߂�l�F	���s�������U�������N���X�̃|�C���^�B
	inline EnemyAttack* AttackSelect() {
		_nowAttack = _AttackSelectSubClass();
		return _nowAttack;
	}

	// �G�l�~�[�̃A�j���[�V�����Đ��֐�(���[�v)�B
	// �����F	�A�j���[�V�����^�C�v(�e�[�u���̂ق�)�B
	//			��Ԏ��ԁB
	inline void PlayAnimation_Loop(const AnimationType animationType, const float interpolateTime) {
		PlayAnimation_OriginIndex(_AnimationNo[static_cast<unsigned int>(animationType)], interpolateTime,-1);
	}

	// �G�l�~�[�̃A�j���[�V�����Đ��֐�(�w��񐔃��[�v)�B
	// �����F	�A�j���[�V�����^�C�v(�e�[�u���̂ق�)�B
	//			��Ԏ��ԁB
	//			���[�v��(�f�t�H���g��1)�B
	inline void PlayAnimation(const AnimationType animationType, const float interpolateTime, const int loopCount = 1) {
		PlayAnimation_OriginIndex(_AnimationNo[static_cast<unsigned int>(animationType)], interpolateTime, loopCount);
	}

	// �G�l�~�[�̃A�j���[�V�����Đ��֐�(�w��񐔃��[�v)�B
	// �����F	�A�j���[�V�����^�C�v(���f�����Ƃ̃A�j���[�V�����ԍ��A-1�ōĐ����Ȃ�)�B
	//			��Ԏ��ԁB
	//			���[�v��(-1�Ŗ������[�v)�B
	inline void PlayAnimation_OriginIndex(const int animationNum, const float interpolateTime, const int loopCount = 1) {
		if (animationNum == -1) {
			// �A�j���[�V�������Đ����Ȃ��B
			return;
		}
		_MyComponent.Animation->PlayAnimation(animationNum, interpolateTime, loopCount);
	}

	// ���Đ��֐��B
	// �����F	���ʉ��e�[�u���̓Y�����B
	inline void EnemyPlaySound(const SoundIndex idx) {
		if (_SoundData[static_cast<int>(idx)].Source) {
			// �T�E���h�\�[�X���쐬����Ă���B
			_SoundData[static_cast<int>(idx)].Source->Play(_SoundData[static_cast<int>(idx)].IsLoop);
		}
	}

	// �G�l�~�[���A�j���[�V�������Đ����Ă��邩�B
	inline bool GetIsPlaying() {
		return _MyComponent.Animation->GetPlaying();
	}

	// �G�l�~�[���ǐՔ͈͊O�ɏo��������B
	bool IsOutsideDiscovery();

	// �����̒l�����Z�������ʁA�G�l�~�[���p�j�͈͊O�ɏo��������B
	bool IsOutsideWandering(const Vector3& Add);

	// �������������������̈ȊO�̍U���R���W�����ƏՓ˂����u�ԌĂ΂��R�[���o�b�N�B
	// �������͏Փ˂����U���R���W�����B
	// �����������Ȃ�������inline�̂܂܂ł�����(��������3�s�ȏ�̏���������悤�ɂȂ�܂�)�B

	virtual void HitAttackCollisionEnter(AttackCollision* hitCollision);

	// �������������������̈ȊO�̍U���R���W�����ɏՓ˂Ă���ԌĂ΂��R�[���o�b�N�B
	// �������͏Փ˂����U���R���W�����B
	// �����������Ȃ�������inline�̂܂܂ł�����(��������3�s�ȏ�̏���������悤�ɂȂ�܂�)�B
	inline virtual void HitAttackCollisionStay(AttackCollision* hitCollision) {

	}

	// �������������������̈ȊO�̍U���R���W�����Ƃ̏Փ˂���O�ꂽ��Ă΂��R�[���o�b�N�B
	// �������͏Փ˂����U���R���W�����B
	// �����������Ȃ�������inline�̂܂܂ł�����(��������3�s�ȏ�̏���������悤�ɂȂ�܂�)�B
	inline virtual void HitAttackCollisionExit(AttackCollision* hitCollision) {

	}


	// �X�e�[�g�������ɃX�e�[�g�����荞�݂Ő؂�ւ���֐��B
	// ���O����X�e�[�g������X�e�[�g�؂�ւ����s�����Ƃ���ꍇ�͂�������ĂԁB
	inline void ChangeStateRequest(State next) {
		_ChangeState(next);
	}

	// ����p����֐��B
	// ����p������s���X�e�[�g����Ăяo���B
	void SearchView();

	// �_���[�W�̂�����ݒ�֐��B
	// �����F	�̂����邩�B
	//			����Ɉ��̂����邩(�f�t�H���g��1)�B
	void ConfigDamageReaction(bool isMotion, unsigned short probability = 1);

	// �G�l�~�[�Ƀ_���[�W��^���鏈���B
	// �����F	�_���[�W���B
	void GiveDamage(const CharacterParameter::DamageInfo& info);

	// �U�������֐��B
	// �����F	�ʒu(���[�J�����W)�B
	//			��]�B
	//			�g�k�B
	//			����(0.0��菬�����l�Ŗ���)�B
	//			�e(�f�t�H���g��null)�B
	//			���@�U����(�f�t�H���g��false)�B
	//			�h�䖳���U����(�f�t�H���g��false)�B
	//			�_���[�W��(�U���̎�ނȂǂɂ��U���͂ɑ΂��銄���A���̒l��0.01f�|�����l���U���͂ɏ�Z����A�P�ʂ̓p�[�Z���g)�B
	// �߂�l:  ���������U���B
	inline AttackCollision* CreateAttack(const Vector3& localPos, const Quaternion& rot, const Vector3& scale, float life, Transform* parent = nullptr,bool isMagic = false,bool isThroughDamage = false,int percentage = 100) {
		//�U���R���W�����쐬�B
		unsigned int priorty = 1;
		AttackCollision* attack = INSTANCE(GameObjectManager)->AddNew<AttackCollision>("attackCollision", priorty);
		attack->Create(move(_MyComponent.Parameter->GiveDamageMass(isMagic, isThroughDamage,nullptr, percentage)),localPos, rot, scale, AttackCollision::CollisionMaster::Enemy, life, 0.0f, parent);
		return attack;
	}


	// �S�p�����[�^�[�ݒ�B
	// �����F	HP�o�[�ɐݒ肷��F(�d�˂�ꍇ�͐�ɒǉ��������̂���\�������)�B
	//			�e��p�����[�^�[�B
	inline void SetParamAll(const vector<BarColor>& color,int param[CharacterParameter::Param::MAX]) const{
		_MyComponent.Parameter->ParamReset(param);
		_MyComponent.HPBar->Create(color, static_cast<float>(_MyComponent.Parameter->GetMaxHP()), static_cast<float>(_MyComponent.Parameter->GetParam(CharacterParameter::Param::HP)), true, false, transform, Vector3(0.0f, 2.0f, 0.0f), Vector2(0.5f, 0.5f),6,false, false);
		_MyComponent.BuffDebuffICon->SetHpBarTransform(_MyComponent.HPBar->GetTransform());
		
	}
	// �S�p�����[�^�[�ݒ�B
	// �����F	HP�o�[�ɐݒ肷��F(�d�˂�ꍇ�͐�ɒǉ��������̂���\�������)�B
	//			�e��p�����[�^�[�B
	inline void SetParamAll(const vector<BarColor>& color, const vector<int>& param) const {
		_MyComponent.Parameter->ParamReset(param);
		_MyComponent.HPBar->Create(color, static_cast<float>(_MyComponent.Parameter->GetMaxHP()), static_cast<float>(_MyComponent.Parameter->GetParam(CharacterParameter::Param::HP)), true, false, transform, Vector3(0.0f, 2.0f, 0.0f), Vector2(0.5f, 0.5f),6 ,false,false);
		_MyComponent.BuffDebuffICon->SetHpBarTransform(_MyComponent.HPBar->GetTransform());
	}

	// ���f���t�@�C���̃p�X��ݒ�B
	inline void SetFileName(const char* name) {
		if (strlen(name) >= FILENAME_MAX) {
			// �t�@�C�����������I�[�o�[�B
			abort();
		}
		strcpy(_FileName, name);
	}

	inline const char* GetFileName()const {
		return _FileName;
	}

	// �X�e�[�g�z���ǂݎ���p�ŕԂ��֐��B
	inline const vector<unique_ptr<EnemyState>>& GetMyState() const {
		return _MyState;
	}

	// �ړ��ʂ�ݒ�B
	inline void SetMoveSpeed(const Vector3& speed) {
		_MoveSpeed = speed;
	}

	// �ړ��ʉ��Z�B
	inline void AddMoveSpeed(const Vector3& speed) {
		_MoveSpeed = _MoveSpeed + speed;
	}

	// ����p�ݒ�(�x)�B
	inline void SetViewAngle(float angle) {
		_ViewAngle = angle;
	}

	// �����鋗���ݒ�B
	inline void SetViewRange(float range) {
		_ViewRange = range;
	}

	// �p�j�͈͕ԋp�B
	inline float GetWanderingRange()const {
		return _WanderingRange;
	}

	// �����ʒu���猻�݂̈ʒu�̈ړ��ʂ�ԋp(Y��������)�B
	inline float GetInitToNowPosRange_XZ()const {
		Vector3 vec(_InitPos - transform->GetPosition());
		vec.y = 0.0f;
		return vec.Length();
	}

	// �����ʒu���猻�݈ʒu�ւ̃x�N�g����ԋp�B
	inline const Vector3& GetInitToNowPosVec() const {
		return transform->GetPosition() - _InitPos;
	}

	// �����ʒu�擾�B
	inline const Vector3& GetInitPos()const {
		return _InitPos;
	}

	// ���݂̃X�e�[�g���擾�B
	inline EnemyState* GetNowState()const {
		return _NowState;
	}

	inline bool GetIsGround() const {
		return _MyComponent.CharacterController->IsOnGround();
	}

	inline ObjectSpawn* GetSpawner()const {
		return _MyComponent.Spawner;
	}

	// �h���b�v����o���l�B
	inline void SetDropEXP(int exp) {
		_dropExp = exp;
	}
	inline int GetDropEXP() const {
		return _dropExp;
	}

	// �h���b�v���鏊�����B
	inline void SetDropMoney(int money) {
		_dropMoney = money;
	}
	inline int GetDropMoney() const {
		return _dropMoney;
	}

	inline float GetWalkSpeed()const {
		return _walkSpeed;
	}

	inline float GetDashSpeed()const {
		return _walkSpeed * 5.0f;

	}

	inline int GetParam(CharacterParameter::Param param)const {
		if (_MyComponent.Parameter) {
			return _MyComponent.Parameter->GetParam(param);
		}
		else {
			return 0;
		}
	}

	inline int GetPigmentParam(CharacterParameter::Param param)const {
		if (_MyComponent.Parameter) {
			return _MyComponent.Parameter->GetPigmentParam(param);
		}
		else {
			return 0;
		}
	}

	inline int GetMaxHP()const {
		if (_MyComponent.Parameter) {
			return _MyComponent.Parameter->GetMaxHP();
		}
		else {
			return 0;
		}
	}

	inline int GetMaxMP()const {
		if (_MyComponent.Parameter) {
			return _MyComponent.Parameter->GetMaxMP();
		}
		else {
			return 0;
		}
	}

	// �G�l�~�[���ʂ̃A�j���[�V�����񋓎q����e���f���̃A�j���[�V�����ԍ����擾�B
	inline int GetAnimationNo(AnimationType type)const {
		return _AnimationNo[static_cast<int>(type)];
	}

	// �����X�e�[�g�擾�B
	inline const State GetInitState()const {
		return _initState;
	}

	// ���݃G�l�~�[���I�����Ă���U���̔���������ԋp�B
	float GetNowSelectAttackRange()const;

	inline EnemyAttack* GetNowSelectAttack() {
		return _nowAttack;
	}



	// ���S���̃h���b�v�����B
	inline void Drop() {
		_DropSubClass();
		_Player->TakeDrop(GetDropEXP(), GetDropMoney());
	}

	/**
	* �A�C�e������.
	*/
	bool ItemEffect(Item::ItemInfo* info);

	/**
	* �G�t�F�N�g�p�X�V.
	*/
	void EffectUpdate();

	inline State GetNowStateIndex()const {
		return _NowStateIdx;
	}

	inline short GetDamageMotionRandNum()const {
		return _damageMotionRandNum;
	}

	inline int GetNowPlayAnimation()const {
		return _MyComponent.Animation->GetPlayAnimNo();
	}

protected:
	// �X�e�[�g�؂�ւ��֐��B
	// ��None��n���ƃX�e�[�g���I�t�ɂȂ�B
	void _ChangeState(State next);

	// �A�j���[�V�����e�[�u���쐬�֐��B
	// �����F	�A�j���[�V�����I�����Ԃ̊i�[�p�z��(���̔z��ɏI�����Ԃ�ݒ肷��A�Y�����̓��f���ɐݒ肳��Ă���A�j���[�V�����ԍ�)�B
	// �󂯎��z����̒l�̓f�t�H���g��-1�ƂȂ��Ă���̂ŁA�A�j���[�V�����̏I�����Ԃ�1�b�ȏ�̂��̂͐ݒ肵�Ȃ��Ă悢�B
	// ���������z�֐��B
	virtual void _BuildAnimationSubClass(vector<double>& datas) = 0;

	// �A�j���[�V�����^�C�v�ɃA�j���[�V�����f�[�^���֘A�t����֐��B
	// �����F	�A�j���[�V�����^�C�v�̗񋓎q�B
	//			�e���f���̃A�j���[�V�����ԍ��B
	inline void _ConfigAnimationType(AnimationType Type, const int animNo) {
		_AnimationNo[static_cast<unsigned int>(Type)] = animNo;
	}

	// �����̃p�����[�^���T�E���h�e�[�u���ɓo�^����֐��B
	// �����F	�o�^����T�E���h�̃^�C�v(�񋓎q)�B
	//			wave�t�@�C���̖��O(.wav�܂Ŋ܂߂�)�B
	//			3D�T�E���h�ɂ��邩�B
	//			���[�v�Đ����邩�B
	void _ConfigSoundData(SoundIndex idx, char* filePath, bool is3D = false, bool isLoop = false);

	// ���݂̃X�e�[�g�̏������I�������Ƃ��ɌĂ΂��R�[���o�b�N�֐��B
	// �����͏I�������X�e�[�g�̃^�C�v�B
	// �����̊֐��͊e�X�e�[�g�������I�ɏI�������ꍇ�ɂ̂݌Ăяo�����B
	// �����̃N���X��ChangeStateRequest�ŏI�������ꍇ�͌Ă΂�Ȃ��B
	// �����[�J���X�e�[�g�̏I�����ɂ��Ă΂�Ȃ��B
	virtual void _EndNowStateCallback(State EndStateType) {};

private:

	// HP�o�[�̕`���ԍX�V�����B
	void _BarRenderUpdate();

	// �p����ł̍X�V�����B
	// ���p����ŏ㏑�����Ďg�p�B
	virtual void _AwakeSubClass() = 0;

	// �p����ł̃V�[���؂�ւ��ɂ�鏉���������B
	// ���p����ŏ㏑�����Ďg�p�B
	virtual void _StartSubClass() {};

	// �p����ł̍X�V�����B
	// ���p����ŏ㏑�����Ďg�p�B
	virtual void _UpdateSubClass() = 0;

	// �p����ł̍X�V�����I����ɌĂ΂�鏈���B
	// ���p����ŏ㏑�����Ďg�p�B
	virtual void _LateUpdateSubClass() {};


	// ���̃N���X���g�p����R���|�[�l���g��ǉ�����֐��B
	virtual void _BuildMyComponents();

	// �R���W������`�֐��B
	// �R���W�����̌`���p�����[�^��ݒ肷��֐��B
	// ���������̂͌p����ɈϏ��B
	virtual void _ConfigCollision() = 0;

	// �L�����N�^�[�R���g���[���[�̃p�����[�^�[��ݒ肷��֐��B
	// �Փ˂���R���W����������d�͂̒l�Ȃǂ������Őݒ肷��B
	// ���������̂͌p����ɈϏ��B
	virtual void _ConfigCharacterController() = 0;

	// �L�����N�^�[�R���g���[�������o���R���|�[�l���g�p�̍��̍쐬�֐��B
	// �R���W����������`��Ȃǂ�ݒ肵�A�쐬����B
	// ���������̂͌p����ɈϏ��B
	virtual void _CreateExtrudeCollision() = 0;

	// �L�����N�^�[�R���g���[�������o���R���|�[�l���g�̃p�����[�^�[��ݒ肷��֐��B
	// �Փ˂���R���W����������d�͂̒l�Ȃǂ������Őݒ肷��B
	// ���������̂͌p����ɈϏ��B
	virtual void _ConfigCharacterExtrude() = 0;

	// �p�����[�^�[�ō��̂𐶐��B
	void _BuildCollision();

	// ���f���f�[�^�������֐��B
	void _BuildModelData();

	// �p����Ŕėp�I�Ɏg�p���邷�ׂẴX�e�[�g��o�^����֐��B
	// ���X�e�[�g��ǉ�������K�����̊֐����ɋL�q��ǉ�����
	// ���ǉ�����ۂ͂��̃N���X��State�񋓑̂ɑΉ����鏇�ԂɂȂ��Ă��邩����������m�F���邱�ƁB
	// ���X�e�[�g��ǉ������ۂ͂��������łȂ����̃N���X����State�񋓑̂��X�V���邱�ƁB
	void _BuildState();

	// ��΂ɑ��̃N���X�ł��g��Ȃ��X�e�[�g�͂������ɓo�^�B
	virtual void _BuildStateSubClass() {};

	// �A�j���[�V�����̃e�[�u���쐬�Ɏg�p����f�[�^���쐬�B
	void _BuildAnimation();

	// �A�j���[�V�����C�x���g��ݒ肷��֐��B
	// ���������̂͌p����ɈϏ��B
	virtual void _ConfigAnimationEvent() = 0;

	// ���ʉ��̃e�[�u���쐬�B
	// ���������̂͌p����ɈϏ��B
	virtual void _BuildSoundTable() = 0;

	// ���S���h���b�v�����B
	// ���p����ɂ���ĈقȂ鏈���B
	virtual inline void _DropSubClass() = 0;

	// �U�����������肷��֐��B
	// �߂�l�F	���s�������U�������N���X�̃|�C���^�B
	// �������U���p�^�[��������ꍇ�͍U�������𕪊򂳂���B
	// ���p����Ŏ����B
	virtual EnemyAttack* _AttackSelectSubClass() = 0;

protected:
	Components _MyComponent;	// ���̃N���X�Ŏg�p����R���|�[�l���g�B

	vector<unique_ptr<EnemyState>> _MyState;	// ���̃N���X�������ׂẴX�e�[�g��o�^�B

	CollisionInfo _collisionInfo;

	int _AnimationNo[static_cast<int>(AnimationType::Max)];	// �e�A�j���[�V�����^�C�v�̃A�j���[�V�����ԍ��ƍĐ����Ԃ̔z��B
	SoundData _SoundData[static_cast<int>(SoundIndex::Max)];

	State _NowStateIdx;		// ���݂̃X�e�[�g�̓Y�����B
	EnemyState* _NowState = nullptr;	// ���݂̃X�e�[�g�B
	State _initState;	// �����X�e�[�g�B

	float _Gravity = -0.98f;	// �d�́B

	Vector3 _InitPos;	// �����ʒu�B

	SearchViewAngle _SearchView;	// ����p����B
	float _ViewAngle = 0.0f;		// ����p(�x)�B
	float _ViewRange = 0.0f;		// �����鋗���B

	float _WanderingRange = 0.0f;	// �p�j�͈́B

	float _discoveryRange = 0.0f;	// �ǐՔ͈́B

	float _walkSpeed = 0.0f;		// ���s���x�B

	short _damageMotionRandNum = 1;	// ���ފm��(�U�����ȊO)�B

	bool _isDamageMotion = true;			// �̂����胂�[�V�������Đ����邩�B
	bool _isDamageMotionRandom = true;		// �̂����胂�[�V�����������_���ōĐ����邩(�����_���ɂ��Ȃ��ꍇ�͕K���̂�����)�B
	unsigned short _damageMotionProbability = 1;	// �̂�����m��(���̕ϐ��ɐݒ肳�ꂽ�񐔂�1��͂̂�����)�B

	EnemyAttack* _nowAttack = nullptr;

private:
	int _dropExp;	// ���Ƃ��o���l�B
	int _dropMoney; // ���Ƃ����z�B

	char _FileName[FILENAME_MAX];	// ���f���̃t�@�C�����B

	Vector3 _MoveSpeed;	// �ŏI�I�Ȉړ���(�ŏI�I�ɃL�����N�^�[�R���g���[���ɓn�����)�B

	Player* _Player = nullptr;			//�v���C���[

public:
	static NearEnemyInfo nearEnemyInfo;
};

