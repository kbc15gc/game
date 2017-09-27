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

	//�񕜂̃G�t�F�N�g�𔭐�������B
	//�����̓G�t�F�N�g�𔭐��������_�B
	void HeelEffect(Transform* parent);

	//�o�t�̃G�t�F�N�g�𔭐�������B
	//�����̓G�t�F�N�g�𔭐��������_�B
	void BuffEffect(Transform* parent);

	//�f�o�t�̃G�t�F�N�g�𔭐�������B
	//�����̓G�t�F�N�g�𔭐��������_�B
	void DeBuffEffect(Transform* parent);

	//�u���ۂ��G�t�F�N�g�𔭐�������B
	//�����̓G�t�F�N�g�𔭐��������_�B
	void FireFly(Transform* parent);
	
	//�񕜃G�t�F�N�g�𔭐������邩�ǂ����̃t���O�B
	//ture�Ŕ����Bfalse�Ŕ��������Ȃ��B
	void SetHeelEffectFlag(bool flag) {
		_HeelParticleEmitter->SetEmitFlg(flag);
		_HeelParticleAssistEmitter->SetEmitFlg(flag);
		_IsHeelFlag = flag;
	}

	//�o�t�f�o�t�𔭐������邩�ǂ����̃t���O�B
	//ture�Ŕ����Bfalse�Ŕ��������Ȃ��B
	void SetEffectFlag(bool flag) {
		_ParticleEmitter->SetEmitFlg(flag);
	}

private:
	ParticleEmitter*	_ParticleEmitter = nullptr;				//�񕜃p�[�e�B�N���������B
	ParticleParameter	_HeelParticleParam;						//�񕜃p�[�e�B�N���p�����[�^�[�B
	ParticleEmitter*	_HeelParticleEmitter = nullptr;			//�񕜃G�t�F�N�g��⏕����p�[�e�B�N���̔������B
	ParticleEmitter*	_HeelParticleAssistEmitter = nullptr;	//�񕜃G�t�F�N�g��⏕����p�[�e�B�N���̔������B
	ParticleParameter	_ParticleParam;							//�p�[�e�B�N���p�����[�^�[�B
	ParticleParameter	_HeelParticleAssistParam;				//�񕜃G�t�F�N�g��⏕����p�[�e�B�N���p�����[�^�[�B
	bool				_IsHeelFlag;							//�񕜃G�t�F�N�g���n�܂����B
	float				_TotalHeelEffectTime = 0.0f;			//�񕜃G�t�F�N�g���n�܂��ĉ��b�o�������̍��v�B
};

