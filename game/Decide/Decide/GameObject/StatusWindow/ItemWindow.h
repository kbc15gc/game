/**
* �A�C�e���\����ʃN���X�̒�`.
*/
#pragma once

#include"fbEngine\_Object\_GameObject\ImageObject.h"
#include"fbEngine\_Object\_GameObject\TextObject.h"

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

private:

	/** �A�C�e���`�惊�X�g. */
	vector<TextObject*> _ItemTextList;

};