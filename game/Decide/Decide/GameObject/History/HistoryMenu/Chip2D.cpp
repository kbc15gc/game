/**
* ヒストリーメニューで表示されるチップクラスの実装.
*/
#include"stdafx.h"
#include"Chip2D.h"

#include"fbEngine\_Object\_Component\_2D\Sprite.h"

namespace
{
	/**
	* サイズ.
	*/
	static const Vector2 SizeData[(int)Chip2D::SizeCodeE::SizeCodeNum] =
	{
		Vector2(117.0f, 192.0f),
		Vector2(78.0f, 128.0f),
	};
}

/**
* 初期化.
*/
void Chip2D::Start(ChipID chipID)
{
	//スプライトクラスを追加.
	_ChipSprite = AddComponent<Sprite>();

	_ChipID = chipID;

	//テクスチャを設定.
	_ChipSprite->SetTexture(LOADTEXTURE((char*)ChipFileName[(int)_ChipID].c_str()));

	SetActive(false);

	_ChipSprite->SetPivot(Vector2(0.5f, 1.0f));

	//サイズを設定.
	_ChipSprite->SetSize(SizeData[(int)SizeCodeE::NoSelect]);

}

/**
* サイズを設定.
*/
void Chip2D::SetSize(SizeCodeE size)
{
	_ChipSprite->SetSize(SizeData[(int)size]);
}
