/**
* �q�X�g���[���j���[�ŕ\�������`�b�v�N���X�̎���.
*/
#include"stdafx.h"
#include"Chip2D.h"


/**
* ������.
*/
void Chip2D::Start(ChipID chipID)
{
	//�X�v���C�g�N���X��ǉ�.
	_ChipSprite = AddComponent<Sprite>();

	_ChipID = chipID;

	//�e�N�X�`����ݒ�.
	_ChipSprite->SetTexture(LOADTEXTURE((char*)ChipFileName[(int)_ChipID].c_str()));

	SetActive(false);

	_ChipSprite->SetPivot(Vector2(0.5f, 1.0f));

	//�T�C�Y��ݒ�.
	_ChipSprite->SetSize(SizeData[(int)SizeCodeE::NoSelect]);

}

/**
* �X�V.
*/
void Chip2D::Update()
{
	if (isMove)
	{
		static float SPEED = 2.0f;
		_LerpRate += SPEED * Time::DeltaTime();
		_LerpRate = min(1.0f, _LerpRate);
		if (_LerpRate >= 1.0f)
		{
			isMove = false;
		}

		Vector3 dest = _DestPos;
		dest.Scale(_LerpRate);
		Vector3 pos = _Pos;
		pos.Scale(1.0f - _LerpRate);

		//���W��ݒ�.
		transform->SetPosition(dest + pos);

		Vector2 destSize = _DestSize;
		destSize *= _LerpRate;
		Vector2 size = _Size;
		_Size *= (1.0f - _LerpRate);

		_ChipSprite->SetSize(_Size + destSize);
	}
}

