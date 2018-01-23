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
	//	// 1秒当たりクロック数を取得
	//	QueryPerformanceFrequency(&Freq);
	//	_dFreq = (double)Freq.QuadPart;
	//}
	//void Start()
	//{
	//	//時刻（クロック数）を取得
	//	QueryPerformanceCounter(&_begin);
	//};
	//void Stop()
	//{
	//	LARGE_INTEGER end;
	//	QueryPerformanceCounter(&end);                     // 現在の時刻を取得し、
	//	LONGLONG LongDef = end.QuadPart - _begin.QuadPart; // 差分カウント数を算出する。
	//	double dDef = (double)LongDef / _dFreq;                     // 倍精度浮動小数点に変換
	//	_Timer = dDef * 1000.0;
	//};
	//const double& GetTime()
	//{
	//	return _Timer;
	//}

	/*!
	*@brief	コンストラクタ。
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
	*@brief	デストラクタ。
	*/
	~CStopWatch()
	{
	}
	/*!
	*@brief	計測開始。
	*/
	void Start()
	{
		::QueryPerformanceCounter((LARGE_INTEGER*)&begin);
	}
	/*!
	*@brief	計測終了
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
	*@brief	経過時間を取得(単位:秒)
	*/
	double GetElapsed() const
	{
		return elapsed;
	}
	/*!
	*@brief	経過時間を取得(単位:ミリ秒)
	*/
	double GetElapsedMillisecond() const
	{
		return elapsedMill;
	}
	/*!
	*@brief	経過時間を取得(単位:マイクロ秒)
	*/
	double GetElapsedMicrosecond() const
	{
		return elapsedMicro;
	}
private:
	////クロックカウント数
	//LARGE_INTEGER _begin;
	//// 1秒当たりクロックカウント数（周波数）
	//double _dFreq;
	//double _Timer;

	LONGLONG freq;
	LONGLONG end;
	LONGLONG begin;
	double elapsed;			//経過時間(単位：秒)
	double elapsedMill;		//経過時間(単位：ミリ秒)
	double elapsedMicro;	//経過時間(単位：マイクロ秒)
};