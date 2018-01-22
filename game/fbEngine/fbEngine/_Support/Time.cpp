#include"fbstdafx.h"
#include "Time.h"

//これは,timeGetTimeを使うver
//他には高分解能カウンタと高分解能周波数を使った,より精度の高い方法もある(KBCFreamworkでみた)

Time* Time::_Instance = nullptr;

namespace
{
	// QueryPerformanceCounter関数を使うフラグ
	const int FPSCOUNTER_QUERYPER_COUNTER = 1;
	// timeGetTime関数を使うフラグ
	const int FPSCOUNTER_TIMEGETTIME = 2;
}

Time::Time() :
	_DeltaTime(0.0f),
	_SumTime(0.0f)
{
	if (QueryPerformanceCounter(&_Counter) != 0)
	{		
		_CounterFlag = FPSCOUNTER_QUERYPER_COUNTER;
		// 生成時の時刻（クロック数）を取得
		_OldLongCount = _Counter.QuadPart;        
		LARGE_INTEGER Freq;
		// 1秒当たりクロック数を取得
		QueryPerformanceFrequency(&Freq);            
		_dFreq = (double)Freq.QuadPart;
	}
	else
	{
		_CounterFlag = FPSCOUNTER_TIMEGETTIME;
		//精度を上げる
		timeBeginPeriod(1);
		lastTime = timeGetTime();
	}

	
	_FrameList.resize(_SampleNum, 0.0f);
}

void Time::_Update()
{
	//時差取得
	double delta = _CalcDeltaTime();
	_DeltaTime = delta / 1000.0f;
	//取得。
	_PopFrame = _FrameList.front();
	//一番古いデータを削除。
	_FrameList.pop_front();
	//新しいデータ追加。
	_FrameList.push_back(delta);

	//FPS算出.
	_CalcFPS(delta);

	// 共通加算部分の更新
	_SumTime += delta - *_FrameList.begin();

	_DeltaTime = delta / 1000.0f;
}

double Time::_CalcDeltaTime()
{
	// 差分時間を計測
	if (_CounterFlag == FPSCOUNTER_QUERYPER_COUNTER)
	{
		// QueryPerformanceCounter関数による計測
		QueryPerformanceCounter(&_Counter);                     // 現在の時刻を取得し、
		LONGLONG LongDef = _Counter.QuadPart - _OldLongCount;   // 差分カウント数を算出する。
		double dDef = (double)LongDef;                          // 倍精度浮動小数点に変換
		_OldLongCount = _Counter.QuadPart;                      // 現在の時刻を保持
		return dDef * 1000 / _dFreq;                            // 差分時間をミリ秒単位で返す
	}

	// timeGetTime関数による計測
	//今の時間を取得(ミリ秒)
	DWORD nowTime = timeGetTime();
	//前回と今回の差を計算。
	DWORD delta = nowTime - lastTime;
	//beforeを更新
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