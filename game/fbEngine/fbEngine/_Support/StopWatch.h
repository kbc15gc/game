#pragma once
#include <time.h>
#include <mmsystem.h>
#include"fbstdafx.h"

class CStopWatch
{
public:
	CStopWatch()
	{
		LARGE_INTEGER Freq;
		// 1�b������N���b�N�����擾
		QueryPerformanceFrequency(&Freq);
		_dFreq = (double)Freq.QuadPart;
	}
	void Start()
	{
		//�����i�N���b�N���j���擾
		QueryPerformanceCounter(&_begin);
	};
	void Stop()
	{
		LARGE_INTEGER end;
		QueryPerformanceCounter(&end);                     // ���݂̎������擾���A
		LONGLONG LongDef = end.QuadPart - _begin.QuadPart; // �����J�E���g�����Z�o����B
		double dDef = (double)LongDef / _dFreq;                     // �{���x���������_�ɕϊ�
		_Timer = dDef * 1000.0;
	};
	const double& GetTime()
	{
		return _Timer;
	}
private:
	//�N���b�N�J�E���g��
	LARGE_INTEGER _begin;
	// 1�b������N���b�N�J�E���g���i���g���j
	double _dFreq;
	double _Timer;
};