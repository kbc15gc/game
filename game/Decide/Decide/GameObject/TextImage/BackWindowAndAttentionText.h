#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"
#include "GameObject\Player\Player.h"
class ImageObject;
class TextObject;

//背景画像と注意文を表示するクラス。
class BackWindowAndAttentionText :
	public GameObject
{
public:
	//コンストラクタ。
	BackWindowAndAttentionText(const char* name);

	//デストラクタ。
	~BackWindowAndAttentionText();

	void Awake()override;

	//テキストと背景画像の作成。
	//表示する文字列、背景画像の座標、背景画像のサイズ、テキストのローカル座標、テキストのサイズ、テキストのカラー。
	void Create(const wchar_t * string,
		const Vector3& windowPos,
		const Vector2& windowSize,
		const Vector3& textPos,
		float textSize,
		const Color& color);

	//更新。
	void Update()override;

#ifdef _DEBUG
	void Debug()override;
#endif


	//背景画像の位置設定。
	void SetWindowPos(const Vector3& pos) {
		_AttentionBackWindowPos = pos;
	}

	//背景画像のサイズ設定。
	void SetWindowSize(const Vector2& size) {
		_AttentionBackWindowSize = size;
	}

	//テキストのローカル座標設定。
	void SetTextPos(const Vector3& pos) {
		_AttentionTextPos = pos;
	}

	//テキストの文字サイズ設定。
	void SetTextFontSize(float size) {
		_AttentionTextFontSize = size;
	}

	//テキストの文字。
	void SetText(const wchar_t * string) {
		_AttentionText->SetText(string);
	}

	//アクティブフラグをtrueに変更。
	void SetActiveTrue() {
		_AttentionBackWindowImage->SetActive(true, true);
		//static_cast<Player*>(INSTANCE(GameObjectManager)->FindObject("Player"))->PlayerStopEnable();
	}

	//アクティブフラグをfalseに変更。
	void SetActiveFalse() {
		_AttentionBackWindowImage->SetActive(false, true);
		//static_cast<Player*>(INSTANCE(GameObjectManager)->FindObject("Player"))->PlayerStopDisable();
	}
private:
	ImageObject*	_AttentionBackWindowImage	= nullptr;			//背景画像。
	Vector3			_AttentionBackWindowPos		= Vector3::zero;	//エラーテキストの背景ウインドウの位置。
	Vector2			_AttentionBackWindowSize	= Vector2::zero;	//エラーテキストの背景ウインドウのサイズ。
	TextObject*		_AttentionText				= nullptr;			//テキスト(注意文)。
	Vector3			_AttentionTextPos			= Vector3::zero;	//エラーテキストのローカル座標。
	float			_AttentionTextFontSize		= 0.0f;				//エラーテキストのフォントサイズ。
};