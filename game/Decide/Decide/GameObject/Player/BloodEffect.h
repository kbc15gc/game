/**
* ���t�G�t�F�N�g�N���X�̒�`.
*/
#pragma once

#include"fbEngine\_Object\_GameObject\ImageObject.h"

/**
* ���t�C���[�W�N���X.
*/
class BloodImage : public GameObject
{
public:

	/**
	* �R���X�g���N�^.
	*/
	BloodImage(char* name) :
		GameObject(name)
	{
	}

	/**
	* �f�X�g���N�^.
	*/
	~BloodImage()
	{
	}

	/**
	* ������.
	*/
	void Awake()override;

	/**
	* �X�V.
	*/
	void LateUpdate()override;

	/**
	* �L���ɂȂ���.
	*/
	void OnEnable()override;

private:

	Sprite* _Image = nullptr;

	float _LimitTime = 1.0f;
	float _LocalTime = 0.0f;

	float _Alpha = 1;

};

/**
* ���t�G�t�F�N�g�N���X.
*/
class BloodEffect : public GameObject
{
public:

	/**
	* �R���X�g���N�^.
	*/
	BloodEffect(char* name) :
		GameObject(name)
	{
	}

	/**
	* �f�X�g���N�^.
	*/
	~BloodEffect()
	{
	}

	/**
	* ������.
	*/
	void Awake()override;

	/**
	* �_���[�W�󂯂�.
	*/
	void Damage(int maxHp, int damage);

	void OnDisable()override
	{
		AllOff();
	}

	void AllOff()
	{
		for (auto bi : _BloodImageList)
		{
			bi->SetActive(false);
		}
	}

private:

	static const int BIMaxSize = 20;
	/**
	* ���X�g.
	*/
	BloodImage* _BloodImageList[BIMaxSize] = { nullptr };

};