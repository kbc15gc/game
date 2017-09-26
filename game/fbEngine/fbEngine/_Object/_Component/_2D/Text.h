#pragma once
#include "_Object\_Component\Component.h"
#include "Sprite.h"

namespace fbText
{
	//�e�L�X�g�̃A���J�[
	enum class TextAnchorE : unsigned int
	{
		//
		Left			= 0b1,
		Center			= 0b10,
		Right			= 0b100,
		//
		Upper			= 0b1000,
		Middle			= 0b10000,
		Lower			= 0b100000,

		UpperLeft		= Upper | Left,
		UpperCenter		= Upper | Center,
		UpperRight		= Upper | Right,
		//
		MiddleLeft		= Middle | Left,
		MiddleCenter	= Middle | Center,
		MiddleRight		= Middle | Right,
		//
		LowerLeft		= Lower | Left,
		LowerCenter		= Lower | Center,
		LowerRight		= Lower | Right
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
	//�e�L�X�g�̏������Ɏg���p�����[�^
	struct TextParameter
	{
	public:
		char* String = "";
		float FontSize = 40.0f;
		Color Color = Color::white;
		fbSprite::SpriteEffectE EffectFlg = fbSprite::SpriteEffectE::NONE;
		char* Style = "�l�r ����";
		fbText::TextAnchorE Anchor = fbText::TextAnchorE::MiddleCenter;
	};
public:
	Text(GameObject* g, Transform* t);
	void Awake()override;
	void PreRender()override;
	void ImageRender()override;
	//�ꊇ������
	void Initialize(const wchar_t* string, const float& size, const Color& color = Color::white,
		const fbSprite::SpriteEffectE& flg = fbSprite::SpriteEffectE::NONE, const char* style = "�l�r ����", fbText::TextAnchorE format = fbText::TextAnchorE::MiddleCenter);
	void Initialize(const TextParameter& param);
	//������Z�b�g
	void SetText(const wchar_t* s);
	void SetText(const char* s);
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
	void SetAnchor(fbText::TextAnchorE anchor);
	//�J�[�j���O�ݒ�
	void SetKerning(const bool& kerning);
	void SetCharNum(const unsigned int& num)
	{
		_DisplayCharNum = num;
	}


	Vector2 GetLength()
	{
		return _Length * _Size;
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
	void _CalcAnchor(Vector3& pos);
	//�e�L�X�g�̒����X�V
	void _UpdateLength();
	//�e�L�X�g�`��
	//��_
	void _RenderText(const Vector3& base);

	int RichText(const wchar_t* text);
private:
	//�����̃X�^�C�����f�t�H���g�́@�l�r ����.
	char _FontStyle[32];
	//���삷�镶����
	wchar_t* _Text;

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
	fbText::TextAnchorE _TextAnchor;

	//�J���[�̕ێ��B
	Color _SaveBlendColor;

	//�`��p�X�v���C�g
	Sprite* _Sprite;
};