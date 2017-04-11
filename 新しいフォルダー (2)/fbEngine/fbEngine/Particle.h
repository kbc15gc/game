#pragma once
#include "GameObject.h"
#include "ParticleEmitter.h"

class Vertex;

//�p�[�e�B�N���N���X
class Particle :public GameObject
{
public:
	enum eState {
		eStateRun,
		eStateFadeOut,
		eStateDead,
	};
	Particle(char* name) :GameObject(name) {}
	void Awake()override;
	void Update()override;
	void Render() override;

	void Init(const ParicleParameter& param,const Vector3& emitPosition);
	/*!
	*@brief	�p�[�e�B�N���ɗ͂�������B
	*@param[in]	_ApplyForce		���������Ɏg�p���闐�������@�B
	*/
	void ApplyForce(Vector3& _ApplyForce)
	{
		this->_ApplyForce = _ApplyForce;
	}
	bool IsDead()
	{
		return _IsDead;
	}
private:		
	static Vertex* _Vertex;						//���_


	TEXTURE*		_Texture;					//�摜
	Effect*			_Effect;					//�V�F�[�_�[�G�t�F�N�g�B
	Camera*			_Camera;					//�J�����B
	float			_Life;						//���C�t�B
	float			_Timer;						//�^�C�}�[�B
	Vector3			_Velocity;					//���x�B
	Vector3			_Gravity;					//�d�́B
	float			_RotateZ;					//Z������̉�]�B
	Vector3			_AddVelocityRandomMargih;	//���x�̐ϕ��̂Ƃ��̃����_�����B
	bool			_IsDead;					//���S�t���O�B
	bool			_IsFade;					//���ʂƂ��Ƀt�F�[�h�A�E�g����H
	float			_FadeTIme;					//�t�F�[�h�̎��ԁB
	eState			_State;						//��ԁB
	float			_InitAlpha;					//�����A���t�@�B
	float			_Alpha;						//�A���t�@�B
	bool			_IsBillboard;				//�r���{�[�h�H
	Vector3			_ApplyForce;				//�O����������́B
	float			_Brightness;				//�P�x�B�u���[�����L���ɂȂ��Ă���Ƃ������������ƌ������܂��B
	int				_AlphaBlendMode;			//0�����������A1���Z�����B
	Color			_MulColor;					//��Z�J���[�B
};