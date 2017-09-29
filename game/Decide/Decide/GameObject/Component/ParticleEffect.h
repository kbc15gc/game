#pragma once
#include "fbEngine\_Object\_GameObject\ParticleEmitter.h"
#include "fbEngine\_Object\_Component\Component.h"

//�p�[�e�B�N�����g�����G�t�F�N�g�𔭐�������N���X�B
class ParticleEffect :public Component
{
public:
	ParticleEffect(GameObject* g, Transform* t) :Component(g, t, typeid(this).name()) {
#ifdef _DEBUG
		mbstowcs_s(nullptr, name, typeid(*this).name(), strlen(typeid(*this).name()));
#endif

	};
	~ParticleEffect() {};
public:
	void Awake()override;

	void Update()override;

	//HP�񕜂̃G�t�F�N�g�𔭐�������B
	//�����̓G�t�F�N�g�𔭐��������_�B
	void HeelHpEffect();

	//MP�񕜂̃G�t�F�N�g�𔭐�������B
	//�����̓G�t�F�N�g�𔭐��������_�B
	void HeelMpEffect();

	//�o�t�̃G�t�F�N�g�𔭐�������B
	//�����̓G�t�F�N�g�𔭐��������_�B
	void BuffEffect();

	//�f�o�t�̃G�t�F�N�g�𔭐�������B
	//�����̓G�t�F�N�g�𔭐��������_�B
	void DeBuffEffect();

	//�u���ۂ��G�t�F�N�g�𔭐�������B
	//�����̓G�t�F�N�g�𔭐��������_�B
	void FireFly();
	
	//Hp�񕜃G�t�F�N�g�𔭐������邩�ǂ����̃t���O�B
	//ture�Ŕ����Bfalse�Ŕ��������Ȃ��B
	void SetHeelHpEffectFlag(bool flag) {
		_HeelHpParticleEmitter->SetEmitFlg(flag);
		_HeelParticleAssistEmitter->SetEmitFlg(flag);
		_IsHeelHpFlag = flag;
	}


	//Mp�񕜃G�t�F�N�g�𔭐������邩�ǂ����̃t���O�B
	//ture�Ŕ����Bfalse�Ŕ��������Ȃ��B
	void SetHeelMpEffectFlag(bool flag) {
		_HeelMpParticleEmitter->SetEmitFlg(flag);
		_HeelParticleAssistEmitter->SetEmitFlg(flag);
		_IsHeelMpFlag = flag;
	}

	//�o�t�𔭐������邩�ǂ����̃t���O�B
	//ture�Ŕ����Bfalse�Ŕ��������Ȃ��B
	void SetBuffEffectFlag(bool flag) {
		_BuffParticleEmitter->SetEmitFlg(flag);
	}

	//�f�o�t�𔭐������邩�ǂ����̃t���O�B
	//ture�Ŕ����Bfalse�Ŕ��������Ȃ��B
	void SetDebuffEffectFlag(bool flag) {
		_DebuffParticleEmitter->SetEmitFlg(flag);
	}

	//�u�𔭐������邩�ǂ����̃t���O�B
	//ture�Ŕ����Bfalse�Ŕ��������Ȃ��B
	void SetFireFlyEffectFlag(bool flag) {
		_FireFlyParticleEmitter->SetEmitFlg(flag);
	}
private:
	ParticleEmitter*	_BuffParticleEmitter = nullptr;			//�o�t�p�[�e�B�N���������B
	ParticleParameter	_BuffParticleParam;						//�o�t�p�[�e�B�N���p�����[�^�[�B
	ParticleEmitter*	_DebuffParticleEmitter = nullptr;		//�f�o�t�p�[�e�B�N���������B
	ParticleParameter	_DebuffParticleParam;					//�f�o�t�p�[�e�B�N���p�����[�^�[�B
	ParticleParameter	_HeelHpParticleParam;					//Hp�񕜃p�[�e�B�N���p�����[�^�[�B
	ParticleEmitter*	_HeelHpParticleEmitter = nullptr;		//Hp�񕜃G�t�F�N�g�p�[�e�B�N���̔������B
	ParticleEmitter*	_HeelParticleAssistEmitter = nullptr;	//�񕜃G�t�F�N�g��⏕����p�[�e�B�N���̔������B	
	ParticleParameter	_HeelParticleAssistParam;				//�񕜃G�t�F�N�g��⏕����p�[�e�B�N���p�����[�^�[�B
	ParticleParameter	_HeelMpParticleParam;					//Mp�񕜃p�[�e�B�N���p�����[�^�[�B
	ParticleEmitter*	_HeelMpParticleEmitter = nullptr;		//Mp�񕜃G�t�F�N�g�p�[�e�B�N���̔������B
	bool				_IsHeelHpFlag;							//Hp�񕜃G�t�F�N�g���n�܂����B
	float				_TotalHeelHpEffectTime = 0.0f;			//Hp�񕜃G�t�F�N�g���n�܂��ĉ��b�o�������̍��v�B
	bool				_IsHeelMpFlag;							//Mp�񕜃G�t�F�N�g���n�܂����B
	float				_TotalHeelMpEffectTime = 0.0f;			//Mp�񕜃G�t�F�N�g���n�܂��ĉ��b�o�������̍��v�B
	ParticleEmitter*	_FireFlyParticleEmitter = nullptr;		//�u�݂����ȃp�[�e�B�N���������B
	ParticleParameter	_FireFlyParticleParam;					//�u�݂����ȃo�t�p�[�e�B�N���p�����[�^�[�B
};

