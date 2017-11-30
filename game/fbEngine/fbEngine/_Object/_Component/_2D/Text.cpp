#include"fbstdafx.h"
#include "Text.h"
#include "FontManager.h"

Text::Text(GameObject* g, Transform* t) :
	Component(g, t, typeid(this).name()),
	_Text(nullptr),
	_Size(1.0f),
	_Spacing(0),
	_Kerning(true),
	_DisplayCharNum(UINT_MAX),
	_MaxCharNum(0),
	_TextAnchor(fbText::TextAnchorE::MiddleCenter),
	_SaveBlendColor(Color::white)
{
	
}

void Text::Awake()
{
	//スプライトを追加
	_Sprite = gameObject->AddComponent<Sprite>();
	//削除する色
	_Sprite->SetClipColor(Color(1.0f, 1.0f, 1.0f, 0.0f));
	//自動描画しないようにする
	_Sprite->Start();
	_Sprite->SetEnable(false);
	strcpy_s(_FontStyle, 32, "ＭＳ 明朝");
}

void Text::PreRender()
{

}

void Text::ImageRender()
{
	//バッファに保持
	Vector3 bufS = transform->GetScale();
	Vector3 bufP = transform->GetPosition();
	//テキストのサイズに変更
	transform->SetScale(Vector3(bufS.x * _Size, bufS.y * _Size, bufS.z * _Size));

	Vector3 baseP = bufP;
	//フォーマットから基点を計算
	_CalcAnchor(baseP);
	
	//テキスト描画
	_RenderText(baseP);
	
	//元に戻す
	transform->SetPosition(bufP);
	transform->SetScale(bufS);
}

void Text::Initialize(const wchar_t * string, const float& size, const Color& color, const fbSprite::SpriteEffectE& flg, const char * style, fbText::TextAnchorE anchor)
{
	SetStyle(style);
	SetSize(size);
	SetText(string);
	SetAnchor(anchor);
	SetBlendColor(color);
	SetEffectFlg(flg,true);
}

void Text::Initialize(const TextParameter & param)
{
	SetStyle(param.Style);
	SetSize(param.FontSize);
	SetText(param.String);
	SetAnchor(param.Anchor);
	SetBlendColor(param.Color);
	SetEffectFlg(param.EffectFlg, true);
}

void Text::SetText(const wchar_t * s)
{
	if (s == nullptr) {
		return;
	}
	//前回の文字列解放
	if (_Text)
		SAFE_DELETE(_Text);
	//長さ(+1は終端文字分)
	size_t len = wcslen(s) + 1;
	//メモリ確保
	_Text = new wchar_t[len]();
	//値コピー
	wcscpy_s(_Text, len, s);
	INSTANCE(FontManager)->Createfont(_Text, _FontStyle);

	_UpdateLength();
}

void Text::SetText(const char * s)
{
	//wcharに変換
	wchar_t w[512];
	setlocale(LC_CTYPE, "jpn");

	mbstowcs(w, s, 512);
	SetText(w);
}

void Text::SetStyle(const char* s)
{
	//"fbText::TextStyleE::"が含まれているかチェック
	if (strstr((char*)s, "fbText::TextStyleE::") != NULL)
	{
		strcpy_s(_FontStyle, 32, s+strlen("fbText::TextStyleE::"));
	}
	else
	{
		//そのままコピー
		strcpy_s(_FontStyle, 32, s);
	}
}

void Text::SetSize(const float& s)
{
	//作られているフォントのサイズで割った倍率
	_Size = s / FontSize;
	_UpdateLength();
}

void Text::SetSpacing(const float& space)
{
	_Spacing = space;
	_UpdateLength();
}

void Text::SetBlendColor(const Color& c)
{
	_SaveBlendColor = c;
	_Sprite->SetBlendColor(c);
}

void Text::SetEffectFlg(const fbSprite::SpriteEffectE& e)
{
	_Sprite->SetEffectFlg((DWORD)e);
}

void Text::SetEffectFlg(const fbSprite::SpriteEffectE& e, const bool& f)
{
	_Sprite->SetEffectFlg((DWORD)e,f);
}

void Text::SetAnchor(fbText::TextAnchorE anchor)
{
	_TextAnchor = anchor;
}

void Text::SetKerning(const bool & kerning)
{
	_Kerning = kerning;
}

void Text::_CalcAnchor(Vector3 & pos)
{
	//フォーマット
	const unsigned int& anchor = (unsigned int)_TextAnchor;
	Vector3 sca = transform->GetScale();
	//移動量
	Vector2 offset(_Length.x * sca.x, -_Length.y * sca.y);
	//半分に
	offset *= 0.5f;
	if (anchor & (unsigned int)fbText::TextAnchorE::Center)
	{
		pos.x -= offset.x;
	}
	else if (anchor & (unsigned int)fbText::TextAnchorE::Right)
	{
		pos.x -= offset.x * 2;
	}

	//上下の移動
	if (anchor & (unsigned int)fbText::TextAnchorE::Middle)
	{
		pos.y -= offset.y;
	}
	else if (anchor & (unsigned int)fbText::TextAnchorE::Lower)
	{
		pos.y -= offset.y * 2;
	}
}

void Text::_UpdateLength()
{
	//nullチェック
	if (!_Text)
		return;
	//初期化
	float width = 0;
	//最大値保持(xとyで扱いが違うので注意されたし)
	_Length = Vector2(0, 0);
	Vector2 MaxLength(0, 0);
	_MostHeight = 0.0f;
	_MaxCharNum = 0;
	//横の長さ計算
	for (short i = 0; _Text[i] != '\0'; i++)
	{
		//文字数カウント
		_MaxCharNum++;
		//改行文字。
		if (_Text[i] == '\n')
		{
			width = 0.0f;
			if (i == 0 && "\n") {
				// 最初の1文字目が改行文字だった。
				abort();
			}
			_Length.y += MaxLength.y;
			//次の改行も使いまわすか？
			if (_Text[i + 1] != '\n') {
				MaxLength.y = 0.0f;
			}
		}
		//カラーコード。
		else if (_Text[i] == '<')
		{
			wchar_t copy[512], *next;
			// < を取り除く。
			wcscpy_s(copy, 512, _Text + i + 1);
			// > までの文字列を取得。
			wchar_t* addres = wcstok_s(copy, L">", &next);
			i += wcslen(addres) + 1;
		}
		//普通の文字。
		else
		{
			//文字のデータ取得
			FontData* data = INSTANCE(FontManager)->Findfont(_Text[i], _FontStyle);
			GLYPHMETRICS gm = data->gm;
			//文字の横幅を足していく
			width += _Kerning ? gm.gmBlackBoxX : (gm.gmCellIncX + _Spacing);

			//横幅更新
			MaxLength.x = max(MaxLength.x, width);
			MaxLength.y = max(MaxLength.y, max(gm.gmBlackBoxY, gm.gmptGlyphOrigin.y));
			//最も大きいものを保持
			_MostHeight = max(_MostHeight, gm.gmptGlyphOrigin.y);
		}
	}
	//横幅は最大のものに
	_Length.x = MaxLength.x;
	//縦幅は最大のものを足す
	_Length.y += MaxLength.y;
}

void Text::_RenderText(const Vector3 & base)
{
	//一つ前の文字のオフセット量セット
	Vector3 offset(0, 0, 0);
	//高さ
	float maxHeight = 0;
	if (_Text == nullptr)
		return;
	//フォントをセットして文字数分描画するっていうのは重い！
	for (short idx = 0,count = 0; _Text[idx] != '\0';)
	{
		//文字数以上になったなら描画止め。
		if (_DisplayCharNum <= count)
			break;

		//この文字のスケール
		float scale = transform->GetScale().x;
		//改行文字ではない
		if (_Text[idx] != '\n')
		{
			//文字のデータ取得
			FontData* data = INSTANCE(FontManager)->Findfont(_Text[idx], _FontStyle);
			GLYPHMETRICS gm = data->gm;
			//画像の左上の座標
			Vector3 origin(0, -gm.gmptGlyphOrigin.y, 0);
			origin.Scale(scale);
			origin.y += _MostHeight * scale;
			//基点　+　移動量　+　各文字の微調整
			Vector3 pos = base + offset + origin;
			pos.x += (gm.gmBlackBoxX / 2)*scale;
			pos.y += (gm.gmBlackBoxY / 2)*scale;

			transform->SetPosition(pos);
			//テクスチャセット
			_Sprite->SetTexture(data->texture);
			//描画
			_Sprite->ImageRender();

			//文字の幅
			float width = _Kerning ? gm.gmBlackBoxX : gm.gmCellIncX + _Spacing;
			//文字の幅分移動量をプラス
			offset.x += width * scale;
			//その行の文字の最大の高さ
			maxHeight = max(maxHeight, gm.gmBlackBoxY * scale);
		}
		else
		{
			//高さ分下にずらす
			offset.y += maxHeight;
			//移動量初期化
			offset.x = 0;
			//次が改行でないなら初期化
			if (_Text[idx + 1] != '\n')
				maxHeight = 0;
		}
		count++;
		idx++;
		//リッチテキスト処理。
		if (_Text[idx] == '<')
			idx += RichText(&_Text[idx]);
	}
}

int Text::RichText(const wchar_t* text)
{
	wchar_t copy[512], *next;
	// < を取り除く。
	wcscpy_s(copy, 512, text + 1);
	// > までの文字列を取得。
	wchar_t* addres = wcstok_s(copy, L">", &next);
	if (addres == nullptr)return 0;


	wchar_t* type;
	//タイプチェック
	if (type = wcsstr(addres, L"color"))
	{
		if (addres[0] == L'/')
		{
			_Sprite->SetBlendColor(_SaveBlendColor);
		}
		else
		{
			//値となりうる文字列を取り出す。
			wchar_t num[20];
			wcscpy_s(num, ARRAY_SIZE(num), type + wcslen(L"color="));
			DWORD color = wcstoul(num, nullptr, 16);
			_Sprite->SetBlendColor(Color((unsigned int)((color & 0xff000000) / pow(256, 3)), (color & 0x00ff0000) / (DWORD)pow(256, 2), (color & 0x0000ff00) / (DWORD)pow(256, 1), (color & 0x000000ff)));
		}
	}
	//+2は <> の二文字分。
	return wcslen(addres) + 2;
}