#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"

#include "fbEngine\_Object\_GameObject\ImageObject.h"
#include "fbEngine\_Object\_GameObject\TextObject.h"
#include "GameObject\Village\MessageManager.h"
#include "fbEngine\_Object\_GameObject\SoundSource.h"

namespace
{
	enum TextBoxStateE: unsigned int
	{
		CLOSE = BIT(0),					//閉じた
		OPEN = BIT(1),					//開いた
		ANIMATION = BIT(2),				//アニメーション中
		TITLE = CLOSE | ANIMATION,		//タイトル
		OPENING = BIT(3) | ANIMATION,	//開いている
		CLOSING = BIT(4) | ANIMATION,	//閉じている
	};
}

class TextBox : public GameObject
{
public:
	TextBox(const char* name);
	~TextBox();
	void Awake()override;
	void PreUpdate()override;
	void Update()override;

	//使用するメッセージ設定。
	void SetMessageID(const int& id);
	//話す
	void Speak();
	//タイトルを表示するかどうか？
	void Title(bool show);
	
	//メッセージを閉じる。
	void CloseMessage();

	//ボックスを閉じる。
	void CloseBox();
	
	//テキスト表示スピード設定。
	void SetTextSpeed(const float speed)
	{
		_TextSpeed = speed;
	}

	// メッセージが終了したか取得。
	inline bool IsMessageEnd()const {
		return (_State == TextBoxStateE::CLOSING);
	}

	// 最後のメッセージが終了したか取得。
	inline bool IsLastMessageEnd()const {
		return (_State == TextBoxStateE::CLOSE && _isEndMessage);
	}

	inline bool IsClose()const {
		return (_State == TextBoxStateE::CLOSING);
	}
	void SetEventNo(int no)
	{
		_EventNo = no;
	}

private:
	//メッセージボックスを開く。
	void _OpenMessage();
	//次のメッセージへ。
	void _NextMessage();
	//テキストをコンポーネントに適用
	void _SetText(const char* text);
	//指定したIDのメッセージを設定
	// 戻り値はメッセージを設定できたか。
	bool _SetMessage(const int& id);
	//ポップアップアニメーションとかする
	void _Animation();
	//表示させる文字数増加
	void _IncreaseChar();

	//ボイスの再生。
	void _PlayVoice(string filepath)
	{
		_Voice.Stop();
		_Voice.Init(filepath.c_str(), false);
		_Voice.Play(false);
	}
private:
	//テキストボックスの背景□とカーソル▼
	ImageObject* _BoxImage[2];
	//Aボタンのイメージ画像。
	ImageObject* _AButton;
	//テキスト
	TextObject* _Text;
	//読み始めるテキストのID
	int _StartTextID;
	//テキストの情報
	MessageInfo* _Message;

	//表示する文字数。
	float _CharNum;
	//テキストを表示するスピード
	float _TextSpeed;
	//メッセージボックスが開いている。
	TextBoxStateE _State;
	//スケール
	float _AnimeTime;

	//実行するイベントの番号。
	int _EventNo = -1;
	//再生するボイス。
	SoundSource _Voice;

	bool _isEndMessage = false;	// 最後のメッセージを表示し終わったか(次回の更新処理の前に再初期化が行われるので、確実に判定を取りたい場合などはLateUpdateなどで行ってください)。
};