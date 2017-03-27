#pragma once

//�����̃f�[�^
struct FontData :Noncopyable
{
	FontData(TEXTURE* t, GLYPHMETRICS g)
	{
		texture = t;
		gm = g;
	}
	//�����̃e�N�X�`��
	TEXTURE* texture;
	//�����̍����Ƃ����Ƃ��̏��
	GLYPHMETRICS gm;
};

class FontManager
{
public:
	FontManager();
	~FontManager();
	//�����̃e�N�X�`������
	//�߂�l�F�Ȃ�
	//�������Fwchar_t* �e�N�X�`���𐶐�������������
	//�������FTCHAR* �����̃X�^�C��
	void Createfont(const wchar_t* string,const  char* Style);
	//��������
	FontData* Findfont(const wchar_t& wchar,const char* Style);
	
	static FontManager* Instance()
	{
		if (_Instance == nullptr)
		{
			_Instance = new FontManager();
		}
		return _Instance;
	}
private:
	FontData* _CreateFontTexture(const int& code);
	FontData* _CreateOutLineFontTexture(const wchar_t& wc,const int& code);
private:
	//�f�o�C�X�R���e�L�X�g
	HDC _HDC;
	//�t�H���g�̑���
	LOGFONT _LFont;
	//�_���t�H���g�̃n���h��
	HFONT _HFont;
	//�F�̊K��
	int _Grad;
	int _GradFlg;
	//�������Ǘ�����}�b�v�̒�`
	typedef std::map<wchar_t, FontData*> FontMap;
	//�}�b�v�̔z��
	std::map<int, FontMap*> _FontMapArray;
	static FontManager* _Instance;
};