#pragma once
#include "_Object\_Component\Component.h"
#include "Sprite.h"

namespace fbText
{
	//�e�L�X�g�̏���
	enum class TextFormatE : unsigned int
	{
		LEFT = BIT(1),		//�e�L�X�g�̍��[�Ƀ|�W�V�������ʒu����
		CENTER = BIT(2),	//�e�L�X�g�̐^�񒆂Ƀ|�W�V�������ʒu����
		RIGHT = BIT(3),		//�e�L�X�g�̉E�[�Ƀ|�W�V�������ʒu����

		UP = BIT(4),		//�e�L�X�g�̈�ԏオ�|�W�V�����ɂȂ�
		DOWN = BIT(5)		//�e�L�X�g�̈�ԉ����|�W�V�����ɂȂ�
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
		const fbSprite::SpriteEffectE& flg = fbSprite::SpriteEffectE::NONE, const char* style = "�l�r ����", const unsigned int& format = (int)fbText::TextFormatE::CENTER);
	//������Z�b�g
	void SetString(const wchar_t* s);
	void SetString(const char* s);
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
	void SetFormat(const unsigned int& format);
	//�J�[�j���O�ݒ�
	void SetKerning(const bool& kerning);
	void SetCharNum(const unsigned int& num)
	{
		_DisplayCharNum = num;
	}


	const Vector2& GetLength()
	{
		return _Length;
	}
	const float& GetSize()
	{
		return _Size;
	}
	//�ő啶������Ԃ��B
	const unsigned int& GetMaxCharNum()
	{
		return _MaxCharNum;
	}
private:
	//�t�H�[�}�b�g�ɂ���Ĉړ�����ʂ��v�Z
	void _CalcFormat(Vector3& pos);
	//�e�L�X�g�̒����X�V
	void _UpdateLength();
	//�e�L�X�g�`��
	//��_
	void _RenderText(const Vector3& base);
private:
	//�����̃X�^�C�����f�t�H���g�́@�l�r ����.
	char _FontStyle[32];
	//���삷�镶����
	wchar_t* _WString;

	//�\�����镶����
	unsigned int _DisplayCharNum;
	//�ő啶����
	unsigned int _MaxCharNum;
	//�e�L�X�g�̒���
	Vector2 _Length;
	//�����T�C�Y
	float _Size;
	//��Ԕw�̍��������̍���
	float _MostHeight;
	//������
	float _Spacing;
	//�������l�߂邩�ǂ����H
	bool _Kerning;
	//�e�L�X�g�̃t�H�[�}�b�g
	fbText::TextFormatE _TextFormat;

	//�`��p�X�v���C�g
	Sprite* _Sprite;
};