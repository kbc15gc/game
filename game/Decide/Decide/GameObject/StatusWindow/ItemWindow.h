/**
* �A�C�e���\����ʃN���X�̒�`.
*/
#pragma once

/**
* �A�C�e���\����ʃN���X.
*/
class ItemWindow : public GameObject
{
public:

	/**
	* �R���X�g���N�^.
	*/
	ItemWindow(char* name): 
		GameObject(name)
	{
	}

	/**
	* �f�X�g���N�^.
	*/
	~ItemWindow()
	{
	}

	/**
	* ������.
	*/
	void Awake()override;

	/**
	* �X�V.
	*/
	void Update()override;

};