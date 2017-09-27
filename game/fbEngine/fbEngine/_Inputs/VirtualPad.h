#pragma once
#include "_Object\Noncopyable.h"


namespace fbEngine
{
	namespace VPad
	{
		//仮想ボタン定義。
		enum ButtonE {
			ButtonUp,		//上。
			ButtonDown,		//下。
			ButtonLeft,		//左。
			ButtonRight,	//右。
			ButtonA,		//Aボタン。
			ButtonB,		//Bボタン。
			ButtonX,		//Xボタン。
			ButtonY,		//Yボタン。
			ButtonSelect,	//セレクトボタン。
			ButtonStart,	//スタートボタン。
			ButtonRB1,		//RB1ボタン。
			ButtonRB2,		//RB2ボタン。
			ButtonRB3,		//RB3ボタン。
			ButtonLB1,		//LB1ボタン。
			ButtonLB2,		//LB2ボタン。
			ButtonLB3,		//LB3ボタン。
			ButtonNum,		//ボタンの数。
		};
	}
}

//仮想パッドクラス。
class VirtualPad :Noncopyable
{
public:
	VirtualPad();
	//
	void Initialize(KeyBoard* key, XInput* xbox);
	//
	bool IsPush(fbEngine::VPad::ButtonE button);
	bool IsPress(fbEngine::VPad::ButtonE button);
private:
	XInput* _Pad;
	KeyBoard* _Key;

	int _Trigger[fbEngine::VPad::ButtonE::ButtonNum];	//トリガー入力のフラグ。
	int _Press[fbEngine::VPad::ButtonE::ButtonNum];		//プレス入力のフラグ。
};