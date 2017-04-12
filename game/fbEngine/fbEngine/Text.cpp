#include "Text.h"
#include "FontManager.h"
#include "GameObject.h"

Text::Text(GameObject* g, Transform* t) :
	Component(g,t,typeid(this).name()),
	_WString(nullptr),
	_Size(1.0f),
	_Spacing(0),
	_Kerning(true),
	_TextFormat(fbText::TextFormatE::CENTER)
{
	
}

void Text::Awake()
{
	//�X�v���C�g��ǉ�
	_Sprite = gameObject->AddComponent<Sprite>();
	//�폜����F
	_Sprite->SetClipColor(Color(1.0f, 1.0f, 1.0f, 0.0f));
	_Sprite->SetPivot(Vector2(0.0f, 0.0f));
	strcpy_s(_FontStyle, 32, "�l�r ����");
}

void Text::PreRender()
{

}

void Text::ImageRender()
{
	Vector3 buf = transform->GetScale();
	Vector3 base = transform->GetPosition();
	transform->SetScale(Vector3(_Size, _Size, _Size));

	Vector3 pos = base;
	switch (_TextFormat)
	{
	case fbText::TextFormatE::LEFT:
		pos.x -= _Length * 0.0f;
		break;
	case fbText::TextFormatE::CENTER:
		pos.x -= _Length * 0.5f;
		break;
	case fbText::TextFormatE::RIGHT:
		pos.x -= _Length * 1.0f;
		break;
	default:
		break;
	}


	//��O�̕����̃I�t�Z�b�g�ʃZ�b�g
	float offset = 0;
	//�t�H���g���Z�b�g���ĕ��������`�悷����Ă����̂͏d���I
	for (short i = 0;_WString[i] != '\0';i++)
	{
		//�����̃f�[�^�擾
		FontData* data = INSTANCE(FontManager)->Findfont(_WString[i], _FontStyle);
		//�O�̕�����offset�����炷
		if (_Kerning)
		{
			pos.x += (offset)* _Size;
		}
		else
		{
			pos.x += (offset + data->gm.gmptGlyphOrigin.x) * _Size;
		}
		//�t�H���g�̍���̍��W������
		pos.y = base.y - (data->gm.gmptGlyphOrigin.y * _Size);
		transform->SetPosition(pos);
		//�e�N�X�`���Z�b�g
		_Sprite->SetTexture(data->texture);
		//�`��
		_Sprite->ImageRender();
		//���̕����̊J�n�ʒu
		if(_Kerning)
		{
			offset = (float)data->gm.gmBlackBoxX;
		}
		else
		{
			offset = (float)data->gm.gmCellIncX + _Spacing;
		}
		
	}
	//nullptr��ݒ肵�ĕ`�悵�Ȃ��悤�ɂ���
	_Sprite->SetTexture(nullptr);
	transform->SetPosition(base);
	transform->SetScale(buf);
}

void Text::Initialize(const wchar_t * string, const float& _Size, const Color& color, const fbSprite::SpriteEffectE& flg, const char * style, fbText::TextFormatE format)
{
	SetStyle((char*)style);
	SetSize(_Size);
	SetString(string);
	SetFormat(format);
	SetBlendColor(color);
	SetEffectFlg(flg,true);
}

void Text::SetString(const wchar_t * s)
{
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

void Text::SetFormat(fbText::TextFormatE format)
{
	_TextFormat = format;
}

void Text::_UpdateLength()
{
	//null�`�F�b�N
	if (!_WString)
		return;
	//������
	_Length = 0.0f;
	//���̒����v�Z
	for (short i = 0; _WString[i] != '\0'; i++)
	{
		//�����̃f�[�^�擾
		FontData* data = INSTANCE(FontManager)->Findfont(_WString[i], _FontStyle);

		//�����̉����𑫂��Ă���
		if (_Kerning)
		{
			_Length += data->gm.gmBlackBoxX;
		}
		else
		{
			_Length += (data->gm.gmCellIncX + data->gm.gmptGlyphOrigin.x + _Spacing);
		}
	}
	//�T�C�Y��������
	_Length *= _Size;
}
