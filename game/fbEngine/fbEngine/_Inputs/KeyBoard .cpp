#include "KeyBoard .h"

HRESULT KeyBoard::Initialize(LPDIRECTINPUT8 Dinput)
{
	//�f�o�C�X�쐬
	if (FAILED(Dinput->CreateDevice(
		GUID_SysKeyboard,	// �L�[�{�[�h���󂯕t����
		&keyDevice,		// IDirectInputDevice8�|�C���^
		NULL)))			// �g��Ȃ�
	{
		return false;
	}

	//�󂯎��\���̂̃t�H�[�}�b�g��ݒ�
	if (FAILED(keyDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return false;
	}


	// ���̓f�o�C�X�ւ̃A�N�Z�X�����擾
	keyDevice->Acquire();

	ZeroMemory(now, sizeof(now));
	ZeroMemory(old, sizeof(old));

	return D3D_OK;
}

void KeyBoard::Update()
{
	//�O�̏����R�s�[
	memcpy(old, now, sizeof(old));
	// �L�[�{�[�h��������󂯎��
	keyDevice->GetDeviceState(
		sizeof(now),	// �p�����[�^ �o�b�t�@�T�C�Y
		&now);		// �󂯎��\���̂̃A�h���X
}

bool KeyBoard::isPush(int key)
{
	//�O�t���[���ŉ�����Ă��Ȃ� &&
	//��������Ă���
	return (!(old[key] & 0x80) && now[key] & 0x80);
}

bool KeyBoard::isPressed(int key)
{
	//�i�[����Ă���Ɛ擪��bit�����̂�0x80�Ƙ_���ς��Ƃ�
	return ((now[key] & 0x80) > 0);
}
