#pragma once
#include "fbEngine\_Object\_Component\Component.h"
#include "fbEngine\_Object\_GameObject\ImageObject.h"

enum BarColor {  Red = 0, Yellow,Green,Blue};
// バーの中身。
class BarElement:public ImageObject{
public:
	BarElement(char* name): ImageObject(name) {
	};
	~BarElement() {};
	// 初期化時にゲージの色を決める。
	void Create(BarColor,float, Transform*);
	void Start()override;
	void Update()override;
	void ImageRender()override;
	void SetValue(float value) {
		_TargetValue = value;
		_WorkValue = (_TargetValue - _Value) / (60.0f * _Time);
		_Dir = _WorkValue / fabsf(_WorkValue);
	}
	float GetValue() {
		return _Value;
	}
	float GetTargetValue() {
		return _TargetValue;
	}
private:
	void _BarValueUpdate();
	void _BarScaling();
private:
	float _TargetValue;	// 一本分の値(更新処理での目標値)。
	float _Value;		// 一本分の値(更新処理で変動させる値で、実際に画面に表示されている数値)。
	float _MaxValue;	// 一本分の最大値。
	float _InitBarScale_X;	// バー画像の最初のスケール(x)。
	float _InitBarPosition_X;	// バー画像の最初のポジション(x)。
	float _WorkValue; // m_Valueをm_TargetValueに向けて徐々に進めていくときの値。
	float _Dir = 1.0f;	// 更新方向(+1か-1)。
	static float _Time;	// m_Valueがm_TargetValueに到達するまでの時間(秒)。
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
class ParameterBar :
	public Component
{
	static const Vector3 CreatePos_DefaultArg;
	static const Vector2 CreateScale_DefaultArg;
public:
	ParameterBar(GameObject* g, Transform* t) :Component(g, t, typeid(this).name()) {
		// このコンポーネントを持つゲームオブジェクトに管理を委譲する。
		// ※オブジェクト指向上あまりよろしくはないと思う。
		if (g) {
			_Transform = g->AddComponent<Transform>();
		}
	};
	~ParameterBar();
	// バー生成関数。
	// 引数:	どの順番でどの色のゲージを表示するかを決めた配列(先に追加した色のゲージから更新)。
	//			バーに設定する最大値(HP最大量など)。
	//			バーに設定する初期値(HP量など)。
	//			バーの枠を描画するか。
	//			親のTransform情報(未設定かnull指定で設定しないようにできる)。
	//			位置(ローカル座標、未設定で画面の左上に表示)。
	//			拡縮(ワールド座標、未設定で画面の左上に表示)。
	//			HUDとして使用するか(デフォルトはtrue)。
	void Create(const vector<BarColor>&, float max, float value, bool isRenderFrame = true, Transform* tr = nullptr,const Vector3& pos = CreatePos_DefaultArg,const Vector2& scale = CreateScale_DefaultArg, bool isHud = true);
	void Update()override;
	void ImageRender()override;

private:
	// バーの枠を生成する関数。
	// 引数：	位置(ワールド座標)。
	//			拡縮(ワールド座標)。
	//			HUDとして使用するか(デフォルトはtrue)。
	void CreateBarFrame(const Vector3& pos, const Vector3& scale, bool isHud);
	// どの順番でどの色を表示するかを決めた配列を渡し、CBarElementのインスタンスを生成する関数。
	// ※先に追加した色のゲージから減っていく。
	void _ActiveBarColor(const vector<BarColor>& BarColorArray, float max, float value,Transform* tr);
	// ワンゲージ削った際のイベント。
	void _BreakEvent();
	// 引数の値でバーのサイズを更新。
	void _UpdateValue(float value);
	// HUDとして使用しない場合にスクリーン座標系に変換する関数。
	void _ToScreenPos();
public:
	inline void SubValue(float value) {
		_UpdateValue(_Value - value);
	}
	void AddValue(float value) {
		_UpdateValue(_Value + value);
	}
	// 何ゲージ重ねるかを返却。
	inline short GetMaxBarNum()const {
		return _MaxBarNum;
	}
	// 今何ゲージ目かを返却。
	inline short GetNowBarNum()const {
		return _NowBarNum;
	}
	// HadBarのTransform情報を取得。
	inline Transform* GetTransform()const {
		return _Transform;
	}
private:
	short _MaxBarNum;	// 何ゲージ分重ねるか。
	vector<unique_ptr<BarElement>> _BarElement;	// バー。
	BarElement* _NowBar = nullptr;	// 現在更新中のバー。
	short _NowBarNum;	// 現在更新中のバーを表す添え字。
	float _Value;		// 全バー合わせての値。
	float _MaxValue;		// 全バー合わせての最大値。
	BarElement* _NowSettingBar = nullptr;	// 現在先行入力中のバー。
	short _NowSettingNum;		// 現在先行入力中のバーを表す添え字。
	short _framePriorty;
	Transform* _Transform = nullptr;	// BarのTransform情報(三次元)。
private:
	// 描画用。
	bool _isHud;		// HUDとして使用するか。
	bool _isRenderFrame;	// バーの枠を描画するか。
	unique_ptr<ImageObject> _BarFrame;	// バーの枠。
};