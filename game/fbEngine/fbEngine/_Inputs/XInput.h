#pragma once


enum class AnalogE {
	TRIGGER = 0b100000,	//�g���K�[
	L_STICK = 0b10000,	//���X�e�B�b�N
	R_STICK = 0b1000,	//�E�X�e�B�b�N
	X = 0b100,			//��
	Y = 0b10,			//�c
	INVERT = 0b1,		//���]
	ANALOG = 0b111110,


	R_TRIGGER = TRIGGER | X,		//�E�ضް
	L_TRIGGER = TRIGGER | Y,		//���ضް
	L_STICKR = L_STICK | X,			//���X�e�B�b�N�E
	L_STICKL = L_STICK | X | INVERT,//���X�e�B�b�N��
	L_STICKU = L_STICK | Y,			//���X�e�B�b�N��
	L_STICKD = L_STICK | Y | INVERT,//���X�e�B�b�N��
	R_STICKR = R_STICK | X,			//�E�X�e�B�b�N�E
	R_STICKL = R_STICK | X | INVERT,//�E�X�e�B�b�N��
	R_STICKU = R_STICK | Y,			//�E�X�e�B�b�N��
	R_STICKD = R_STICK | Y | INVERT,//�E�X�e�B�b�N��
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
	//�L�[���s�[�g�B
	//[in] �{�^���B
	//[in] �Ԋu�B
	bool KeyRepeat(int in,float interval);
	//�A�i���O�X�e�B�b�N�A�g���K�[���u�ԓI�ɓ��͂���Ă��邩
	bool IsPushAnalog(AnalogE a);
	//�A�i���O�X�e�B�b�N�A�g���K�[�̌p���I�ȉ���
	//[in] ���肵�����A�i���O�{�^���B
	//[in] �ł��傫���l�ȊO�𖳎����邩�H
	bool IsPressAnalog(AnalogE a, bool exclusive = false);
	//�g���K�[��X�e�B�b�N�̒l���~�����Ƃ��ɂǂ����B
	//�X�e�B�b�N��-32768 �` 32767(65536)
	//�g���K�[��0 �` 255
	Vector2 GetAnalog(AnalogE in);
	//
	bool AnalogRepeat(AnalogE analog, float interval, bool exclusive = false);
	//���[�^�[��U��������
	//�������Fint �E���[�^�[�̐U����
	//�������Fint �����[�^�[�̐U����
	void Vibration(int Rmoter, int Lmoter);
private:
	//�ۂ߂�
	void _Rounding();
private:
	int _UserIndex;			//�R���g���[���[�̃i���o�[(0~3)
	bool _IsConnect;			//�ڑ�����Ă��邩�ǂ���
	XINPUT_STATE _State;		//���݂̓��͏��
	XINPUT_STATE _BeforeState;	//1�t���[���O�̃X�e�[�g
	
	//
	map<AnalogE,int> _AnalogValue;
	//�ł��l���傫�����́B
	AnalogE _Most;
	//�L�[���s�[�g�@�\�Ŏg���^�C�}�[�B
	map<int,float> _RepeatTimer[2];
	//
	map<float, float> _Interval[2];
};