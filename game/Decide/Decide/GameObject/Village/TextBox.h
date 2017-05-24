#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"

#include "fbEngine\_Object\_GameObject\ImageObject.h"
#include "fbEngine\_Object\_GameObject\TextObject.h"
#include "GameObject\Village\MessageManager.h"

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
	void Update()override;

	//使用するメッセージ設定。
	void SetMessageID(const int& id);
	//話す
	void Speak();
	//タイトルを表示するかどうか？
	void Title(bool show);
	
	//メッセージボックスを閉じる。
	void CloseMessage();
	
	//テキスト表示スピード設定。
	void SetTextSpeed(const float speed)
	{
		_TextSpeed = speed;
	}
private:
	//メッセージボックスを開く。
	void _OpenMessage();
	//次のメッセージへ。
	void _NextMessage();
	//テキストをコンポーネントに適用
	void _SetText(const char* text);
	//指定したIDのメッセージを設定
	void _SetMessage(const int& id);
	//ポップアップアニメーションとかする
	void _Animation();
	//表示させる文字数増加
	void _IncreaseChar();
private:
	//テキストボックスの背景□とカーソル▼
	ImageObject* _BoxImage[2];
	//テキスト
	TextObject* _Text;
	//最初のテキストID
	int _TextID;
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
};