/**
* �q�X�g���[���j���[�ŕ\�������`�b�v�N���X�̎���.
*/
#include"stdafx.h"
#include"Chip2D.h"

#include"fbEngine\_Object\_Component\_2D\Sprite.h"

namespace
{
	/**
	* �T�C�Y.
	*/
	static const Vector2 SizeData[(int)Chip2D::SizeCodeE::SizeCodeNum] =
	{
		Vector2(117.0f, 192.0f),
		Vector2(78.0f, 128.0f),
	};
}

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
* �T�C�Y��ݒ�.
*/
void Chip2D::SetSize(SizeCodeE size)
{
	_ChipSprite->SetSize(SizeData[(int)size]);
}
