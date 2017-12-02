#pragma once
#include "GameObject\Camera\GameCamera.h"

namespace
{
	static const int MAX_ARRAY_NUM = 20;
	//イベントカメラを起動するのに必要な情報。
	struct EventCameraInfo
	{
		Vector3 pos[MAX_ARRAY_NUM];			//座標。
		Quaternion rot[MAX_ARRAY_NUM];	//回転。
		float times[MAX_ARRAY_NUM];			//時間。
		int size;							//配列の要素数。
	};

	const Support::DATARECORD EventCameraData[] =
	{
		{ "pos",Support::DataTypeE::VECTOR3_ARRAY ,offsetof(struct EventCameraInfo,pos),sizeof(Vector3) * MAX_ARRAY_NUM },
		{ "rot",Support::DataTypeE::QUATERNION_ARRAY ,offsetof(struct EventCameraInfo,rot),sizeof(Quaternion) * MAX_ARRAY_NUM },
		{ "times",Support::DataTypeE::FLOAT_ARRAY ,offsetof(struct EventCameraInfo,times),sizeof(float) * MAX_ARRAY_NUM },
		{ "size",Support::DataTypeE::INT ,offsetof(struct EventCameraInfo,size),sizeof(int) },
	};
}

//イベントで使うカメラ。
class EventCamera :public GameCamera
{
public:
	EventCamera(const char* name) :GameCamera(name) {};
	~EventCamera() {};

	//コンストラクタ後の初期化。
	void Awake()override;
	//初期化。
	void Start()override;
	//更新。
	void UpdateSubClass()override;

	//実行。
	void Excute(int id);
	//イベントカメラ終了。
	void EndEvent();
private:
	virtual void _Move()override;
private:
	//実行中フラグ。
	bool _Runtime = false;
	//現在、添え字の何番目か？
	int _Index = 0;
	//タイマー。
	float _Timer = 0.0f;
	//イベントカメラの情報。
	EventCameraInfo _Info;
};