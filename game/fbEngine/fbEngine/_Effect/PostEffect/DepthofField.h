/**
* DOF(��ʊE�[�x)�N���X�̒�`.
*/
#pragma once

#include"_Effect\GaussianBlur.h"

class Vertex;
class Effect;

/**
* DOF(��ʊE�[�x)�N���X.
*/
class DepthofField : Noncopyable
{
public:

	/**
	* �R���X�g���N�^.
	*/
	DepthofField()
	{
	}
	
	/**
	* �f�X�g���N�^.
	*/
	~DepthofField()
	{
	}

	/**
	* �쐬.
	*/
	void Create();

	/**
	* �`��.
	*/
	void Render();

	/**
	* ���.
	*/
	void Release();

	TEXTURE* GetBlurForward()
	{
		return _BlurForward.GetTexture();
	}
	TEXTURE* GetBlurBack()
	{
		return _BlurBack.GetTexture();
	}

	/**
	* �s���g��ݒ�.
	*/
	void SetPint(float pint)
	{
		_Pint = pint;
	}

	/**
	* �œ_������ݒ�.
	*/
	void SetFocalLength(float len)
	{
		_FocalLength = len;
	}

	/**
	* F�l��ݒ�.
	*/
	void SetFParam(float f)
	{
		_F = f;
	}

	/**
	* �L���t���O��ݒ�.
	*/
	void SetEnable(bool value)
	{
		_isEnable = value;
	}

	/**
	* �u���[���x��ݒ�.
	*/
	void SetBlurPower(float value)
	{
		_BlurBack.SetBlurPower(value);
	}

private:

	/** �L���t���O. */
	bool _isEnable = true;

	Vertex* _Vertex = nullptr;

	/** �O�{�P�p�u���[. */
	GaussianBlur _BlurForward;
	/** ���{�P�p�u���[. */
	GaussianBlur _BlurBack;

	/** �{�P�����p�̃����_�����O�^�[�Q�b�g. */
	RenderTarget _CombineRenderTarget;

	/** �G�t�F�N�g. */
	Effect* _Effect = nullptr;

	/** �œ_����. */
	float _FocalLength = 26.0f;
	/** F�l. */
	float _F = 5.6f;
	/** �s���g. */
	float _Pint = 3.0f;

};