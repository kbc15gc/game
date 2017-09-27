#include"stdafx.h"
#include "TextBox.h"
#include "GameObject\Village\EventManager.h"

TextBox::TextBox(const char * name):
	GameObject(name),
	_CharNum(0),
	_StartTextID(-1),
	_TextSpeed(1.0f),
	_State(TextBoxStateE::CLOSE),
	_AnimeTime(0.0f)
{
}

TextBox::~TextBox()
{
	INSTANCE(GameObjectManager)->AddRemoveList(_BoxImage[0]);
	INSTANCE(GameObjectManager)->AddRemoveList(_BoxImage[1]);
	INSTANCE(GameObjectManager)->AddRemoveList(_Text);
}

void TextBox::Awake()
{
	_BoxImage[0] = INSTANCE(GameObjectManager)->AddNew<ImageObject>("BoxImage",_Priority);
	_BoxImage[1] = INSTANCE(GameObjectManager)->AddNew<ImageObject>("BoxImage", _Priority);
	_Text = INSTANCE(GameObjectManager)->AddNew<TextObject>("Text", _Priority);

	//�J�[�\��
	_BoxImage[0]->SetTexture(LOADTEXTURE("cursor.png"));
	_BoxImage[0]->SetPivot(0.5, 1.0);
	//�{��
	_BoxImage[1]->SetTexture(LOADTEXTURE("window.png"));
	
	_Text->Initialize(L"", 25.0f);
	_Text->SetAnchor(fbText::TextAnchorE::UpperCenter);
	
	//�e�q�֌W��t����B
	_BoxImage[0]->transform->SetParent(transform);
	_BoxImage[1]->transform->SetParent(_BoxImage[0]->transform);
	_Text->transform->SetParent(_BoxImage[1]->transform);

	//�I�u�W�F�N�g���\���ɂ���
	for each (Transform* child in transform->GetChildren())
	{
		child->gameObject->SetActive(false, true);
	}
	transform->SetScale(Vector3::zero);
}

void TextBox::Update()
{
	//�A�j���[�V�����B
	_Animation();
	//�\�����镶�����̑����B
	_IncreaseChar();
}

void TextBox::SetMessageID(const int & id)
{
	_StartTextID = id;
	//���b�Z�[�W���擾
	_Message = INSTANCE(MessageManager)->GetMess(_StartTextID);
}

void TextBox::Speak()
{
	if (_Message)
	{
		if (_State & TextBoxStateE::CLOSE)
		{
			//��b�J�n
			_OpenMessage();
		}
		else if (_State == TextBoxStateE::OPEN)
		{
			//���̉�b��
			_NextMessage();
		}
	}
}

void TextBox::Title(bool show)
{
	if (show)
	{
		if (_State == TextBoxStateE::CLOSE)
		{
			//�X�e�[�g�ݒ�B
			_State = TextBoxStateE::TITLE;
			//�e�L�X�g�ݒ�B
			_SetText(_Message->Title);
			_Text->SetCharNum(UINT_MAX);
			//�S�Ă��A�N�e�B�u�ɂ���B
			for each (Transform* child in transform->GetChildren())
			{
				child->gameObject->SetActive(true, true);
			}
			transform->SetScale(Vector3::one);
		}
	}
	else
	{
		//����B
		CloseMessage();
	}
}

void TextBox::_OpenMessage()
{
	//���Ă��邩
	if (_State & TextBoxStateE::CLOSE)
	{
		//�X�e�[�g�ݒ�B
		_State = TextBoxStateE::OPENING;
		//�e�L�X�g�ݒ�B
		_SetText(_Message->Text);

		//�{�b�N�X��\������
		for each (ImageObject* image in _BoxImage)
		{
			image->SetActive(true);
		}
		_AnimeTime = 0.0f;
	}
}

void TextBox::CloseMessage()
{
	if (_State & TextBoxStateE::OPEN ||
		_State == TextBoxStateE::TITLE)
	{
		//�ŏ��̃��b�Z�[�W�擾
		_Message = INSTANCE(MessageManager)->GetMess(_StartTextID);
		_State = TextBoxStateE::CLOSING;
		//�e�L�X�g�����B
		_Text->SetText("");
		_Text->SetActive(false);

		_AnimeTime = 0.0f;
	}
}

void TextBox::_NextMessage()
{
	//�J���Ă��鎞�̂�
	if (_State == TextBoxStateE::OPEN)
	{
		//�ő啶�����擾
		int max = _Text->GetMaxCharNum();
		//�e�L�X�g�̓r���Ȃ�Ō�܂ŏo���B
		if (_CharNum < max)
		{
			_CharNum = max;
			_Text->SetCharNum(_CharNum);
		}
		else
		{
			//���̃��b�Z�[�W���Đ�
			_SetMessage(_Message->NextID);
			//0�ȏ�Ȃ�
			if (_Message->EventID >= 0)
				INSTANCE(EventManager)->Execute(Event::EventID(_Message->EventID), 0);
		}
	}
}

void TextBox::_SetText(const char * text)
{
	//�\��������������
	_CharNum = 0;
	_Text->SetCharNum((unsigned int)_CharNum);

	//�e�L�X�g�ݒ�B
	_Text->SetText(text);
	Vector2 space(_Text->GetText()->GetSize() * FontSize);
	Vector2 len(_Text->GetLength());
	//�e�L�X�g�̃T�C�Y+�]�����B
	Vector2 textSize = len + space;
	//���b�Z�[�W�{�b�N�X�̃T�C�Y�ݒ�B
	_BoxImage[1]->SetSize(textSize);
	//�{�b�N�X�̔����̍����B
	float halfHeight = textSize.y / 2;
	//�J�[�\���̏�ɂ���悤�Ɉړ��B
	_BoxImage[1]->transform->SetLocalPosition(Vector3(0, -(halfHeight + _BoxImage[0]->GetTexture()->Size.y), 0));
}

void TextBox::_SetMessage(const int & id)
{
	if (id >= 0)
	{
		INSTANCE(EventManager)->Execute(Event::EventID::ShopF, 0);
		//���b�Z�[�W���擾
		_Message = INSTANCE(MessageManager)->GetMess(id);
		if (_Message)
		{
			//�e�L�X�g�ƃ{�b�N�X���Z�b�g����B
			_SetText(_Message->Text);
		}
		else
		{
			//���b�Z�[�W��������Ȃ������̂ŕ���B
			CloseMessage();
		}
	}
	//-1�ȉ��Ȃ烁�b�Z�[�W�����
	else
	{
		CloseMessage();
	}
}

void TextBox::_Animation()
{
	//�A�j���[�V�����ł͂Ȃ�
	if ((_State & TextBoxStateE::ANIMATION) == 0)
		return;
	Vector3 sca = Vector3::zero;
	//�A�j���[�V�����Ɋ|���鎞��
	float animationTime = 0.5f;
	_AnimeTime += Time::DeltaTime() / animationTime;
	switch (_State)
	{
	case TextBoxStateE::TITLE:
		break;
	case TextBoxStateE::OPENING:
		//���񂾂�傫������B
		sca = Vector3::Lerp(Vector3::zero, Vector3::one, _AnimeTime);
		transform->SetScale(sca);

		//�J���������Ȃ�
		if (sca.Length() >= (Vector3::one.Length()))
		{
			_State = TextBoxStateE::OPEN;
			//�e�L�X�g�\��
			_Text->SetActive(true);
		}
		break;
	case TextBoxStateE::CLOSING:
		//���񂾂񏬂�������B
		sca = Vector3::Lerp(Vector3::one, Vector3::zero, _AnimeTime);
		transform->SetScale(sca);

		//�傫�����Ȃ��Ȃ����Ȃ�
		if (sca.Length() <= Vector3::zero.Length())
		{
			//�{�b�N�X�����B
			for each (ImageObject* image in _BoxImage)
			{
				image->SetActive(false);
			}
			_State = TextBoxStateE::CLOSE;
		}
		break;
	default:
		break;
	}
}


void TextBox::_IncreaseChar()
{
	//�������o�Ă��鎞�ɉ��Z
	if (_State == TextBoxStateE::OPEN)
	{
		//�ő吔�ȉ��Ȃ瑝�₷�B
		if (_CharNum < _Text->GetMaxCharNum())
		{
			//���񂾂�\�����镶�����𑝂₷�B
			_CharNum += Time::DeltaTime()*_TextSpeed;
			_Text->SetCharNum((unsigned int)_CharNum);
		}
	}
}
