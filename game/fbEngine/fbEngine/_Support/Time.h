#pragma once
#pragma comment(lib, "winmm.lib ")
#include <time.h>
#include <mmsystem.h>

//時間クラス
class Time
{
public:
	Time();
	~Time()
	{
		//変更を戻す
		timeEndPeriod(1);
	}

	static void Update() 
	{
		GetInstance()->_Update();
	}
	//デルタタイム取得
	static const double DeltaTime()
	{
		return GetInstance()->_GetDeltaTime();
	}
	//FPS取得
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
	//デルタタイム取得
	const double _GetDeltaTime();
	//FPS取得
	const float& _GetFPS();
private:
	// 使用する計測関数の判定フラグ
	int _CounterFlag;                
	//クロックカウント数
	LARGE_INTEGER _Counter;
	// 1秒当たりクロックカウント数（周波数）
	double _dFreq;     
	// 以前のクロックカウント数
	LONGLONG _OldLongCount;           


	//前回のアップデートと今のアップデートとの時差を格納(ミリ秒単位)
	double _DeltaTime;
	//前回の時間格納
	DWORD  lastTime;

	//移動平均をする際のサンプル数。
	int _SampleNum = 20;
	//フレームリスト。
	list<double> _FrameList;
	//合計時間。
	double  _SumTime;
	//FPS.
	float _FPS;


	//シングルトン
	static Time* _Instance;
};