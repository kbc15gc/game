#include "stdafx.h"
#include "Money2D.h"
#include "fbEngine\_Object\_GameObject\TextObject.h"
#include "fbEngine\_Object\_GameObject\ImageObject.h"

Money2D::Money2D(const char * name) :
	GameObject(name)
{
	//テキストを表示する場所。
	_TextPos = Vector3(50.0f, 660.0f, 0.0f);

	//画像を表示する場所。
	_ImagePos = Vector3(60.0f, 600.0f, 0.0f);

	//ログ用背景の場所。
	_LogBackPos = Vector3(110.0f, 600.0f, 0.0f);

	//最大表示時間。
	_LifeTime = 1.0f;

	//現在の時間。
	_NowTime = 0.0f;

	//α値。
	_alpha = 1.0f;

}

Money2D::~Money2D()
{
	char text[256];
	sprintf(text, "~Money2D address %x\n", *this);
	OutputDebugString(text);
}

void Money2D::Awake()
{
	/*_MoneyText = INSTANCE(GameObjectManager)->AddNew<TextObject>("Money2DText", 7);
	_MoneyText->transform->SetLocalPosition(_TextPos);

	_GetImage = INSTANCE(GameObjectManager)->AddNew<ImageObject>("GetImage", 7);
	_GetImage->SetTexture(LOADTEXTURE("GetImage.png"));
	_GetImage->SetActive(true);
	_GetImage->transform->SetLocalPosition(Vector3(_ImagePos.x, _ImagePos.y, 0));

	_LogBackImage = INSTANCE(GameObjectManager)->AddNew<ImageObject>("LogBack", 6);
	_LogBackImage->SetTexture(LOADTEXTURE("window.png"));
	_LogBackImage->transform->SetLocalPosition(Vector3(_LogBackPos.x, _LogBackPos.y, 0.0f));
	_LogBackImage->SetSize(Vector2(200.0f, 160.0f));
	_LogBackImage->SetActive(true);*/
}

void Money2D::Start()
{
	
}

void Money2D::Update()
{
	//_NowTime += Time::DeltaTime();
	//if (_NowTime<_LifeTime) {
	//	//_MoneyText->transform->SetPosition(_TextPos.x, _TextPos.y -= 1.0f, 0.0f);
	//	//_GetImage->transform->SetPosition(_ImagePos.x, _ImagePos.y -= 1.0f, 0.0f);
	//	Color cl;
	//	cl.r = 1.0f;
	//	cl.g = 1.0f;
	//	cl.b = 1.0f;
	//	cl.a = _alpha;
	//	_MoneyText->SetBlendColor(cl);
	//	_alpha -= 0.1f;
	//}
	//else
	//{
	//	//テキストを削除。
	//	//INSTANCE(GameObjectManager)->AddRemoveList(_MoneyText);
	//	//画像を削除。
	//	//INSTANCE(GameObjectManager)->AddRemoveList(_GetImage);
	//	//削除。
	//	//INSTANCE(GameObjectManager)->AddRemoveList(this);
	//}
	//for (int i = 0; i < 5; i++)
	//{
	//	//インスタンス化。
	//	TextObject* text = INSTANCE(GameObjectManager)->AddNew<TextObject>("MoneyText", 7);
	//	text->Initialize(L"", 22.0f);
	//	//テキスト設定。
	//	char t[256];
	//	string str = "入手しました。";
	//	sprintf(t, "%s,%s", _AddMoenyLog[i], str);
	//	text->SetString(t);
	//	text->SetFontSize(22.0f);
	//	text->SetFormat((unsigned int)fbText::TextFormatE::LEFT);
	//	text->transform->SetLocalPosition(Vector3(_TextPos.x, _TextPos.y - i*4.0f, 0.0f));
	//	i++;
	//}
}

void Money2D::Initialize(int money)
{
	_AddMoenyLog[_UseLogCounter] = to_string(money);
	_UseLogCounter++;
	//_MoneyText->Initialize(L"", 22.0f,Color::white);
	//	string Money = to_string(money);
	//_MoneyText->SetString(Money.data());
}
