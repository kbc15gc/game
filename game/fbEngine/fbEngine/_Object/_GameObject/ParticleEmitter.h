#pragma once
#include "GameObject.h"

class Particle;

//�p�[�e�B�N�������p�����[�^
struct ParticleParameter {
	//�������B
	void Init()
	{
		memset(this, 0, sizeof(ParticleParameter));
		size = Vector2(1, 1);
		initAlpha = 1.0f;
		brightness = 1.0f;
		isBillboard = true;
		mulColor = Color::white;
		isParent = false;
		isZTest = true;
	}
	const char* texturePath;						//!<�e�N�X�`���̃t�@�C���p�X(Asset/Texture�̒��ɂ������(Asset/Texture������̃p�X�̎w��݂̂�OK))�B
	Vector3		initVelocity;						//!<�����x(�G�~�b�^�[�Ɛe�q�֌W��g��ł���ꍇ�̓G�~�b�^�[�̍��W�n�A�����łȂ��ꍇ�̓��[���h���W)�B
	Vector2		size;								//�p�[�e�B�N���T�C�Y
	float		life;								//!<�����B�P�ʂ͕b(0��菬�����l�Ŗ���)�B
	float		intervalTime;						//!<�������ԁB�P�ʂ͕b�B
	Vector3		initPositionRandomMargin;			//!<�����ʒu�̃����_�����B
	Vector3		initVelocityVelocityRandomMargin;	//!<�����x�̃����_�����B
	Vector3		addVelocityRandomMargih;			//!<���x�̐ϕ��̂Ƃ��̃����_�����B
	Vector3		gravity;							//!<�d�́B
	bool		isFade;								//!<���ʂƂ��Ƀt�F�[�h�A�E�g����H
	float		fadeTime;							//!<�t�F�[�h���鎞�ԁB
	float		initAlpha;							//!<�����A���t�@�l�B
	bool		isBillboard;						//!<�r���{�[�h�H
	float		brightness;							//!<�P�x�B�u���[�����L���ɂȂ��Ă���Ƃ������������ƌ������܂��B
	int			alphaBlendMode;						//!<0�����������A1���Z�����B
	Color		mulColor;							//!<��Z�J���[�B
	bool		isParent;							//!<�G�~�b�^�[�Ɛe�q�֌W��g�ނ��B
	bool isZTest;
};


	
//�p�[�e�B�N���̔����@	
class ParticleEmitter : public GameObject {
public:
	ParticleEmitter(char* name) :GameObject(name) {};
	~ParticleEmitter() {
		ReleaseParticleAll();
	};
	/*!
	 *@brief	�������B
	 *@_Param[in]	random		���������Ɏg�p���闐�������@�B
	 *@_Param[in]	camera		�p�[�e�B�N���̕`��Ŏg�p����J�����B
	 *@_Param[in]	_Param		�p�[�e�B�N�������p�̃p�����[�^�B
	 *@_Param[in]	emitPosition	�G�~�b�^�[�̍��W�B
	 */
	void Init(const ParticleParameter& _Param);
	void Start() override;
	void Update() override;
	void Render() override;
	/*!
	*@brief	�p�[�e�B�N���ɗ͂�������B
	*@_Param[in]	applyForce		���������Ɏg�p���闐�������@�B
	*/
	void ApplyForce(Vector3& applyForce);

	// �����x�Đݒ�B
	// �����ɍ쐬���ꂽ�p�[�e�B�N���ɑ΂��Ă͖����B
	// �����ꂩ��쐬�������̂ɑ΂��Ă̂ݗL���B
	inline void ResetInitVelocity(const Vector3& newVelocity) {
		_Param.initVelocity = newVelocity;
	}

	// �p�����[�^�[�Đݒ�(�쐬�ς݂̃p�[�e�B�N���̃p�����[�^�[���ύX)�B
	void ResetParameterAlreadyCreated(const ParticleParameter& param);

	// ���̊֐����Ă�ł���AchievedCreateParticleEnd�֐����Ă΂��܂łɐ������ꂽ�p�[�e�B�N���ւ̃|�C���^���擾��������֐��B
	// �����F	���������p�[�e�B�N�����i�[����z��ւ̃|�C���^�B
	void AchievedCreateParticleStart(vector<Particle*>* array);

	// ���������p�[�e�B�N���̎擾�I���B
	void AchievedCreateParticleEnd();

	void SetEmitFlg(bool b);

	inline bool GetEmitFlg()const {
		return emit;
	}

	// �p�����[�^�[�ݒ�(�쐬�ς݂̃p�[�e�B�N���ɂ͖���)�B
	inline void SetParam(const ParticleParameter& param) {
		_Param = param;
	}
	inline const ParticleParameter& GetParam()const {
		return _Param;
	}

	inline const Vector3& GetInitVelocity() const{
		return _Param.initVelocity;
	}
	
	// �������Ă���p�[�e�B�N���̂����A�ŏ��ɐ������ꂽ���̂�ԋp����֐��B
	// �����X�g�Ƀp�[�e�B�N�����i�[����ĂȂ��ꍇ��null���ԋp�����B
	Particle* GetParticleBegin()const;
	// �������Ă���p�[�e�B�N���̂����A�Ō�ɐ������ꂽ���̂�ԋp����֐��B
	// �����X�g�Ƀp�[�e�B�N�����i�[����ĂȂ��ꍇ��null���ԋp�����B
	Particle* GetParticleEnd()const;

	void ReleaseParticleAll();
	//�p�[�e�B�N������
	void Emit();
private:
	bool	emit;//�����t���O
	float					_Timer;			//!<�^�C�}�[
	ParticleParameter		_Param;			//!<�p�����[�^�B
	std::list<Particle*>	_ParticleList;	//!<�p�[�e�B�N���̃��X�g�B
	vector<Particle*>* _achievedArray = nullptr;
};