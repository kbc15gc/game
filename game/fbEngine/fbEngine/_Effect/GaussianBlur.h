/**
* �K�E�V�A���u���[�N���X�̒�`.
*/
#pragma once

/**
* �K�E�V�A���u���[�N���X.
*/
class GaussianBlur : Noncopyable
{
public:

	/** �E�G�C�g�̎��. */
	enum WeightType
	{
		Weight_2,	//!< �E�G�C�g2�{
		Weight_4,	//!< �E�G�C�g4�{
		Weight_6,	//!< �E�G�C�g6�{
		Weight_8,	//!< �E�G�C�g8�{
		Weight_Num	//!< ��ސ�.
	};

public:

	/**
	* �R���X�g���N�^.
	*/
	GaussianBlur()
	{
	}

	/**
	* �f�X�g���N�^.
	*/
	~GaussianBlur()
	{
	}

	/**
	* ������.
	*/
	void Create(int w, int h, const TEXTURE& srcTexture);
	void Create(int w, int h, _D3DFORMAT fmt);

	/**
	* �\�[�X�e�N�X�`����ݒ�.
	*/
	void SetSrcTexture(const TEXTURE& texture)
	{
		_SrcTexture = &texture;
	}

	/**
	* �`��.
	*/
	void Render();

	TEXTURE* GetTexture() const
	{
		return _BlurRT[1].texture;
	}
	const RenderTarget& GetRenderTarget() const
	{
		return _BlurRT[1];
	}

	/**
	* �u���[�̋��x��ݒ�.
	*/
	void SetBlurPower(float blurPower)
	{
		_BlurPower = blurPower;
	}
	
	/**
	* �g�p����E�F�C�g�̐���ݒ�.
	*/
	void SetUseWeights(WeightType w)
	{
		_WeightType = w;
	}

private:

	/**
	* �d�݂��v�Z.
	*/
	void UpdateWeight(float dispersion);

private:

	/** ���_�������N���X. */
	Vertex* _Vertex = nullptr;

	/** �u���[���ʂ��������ރ����_�����O�^�[�Q�b�g. */
	RenderTarget _BlurRT[2];

	/** �G�t�F�N�g. */
	Effect* _Effect = nullptr;

	/** �e�N�X�`��. */
	const TEXTURE* _SrcTexture = nullptr;
	/** �e�N�X�`���̃T�C�Y. */
	int _SrcTextureSize[2];

	/** �E�G�C�g�̎��. */
	WeightType _WeightType;
	/** �E�G�C�g�̍ő吔. */
	static const int MAX_WEIGHT = 8;
	/** �E�G�C�g�z��. */
	float _Weight[MAX_WEIGHT];
	/** �u���[�̋���. */
	float _BlurPower = 0;

};