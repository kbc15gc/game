/**
* �e�X�g�I�u�W�F�N�g�N���X�̒�`.
*/
#pragma once

/**
* �e�X�g�I�u�W�F�N�g�N���X.
*/
class TestObject : public GameObject
{
public:

	/**
	* �R���X�g���N�^.
	*/
	TestObject(const char* name) :
		GameObject(name)
	{
	}

	/**
	* �f�X�g���N�^.
	*/
	~TestObject()
	{
	}

	/**
	* ������.
	*/
	void Awake()override;

private:

};