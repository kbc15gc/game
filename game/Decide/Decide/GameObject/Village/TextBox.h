#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"

#include "fbEngine\_Object\_GameObject\ImageObject.h"
#include "fbEngine\_Object\_GameObject\TextObject.h"
#include "GameObject\Village\MessageManager.h"
class TextBox : public GameObject
{
public:
	TextBox(const char* name);
	~TextBox();
	void Awake()override;
	void Update()override;

	//メッセージボックスを開く。
	void OpenMessage(const int& id);
	//メッセージボックスを閉じる。
	void CloseMessage();
	//次のメッセージへ。
	void NextMessage();
	void SetTextSpeed(const float speed)
	{
		_TextSpeed = speed;
	}
private:
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
	//テキストの情報
	MessageInfo* _Message;
	//表示する文字数。
	float _CharNum;
	//テキストを表示するスピード
	float _TextSpeed;
	//メッセージボックスが開いている。
	bool _Open;
	//アニメーション中である
	bool _PlayingAnim;
	//スケール
	float _Scale;
};