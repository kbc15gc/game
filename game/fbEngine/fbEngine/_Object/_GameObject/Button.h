#pragma once
#include "GameObject.h"
#include "_Object\_Component\_2D\Sprite.h"
#include "_Object\_Component\_2D\SquareCollision.h"


namespace fbButton
{
	//ボタンのタイプ
	enum class ButtonTypeE : unsigned int
	{
		PRESS,		//押されている間のみONになる
		TRIGGER		//トリガー
	};
}

namespace
{
	//ボタンの状態
	enum class ButtonStateE : unsigned int
	{
		NORMAL,		//通常
		ON,			//上にカーソルがある
		PRESS,		//押されている
		DISABLED,	//無効
		NUM
	};
}

class Button:public GameObject
{
public:
	Button(char* name) :GameObject(name) {}
	void Awake()override;
	void Update()override;

	//一括設定
	void Initialize(TEXTURE* on, TEXTURE* off, const Vector2& size, const fbButton::ButtonTypeE& type = fbButton::ButtonTypeE::PRESS);

	//ボタンのテクスチャ設定(on,offでボタンが変わる)
	void SetTexture(TEXTURE* on,TEXTURE* off=nullptr);
	//コリジョンのサイズ設定
	void SetSize(const Vector2& size);
	//ボタンのタイプ設定
	void SetButtonType(const fbButton::ButtonTypeE& type);
	//色変更
	void SetButtonColor(const ButtonStateE state, const Color& color);
	//オンオフを外部から設定(トリガーじゃないと意味ないけど。)
	void SetOnOff(const bool& f);
	const bool& GetIsOn()
	{
		return _IsOn;
	}
	Sprite* GetSprite()
	{
		return _Sprite;
	}
private:
	//ステータス更新チェック,戻り値は変更されてかどうか？
	bool _CheckState();
	//ステート変化に伴った変更
	void _ChangeState();
private:
	Sprite* _Sprite;			//スプライト
	SquareCollision* _Square;	//四角形のあたり判定

	fbButton::ButtonTypeE _Type;	//ボタンのタイプ
	ButtonStateE _State;	//ステート
	bool _IsOn;	//ONかOFFか？

	//ON,OFFの時のテクスチャ
	TEXTURE *_Tex[2];
	//初期値は白からだんだん黒くなっていく
	//各ステータスの時の色
	Color _Color[(unsigned int)ButtonStateE::NUM] = { Color::white,Color(0.8f,0.8f,0.8f),Color(0.5f,0.5f,0.5f),Color(0.3f,0.3f,0.3f) };
};