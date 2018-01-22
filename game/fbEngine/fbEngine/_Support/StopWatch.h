#pragma once
#include <time.h>
#include <mmsystem.h>
#include"fbstdafx.h"

class CStopWatch
{
public:
	//CStopWatch()
	//{
	//	LARGE_INTEGER Freq;
	//	// 1�b������N���b�N�����擾
	//	QueryPerformanceFrequency(&Freq);
	//	_dFreq = (double)Freq.QuadPart;
	//}
	//void Start()
	//{
	//	//�����i�N���b�N���j���擾
	//	QueryPerformanceCounter(&_begin);
	//};
	//void Stop()
	//{
	//	LARGE_INTEGER end;
	//	QueryPerformanceCounter(&end);                     // ���݂̎������擾���A
	//	LONGLONG LongDef = end.QuadPart - _begin.QuadPart; // �����J�E���g�����Z�o����B
	//	double dDef = (double)LongDef / _dFreq;                     // �{���x���������_�ɕϊ�
	//	_Timer = dDef * 1000.0;
	//};
	//const double& GetTime()
	//{
	//	return _Timer;
	//}

	/*!
	*@brief	�R���X�g���N�^�B
	*/
	CStopWatch()
	{
		freq = 0;
		end = 0;
		begin = 0;
		elapsed = 0.0;
		elapsedMill = 0.0;
		elapsedMicro = 0.0;
	}
	/*!
	*@brief	�f�X�g���N�^�B
	*/
	~CStopWatch()
	{
	}
	/*!
	*@brief	�v���J�n�B
	*/
	void Start()
	{
		::QueryPerformanceCounter((LARGE_INTEGER*)&begin);
	}
	/*!
	*@brief	�v���I��
	*/
	void Stop()
	{
		::QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
		::QueryPerformanceCounter((LARGE_INTEGER*)&end);
		elapsed = double(end - begin) / freq;
		elapsedMill = elapsed * 1000.0;
		elapsedMicro = elapsedMill * 1000.0;
	}
	/*!
	*@brief	�o�ߎ��Ԃ��擾(�P��:�b)
	*/
	double GetElapsed() const
	{
		return elapsed;
	}
	/*!
	*@brief	�o�ߎ��Ԃ��擾(�P��:�~���b)
	*/
	double GetElapsedMillisecond() const
	{
		return elapsedMill;
	}
	/*!
	*@brief	�o�ߎ��Ԃ��擾(�P��:�}�C�N���b)
	*/
	double GetElapsedMicrosecond() const
	{
		return elapsedMicro;
	}
private:
	////�N���b�N�J�E���g��
	//LARGE_INTEGER _begin;
	//// 1�b������N���b�N�J�E���g���i���g���j
	//double _dFreq;
	//double _Timer;

	LONGLONG freq;
	LONGLONG end;
	LONGLONG begin;
	double elapsed;			//�o�ߎ���(�P�ʁF�b)
	double elapsedMill;		//�o�ߎ���(�P�ʁF�~���b)
	double elapsedMicro;	//�o�ߎ���(�P�ʁF�}�C�N���b)
};