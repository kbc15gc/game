#include"fbstdafx.h"
#include "Text.h"
#include "FontManager.h"

Text::Text(GameObject* g, Transform* t) :
	Component(g,t,typeid(this).name()),
	_WString(nullptr),
	_Size(1.0f),
	_Spacing(0),
	_Kerning(true),
	_DisplayCharNum(UINT_MAX),
	_MaxCharNum(0),
	_TextFormat(fbText::TextFormatE::CENTER)
{
	
}

void Text::Awake()
{
	//�X�v���C�g��ǉ�
	_Sprite = gameObject->AddComponent<Sprite>();
	//�폜����F
	_Sprite->SetClipColor(Color(1.0f, 1.0f, 1.0f, 0.0f));
	//�����`�悵�Ȃ��悤�ɂ���
	_Sprite->Start();
	_Sprite->enable = false;
	strcpy_s(_FontStyle, 32, "�l�r ����");
}

void Text::PreRender()
{

}

void Text::ImageRender()
{
	//�o�b�t�@�ɕێ�
	Vector3 bufS = transform->GetScale();
	Vector3 bufP = transform->GetPosition();
	//�e�L�X�g�̃T�C�Y�ɕύX
	transform->SetScale(Vector3(bufS.x * _Size, bufS.y * _Size, bufS.z * _Size));

	Vector3 baseP = bufP;
	//�t�H�[�}�b�g�����_���v�Z
	_CalcFormat(baseP);
	
	//�e�L�X�g�`��
	_RenderText(baseP);
	
	//���ɖ߂�
	transform->SetPosition(bufP);
	transform->SetScale(bufS);
}

void Text::Initialize(const wchar_t * string, const float& size, const Color& color, const fbSprite::SpriteEffectE& flg, const char * style, const unsigned int& format)
{
	SetStyle(style);
	SetSize(size);
	SetString(string);
	SetFormat(format);
	SetBlendColor(color);
	SetEffectFlg(flg,true);
}

void Text::Initialize(const TextParameter & param)
{
	SetStyle(param.Style);
	SetSize(param.FontSize);
	SetString(param.String);
	SetFormat(param.Format);
	SetBlendColor(param.Color);
	SetEffectFlg(param.EffectFlg, true);
}

void Text::SetString(const wchar_t * s)
{
	if (s == nullptr) {
		return;
	}
	//�O��̕�������
	if (_WString)
		SAFE_DELETE(_WString);
	//����(+1�͏I�[������)
	size_t len = wcslen(s) + 1;
	//�������m��
	_WString = new wchar_t[len]();
	//�l�R�s�[
	wcscpy_s(_WString, len, s);
	INSTANCE(FontManager)->Createfont(_WString, _FontStyle);

	_UpdateLength();
}

void Text::SetString(const char * s)
{
	//wchar�ɕϊ�
	wchar_t w[256];
	setlocale(LC_CTYPE, "jpn");

	mbstowcs(w, s, 256);
	SetString(w);
}

void Text::SetStyle(const char* s)
{
	//"fbText::TextStyleE::"���܂܂�Ă��邩�`�F�b�N
	if (strstr((char*)s, "fbText::TextStyleE::") != NULL)
	{
		strcpy_s(_FontStyle, 32, s+strlen("fbText::TextStyleE::"));
	}
	else
	{
		//���̂܂܃R�s�[
		strcpy_s(_FontStyle, 32, s);
	}
}

void Text::SetSize(const float& s)
{
	//����Ă���t�H���g�̃T�C�Y�Ŋ������{��
	_Size = s / FontSize;
	_UpdateLength();
}

void Text::SetSpacing(const float& space)
{
	_Spacing = space;
	_UpdateLength();
}

void Text::SetBlendColor(const Color& c)
{
	_Sprite->SetBlendColor(c);
}

void Text::SetEffectFlg(const fbSprite::SpriteEffectE& e)
{
	_Sprite->SetEffectFlg((DWORD)e);
}

void Text::SetEffectFlg(const fbSprite::SpriteEffectE& e, const bool& f)
{
	_Sprite->SetEffectFlg((DWORD)e,f);
}

void Text::SetFormat(const unsigned int& format)
{
	_TextFormat = (fbText::TextFormatE)format;
}

void Text::SetKerning(const bool & kerning)
{
	_Kerning = kerning;
}

void Text::_CalcFormat(Vector3 & pos)
{
	//�t�H�[�}�b�g
	const unsigned int& format = (unsigned int)_TextFormat;
	Vector3 sca = transform->GetScale();
	//�ړ���
	Vector2 offset(_Length.x * sca.x, -_Length.y * sca.y);
	//������
	offset *= 0.5f;
	if (format & (unsigned int)fbText::TextFormatE::CENTER)
	{
		pos.x -= offset.x;
	}
	else if (format & (unsigned int)fbText::TextFormatE::RIGHT)
	{
		pos.x -= offset.x * 2;
	}

	//�㉺�̈ړ�
	if (format & (unsigned int)fbText::TextFormatE::UP)
	{
		pos.y += offset.y;
	}
	else if (format & (unsigned int)fbText::TextFormatE::DOWN)
	{
		pos.y -= offset.y;
	}
}

void Text::_UpdateLength()
{
	//null�`�F�b�N
	if (!_WString)
		return;
	//������
	float width = 0;
	//�ő�l�ێ�(x��y�ň������Ⴄ�̂Œ��ӂ��ꂽ��)
	_Length = Vector2(0, 0);
	Vector2 MaxLength(0, 0);
	_MostHeight = 0.0f;
	_MaxCharNum = 0;
	//���̒����v�Z
	for (short i = 0; _WString[i] != '\0'; i++)
	{
		//�������J�E���g
		_MaxCharNum++;
		//���s�����ł͂Ȃ��B
		if (_WString[i] != '\n')
		{
			//�����̃f�[�^�擾
			FontData* data = INSTANCE(FontManager)->Findfont(_WString[i], _FontStyle);
			GLYPHMETRICS gm = data->gm;
			//�����̉����𑫂��Ă���
			width += _Kerning ? gm.gmBlackBoxX : (gm.gmCellIncX + _Spacing);

			//�����X�V
			MaxLength.x = max(MaxLength.x, width);
			MaxLength.y = max(MaxLength.y, max(gm.gmBlackBoxY, gm.gmptGlyphOrigin.y));
			//�ł��傫�����̂�ێ�
			_MostHeight = max(_MostHeight, gm.gmptGlyphOrigin.y);
		}
		else
		{
			//���s�����������B
			width = 0.0f;
			_Length.y += MaxLength.y;
			//���̉��s���g���܂킷���H
			if (_WString[i + 1] != '\n')
				MaxLength.y = 0.0f;
		}
	}
	//�����͍ő�̂��̂�
	_Length.x = MaxLength.x;
	//�c���͍ő�̂��̂𑫂�
	_Length.y += MaxLength.y;
}

void Text::_RenderText(const Vector3 & base)
{
	//��O�̕����̃I�t�Z�b�g�ʃZ�b�g
	Vector3 offset(0, 0, 0);
	//����
	float maxHeight = 0;
	//�t�H���g���Z�b�g���ĕ��������`�悷����Ă����̂͏d���I
	for (short i = 0; _WString[i] != '\0'; i++)
	{
		//�������ȏ�ɂȂ����Ȃ�`��~�߁B
		if (_DisplayCharNum <= i)
			break;

		//���̕����̃X�P�[��
		float scale = transform->GetScale().x;
		//���s�����ł͂Ȃ�
		if (_WString[i] != '\n')
		{
			//�����̃f�[�^�擾
			FontData* data = INSTANCE(FontManager)->Findfont(_WString[i], _FontStyle);
			GLYPHMETRICS gm = data->gm;
			//�摜�̍���̍��W
			Vector3 origin(0, -gm.gmptGlyphOrigin.y, 0);
			origin.Scale(scale);
			origin.y += _MostHeight * scale;
			//��_�@+�@�ړ��ʁ@+�@�e�����̔�����
			Vector3 pos = base + offset + origin;
			pos.x += (gm.gmBlackBoxX / 2)*scale;
			pos.y += (gm.gmBlackBoxY / 2)*scale;

			transform->SetPosition(pos);
			//�e�N�X�`���Z�b�g
			_Sprite->SetTexture(data->texture);
			//�`��
			_Sprite->ImageRender();

			//�����̕�
			float width = _Kerning ? gm.gmBlackBoxX : gm.gmCellIncX + _Spacing;
			//�����̕����ړ��ʂ��v���X
			offset.x += width * scale;
			//���̍s�̕����̍ő�̍���
			maxHeight = max(maxHeight, gm.gmBlackBoxY * scale);
		}
		else
		{
			//���������ɂ��炷
			offset.y += maxHeight;
			//�ړ��ʏ�����
			offset.x = 0;
			//�������s�łȂ��Ȃ珉����
			if (_WString[i + 1] != '\n')
				maxHeight = 0;
		}
	}
}