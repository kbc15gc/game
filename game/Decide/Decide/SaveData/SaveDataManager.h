/**
* �Z�[�u�f�[�^�Ǘ��N���X�̒�`.
*/
#pragma once

/**
* �Z�[�u�f�[�^�Ǘ��N���X.
*/
class SaveDataManager
{
private:

	/**
	* �R���X�g���N�^.
	*/
	SaveDataManager()
	{
	}
	
	/**
	* �f�X�g���N�^.
	*/
	~SaveDataManager()
	{
	}

public:

	/**
	* �C���X�^���X���擾.
	*/
	static SaveDataManager& Instance()
	{
		static SaveDataManager instance;
		return instance;
	}

private:

	/** �R���e�B�j���[�t���O. */
	bool _IsContinue = false;

};
