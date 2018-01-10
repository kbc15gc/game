#pragma once
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
	//�f���^�^�C���擾(�b)�B
	static const float DeltaTime()
	{
		return GetInstance()->_GetDeltaTime();
	}
	static void SetDeltaTime(const double& delta)
	{
		GetInstance()->_SetDeltaTime(delta);
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
	//
	void _CalcFPS(const double& delta);
	//�f���^�^�C���擾
	const double _GetDeltaTime();
	void _SetDeltaTime(const double& delta)
	{
		_SumTime -= _FrameList.back();
		//_SumTime += _PopFrame;

		_DeltaTime = delta;
		_FrameList.pop_back();
		_FrameList.push_back(_DeltaTime);
		_CalcFPS(_DeltaTime + _PopFrame);
		// ���ʉ��Z�����̍X�V
		_SumTime += delta;
	}
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
	//�O��̎��ԁB
	DWORD lastTime;
	//�Ƃ肾���ꂽ���Ԃ��i�[�B
	double  _PopFrame;

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