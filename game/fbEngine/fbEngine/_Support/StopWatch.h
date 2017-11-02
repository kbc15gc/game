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
		// 1秒当たりクロック数を取得
		QueryPerformanceFrequency(&Freq);
		_dFreq = (double)Freq.QuadPart;
	}
	void Start()
	{
		//時刻（クロック数）を取得
		QueryPerformanceCounter(&_begin);
	};
	void Stop()
	{
		LARGE_INTEGER end;
		QueryPerformanceCounter(&end);                     // 現在の時刻を取得し、
		LONGLONG LongDef = end.QuadPart - _begin.QuadPart; // 差分カウント数を算出する。
		double dDef = (double)LongDef / _dFreq;                     // 倍精度浮動小数点に変換
		_Timer = dDef * 1000.0;
	};
	const double& GetTime()
	{
		return _Timer;
	}
private:
	//クロックカウント数
	LARGE_INTEGER _begin;
	// 1秒当たりクロックカウント数（周波数）
	double _dFreq;
	double _Timer;
};