/**
* �A���`�G�C���A�X�N���X�̒�`.
*/
#pragma once

class Vertex;

/**
* �A���`�G�C���A�X�N���X.
*/
class AntiAliasing : Noncopyable
{
public:

	/**
	* �R���X�g���N�^.
	*/
	AntiAliasing()
	{
	}

	/**
	* �f�X�g���N�^.
	*/
	~AntiAliasing()
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

private:

	/** �L���t���O. */
	bool _isEnable = true;

	/** ���_�������N���X. */
	Vertex* _Vertex = nullptr;

	/** �G�t�F�N�g�N���X. */
	Effect* _Effect = nullptr;

};