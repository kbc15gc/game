#pragma once
#include "GameObject.h"
#include "Text.h"

//�e�L�X�g��\������Q�[���I�u�W�F�N�g
class TextObject :public GameObject
{
public:
	TextObject(char* name) :GameObject(name) {}
	void Awake()override;
	//�ꊇ�Őݒ�
	void Initialize(const wchar_t* string, const float& size,const Color& color = Color::white,
		const fbSprite::SpriteEffectE& flg = fbSprite::SpriteEffectE::NONE, const char* style = "�l�r ����", fbText::TextFormatE format = fbText::TextFormatE::CENTER);

	void SetString(const wchar_t* ws);
	void SetFontSize(const float& size);
	void SetFontStyle(const char* style);
	void SetBlendColor(const Color& c);
	void SetEffectFlg(const fbSprite::SpriteEffectE& e);
	void SetEffectFlg(const fbSprite::SpriteEffectE& e, const bool& f);
	//�����ݒ�
	void SetFormat(fbText::TextFormatE format);
private:
	Text* _Text;
};
