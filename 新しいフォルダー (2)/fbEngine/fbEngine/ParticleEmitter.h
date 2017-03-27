#pragma once
#include "GameObject.h"
class Particle;

//�p�[�e�B�N�������p�����[�^
struct ParicleParameter {
	//�������B
	void Init()
	{
		memset(this, 0, sizeof(ParicleParameter));
		size = Vector2(1, 1);
		initAlpha = 1.0f;
		brightness = 1.0f;
		isBillboard = true;
		mulColor = Color::white;
	}
	const char* texturePath;						//!<�e�N�X�`���̃t�@�C���p�X�B
	Vector3		initVelocity;						//!<�����x�B
	Vector2		size;								//�p�[�e�B�N���T�C�Y
	float		life;								//!<�����B�P�ʂ͕b�B
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
};
	
//�p�[�e�B�N���̔����@	
class ParticleEmitter : public GameObject {
public:
	ParticleEmitter(char* name) :GameObject(name) {};
	~ParticleEmitter() {};
	/*!
	 *@brief	�������B
	 *@_Param[in]	random		���������Ɏg�p���闐�������@�B
	 *@_Param[in]	camera		�p�[�e�B�N���̕`��Ŏg�p����J�����B
	 *@_Param[in]	_Param		�p�[�e�B�N�������p�̃p�����[�^�B
	 *@_Param[in]	emitPosition	�G�~�b�^�[�̍��W�B
	 */
	void Init(const ParicleParameter& _Param);
	void Start() override;
	void Update() override;
	void Render() override;
	/*!
	*@brief	�p�[�e�B�N���ɗ͂�������B
	*@_Param[in]	applyForce		���������Ɏg�p���闐�������@�B
	*/
	void ApplyForce(Vector3& applyForce);
	void SetEmitFlg(bool b);
private:
	//�p�[�e�B�N������
	void Emit();
private:
	bool	emit;//�����t���O
	float					_Timer;			//!<�^�C�}�[
	ParicleParameter		_Param;			//!<�p�����[�^�B
	std::list<Particle*>	_ParticleList;	//!<�p�[�e�B�N���̃��X�g�B
};