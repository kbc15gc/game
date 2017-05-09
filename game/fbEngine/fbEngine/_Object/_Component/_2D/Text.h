#pragma once
#include "_Object\_Component\Component.h"
#include "Sprite.h"

namespace fbText
{
	//テキストの書式
	enum class TextFormatE : unsigned int
	{
		LEFT = BIT(1),		//テキストの左端にポジションが位置する
		CENTER = BIT(2),	//テキストの真ん中にポジションが位置する
		RIGHT = BIT(3),		//テキストの右端にポジションが位置する

		UP = BIT(4),		//テキストの一番上がポジションになる
		DOWN = BIT(5)		//テキストの一番下がポジションになる
	};
	//よく使うかもしれないフォントのスタイル
	enum class TextStyleE
	{
		ＭＳ_明朝,
		DEFALT,
		NewDokabenFont,		//ドカベンフォント
	};
}

//文字列を描画するだけのクラス
class Text : public Component
{
public:
	Text(GameObject* g, Transform* t);
	void Awake()override;
	void PreRender()override;
	void ImageRender()override;
	//フォントスタイルを文字列で指定
	void Initialize(const wchar_t* string, const float& size, const Color& color = Color::white,
		const fbSprite::SpriteEffectE& flg = fbSprite::SpriteEffectE::NONE, const char* style = "ＭＳ 明朝", const unsigned int& format = (int)fbText::TextFormatE::CENTER);
	//文字列セット
	void SetString(const wchar_t* s);
	void SetString(const char* s);
	//フォントのサイズ
	void SetSize(const float& s);
	//間隔設定
	void SetSpacing(const float& space);
	//文字スタイルセット
	void SetStyle(const char* s);
	//ブレンドする色
	void SetBlendColor(const Color& c);
	//エフェクトフラグセット
	void SetEffectFlg(const fbSprite::SpriteEffectE& e);
	void SetEffectFlg(const fbSprite::SpriteEffectE& e,const bool& f);
	//書式設定
	void SetFormat(const unsigned int& format);
	//カーニング設定
	void SetKerning(const bool& kerning);
	void SetCharNum(const unsigned int& num)
	{
		_DisplayCharNum = num;
	}


	const Vector2& GetLength()
	{
		return _Length;
	}
	const float& GetSize()
	{
		return _Size;
	}
	//最大文字数を返す。
	const unsigned int& GetMaxCharNum()
	{
		return _MaxCharNum;
	}
private:
	//フォーマットによって移動する量を計算
	void _CalcFormat(Vector3& pos);
	//テキストの長さ更新
	void _UpdateLength();
	//テキスト描画
	//基点
	void _RenderText(const Vector3& base);
private:
	//文字のスタイル名デフォルトは　ＭＳ 明朝.
	char _FontStyle[32];
	//制作する文字列
	wchar_t* _WString;

	//表示する文字数
	unsigned int _DisplayCharNum;
	//最大文字数
	unsigned int _MaxCharNum;
	//テキストの長さ
	Vector2 _Length;
	//文字サイズ
	float _Size;
	//一番背の高い文字の高さ
	float _MostHeight;
	//文字間
	float _Spacing;
	//文字を詰めるかどうか？
	bool _Kerning;
	//テキストのフォーマット
	fbText::TextFormatE _TextFormat;

	//描画用スプライト
	Sprite* _Sprite;
};