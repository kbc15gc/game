/**
* SSAO�N���X�̒�`.
*/
#pragma once

/**
* SSAO�N���X.
*/
class SSAO : Noncopyable
{
public:

	/**
	* �R���X�g���N�^.
	*/
	SSAO()
	{
	}

	/**
	* �f�X�g���N�^.
	*/
	~SSAO()
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
	* �L���t���O��ݒ�.
	*/
	void SetEnable(bool value)
	{
		_isEnable = value;
	}

	/**
	* �@���������ݗp�����_�����O�^�[�Q�b�g�̎擾.
	*/
	RenderTarget* GetNormalRenderTarget()
	{
		return &_NormalRT;
	}

private:

	/** �L���t���O. */
	bool _isEnable = true;

	/** �@���������ݗp�����_�����O�^�[�Q�b�g. */
	RenderTarget _NormalRT;

	/** ���_�������N���X. */
	Vertex* _Vertex = nullptr;

	/** �G�t�F�N�g�N���X. */
	Effect* _Effect = nullptr;

};