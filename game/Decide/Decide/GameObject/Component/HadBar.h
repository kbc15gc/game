#pragma once
#include "fbEngine\_Object\_Component\Component.h"
#include "fbEngine\_Object\_GameObject\ImageObject.h"

enum BarColor {  Red = 0, Yellow,Green,Blue};
// バーの中身。
class CBarElement:public ImageObject{
public:
	CBarElement(char* name): ImageObject(name) {
	};
	~CBarElement() {};
	// 初期化時にゲージの色を決める。
	void Create(BarColor,float,float, Transform*);
	void Update()override;
	void SetValue(float value) {
		m_TargetValue = value;
		m_WorkValue = (m_TargetValue - m_Value) / (60.0f * m_Time);
		m_Dir = m_WorkValue / fabsf(m_WorkValue);
	}
	float GetValue() {
		return m_Value;
	}
	float GetTargetValue() {
		return m_TargetValue;
	}
	//void AddValue(int value) {
	//	m_Value += value;
	//}
private:
	void BarValueUpdate();
	void BarScaling();
private:
	float m_TargetValue;	// 一本分の値(更新処理での目標値)。
	float m_Value;		// 一本分の値(更新処理で変動させる値で、実際に画面に表示されている数値)。
	float m_MaxValue;	// 一本分の最大値。
	float m_InitBarScale_X;	// バー画像の最初のスケール(x)。
	float m_InitBarPosition_X;	// バー画像の最初のポジション(x)。
	float m_WorkValue; // m_Valueをm_TargetValueに向けて徐々に進めていくときの値。
	float m_Dir = 1.0f;	// 更新方向(+1か-1)。
	static float m_Time;	// m_Valueがm_TargetValueに到達するまでの時間(秒)。
};

namespace {
	static const char* BarFileNameArray[] = {
		"hp.png",	// 赤ゲージ。
		"hp_back.png",	// 黄色ゲージ。
		"hp_Green.png",	// 緑ゲージ。
		"hp_Blue.png",	// 青ゲージ。
	};
}

// バー。
// ※パラメーターの減算にはまだ非対応。
class CHadBar :
	public Component
{
public:
	CHadBar(GameObject* g, Transform* t) :Component(g, t, typeid(this).name()){};
	~CHadBar();
	// バー生成関数。
	// 引数:	位置(ローカル座標)。
	//			回転(ローカル座標)。
	//			拡縮(ワールド座標)。
	//			バーの最大値。
	//			初期値。
	void Create(const vector<BarColor>&, float max, float value, Transform* tr = nullptr, Vector3 pos = Vector3(175.0f, 20.0f, 0.0f),Vector2 scale = Vector2(1.0f, 3.0f));
	void Update()override;

private:
	// どの順番でどの色を表示するかを決めた配列を渡し、CBarElementのインスタンスを生成する関数。
	// ※先に追加した色のゲージから減っていく。
	void ActiveBarColor(const vector<BarColor>& BarColorArray, float max, float value,Transform* tr);
	// ワンゲージ削った際のイベント。
	void BreakEvent();

public:
	void SetValue(float value);
	//void AddValue(int value) {
	//	m_NowBar->AddValue(value);
	//}
	// 何ゲージ重ねるかを返却。
	inline short GetMaxBarNum()const {
		return m_MaxBarNum;
	}
	// 今何ゲージ目かを返却。
	inline short GetNowBarNum()const {
		return m_NowBarNum;
	}

private:
	short m_MaxBarNum;	// 何ゲージ分重ねるか。
	vector<CBarElement*> m_BarElement;	// バー。
	CBarElement* m_NowBar = nullptr;	// 現在更新中のバー。
	short m_NowBarNum;	// 現在更新中のバーを表す添え字。
	float m_Varue;		// 全バー合わせての値。
	float m_MaxValue;		// 全バー合わせての最大値。
	CBarElement* m_NowSettingBar = nullptr;	// 現在先行入力中のバー。
	short m_NowSettingNum;		// 現在先行入力中のバーを表す添え字。
	short _framePriorty;
private:
	// 描画用。
	ImageObject* _BarFrame = nullptr;	// バーの枠。
};