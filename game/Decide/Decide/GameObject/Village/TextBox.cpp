#include"stdafx.h"
#include "TextBox.h"
#include "GameObject\Village\EventManager.h"

TextBox::TextBox(const char * name):
	GameObject(name),
	_CharNum(0),
	_StartTextID(-1),
	_TextSpeed(1.0f),
	_State(TextBoxStateE::CLOSE),
	_AnimeTime(0.0f)
{
}

TextBox::~TextBox()
{
	INSTANCE(GameObjectManager)->AddRemoveList(_BoxImage[0]);
	INSTANCE(GameObjectManager)->AddRemoveList(_BoxImage[1]);
	INSTANCE(GameObjectManager)->AddRemoveList(_Text);
}

void TextBox::Awake()
{
	_BoxImage[0] = INSTANCE(GameObjectManager)->AddNew<ImageObject>("BoxImage",_Priority);
	_BoxImage[1] = INSTANCE(GameObjectManager)->AddNew<ImageObject>("BoxImage", _Priority);
	_Text = INSTANCE(GameObjectManager)->AddNew<TextObject>("Text", _Priority);

	//カーソル
	_BoxImage[0]->SetTexture(LOADTEXTURE("cursor.png"));
	_BoxImage[0]->SetPivot(0.5, 1.0);
	//本体
	_BoxImage[1]->SetTexture(LOADTEXTURE("window.png"));
	
	_Text->Initialize(L"", 25.0f);
	_Text->SetAnchor(fbText::TextAnchorE::UpperCenter);
	
	//親子関係を付ける。
	_BoxImage[0]->transform->SetParent(transform);
	_BoxImage[1]->transform->SetParent(_BoxImage[0]->transform);
	_Text->transform->SetParent(_BoxImage[1]->transform);

	//オブジェクトを非表示にする
	for each (Transform* child in transform->GetChildren())
	{
		child->gameObject->SetActive(false, true);
	}
	transform->SetScale(Vector3::zero);
}

void TextBox::Update()
{
	//アニメーション。
	_Animation();
	//表示する文字数の増加。
	_IncreaseChar();
}

void TextBox::SetMessageID(const int & id)
{
	_StartTextID = id;
	//メッセージ情報取得
	_Message = INSTANCE(MessageManager)->GetMess(_StartTextID);
}

void TextBox::Speak()
{
	if (_Message)
	{
		if (_State & TextBoxStateE::CLOSE)
		{
			//会話開始
			_OpenMessage();
		}
		else if (_State == TextBoxStateE::OPEN)
		{
			//次の会話へ
			_NextMessage();
		}
	}
}

void TextBox::Title(bool show)
{
	if (show)
	{
		if (_State == TextBoxStateE::CLOSE)
		{
			//ステート設定。
			_State = TextBoxStateE::TITLE;
			//テキスト設定。
			_SetText(_Message->Title);
			_Text->SetCharNum(UINT_MAX);
			//全てをアクティブにする。
			for each (Transform* child in transform->GetChildren())
			{
				child->gameObject->SetActive(true, true);
			}
			transform->SetScale(Vector3::one);
		}
	}
	else
	{
		//閉じる。
		CloseMessage();
	}
}

void TextBox::_OpenMessage()
{
	//閉じているか
	if (_State & TextBoxStateE::CLOSE)
	{
		//ステート設定。
		_State = TextBoxStateE::OPENING;
		//テキスト設定。
		_SetText(_Message->Text);

		//ボックスを表示する
		for each (ImageObject* image in _BoxImage)
		{
			image->SetActive(true);
		}
		_AnimeTime = 0.0f;
	}
}

void TextBox::CloseMessage()
{
	if (_State & TextBoxStateE::OPEN ||
		_State == TextBoxStateE::TITLE)
	{
		//最初のメッセージ取得
		_Message = INSTANCE(MessageManager)->GetMess(_StartTextID);
		_State = TextBoxStateE::CLOSING;
		//テキストを閉じる。
		_Text->SetText("");
		_Text->SetActive(false);

		_AnimeTime = 0.0f;
	}
}

void TextBox::_NextMessage()
{
	//開いている時のみ
	if (_State == TextBoxStateE::OPEN)
	{
		//最大文字数取得
		int max = _Text->GetMaxCharNum();
		//テキストの途中なら最後まで出す。
		if (_CharNum < max)
		{
			_CharNum = max;
			_Text->SetCharNum(_CharNum);
		}
		else
		{
			//次のメッセージを再生
			_SetMessage(_Message->NextID);
			//0以上なら
			if (_Message->EventID >= 0)
				INSTANCE(EventManager)->Execute(Event::EventID(_Message->EventID), 0);
		}
	}
}

void TextBox::_SetText(const char * text)
{
	//表示文字数初期化
	_CharNum = 0;
	_Text->SetCharNum((unsigned int)_CharNum);

	//テキスト設定。
	_Text->SetText(text);
	Vector2 space(_Text->GetText()->GetSize() * FontSize);
	Vector2 len(_Text->GetLength());
	//テキストのサイズ+余白分。
	Vector2 textSize = len + space;
	//メッセージボックスのサイズ設定。
	_BoxImage[1]->SetSize(textSize);
	//ボックスの半分の高さ。
	float halfHeight = textSize.y / 2;
	//カーソルの上にくるように移動。
	_BoxImage[1]->transform->SetLocalPosition(Vector3(0, -(halfHeight + _BoxImage[0]->GetTexture()->Size.y), 0));
}

void TextBox::_SetMessage(const int & id)
{
	if (id >= 0)
	{
		INSTANCE(EventManager)->Execute(Event::EventID::ShopF, 0);
		//メッセージ情報取得
		_Message = INSTANCE(MessageManager)->GetMess(id);
		if (_Message)
		{
			//テキストとボックスをセットする。
			_SetText(_Message->Text);
		}
		else
		{
			//メッセージが見つからなかったので閉じる。
			CloseMessage();
		}
	}
	//-1以下ならメッセージを閉じる
	else
	{
		CloseMessage();
	}
}

void TextBox::_Animation()
{
	//アニメーションではない
	if ((_State & TextBoxStateE::ANIMATION) == 0)
		return;
	Vector3 sca = Vector3::zero;
	//アニメーションに掛ける時間
	float animationTime = 0.5f;
	_AnimeTime += Time::DeltaTime() / animationTime;
	switch (_State)
	{
	case TextBoxStateE::TITLE:
		break;
	case TextBoxStateE::OPENING:
		//だんだん大きくする。
		sca = Vector3::Lerp(Vector3::zero, Vector3::one, _AnimeTime);
		transform->SetScale(sca);

		//開ききったなら
		if (sca.Length() >= (Vector3::one.Length()))
		{
			_State = TextBoxStateE::OPEN;
			//テキスト表示
			_Text->SetActive(true);
		}
		break;
	case TextBoxStateE::CLOSING:
		//だんだん小さくする。
		sca = Vector3::Lerp(Vector3::one, Vector3::zero, _AnimeTime);
		transform->SetScale(sca);

		//大きさがなくなったなら
		if (sca.Length() <= Vector3::zero.Length())
		{
			//ボックスを閉じる。
			for each (ImageObject* image in _BoxImage)
			{
				image->SetActive(false);
			}
			_State = TextBoxStateE::CLOSE;
		}
		break;
	default:
		break;
	}
}


void TextBox::_IncreaseChar()
{
	//文字が出ている時に加算
	if (_State == TextBoxStateE::OPEN)
	{
		//最大数以下なら増やす。
		if (_CharNum < _Text->GetMaxCharNum())
		{
			//だんだん表示する文字数を増やす。
			_CharNum += Time::DeltaTime()*_TextSpeed;
			_Text->SetCharNum((unsigned int)_CharNum);
		}
	}
}
