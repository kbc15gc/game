#include"fbstdafx.h"
#include "TextObject.h"

void TextObject::Awake()
{
	_Text = AddComponent<Text>();
}

void TextObject::Initialize(const wchar_t * string, float size, const Color& color, fbSprite::SpriteEffectE flg , const char * style, fbText::TextAnchorE anchor)
{
	_Text->Initialize(string, size, color, flg, style, anchor);
}

void TextObject::SetText(const wchar_t* ws)
{
	_Text->SetText(ws);
}

void TextObject::SetText(const char * s)
{
	_Text->SetText(s);
}

void TextObject::SetFontSize(float size)
{
	_Text->SetSize(size);
}

void TextObject::SetFontStyle(const char* style)
{
	_Text->SetStyle(style);
}

void TextObject::SetBlendColor(const Color& c)
{
	_Text->SetBlendColor(c);
}

void TextObject::SetEffectFlg(fbSprite::SpriteEffectE e)
{
	_Text->SetEffectFlg(e);
}

void TextObject::SetEffectFlg(fbSprite::SpriteEffectE e, bool f)
{
	_Text->SetEffectFlg(e,f);
}

void TextObject::SetAnchor(fbText::TextAnchorE anchor)
{
	_Text->SetAnchor(anchor);
}

void TextObject::SetKerning(bool kerning)
{
	_Text->SetKerning(kerning);
}

const Vector2 TextObject::GetLength()
{
	return _Text->GetLength();
}
