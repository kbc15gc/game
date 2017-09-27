#pragma once
#include "fbEngine\_Object\_GameObject\ParticleEmitter.h"
#include "fbEngine\_Object\_Component\Component.h"

//�p�[�e�B�N�����g�����G�t�F�N�g�𔭐�������N���X�B
class ParticleEffect :public Component
{
public:
	ParticleEffect(GameObject* g, Transform* t) :Component(g, t, typeid(this).name()) {
		Initialize();
#ifdef _DEBUG
		mbstowcs_s(nullptr, name, typeid(*this).name(), strlen(typeid(*this).name()));
#endif

	};
	~ParticleEffect() {};

public:
	//�G�t�F�N�g�Ŏg���G�~�b�^�[�ƃp�[�e�B�N���̏������B
	//�K���Ă�ŁB
	void Initialize();

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
	
	//�G�t�F�N�g�𔭐������邩�ǂ����̃t���O�B
	//ture�Ŕ����Bfalse�Ŕ��������Ȃ��B
	void SetEffectFlag(bool flag) {
		_ParticleEmitter->SetEmitFlg(flag);
	}

private:
	ParticleEmitter*	_ParticleEmitter = nullptr;				//�p�[�e�B�N���������B
	ParticleEmitter*	_HeelParticleAssistEmitter = nullptr;	//�񕜃G�t�F�N�g��⏕����p�[�e�B�N���̔������B
	ParticleParameter	_ParticleParam;							//�p�[�e�B�N���p�����[�^�[�B
	ParticleParameter	_HeelParticleAssistParam;				//�񕜃G�t�F�N�g��⏕����p�[�e�B�N���p�����[�^�[�B
};

