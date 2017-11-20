/**
* �V���h�E�}�b�v�N���X�̒�`.
*/
#pragma once

#include"_Effect\Blur.h"
#include"_Object\_Component\_3D\Camera.h"

/**
* �V���h�E�}�b�v�N���X.
*/
class ShadowMap : Noncopyable
{
public:

	/** �V���h�E�}�b�v�e�N�X�`���̐�. */
	static const int SHADOWMAP_NUM = 3;

	/**
	* �V���h�E���V�[�o�[�p�p�����[�^.
	*/
	struct ShadowReceiverParam
	{
	public:

		/** ���C�g�r���[�v���W�F�N�V�����s��. */
		D3DXMATRIX _LVPMatrix[SHADOWMAP_NUM];
		/** �V���h�E�}�b�v�e�N�X�`���̐�. */
		int _ShadowMapNum = SHADOWMAP_NUM;
		/** �o���A���X�V���h�E�}�b�v�����邩. */
		int _IsVSM = true;
	};

public:

	/**
	* �R���X�g���N�^.
	*/
	ShadowMap();

	/**
	* �f�X�g���N�^.
	*/
	~ShadowMap()
	{
	}

	/**
	* �쐬.
	*/
	void Create();

	/**
	* �X�V.
	*/
	void Update();

	/**
	* �`��.
	*/
	void Render();

	/**
	* ���.
	*/
	void Release();

	/**
	* �L���X�^�[��o�^����.
	*
	* @param model	�X�L�����f���f�[�^.
	*/
	void EntryModel(SkinModel* model)
	{
		_CasterModelList.push_back(model);
	}

	/**
	* ���C�g�̎��_��ݒ�.
	*/
	void SetLightPosition(const Vector3& pos)
	{
		_LightPosition = pos;
	}

	/**
	* ���C�g�̒����_��ݒ�.
	*/
	void SetLightTarget(const Vector3& target)
	{
		_LightTarget = target;
	}

	/** 
	* ���C�g�r���[�s��̎擾.
	*/
	D3DXMATRIX* GetLVMatrix()
	{
		return &_LVMatrix[_NowRenderShadowMap];
	}

	/**
	* ���C�g�v���W�F�N�V�����s��̎擾.
	*/
	D3DXMATRIX* GetLPMatrix()
	{
		return &_LPMatrix[_NowRenderShadowMap];
	}

	/**
	* �V���h�E���V�[�o�[�p�p�����[�^�̎擾.
	*/
	ShadowReceiverParam& GetShadowReceiverParam()
	{
		return _ShadowReceiverParam;
	}

	/**
	* �[�x�������݃e�N�X�`���̎擾.
	*/
	IDirect3DTexture9* GetShadowMapTexture(int idx,bool isVSM = true)
	{
		if (isVSM)
		{
			return _Blur[idx].GetTexture()->pTexture;
		}
		return _ShadowMapRT[idx].texture->pTexture;
	}

	TEXTURE* GetTexture(int idx, bool isVSM = true)
	{
		if (isVSM)
		{
			return _Blur[idx].GetTexture();
		}
		return _ShadowMapRT[idx].texture;
	}

	///**
	//* �ꖇ�ڂ̃e�N�X�`���̃T�C�Y���擾.
	//*/
	//const Vector2& GetSize()
	//{
	//	return _ShadowArea[0];
	//}

	/**
	* �J�����̃|�C���^��ݒ�.
	*/
	void SetCamera(Camera* camera)
	{
		_Camera = camera;
	}

	void SetNear(float n)
	{
		_Near = n;
	}

	void SetFar(float f)
	{
		_Far = f;
	}

private:

	/**
	* ���C�g�r���[�v���W�F�N�V�����s����v�Z.
	*/
	void CalcLVPMatrixFromCamera();

private:

	/** �[�x�p�����_�����O�^�[�Q�b�g. */
	RenderTarget _ShadowMapRT[SHADOWMAP_NUM];

	/** ���ݕ`�撆�̃����_�����O�^�[�Q�b�g. */
	int _NowRenderShadowMap = -1;

	/** �V���h�E�L���X�^�[�̃��X�g. */
	vector<SkinModel*> _CasterModelList;

	/** ���C�g�̎��_. */
	Vector3 _LightPosition = Vector3::up;
	/** ���C�g�̕���. */
	Vector3 _LightDirection = Vector3::down;
	/** ���C�g�̒����_. */
	Vector3 _LightTarget = Vector3::zero;

	/** �ߕ���. */
	float _Near = 2.0f;
	/** ������. */
	float _Far = 100.0f;
	/** �A�X�y�N�g��. */
	float _Aspect = 1.0f;

	/** ���C�g�r���[�s��. */
	D3DXMATRIX _LVMatrix[SHADOWMAP_NUM];
	/** ���C�g�v���W�F�N�V�����s��. */
	D3DXMATRIX _LPMatrix[SHADOWMAP_NUM];

	/** �V���h�E���V�[�o�[�p�̃p�����[�^. */
	ShadowReceiverParam _ShadowReceiverParam;

	/** �u���[�N���X. */
	Blur _Blur[SHADOWMAP_NUM];

	/** �J�����N���X�̃|�C���^. */
	Camera* _Camera = nullptr;

};