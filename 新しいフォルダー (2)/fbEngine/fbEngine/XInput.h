#pragma once

enum AnalogE {
	L_TRIGGER,	//���ضް
	R_TRIGGER,	//�E�ضް
	L_STICKR,	//���X�e�B�b�N�E
	L_STICKL,	//���X�e�B�b�N��
	L_STICKU,	//���X�e�B�b�N��
	L_STICKD,	//���X�e�B�b�N��
	R_STICKR,	//�E�X�e�B�b�N�E
	R_STICKL,	//�E�X�e�B�b�N��
	R_STICKU,	//�E�X�e�B�b�N��
	R_STICKD,	//�E�X�e�B�b�N��
};

enum AnalogInputE
{
	TRIGGER,	//�g���K�[
	L_STICK,	//���X�e�B�b�N
	R_STICK,	//�E�X�e�B�b�N
};

//Xbox360�̃R���g���[���[
class XInput
{
public:
	XInput();
	//�������Fint �Y����(0~3)
	void Initialize(int index);
	//�l�̍X�V
	void Update();
	//�ڑ��m�F
	bool IsConnected();
	//�{�^���̏u�ԓI�ȉ���
	bool IsPushButton(int in);
	//�{�^���̌p���I�ȉ���
	bool IsPressButton(int in);
	//�A�i���O�X�e�B�b�N�A�g���K�[���u�ԓI�ɓ��͂���Ă��邩
	bool IsPushAnalog(AnalogE a);
	//�g���K�[��X�e�B�b�N�̒l���~�����Ƃ��ɂǂ����B
	//�X�e�B�b�N��-32768 �` 32767(65536)
	//�g���K�[��0 �` 255
	Vector2 GetAnalog(AnalogInputE in);
	//���[�^�[��U��������
	//�������Fint �E���[�^�[�̐U����
	//�������Fint �����[�^�[�̐U����
	void Vibration(int Rmoter, int Lmoter);
private:
	//�ۂ߂�
	void _Rounding();

	int _UserIndex;			//�R���g���[���[�̃i���o�[(0~3)
	bool _IsConnect;			//�ڑ�����Ă��邩�ǂ���
	XINPUT_STATE _State;		//���݂̓��͏��
	XINPUT_STATE _BeforeState;	//1�t���[���O�̃X�e�[�g
};