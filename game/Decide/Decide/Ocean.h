/**
* �C�N���X�̒�`.
*/
#pragma once

/**
* �C�N���X.
*/
class Ocean : public GameObject
{
public:

	/**
	* �R���X�g���N�^.
	*/
	Ocean(const char* name) :GameObject(name)
	{
	}

	/**
	* �f�X�g���N�^.
	*/
	~Ocean()
	{
	}

	/**
	* ���߂̏�����.
	*/
	void Awake()override;

	/**
	* ������.
	*/
	void Start()override;

	/**
	* �X�V.
	*/
	void Update()override;

	/**
	* �`��.
	*/
	void Render()override;

private:

	/** ���_�������N���X. */
	Vertex* _Vertex = nullptr;

	/** �G�t�F�N�g�N���X. */
	Effect* _Effect = nullptr;

	/** �e�N�X�`��. */
	TEXTURE* _Texture = nullptr;

};