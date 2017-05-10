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
	//ステートの変化チェック
	if(_CheckState())
	{
		//いろいろ変更
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
	//ないなら同じテクスチャ
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
	//変更前のステータス保持
	ButtonStateE before = _State;
	//範囲に入っているか？
	if (_Square->Judgment(MouseInput->GetCursorPosOnWindow(g_MainWindow)))
	{
		if (MouseInput->GetValue(MouseInE::L_CLICK))
		{
			//ボタンが押されている
			_State = ButtonStateE::PRESS;
		}
		else
		{
			if (_Type == fbButton::ButtonTypeE::TRIGGER)
			{
				if (MouseInput->GetUp(MouseInE::L_CLICK))
					_IsOn = !_IsOn;
			}
			//ボタンの上にカーソルがある
			_State = ButtonStateE::ON;
		}
	}
	else
	{
		//通常
		_State = ButtonStateE::NORMAL;
	}

	if (_Type == fbButton::ButtonTypeE::PRESS)
	{
		//押されているならON
		_IsOn = (_State == ButtonStateE::PRESS);
	}

	//変更されたかどうか？
	return (before != _State);
}

void Button::_ChangeState()
{
	//テクスチャ設定
	_Sprite->SetTexture(_IsOn ? _Tex[0] : _Tex[1]);
	//色設定
	_Sprite->SetBlendColor(_Color[(unsigned int)_State]);
}
