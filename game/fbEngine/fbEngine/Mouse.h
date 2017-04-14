#pragma once
//�}�E�X�̓��͒l�H
enum MouseInE
{
	L_CLICK = 0,//���N���b�N
	R_CLICK,	//�E�N���b�N
	WHEEL_PUSH,	//�z�C�[�����������Ƃ�
	X = 9,		//�}�E�X��X��
	Y,			//�}�E�X��Y��
	WHEEL_ROLL	//�z�C�[���̉�]
};
//�}�E�X����̓��͎�t
class Mouse
{
public:
	HRESULT Initialize(LPDIRECTINPUT8 Dinput);
	void Update();
	//�l���擾
	int GetValue(MouseInE m);
	//�X�N���[����̃}�E�X�J�[�\���̈ʒu���擾
	Vector2 GetCursorPosOnScreen();
	//�E�B���h�E�̍������Ƃ������ΓI�ȃ}�E�X�̃|�W�V�����擾
	Vector2 GetCursorPosOnWindow(HWND hwnd);
private:
	// DirectInput�f�o�C�X
	LPDIRECTINPUTDEVICE8 mouseDevice;
	//�}�E�X�̏��
	DIMOUSESTATE2 mouseState;
};