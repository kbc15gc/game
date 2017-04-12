#pragma once
#include "Component.h"
#include "Sprite.h"

namespace fbText
{
	//�e�L�X�g�̏���
	enum class TextFormatE : unsigned int
	{
		LEFT,		//������
		CENTER,		//��������
		RIGHT		//�E����
	};
	//�悭�g����������Ȃ��t�H���g�̃X�^�C��
	enum class TextStyleE
	{
		�l�r_����,
		DEFALT,
		NewDokabenFont,		//�h�J�x���t�H���g
	};
}

//�������`�悷�邾���̃N���X
class Text : public Component
{
public:
	Text(GameObject* g, Transform* t);
	void Awake()override;
	void PreRender()override;
	void ImageRender()override;
	//�t�H���g�X�^�C���𕶎���Ŏw��
	void Initialize(const wchar_t* string, const float& size, const Color& color = Color::white,
		const fbSprite::SpriteEffectE& flg = fbSprite::SpriteEffectE::NONE, const char* style = "�l�r ����",fbText::TextFormatE format = fbText::TextFormatE::CENTER);
	//������Z�b�g
	void SetString(const wchar_t* s);
	//�t�H���g�̃T�C�Y
	void SetSize(const float& s);
	//�Ԋu�ݒ�
	void SetSpacing(const float& space);
	//�����X�^�C���Z�b�g
	void SetStyle(const char* s);
	//�u�����h����F
	void SetBlendColor(const Color& c);
	//�G�t�F�N�g�t���O�Z�b�g
	void SetEffectFlg(const fbSprite::SpriteEffectE& e);
	void SetEffectFlg(const fbSprite::SpriteEffectE& e,const bool& f);
	//�����ݒ�
	void SetFormat(fbText::TextFormatE format);
private:
	//�e�L�X�g�̒����X�V
	void _UpdateLength();
private:
	//���삷�镶����
	wchar_t* _WString;
	//�e�L�X�g�̉��̒���
	float _Length;
	//�T�C�Y
	float _Size;
	//�����̊Ԋu
	float _Spacing;
	//�����̃X�^�C�����f�t�H���g�́@�l�r ����.
	char _FontStyle[32];

	//�������l�߂邩�ǂ����H
	bool _Kerning;
	//�e�L�X�g�̃t�H�[�}�b�g
	fbText::TextFormatE _TextFormat;

	//�`��p�X�v���C�g
	Sprite* _Sprite;
};