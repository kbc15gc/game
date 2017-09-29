#pragma once


enum AnalogE {
	L_TRIGGER,	//左ﾄﾘｶﾞｰ
	R_TRIGGER,	//右ﾄﾘｶﾞｰ
	L_STICKR,	//左スティック右
	L_STICKL,	//左スティック左
	L_STICKU,	//左スティック上
	L_STICKD,	//左スティック下
	R_STICKR,	//右スティック右
	R_STICKL,	//右スティック左
	R_STICKU,	//右スティック上
	R_STICKD,	//右スティック下
};

enum AnalogInputE
{
	TRIGGER,	//トリガー
	L_STICK,	//左スティック
	R_STICK,	//右スティック
};

//Xbox360のコントローラー
class XInput
{
public:
	XInput();
	//第一引数：int 添え字(0~3)
	void Initialize(int index);
	//値の更新
	void Update();
	//接続確認
	bool IsConnected();
	//ボタンの瞬間的な押下
	bool IsPushButton(int in);
	//ボタンの継続的な押下
	bool IsPressButton(int in);
	//キーリピート。
	//[in] ボタン。
	//[in] 間隔。
	bool KeyRepeat(int in,float interval);
	//アナログスティック、トリガーが瞬間的に入力されているか
	bool IsPushAnalog(AnalogE a);
	//アナログスティック、トリガーの継続的な押下
	bool IsPressAnalog(AnalogE a);
	//トリガーやスティックの値が欲しいときにどうぞ。
	//スティックは-32768 ～ 32767(65536)
	//トリガーは0 ～ 255
	Vector2 GetAnalog(AnalogInputE in);
	//
	bool AnalogRepeat(AnalogE analog,float interval);
	//モーターを振動させる
	//第一引数：int 右モーターの振動数
	//第二引数：int 左モーターの振動数
	void Vibration(int Rmoter, int Lmoter);
private:
	//丸める
	void _Rounding();

	int _UserIndex;			//コントローラーのナンバー(0~3)
	bool _IsConnect;			//接続されているかどうか
	XINPUT_STATE _State;		//現在の入力情報
	XINPUT_STATE _BeforeState;	//1フレーム前のステート
	
	//キーリピート機能で使うタイマー。
	float _RepeatTimer = 0.0f;
};