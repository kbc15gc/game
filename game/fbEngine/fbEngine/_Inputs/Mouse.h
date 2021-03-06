#pragma once
//マウスの入力値？
enum MouseInE
{
	L_CLICK = 0,//左クリック
	R_CLICK,	//右クリック
	WHEEL_PUSH,	//ホイールを押したとき
	X = 9,		//マウスのX軸
	Y,			//マウスのY軸
	WHEEL_ROLL	//ホイールの回転
};
//マウスからの入力受付
class Mouse
{
public:
	HRESULT Initialize(LPDIRECTINPUT8 Dinput);
	void Update();
	//値を取得
	int GetValue(MouseInE m);
	//離した瞬間を取得
	int GetUp(MouseInE m);
	//瞬間的な押下を取得
	int GetDown(MouseInE m);
	//スクリーン上のマウスカーソルの位置を取得
	Vector2 GetCursorPosOnScreen();
	//ウィンドウの左上を基準とした相対的なマウスのポジション取得
	Vector2 GetCursorPosOnWindow(HWND hwnd);
private:
	// DirectInputデバイス
	LPDIRECTINPUTDEVICE8 _MouseDevice;
	//マウスの情報
	DIMOUSESTATE2 _State, _Old;
};