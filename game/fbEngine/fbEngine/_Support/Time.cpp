#include"fbstdafx.h"
#include "Time.h"

//�����,timeGetTime���g��ver
//���ɂ͍�����\�J�E���^�ƍ�����\���g�����g����,��萸�x�̍������@������(KBCFreamwork�ł݂�)

Time* Time::_Instance = nullptr;

namespace
{
	// QueryPerformanceCounter�֐����g���t���O
	const int FPSCOUNTER_QUERYPER_COUNTER = 1;
	// timeGetTime�֐����g���t���O
	const int FPSCOUNTER_TIMEGETTIME = 2;
}

Time::Time() :
	_DeltaTime(0.0f),
	_SumTime(0.0f)
{
	if (QueryPerformanceCounter(&_Counter) != 0)
	{		
		_CounterFlag = FPSCOUNTER_QUERYPER_COUNTER;
		// �������̎����i�N���b�N���j���擾
		_OldLongCount = _Counter.QuadPart;        
		LARGE_INTEGER Freq;
		// 1�b������N���b�N�����擾
		QueryPerformanceFrequency(&Freq);            
		_dFreq = (double)Freq.QuadPart;
	}
	else
	{
		_CounterFlag = FPSCOUNTER_TIMEGETTIME;
		//���x���グ��
		timeBeginPeriod(1);
		lastTime = timeGetTime();
	}

	
	_FrameList.resize(_SampleNum, 0.0f);
}

void Time::_Update()
{
	//�����擾
	double delta = _CalcDeltaTime();
	_DeltaTime = delta / 1000.0f;
	//�擾�B
	_PopFrame = _FrameList.front();
	//��ԌÂ��f�[�^���폜�B
	_FrameList.pop_front();
	//�V�����f�[�^�ǉ��B
	_FrameList.push_back(delta);

	//FPS�Z�o.
	_CalcFPS(delta);

	// ���ʉ��Z�����̍X�V
	_SumTime += delta - *_FrameList.begin();

	_DeltaTime = delta / 1000.0f;
}

double Time::_CalcDeltaTime()
{
	// �������Ԃ��v��
	if (_CounterFlag == FPSCOUNTER_QUERYPER_COUNTER)
	{
		// QueryPerformanceCounter�֐��ɂ��v��
		QueryPerformanceCounter(&_Counter);                     // ���݂̎������擾���A
		LONGLONG LongDef = _Counter.QuadPart - _OldLongCount;   // �����J�E���g�����Z�o����B
		double dDef = (double)LongDef;                          // �{���x���������_�ɕϊ�
		_OldLongCount = _Counter.QuadPart;                      // ���݂̎�����ێ�
		return dDef * 1000 / _dFreq;                            // �������Ԃ��~���b�P�ʂŕԂ�
	}

	// timeGetTime�֐��ɂ��v��
	//���̎��Ԃ��擾(�~���b)
	DWORD nowTime = timeGetTime();
	//�O��ƍ���̍����v�Z�B
	DWORD delta = nowTime - lastTime;
	//before���X�V
	lastTime = nowTime;
	return delta;
}

void Time::_CalcFPS(const double& delta)
{
	double AveDef = (_SumTime + delta) / _SampleNum;
	if (AveDef != 0)
		_FPS = 1000.0 / AveDef;
}

const double Time::_GetDeltaTime()
{
		return (1.0 / 30.0);
	//return _DeltaTime;
}

const float& Time::_GetFPS()
{
	return _FPS;
}