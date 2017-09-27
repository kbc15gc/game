/**
* �q�X�g���[���j���[�ŕ\�������`�b�v�N���X�̒�`.
*/
#pragma once

#include"..\HistoryInfo.h"
#include"fbEngine\_Object\_Component\_2D\Sprite.h"


/**
* �q�X�g���[���j���[�ŕ\�������`�b�v�N���X.
*/
class Chip2D : public GameObject
{
public:

	/**
	* �T�C�Y�R�[�h.
	*/
	enum class SizeCodeE
	{
		Select,		//!< �I��.
		NoSelect,	//!< �I���O.
		SizeCodeNum,	//!< �T�C�Y��.
	};



public:

	/**
	* �R���X�g���N�^.
	*/
	Chip2D(const char* name) : 
		GameObject(name)
	{
	}

	/**
	* �f�X�g���N�^.
	*/
	~Chip2D()
	{
	}

	/**
	* ������.
	*/
	void Start(ChipID chipID);

	/**
	* �X�V.
	*/
	void Update()override;

	/**
	* �T�C�Y��ݒ�.
	*/
	void SetMove(SizeCodeE size, Vector3 pos)
	{
		if (!isMove)
		{
			_DestSize = SizeData[(int)size];
			_Size = _ChipSprite->GetSize();
			_DestPos = pos;
			_Pos = transform->GetPosition();
			_LerpRate = 0.0f;
			isMove = true;
		}
	}

	ChipID GetChipID()
	{
		return _ChipID;
	}

	void SetSize(SizeCodeE size)
	{
		_ChipSprite->SetSize(SizeData[(int)size]);
	}

private:

	/**
	* �T�C�Y.
	*/
	const Vector2 SizeData[(int)Chip2D::SizeCodeE::SizeCodeNum] =
	{
		Vector2(117.0f, 192.0f),
		Vector2(78.0f, 128.0f),
	};

	/** �`�b�v�\���N���X. */
	Sprite* _ChipSprite = nullptr;

	/** �`�b�vID. */
	ChipID _ChipID = ChipID::None;

	Vector3 _Pos = Vector3::zero;
	/** �ړI���W. */
	Vector3 _DestPos = Vector3::zero;
	Vector2 _Size = Vector2(0.0f, 0.0f);
	/** �ړI�T�C�Y. */
	Vector2 _DestSize = Vector2(0.0f, 0.0f);
	/** ��ԃ��[�g. */
	float _LerpRate = 0.0f;

	bool isMove = true;

};