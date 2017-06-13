/**
* ���}�b�v�N���X�̒�`.
*/
#pragma once

/**
* ���}�b�v�N���X.
*/
class EnvironmentMap : Noncopyable
{
public:

	/**
	* �R���X�g���N�^.
	*/
	EnvironmentMap()
	{
	}

	/**
	* �f�X�g���N�^.
	*/
	~EnvironmentMap()
	{
	}

	/**
	* �쐬.
	*/
	bool Create();

	/**
	* �X�V.
	*/
	void Update();

	/**
	* �`��.
	*/
	void Render();

	/**
	* ���}�b�v�ɏ������ރ��f���̒ǉ�.
	*/
	void EntryModel(SkinModel* model)
	{
		_ModelList.push_back(model);
	}

	/**
	* �r���[�s����擾.
	*/
	const D3DXMATRIX& GetViewMatrix()
	{
		return _ViewMatrix;
	}

	/**
	* �v���W�F�N�V�����s����擾.
	*/
	const D3DXMATRIX& GetProjMatrix()
	{
		return _ProjMatrix;
	}

	/**
	* �L���[�u�e�N�X�`�����擾.
	*/
	IDirect3DCubeTexture9* GetCubeTexture()
	{
		return _D3DCubeTexture;
	}

private:

	/** �L���[�u�e�N�X�`��. */
	IDirect3DCubeTexture9* _D3DCubeTexture = nullptr;

	/** ���f�����X�g. */
	vector<SkinModel*> _ModelList;

	/** �r���[�s��. */
	D3DXMATRIX _ViewMatrix;
	/** �v���W�F�N�V�����s��. */
	D3DXMATRIX _ProjMatrix;

	/** �`��t���O. */
	bool _isRender = true;

	/** ���b�Ɉ�x�X�V���邩. */
	float _RenderTime = 5.0f;
	float _LocalTime = 0.0f;

};
