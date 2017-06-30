#pragma once
#include "fbEngine\_Object\_Component\Component.h"
#include "fbEngine\_Object\_Component\_2D\Text.h"

#ifdef _DEBUG
class TextObject;

// デバッグ用テキスト出力用コンポーネント。
class OutputData : public Component {
public:
	OutputData(GameObject* g, Transform* t) :Component(g, t, typeid(this).name()) {
	};
	~OutputData() {
	};

	void Start()override;
	void Update()override;

private:
	// 表示用オブジェクト生成関数
	// 引数：	追加したテキストオブジェクトへのポインタ。
	//			テキストオブジェクトにつける名前。
	//			ローカルの位置(_lastChildrenがあればそこを基点に配置される、なければ無効)。
	//			設定するテキストの初期値(デフォルトは空)。
	//			テキストのサイズ。
	//			テキストの色(デフォルト白)。
	//			書式(デフォルト左揃え)。
	TextObject* _CreateTextObject(TextObject** obj, const char* name, const Vector3& localPos = Vector3(0.0f,20.0f,0.0f), const char* initText = "", const float textSize = 40.0f , Color textColor = Color::white, fbText::TextFormatE format = fbText::TextFormatE::LEFT);

	// 指定した文字列を指定した文字列で挟む関数。
	// 引数：	挟む文字列(左)。
	//			挟んでほしい文字列。
	//			挟む文字列(右)。			
	wchar_t* _TuckText(wchar_t* left, wchar_t* text, wchar_t* right);
	void OutputTextParam();
	void OutputTextTrans();

private:
	// デバッグ用出力。
	TextObject* _textName = nullptr;	// 名前。
	TextObject* _textParam = nullptr;	// パラメーター。
	TextObject* _textTransform = nullptr;	// Transform情報。
	TextObject* _lastChildren = nullptr;	// 最後に追加したテキストオブジェクト。
	Vector3 _rootPos = Vector3(750.0f, 130.0f, 0.0f);	// 最初に表示するテキストの位置。
};
#endif
