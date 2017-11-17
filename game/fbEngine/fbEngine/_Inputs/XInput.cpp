#include"fbstdafx.h"
#include "XInput.h"

namespace
{
	const int BUTTON = 0;
	const int ANALOG = 1;
}

XInput::XInput()
{

}

void XInput::Initialize(int index)
{
	_UserIndex = index;
	ZeroMemory(&_State, sizeof(XINPUT_STATE)); // ������
	ZeroMemory(&_BeforeState, sizeof(XINPUT_STATE)); // ������
}

void XInput::Update()
{
	//�O�̃X�e�[�g�ۑ�
	_BeforeState = _State;
	//�X�e�[�g�擾
	DWORD er = XInputGetState(_UserIndex, &_State);
	//�f�b�h�]�[���Ȃ�l���ۂ߂�
	_Rounding();
	//�ڑ��ł��Ă��邩�ǂ���
	if (er == ERROR_DEVICE_NOT_CONNECTED)
	{
		_IsConnect = false;
	}
	else if (er == ERROR_SUCCESS)
	{
		_IsConnect = true;

		_AnalogValue[AnalogE::R_TRIGGER] = _State.Gamepad.bRightTrigger;
		_AnalogValue[AnalogE::L_TRIGGER] = _State.Gamepad.bLeftTrigger;
		_AnalogValue[AnalogE::L_STICKR] = _State.Gamepad.sThumbLX;
		_AnalogValue[AnalogE::L_STICKU] = _State.Gamepad.sThumbLY;
		_AnalogValue[AnalogE::R_STICKR] = _State.Gamepad.sThumbRX;
		_AnalogValue[AnalogE::R_STICKU] = _State.Gamepad.sThumbRY;

		int max = 0;
		for (auto analog : _AnalogValue)
		{
			int val = fabs(analog.second);
			if (max < val)
			{
				max = val;
				_Most = analog.first;
			}
		}
	}
}

bool XInput::IsConnected()
{
	return _IsConnect;
}

bool XInput::IsPushButton(int gamepad)
{
	if (!(_BeforeState.Gamepad.wButtons & gamepad) &&		//�O�t���[���ɉ�����Ă��Ȃ�
		(_State.Gamepad.wButtons & gamepad))				//��������Ă���
	{
		return true;
	}
	return false;
}

bool XInput::IsPressButton(int gamepad)
{
	//�_���ς��Ƃ�
	return ((_State.Gamepad.wButtons & gamepad) > 0);
}

bool XInput::KeyRepeat(int in, float interval)
{
	if (IsPressButton(in))
	{
		if (IsPushButton(in))
		{
			_RepeatTimer[BUTTON][in] = 0.0f;
			_Interval[BUTTON][in] = 0.001f;
			return true;
		}
		//���ԉ��Z�B
		_RepeatTimer[BUTTON][in] += Time::DeltaTime();
		if (_RepeatTimer[BUTTON][in] >= _Interval[BUTTON][in])
		{
			_RepeatTimer[BUTTON][in] = 0.0f;
			return true;
		}
	}
	else
	{
		_Interval[BUTTON][in] = interval;
	}

	return false;
}

bool XInput::IsPushAnalog(AnalogE a)
{
	//�ڑ��m�F
	if (!IsConnected())
	{
		return false;
	}

	switch (a)
	{
	case AnalogE::L_TRIGGER:
		if (_BeforeState.Gamepad.bLeftTrigger == 0 &&		//������Ă��Ȃ�
			_State.Gamepad.bLeftTrigger > 0)				//��������Ă���
			return true;
		break;
	case AnalogE::R_TRIGGER:
		if (_BeforeState.Gamepad.bRightTrigger == 0 &&		//������Ă��Ȃ�
			_State.Gamepad.bRightTrigger > 0)				//��������Ă���
			return true;
		break;
	case AnalogE::L_STICKR:
		if (_BeforeState.Gamepad.sThumbLX == 0 &&		//������Ă��Ȃ�
			_State.Gamepad.sThumbLX > 0)				//��������Ă���
			return true;
		break;
	case AnalogE::L_STICKL:
		if (_BeforeState.Gamepad.sThumbLX == 0 &&		//������Ă��Ȃ�
			_State.Gamepad.sThumbLX < 0)				//��������Ă���
			return true;
		break;
	case AnalogE::L_STICKU:
		if (_BeforeState.Gamepad.sThumbLY == 0 &&		//������Ă��Ȃ�
			_State.Gamepad.sThumbLY > 0)				//��������Ă���
			return true;
		break; 
	case AnalogE::L_STICKD:
		if (_BeforeState.Gamepad.sThumbLY == 0 &&		//������Ă��Ȃ�
			_State.Gamepad.sThumbLY < 0)				//��������Ă���
			return true;
		break;
	case AnalogE::R_STICKR:
		if (_BeforeState.Gamepad.sThumbRX == 0 &&		//������Ă��Ȃ�
			_State.Gamepad.sThumbRX > 0)				//��������Ă���
			return true;
		break;
	case AnalogE::R_STICKL:
		if (_BeforeState.Gamepad.sThumbRX == 0 &&		//������Ă��Ȃ�
			_State.Gamepad.sThumbRX < 0)				//��������Ă���
			return true;
		break;
	case AnalogE::R_STICKU:
		if (_BeforeState.Gamepad.sThumbRY == 0 &&		//������Ă��Ȃ�
			_State.Gamepad.sThumbRY > 0)				//��������Ă���
			return true;
		break;
	case AnalogE::R_STICKD:
		if (_BeforeState.Gamepad.sThumbRY == 0 &&		//������Ă��Ȃ�
			_State.Gamepad.sThumbRY < 0)				//��������Ă���
			return true;
		break;
	default:
		return false;
	}
	return false;
}

bool XInput::IsPressAnalog(AnalogE a, bool exclusive)
{
	//���]�ȊO�̏����擾�B
	AnalogE analog = (AnalogE)((int)a & (int)AnalogE::ANALOG);
	//�ڑ��m�F
	if (!IsConnected() || analog != _Most)
	{
		return false;
	}

	//���]�B
	if ((int)a & (int)AnalogE::INVERT)
	{
		return (_AnalogValue[analog] < 0);
	}
	else
	{
		return (_AnalogValue[analog] > 0);
	}
	return false;
}

Vector2 XInput::GetAnalog(AnalogE in)
{
	int analog = (int)in;
	Vector2 value;
	if (analog & (int)AnalogE::TRIGGER)
	{
		return Vector2(_State.Gamepad.bRightTrigger, _State.Gamepad.bLeftTrigger);
	}
	else if (analog & (int)AnalogE::R_STICK)
	{
		return Vector2(_State.Gamepad.sThumbRX, _State.Gamepad.sThumbRY);
	}
	else if (analog & (int)AnalogE::L_STICK)
	{
		return Vector2(_State.Gamepad.sThumbLX, _State.Gamepad.sThumbLY);
	}
}

bool XInput::AnalogRepeat(AnalogE analog, float interval, bool exclusive)
{
	int in = (int)analog;
	if (IsPressAnalog(analog, exclusive))
	{
		if (IsPushAnalog(analog))
		{
			_RepeatTimer[ANALOG][in] = 0.0f;
			_Interval[ANALOG][in] = 0.001f;
			return true;
		}
		//���ԉ��Z�B
		_RepeatTimer[ANALOG][in] += Time::DeltaTime();
		if (_RepeatTimer[ANALOG][in] >= _Interval[ANALOG][in])
		{
			_RepeatTimer[ANALOG][in] = 0.0f;
			return true;
		}
	}
	else
	{
		_Interval[ANALOG][in] = interval;
	}

	return false;
}
;

void XInput::Vibration(int Rmoter, int Lmoter)
{
	//�U��
	XINPUT_VIBRATION vibration;
	//������
	ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
	vibration.wRightMotorSpeed = Rmoter; //�E���[�^�[�̉�]�� use any value between 0-65535 here
	vibration.wLeftMotorSpeed = Lmoter; //�����[�^�[�̉�]�� use any value between 0-65535 here
	//�U���Z�b�g , �G���[�`�F�b�N
	if(XInputSetState(_UserIndex, &vibration) == ERROR_DEVICE_NOT_CONNECTED)
	{
		//�ڑ�����ĂȂ�
		_IsConnect = false;
	}
}

void XInput::_Rounding()
{
	/*const int LowerLimit = 100;
	const int UpperLimit = 65535;*/

	if (-XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE < _State.Gamepad.sThumbLY &&
		_State.Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		_State.Gamepad.sThumbLY = 0;
	}

	if (-XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE < _State.Gamepad.sThumbLX &&
		_State.Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		_State.Gamepad.sThumbLX = 0;
	}

	if (-XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE < _State.Gamepad.sThumbRY &&
		_State.Gamepad.sThumbRY <  XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
	{ _State.Gamepad.sThumbRY = 0; }

	if (-XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE < _State.Gamepad.sThumbRX &&
		_State.Gamepad.sThumbRX <  XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
	{ _State.Gamepad.sThumbRX = 0; }
}