#include"fbstdafx.h"
#include "Text.h"
#include "FontManager.h"

Text::Text(GameObject* g, Transform* t) :
	Component(g, t, typeid(this).name()),
	_Text(nullptr),
	_Size(1.0f),
	_Spacing(0),
	_Kerning(true),
	_DisplayCharNum(UINT_MAX),
	_MaxCharNum(0),
	_TextAnchor(fbText::TextAnchorE::MiddleCenter),
	_SaveBlendColor(Color::white)
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
	_Sprite->SetEnable(false);
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
	_CalcAnchor(baseP);
	
	//�e�L�X�g�`��
	_RenderText(baseP);
	
	//���ɖ߂�
	transform->SetPosition(bufP);
	transform->SetScale(bufS);
}

void Text::Initialize(const wchar_t * string, const float& size, const Color& color, const fbSprite::SpriteEffectE& flg, const char * style, fbText::TextAnchorE anchor)
{
	SetStyle(style);
	SetSize(size);
	SetText(string);
	SetAnchor(anchor);
	SetBlendColor(color);
	SetEffectFlg(flg,true);
}

void Text::Initialize(const TextParameter & param)
{
	SetStyle(param.Style);
	SetSize(param.FontSize);
	SetText(param.String);
	SetAnchor(param.Anchor);
	SetBlendColor(param.Color);
	SetEffectFlg(param.EffectFlg, true);
}

void Text::SetText(const wchar_t * s)
{
	if (s == nullptr) {
		return;
	}
	//�O��̕�������
	if (_Text)
		SAFE_DELETE(_Text);
	//����(+1�͏I�[������)
	size_t len = wcslen(s) + 1;
	//�������m��
	_Text = new wchar_t[len]();
	//�l�R�s�[
	wcscpy_s(_Text, len, s);
	INSTANCE(FontManager)->Createfont(_Text, _FontStyle);

	_UpdateLength();
}

void Text::SetText(const char * s)
{
	//wchar�ɕϊ�
	wchar_t w[512];
	setlocale(LC_CTYPE, "jpn");

	mbstowcs(w, s, 512);
	SetText(w);
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
	_SaveBlendColor = c;
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

void Text::SetAnchor(fbText::TextAnchorE anchor)
{
	_TextAnchor = anchor;
}

void Text::SetKerning(const bool & kerning)
{
	_Kerning = kerning;
}

void Text::_CalcAnchor(Vector3 & pos)
{
	//�t�H�[�}�b�g
	const unsigned int& anchor = (unsigned int)_TextAnchor;
	Vector3 sca = transform->GetScale();
	//�ړ���
	Vector2 offset(_Length.x * sca.x, -_Length.y * sca.y);
	//������
	offset *= 0.5f;
	if (anchor & (unsigned int)fbText::TextAnchorE::Center)
	{
		pos.x -= offset.x;
	}
	else if (anchor & (unsigned int)fbText::TextAnchorE::Right)
	{
		pos.x -= offset.x * 2;
	}

	//�㉺�̈ړ�
	if (anchor & (unsigned int)fbText::TextAnchorE::Middle)
	{
		pos.y -= offset.y;
	}
	else if (anchor & (unsigned int)fbText::TextAnchorE::Lower)
	{
		pos.y -= offset.y * 2;
	}
}

void Text::_UpdateLength()
{
	//null�`�F�b�N
	if (!_Text)
		return;
	//������
	float width = 0;
	//�ő�l�ێ�(x��y�ň������Ⴄ�̂Œ��ӂ��ꂽ��)
	_Length = Vector2(0, 0);
	Vector2 MaxLength(0, 0);
	_MostHeight = 0.0f;
	_MaxCharNum = 0;
	//���̒����v�Z
	for (short i = 0; _Text[i] != '\0'; i++)
	{
		//�������J�E���g
		_MaxCharNum++;
		//���s�����B
		if (_Text[i] == '\n')
		{
			width = 0.0f;
			if (i == 0 && "\n") {
				// �ŏ���1�����ڂ����s�����������B
				abort();
			}
			_Length.y += MaxLength.y;
			//���̉��s���g���܂킷���H
			if (_Text[i + 1] != '\n') {
				MaxLength.y = 0.0f;
			}
		}
		//�J���[�R�[�h�B
		else if (_Text[i] == '<')
		{
			wchar_t copy[512], *next;
			// < ����菜���B
			wcscpy_s(copy, 512, _Text + i + 1);
			// > �܂ł̕�������擾�B
			wchar_t* addres = wcstok_s(copy, L">", &next);
			i += wcslen(addres) + 1;
		}
		//���ʂ̕����B
		else
		{
			//�����̃f�[�^�擾
			FontData* data = INSTANCE(FontManager)->Findfont(_Text[i], _FontStyle);
			GLYPHMETRICS gm = data->gm;
			//�����̉����𑫂��Ă���
			width += _Kerning ? gm.gmBlackBoxX : (gm.gmCellIncX + _Spacing);

			//�����X�V
			MaxLength.x = max(MaxLength.x, width);
			MaxLength.y = max(MaxLength.y, max(gm.gmBlackBoxY, gm.gmptGlyphOrigin.y));
			//�ł��傫�����̂�ێ�
			_MostHeight = max(_MostHeight, gm.gmptGlyphOrigin.y);
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
	if (_Text == nullptr)
		return;
	//�t�H���g���Z�b�g���ĕ��������`�悷����Ă����̂͏d���I
	for (short idx = 0,count = 0; _Text[idx] != '\0';)
	{
		//�������ȏ�ɂȂ����Ȃ�`��~�߁B
		if (_DisplayCharNum <= count)
			break;

		//���̕����̃X�P�[��
		float scale = transform->GetScale().x;
		//���s�����ł͂Ȃ�
		if (_Text[idx] != '\n')
		{
			//�����̃f�[�^�擾
			FontData* data = INSTANCE(FontManager)->Findfont(_Text[idx], _FontStyle);
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
			if (_Text[idx + 1] != '\n')
				maxHeight = 0;
		}
		count++;
		idx++;
		//���b�`�e�L�X�g�����B
		if (_Text[idx] == '<')
			idx += RichText(&_Text[idx]);
	}
}

int Text::RichText(const wchar_t* text)
{
	wchar_t copy[512], *next;
	// < ����菜���B
	wcscpy_s(copy, 512, text + 1);
	// > �܂ł̕�������擾�B
	wchar_t* addres = wcstok_s(copy, L">", &next);
	if (addres == nullptr)return 0;


	wchar_t* type;
	//�^�C�v�`�F�b�N
	if (type = wcsstr(addres, L"color"))
	{
		if (addres[0] == L'/')
		{
			_Sprite->SetBlendColor(_SaveBlendColor);
		}
		else
		{
			//�l�ƂȂ肤�镶��������o���B
			wchar_t num[20];
			wcscpy_s(num, ARRAY_SIZE(num), type + wcslen(L"color="));
			DWORD color = wcstoul(num, nullptr, 16);
			_Sprite->SetBlendColor(Color((unsigned int)((color & 0xff000000) / pow(256, 3)), (color & 0x00ff0000) / (DWORD)pow(256, 2), (color & 0x0000ff00) / (DWORD)pow(256, 1), (color & 0x000000ff)));
		}
	}
	//+2�� <> �̓񕶎����B
	return wcslen(addres) + 2;
}