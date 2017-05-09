#include "Button.h"
#include "_Inputs\Mouse.h"
void Button::Awake()
{
	_Sprite = AddComponent<Sprite>();
	_Square = AddComponent<SquareCollision>();

	_IsOn = false;
	_State = ButtonStateE::NORMAL;

	SetButtonType(fbButton::ButtonTypeE::PRESS);
}

void Button::Update()
{
	//�X�e�[�g�̕ω��`�F�b�N
	if(_CheckState())
	{
		//���낢��ύX
		_ChangeState();
	}
}

void Button::Initialize(TEXTURE * on, TEXTURE * off, const Vector2 & size, const fbButton::ButtonTypeE & type)
{
	SetTexture(on, off);
	SetSize(size);
	SetButtonType(type);
}

void Button::SetTexture(TEXTURE * on, TEXTURE * off)
{
	_Tex[0] = on;
	//�Ȃ��Ȃ瓯���e�N�X�`��
	if (off)
		_Tex[1] = off;
	else
		_Tex[1] = on;

	_ChangeState();
}

void Button::SetSize(const Vector2 & size)
{
	_Square->SetSize(size);
}

void Button::SetButtonType(const fbButton::ButtonTypeE& type)
{
	_Type = type;
}

void Button::SetButtonColor(const ButtonStateE state, const Color & color)
{
	_Color[(unsigned int)state] = color;
	_ChangeState();
}

void Button::SetOnOff(const bool & f)
{
	_IsOn = f;
	_ChangeState();
}

bool Button::_CheckState()
{
	//�ύX�O�̃X�e�[�^�X�ێ�
	ButtonStateE before = _State;
	//�͈͂ɓ����Ă��邩�H
	if (_Square->Judgment(MouseInput->GetCursorPosOnWindow(g_MainWindow)))
	{
		if (MouseInput->GetValue(MouseInE::L_CLICK))
		{
			//�{�^����������Ă���
			_State = ButtonStateE::PRESS;
		}
		else
		{
			if (_Type == fbButton::ButtonTypeE::TRIGGER)
			{
				if (MouseInput->GetUp(MouseInE::L_CLICK))
					_IsOn = !_IsOn;
			}
			//�{�^���̏�ɃJ�[�\��������
			_State = ButtonStateE::ON;
		}
	}
	else
	{
		//�ʏ�
		_State = ButtonStateE::NORMAL;
	}

	if (_Type == fbButton::ButtonTypeE::PRESS)
	{
		//������Ă���Ȃ�ON
		_IsOn = (_State == ButtonStateE::PRESS);
	}

	//�ύX���ꂽ���ǂ����H
	return (before != _State);
}

void Button::_ChangeState()
{
	//�e�N�X�`���ݒ�
	_Sprite->SetTexture(_IsOn ? _Tex[0] : _Tex[1]);
	//�F�ݒ�
	_Sprite->SetBlendColor(_Color[(unsigned int)_State]);
}
