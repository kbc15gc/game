#pragma once
#include "GameObject.h"
#include "_Object\_Component\_2D\Text.h"

//テキストを表示するゲームオブジェクト
class TextObject :public GameObject
{
public:
	TextObject(char* name) :GameObject(name) {}
	void Awake()override;
	//一括で設定
	void Initialize(const wchar_t* string, float size,const Color& color = Color::white,
		fbSprite::SpriteEffectE flg = fbSprite::SpriteEffectE::NONE, const char* style = "ＭＳ 明朝", fbText::TextAnchorE anchor = fbText::TextAnchorE::MiddleCenter);

	void SetText(const wchar_t* ws);
	void SetText(const char* s);

	void SetFontSize(float size);
	void SetFontStyle(const char* style);
	void SetBlendColor(const Color& c);
	void SetEffectFlg(fbSprite::SpriteEffectE e);
	void SetEffectFlg(fbSprite::SpriteEffectE e, bool f);
	//書式設定
	void SetAnchor(fbText::TextAnchorE format);
	//カーニング設定
	void SetKerning(bool kerning);
	void SetCharNum(unsigned int num)
	{
		_Text->SetCharNum(num);
	}
	const Vector2 GetLength();
	unsigned int GetMaxCharNum()
	{
		return _Text->GetMaxCharNum();
	}
	inline void SetSpacing(const float spacing) {
		_Text->SetSpacing(spacing);
	}
	Text* GetText()
	{
		return _Text;
	}
private:
	Text* _Text;
};
