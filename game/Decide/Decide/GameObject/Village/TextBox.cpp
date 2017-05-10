#include "TextBox.h"

TextBox::TextBox(const char * name):
	GameObject(name),
	_CharNum(0),
	_TextSpeed(1.0f),
	_Open(false),
	_PlayingAnim(false),
	_Scale(0.0f)
{
}

TextBox::~TextBox()
{
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
	_Text->SetFormat((int)fbText::TextFormatE::CENTER | (int)fbText::TextFormatE::UP);
	
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

	//アニメーションしていない。
	if (!_PlayingAnim)
	{
		//メッセージを次へ(デバッグ用)
		if (KeyBoardInput->isPush(DIK_SPACE))
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
				//次のメッセージ
				NextMessage();
			}
		}
	}
}

void TextBox::OpenMessage(const int & id)
{
	//完全に閉じているときに開くことができる。
	if (!_Open && !_PlayingAnim)
	{
		_SetMessage(id);
		_PlayingAnim = true;
		_Scale = 0.0f;
	}
}

void TextBox::CloseMessage()
{
	_Message = nullptr;
	_Open = false;
	_PlayingAnim = true;
	_Scale = 0.0f;

	//テキストを閉じる。
	_Text->SetActive(_Open);
}

void TextBox::NextMessage()
{
	//次のメッセージを再生
	if (_Message)
	{
		_SetMessage(_Message->NextID);
	}
}

void TextBox::_SetMessage(const int & id)
{
	//表示文字数初期化
	_CharNum = 0;
	_Text->SetCharNum((unsigned int)_CharNum);
	if (id >= 0)
	{
		//メッセージ情報取得
		_Message = INSTANCE(MessageManager)->GetMess(id);
		if (_Message)
		{
			_Text->SetString(_Message->Text);
			Vector2 textSize = _Text->GetLength() + Vector2(_Text->GetText()->GetSize() * FontSize);
			_BoxImage[1]->SetSize(textSize);
			float halfHeight = textSize.y / 2;
			_BoxImage[1]->transform->SetLocalPosition(Vector3(0, -(halfHeight + _BoxImage[0]->GetTexture()->Size.y), 0));

			_Open = true;
			//全てをアクティブにする。
			for each (Transform* child in transform->GetChildren())
			{
				child->gameObject->SetActive(_Open,true);
			}
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
	//アニメーション再生中なら
	if (_PlayingAnim)
	{
		//掛ける時間
		float time = 1.0f;
		_Scale += Time::DeltaTime() / time;
		if (_Open)
		{
			//だんだん大きくする。
			Vector3 sca = Vector3::Lerp(Vector3::zero, Vector3::one, _Scale);
			transform->SetScale(sca);

			//開ききったなら
			if (sca.Length() >= Vector3::one.Length())
			{
				_PlayingAnim = false;
			}
		}
		else
		{
			//だんだん小さくする。
			Vector3 sca = Vector3::Lerp(Vector3::one, Vector3::zero, _Scale);
			transform->SetScale(sca);

			//大きさがなくなったなら
			if (sca.Length() <= Vector3::zero.Length())
			{
				//ボックスを閉じる。
				for each (ImageObject* image in _BoxImage)
				{
					image->SetActive(_Open);
				}
				_PlayingAnim = false;
			}
		}
	}
}

void TextBox::_IncreaseChar()
{
	//アニメーションしていないときに増加させる。
	if (!_PlayingAnim)
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
