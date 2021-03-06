#pragma once
#include "GameObject.h"
#include "_Object\_Component\_2D\Sprite.h"

//画像を表示するゲームオブジェクト
class ImageObject :public GameObject
{
public:
	ImageObject(char* name) :GameObject(name) {}
	void Awake()override;

	void SetTexture(TEXTURE* t);
	void SetSize(const Vector2& size);
	const Vector2& GetSize();
	TEXTURE* GetTexture();
	void SetPivot(const Vector2& v);
	void SetPivot(const float& x, const float& y);
	//混ぜる色設定
	void SetBlendColor(const Color& c);
	//削除する色設定
	void SetClipColor(const Color& c);
	//そのままセット
	void SetEffectFlg(const fbSprite::SpriteEffectE& e);
	//trueなら足す、falseなら引く
	void SetEffectFlg(const fbSprite::SpriteEffectE& e,bool f);
	//両面描画フラグセット
	void SetDoubleSided(const bool& f);

	void SetUV(const Vector4& uv)
	{
		_Sprite->SetUV(uv);
	}
private:
	Sprite* _Sprite;
};
