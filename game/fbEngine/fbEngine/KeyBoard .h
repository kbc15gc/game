#pragma once

//�L�[�{�[�h����̓��͎�t
class KeyBoard
{
public:
	HRESULT Initialize(LPDIRECTINPUT8 Dinput);
	void Update();

	//key�ɂ�DIK_??���g���Ƃ悢
	//��u
	bool isPush(int key);
	//�p���I
	bool isPressed(int key);
private:
	// DirectInput�f�o�C�X
	LPDIRECTINPUTDEVICE8 keyDevice;
	//�L�[�{�[�h�̏�񂪊i�[�����
	BYTE now[256];
	//�O�̃t���[���̏�Ԃ��i�[
	BYTE old[256];
};