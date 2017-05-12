/**
* DOF(��ʊE�[�x)�N���X�̒�`.
*/
#pragma once

#include"_Effect\Blur.h"

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

	/**
	* �[�x�������ݗp�����_�����O�^�[�Q�b�g�̎擾.
	*/
	RenderTarget* GetDepthRenderTarget()
	{
		return &_DepthRT;
	}

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
	* �I�_������ݒ�.
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

private:

	/** �L���t���O. */
	bool _isEnable = true;

	/** �[�x�������ݗp�����_�����O�^�[�Q�b�g. */
	RenderTarget _DepthRT;

	Vertex* _Vertex = nullptr;

	/** �O�{�P�p�u���[. */
	Blur _BlurForward;
	/** ���{�P�p�u���[. */
	Blur _BlurBack;

	/** �G�t�F�N�g. */
	Effect* _Effect = nullptr;

	/** �œ_����. */
	float _FocalLength = 0.0f;
	/** F�l. */
	float _F = 0.0f;
	/** �s���g. */
	float _Pint = 0.0f;

};