#pragma once
#include "fbEngine\_Object\_Component\Component.h"
#include "fbEngine\_Object\_GameObject\ImageObject.h"

class ParameterBar;

enum BarColor { None = -1,Red = 0, Yellow, Green, Blue,Max };

// バーの中身。
class BarElement:public ImageObject{
public:
	BarElement(char* name): ImageObject(name) {
	};
	~BarElement() {
	};
	// 初期化時にゲージの色を決める。
	void Create(BarColor,float, Transform*);
	void Start()override;
	void Update()override;
	void ImageRender()override;

	// バーを新しい値で再初期化。
	void Reset(float max) {
		_MaxValue = max;
		_TargetValue = max;
		_Value = max;
		transform->SetLocalPosition(Vector3(-0.9f, 0.0f, 0.0f));
		transform->SetLocalScale(Vector3(2.54f, 1.0f, 1.0f));
	}


	void SetValue(float value) {
		if (fabsf(_TargetValue - value) <= 0.0001f) {
			// 同じ値が再設定されたと判断。
			return;
		}
		_TargetValue = value;
		_WorkValue = (_TargetValue - _Value) / (60.0f * _Time);
		_Dir = _WorkValue / fabsf(_WorkValue);
	}
	inline float GetTargetValue() const{
		return _TargetValue;
	}
	inline float GetValue() const {
		return _Value;
	}
	inline float GetMaxValue()const {
		return _MaxValue;
	}

	inline void SetIsInterpolation(bool flg) {
		_isInterpolation = flg;
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
	bool _isInterpolation = false;	// バーのスケーリングを補間するか。
};

namespace {
	static const char* BarFileNameArray[] = {
		"hp.png",	// 赤ゲージ。
		"hp_Yellow.png",	// 黄色ゲージ。
		"hp_Green.png",	// 緑ゲージ。
		"hp_Blue.png",	// 青ゲージ。
	};
}

// バーはキャラクターの実行優先度とは独立して更新したいので、レイヤーとしてクラスを挟む。
class BarAdapter :public GameObject {
public:
	BarAdapter(char* name) : GameObject(name) {
		//constNum++;
	};
	~BarAdapter();
	// バー生成関数。
	// 引数:	どの順番でどの色のゲージを表示するかを決めた配列(先に追加した色のゲージから更新)。
	//			バーに設定する最大値(HP最大量など)。
	//			バーに設定する初期値(HP量など)。
	//			バーのスケーリングの際に補間処理を使用するか。
	//			バーの枠を描画するか。
	//			親のTransform情報(未設定かnull指定で設定しないようにできる)。
	//			位置(ローカル座標、未設定で画面の左上に表示)。
	//			拡縮(ワールド座標、未設定で画面の左上に表示)。
	//			バーの背景を描画するか。
	//			HUDとして使用するか。
	void Create(const vector<BarColor>& colors, float max, float value, bool isInterpolation, bool isRenderFrame, Transform* tr, const Vector3& pos, const Vector2& scale,bool isBackColor, bool isHud);
	void Update();
	void ImageRender()override;

	// 作成済みのバーに値を設定しなおす関数。
	// 引数：	最大値。
	//			現在値。
	void Reset(float max, float value, bool isInterpolation);

	void SetActive(const bool act, const bool children = false)override
	{
		_BarFrame->SetActive(act);
		// バー関連のオブジェクトを全部切り替え。
		for (auto& element : _BarElement) {
			element->SetActive(act);
		}
		_BarBack->SetActive(act);
		GameObject::SetActive(act, children);
	}

private:
	// バーの枠を生成する関数。
	// 引数：	位置(ワールド座標)。
	//			拡縮(ワールド座標)。
	//			HUDとして使用するか(デフォルトはtrue)。
	void _CreateBarFrame(const Vector3& pos, const Vector3& scale, bool isHud);
	// どの順番でどの色を表示するかを決めた配列を渡し、CBarElementのインスタンスを生成する関数。
	// ※先に追加した色のゲージから減っていく。
	void _ActiveBarColor(const vector<BarColor>& BarColorArray, float max, Transform* tr);
	// ワンゲージ削った際のイベント。
	void _BreakEvent();
	// 引数の値で各バーに設定する値を更新。
	void _UpdateValue(float value);
	// バーの値を減算更新。
	// 引数：	今の値との差分。
	void _UpdateSubValue(float Difference);
	// バーの値を加算更新。
	void _UpdateAddValue(float Difference);
	// HUDとして使用しない場合にスクリーン座標系に変換する関数。
	void _ToScreenPos();

public:
	inline void SubValue(float value) {
		_UpdateValue(_Value - value);
	}
	inline void AddValue(float value) {
		_UpdateValue(_Value + value);
	}
	inline void SetValue(float value) {
		_UpdateValue(value);
	}
	// 何ゲージ重ねるかを返却。
	inline short GetMaxBarNum()const {
		return _MaxBarNum;
	}
	// 今何ゲージ目かを返却。
	inline short GetNowBarNum()const {
		return _NowBarNum;
	}

	inline void SetParentComponet(ParameterBar* parent) {
		_parentComponent = parent;
	}

	inline void SetIsRender(bool flg) {
     		_isRender = flg;
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
private:
	// 描画用。
	bool _isHud;		// HUDとして使用するか。
	bool _isRenderFrame;	// バーの枠を描画するか。
	unique_ptr<ImageObject> _BarFrame;	// バーの枠。
	unique_ptr<ImageObject> _BarBack;	// バーの背景。
private:
	ParameterBar* _parentComponent = nullptr;	// このアダプターを生成した親コンポーネント。
	bool _isRender = true;
	bool _isBackColor = false;	// バーの背景を描画するか。

	//static int destNum;
	//static int constNum;
};

// バー。
class ParameterBar :
	public Component
{
	static const Vector3 CreatePos_DefaultArg;
	static const Vector2 CreateScale_DefaultArg;
public:
	ParameterBar(GameObject* g, Transform* t) :Component(g, t, typeid(this).name()) {
		//constNum++;
#ifdef _DEBUG
		mbstowcs_s(nullptr, name, typeid(*this).name(), strlen(typeid(*this).name()));
#endif

	};
	~ParameterBar() {};

	void OnDestroy()override;

	// バー生成関数。
	// 引数:	どの順番でどの色のゲージを表示するかを決めた配列(先に追加した色のゲージから更新)。
	//			バーに設定する最大値(HP最大量など)。
	//			バーに設定する初期値(HP量など)。
	//			バーのスケーリングの際に補間処理を使用するか(デフォルトはtrue)。
	//			バーの枠を描画するか(デフォルトはtrue)。
	//			親のTransform情報(未設定かnull指定で設定しないようにできる)。
	//			更新優先度(デフォルトは5)。
	//			位置(ローカル座標、未設定で画面の左上に表示)。
	//			拡縮(ワールド座標、未設定で画面の左上に表示)。
	//			バーの背景を使用するか(デフォルトはfalse)。
	//			HUDとして使用するか(デフォルトはtrue)。
	inline void Create(const vector<BarColor>& colors, float max, float value,bool isInterpolation = true, bool isRenderFrame = true, Transform* tr = nullptr, int priorty = 5, const Vector3& pos = CreatePos_DefaultArg, const Vector2& scale = CreateScale_DefaultArg, bool isBackColor = false,bool isHud = true) {
		if (_Object) {
			INSTANCE(GameObjectManager)->AddRemoveList(_Object);
		}
		_Object = INSTANCE(GameObjectManager)->AddNew<BarAdapter>("ParamterBar", priorty);
		_Object->Create(colors,max, value, isInterpolation,isRenderFrame, tr, pos, scale, isBackColor,isHud);
		_Object->SetParentComponet(this);	// 更新管理を行うためにこのクラスのポインタを渡す。
	}

	// 作成済みのバーに値を設定しなおす関数。
	// 引数：	最大値。
	//			現在値。
	//			バーのスケーリングに補間処理を使用するか。
	inline void Reset(float max, float value,bool isInterpolaation) {
		_Object->Reset(max,value, isInterpolaation);
	}
public:
	inline void SubValue(float value) {
		_Object->SubValue(value);
	}
	inline void AddValue(float value) {
		_Object->AddValue(value);
	}
	inline void SetValue(float value) {
		_Object->SetValue(value);
	}
	// 何ゲージ重ねるかを返却。
	inline short GetMaxBarNum()const {
		return _Object->GetMaxBarNum();
	}
	// 今何ゲージ目かを返却。
	inline short GetNowBarNum()const {
		return _Object->GetNowBarNum();
	}
	// HadBarのTransform情報を取得。
	inline Transform* GetTransform()const {
		return _Object->transform;
	}

	inline void RenderEnable() {
		_Object->SetIsRender(true);
	}
	inline void RenderDisable() {
		_Object->SetIsRender(false);
	}

	void SetEnable(const bool flg)override
	{
		if (_Object) {
			_Object->SetActive(flg);
		}
		Component::SetEnable(flg);
	}

private:
	BarAdapter* _Object = nullptr;
	//static int destNum;
	//static int constNum;
};