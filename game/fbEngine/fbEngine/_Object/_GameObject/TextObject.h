#pragma once
#include "GameObject.h"
#include "_Object\_Component\_2D\Text.h"

//�e�L�X�g��\������Q�[���I�u�W�F�N�g
class TextObject :public GameObject
{
public:
	TextObject(char* name) :GameObject(name) {}
	void Awake()override;
	//�ꊇ�Őݒ�
	void Initialize(const wchar_t* string, const float& size,const Color& color = Color::white,
		const fbSprite::SpriteEffectE& flg = fbSprite::SpriteEffectE::NONE, const char* style = "�l�r ����", const unsigned int& format = (int)fbText::TextFormatE::CENTER);

	void SetString(const wchar_t* ws);
	void SetString(const char* s);
	void SetFontSize(const float& size);
	void SetFontStyle(const char* style);
	void SetBlendColor(const Color& c);
	void SetEffectFlg(const fbSprite::SpriteEffectE& e);
	void SetEffectFlg(const fbSprite::SpriteEffectE& e, const bool& f);
	//�����ݒ�
	void SetFormat(const unsigned int& format);
	void SetFormat(fbText::TextFormatE format);
	//�J�[�j���O�ݒ�
	void SetKerning(const bool& kerning);
	void SetCharNum(const unsigned int& num)
	{
		_Text->SetCharNum(num);
	}
	const Vector2& GetLength();
	const unsigned int& GetMaxCharNum()
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
