#pragma once


enum class AnalogE {
	TRIGGER = 0b100000,	//トリガー
	L_STICK = 0b10000,	//左スティック
	R_STICK = 0b1000,	//右スティック
	X = 0b100,			//横
	Y = 0b10,			//縦
	INVERT = 0b1,		//反転
	ANALOG = 0b111110,


	R_TRIGGER = TRIGGER | X,		//右ﾄﾘｶﾞｰ
	L_TRIGGER = TRIGGER | Y,		//左ﾄﾘｶﾞｰ
	L_STICKR = L_STICK | X,			//左スティック右
	L_STICKL = L_STICK | X | INVERT,//左スティック左
	L_STICKU = L_STICK | Y,			//左スティック上
	L_STICKD = L_STICK | Y | INVERT,//左スティック下
	R_STICKR = R_STICK | X,			//右スティック右
	R_STICKL = R_STICK | X | INVERT,//右スティック左
	R_STICKU = R_STICK | Y,			//右スティック上
	R_STICKD = R_STICK | Y | INVERT,//右スティック下
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
	//[in] 判定したいアナログボタン。
	//[in] 最も大きい値以外を無視するか？
	bool IsPressAnalog(AnalogE a, bool exclusive = false);
	//トリガーやスティックの値が欲しいときにどうぞ。
	//スティックは-32768 ～ 32767(65536)
	//トリガーは0 ～ 255
	Vector2 GetAnalog(AnalogE in);
	//
	bool AnalogRepeat(AnalogE analog, float interval, bool exclusive = false);
	//モーターを振動させる
	//第一引数：int 右モーターの振動数
	//第二引数：int 左モーターの振動数
	void Vibration(int Rmoter, int Lmoter);
private:
	//丸める
	void _Rounding();
private:
	int _UserIndex;			//コントローラーのナンバー(0~3)
	bool _IsConnect;			//接続されているかどうか
	XINPUT_STATE _State;		//現在の入力情報
	XINPUT_STATE _BeforeState;	//1フレーム前のステート
	
	//
	map<AnalogE,int> _AnalogValue;
	//最も値が大きいもの。
	AnalogE _Most;
	//キーリピート機能で使うタイマー。
	map<int,float> _RepeatTimer[2];
	//
	map<float, float> _Interval[2];
};