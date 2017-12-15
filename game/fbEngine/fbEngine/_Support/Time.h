#pragma once
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
	//デルタタイム取得(秒)。
	static const float DeltaTime()
	{
		return GetInstance()->_GetDeltaTime();
	}
	static void SetDeltaTime(const double& delta)
	{
		GetInstance()->_SetDeltaTime(delta);
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
	//
	void _CalcFPS(const double& delta);
	//デルタタイム取得
	const double _GetDeltaTime();
	void _SetDeltaTime(const double& delta)
	{
		_SumTime -= _FrameList.back();
		//_SumTime += _PopFrame;

		_DeltaTime = delta;
		_FrameList.pop_back();
		_FrameList.push_back(_DeltaTime);
		_CalcFPS(_DeltaTime + _PopFrame);
		// 共通加算部分の更新
		_SumTime += delta;
	}
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
	//前回の時間。
	DWORD lastTime;
	//とりだされた時間を格納。
	double  _PopFrame;

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