/**
* ヒストリーメニューで表示されるチップクラスの実装.
*/
#include"stdafx.h"
#include"Chip2D.h"


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
* 更新.
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

		//座標を設定.
		transform->SetPosition(dest + pos);

		Vector2 destSize = _DestSize;
		destSize *= _LerpRate;
		Vector2 size = _Size;
		_Size *= (1.0f - _LerpRate);

		_ChipSprite->SetSize(_Size + destSize);
	}
}

