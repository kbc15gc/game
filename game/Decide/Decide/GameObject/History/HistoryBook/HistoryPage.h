/**
* ���j���̃y�[�W�N���X�̒�`.
*/
#pragma once

/**
* ���j���̃y�[�W�N���X.
*/
class HistoryPage : public GameObject
{
public:

	/**
	* �R���X�g���N�^.
	*/
	HistoryPage(const char* name) : 
		GameObject(name)
	{
	}

	/**
	* �f�X�g���N�^.
	*/
	~HistoryPage()
	{
	}

	/**
	* �R���X�g���N�^��̏�����.
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

private:

	/** �y�[�W�̃��f��. */
	SkinModel* _Model = nullptr;

};