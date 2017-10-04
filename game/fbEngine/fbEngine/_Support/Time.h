#pragma once
#pragma comment(lib, "winmm.lib ")
#include <time.h>
#include <mmsystem.h>

//���ԃN���X
class Time
{
public:
	Time();
	~Time()
	{
		//�ύX��߂�
		timeEndPeriod(1);
	}

	static void Update() 
	{
		GetInstance()->_Update();
	}
	//�f���^�^�C���擾
	static const double DeltaTime()
	{
		return GetInstance()->_GetDeltaTime();
	}
	//FPS�擾
	static const float& GetFPS()
	{
		return GetInstance()->_GetFPS();
	}
	static Time* GetInstance()
	{
		if (_Instance == nullptr) 
		{
			_Instance = new Time();
		}
		return _Instance;
	}

private:
	void _Update();
	//
	double _CalcDeltaTime();
	//�f���^�^�C���擾
	const double _GetDeltaTime();
	//FPS�擾
	const float& _GetFPS();
private:
	// �g�p����v���֐��̔���t���O
	int _CounterFlag;                
	//�N���b�N�J�E���g��
	LARGE_INTEGER _Counter;
	// 1�b������N���b�N�J�E���g���i���g���j
	double _dFreq;     
	// �ȑO�̃N���b�N�J�E���g��
	LONGLONG _OldLongCount;           


	//�O��̃A�b�v�f�[�g�ƍ��̃A�b�v�f�[�g�Ƃ̎������i�[(�~���b�P��)
	double _DeltaTime;
	//�O��̎��Ԋi�[
	DWORD  lastTime;

	//�ړ����ς�����ۂ̃T���v�����B
	int _SampleNum = 20;
	//�t���[�����X�g�B
	list<double> _FrameList;
	//���v���ԁB
	double  _SumTime;
	//FPS.
	float _FPS;


	//�V���O���g��
	static Time* _Instance;
};