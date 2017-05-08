/**
* �u���[�N���X�̒�`.
*/
#pragma once

/**
* �u���[�N���X.
*/
class Blur : Noncopyable
{
public:

	/**
	* �R���X�g���N�^.
	*/
	Blur()
	{
	}

	/**
	* �f�X�g���N�^.
	*/
	~Blur()
	{
	}

	/**
	* �쐬.
	*
	* @param w		����.
	* @param h		�c��.
	* @param tex	�e�N�X�`���[.	.
	*/
	void Create(int w,int h,TEXTURE* tex);

	/**
	* �`��.
	*/
	void Render();

	/**
	* �e�N�X�`�����擾.
	*/
	TEXTURE* GetTexture()
	{
		return _BlurRT[1].texture;
	}

private:

	/** ���_�������N���X. */
	Vertex* _Vertex = nullptr;

	/** �u���[���ʂ��������ރ����_�����O�^�[�Q�b�g. */
	RenderTarget _BlurRT[2];

	/** �G�t�F�N�g. */
	Effect* _Effect = nullptr;

	/** �e�N�X�`��. */
	TEXTURE* _SrcTexture = nullptr;
	/** �e�N�X�`���̃T�C�Y. */
	int _SrcTextureSize[2];




};