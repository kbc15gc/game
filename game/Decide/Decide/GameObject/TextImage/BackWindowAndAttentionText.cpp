#include "stdafx.h"
#include "BackWindowAndAttentionText.h"
#include "fbEngine\_Object\_GameObject\ImageObject.h"
#include "fbEngine\_Object\_GameObject\TextObject.h"


//コンストラクタ。
BackWindowAndAttentionText::BackWindowAndAttentionText(const char* name):
GameObject(name)
{

}

//デストラクタ。
BackWindowAndAttentionText::~BackWindowAndAttentionText()
{
	if (_AttentionBackWindowImage) {
		INSTANCE(GameObjectManager)->AddRemoveList(_AttentionBackWindowImage);
	}

	if (_AttentionText) {
		INSTANCE(GameObjectManager)->AddRemoveList(_AttentionText);
	}
}

void BackWindowAndAttentionText::Awake() {
	_AttentionBackWindowImage = INSTANCE(GameObjectManager)->AddNew<ImageObject>("AttentionBackWindowImage", 9);
	_AttentionText	 = INSTANCE(GameObjectManager)->AddNew<TextObject>("AttentionText", 9);

	SetActiveFalse();
}

//テキストと背景画像の作成。
void BackWindowAndAttentionText::Create(const wchar_t * string,
	const Vector3& windowPos,
	const Vector2& windowSize,
	const Vector3& textPos,
	float textSize,
	const Color& color) 
{
	_AttentionBackWindowPos = windowPos;
	_AttentionBackWindowSize = windowSize;
	_AttentionTextPos = textPos;
	_AttentionTextFontSize = textSize;

	//背景ウインドウ初期化。
	_AttentionBackWindowImage->SetTexture(LOADTEXTURE("UI/Panel 5.png"));
	_AttentionBackWindowImage->transform->SetPosition(_AttentionBackWindowPos);
	_AttentionBackWindowImage->SetSize(_AttentionBackWindowSize);

	//テキストの初期化。
	_AttentionText->Initialize(L"", _AttentionTextFontSize);
	_AttentionText->SetText(string);
	_AttentionText->SetAnchor(fbText::TextAnchorE::Center);
	_AttentionText->transform->SetParent(_AttentionBackWindowImage->transform);
	_AttentionText->transform->SetLocalPosition(_AttentionTextPos);
	_AttentionText->SetBlendColor(color);

	SetActiveTrue();
}

//更新。
void BackWindowAndAttentionText::Update() {

	if (VPadInput->IsPush(fbEngine::VPad::ButtonA)) {
		SetActiveFalse();
	}

#ifdef _DEBUG
	Debug();
#endif // _DEBUG

}

//デバッグ。
#ifdef _DEBUG

void BackWindowAndAttentionText::Debug()
{
	//背景ウインドウの位置調整、
	if (KeyBoardInput->isPressed(DIK_H) && KeyBoardInput->isPressed(DIK_UP)) {
		_AttentionBackWindowPos.y -= 1.0f;
		_AttentionBackWindowImage->transform->SetPosition(Vector3(_AttentionBackWindowPos.x, _AttentionBackWindowPos.y, 0.0f));
	}

	if (KeyBoardInput->isPressed(DIK_H) && KeyBoardInput->isPressed(DIK_DOWN)) {
		_AttentionBackWindowPos.y += 1.0f;
		_AttentionBackWindowImage->transform->SetPosition(Vector3(_AttentionBackWindowPos.x, _AttentionBackWindowPos.y, 0.0f));
	}

	if (KeyBoardInput->isPressed(DIK_H) && KeyBoardInput->isPressed(DIK_RIGHT)) {
		_AttentionBackWindowPos.x += 1.0f;
		_AttentionBackWindowImage->transform->SetPosition(Vector3(_AttentionBackWindowPos.x, _AttentionBackWindowPos.y, 0.0f));
	}

	if (KeyBoardInput->isPressed(DIK_H) && KeyBoardInput->isPressed(DIK_LEFT)) {
		_AttentionBackWindowPos.x -= 1.0f;
		_AttentionBackWindowImage->transform->SetPosition(Vector3(_AttentionBackWindowPos.x, _AttentionBackWindowPos.y, 0.0f));
	}

	//背景ウィンドウのサイズ調整。
	if (KeyBoardInput->isPressed(DIK_K) && KeyBoardInput->isPressed(DIK_UP)) {
		_AttentionBackWindowSize.y--;
		_AttentionBackWindowImage->SetSize(_AttentionBackWindowSize);
	}

	if (KeyBoardInput->isPressed(DIK_K) && KeyBoardInput->isPressed(DIK_DOWN)) {
		_AttentionBackWindowSize.y++;
		_AttentionBackWindowImage->SetSize(_AttentionBackWindowSize);
	}

	if (KeyBoardInput->isPressed(DIK_K) && KeyBoardInput->isPressed(DIK_RIGHT)) {
		_AttentionBackWindowSize.x++;
		_AttentionBackWindowImage->SetSize(_AttentionBackWindowSize);
	}

	if (KeyBoardInput->isPressed(DIK_K) && KeyBoardInput->isPressed(DIK_LEFT)) {
		_AttentionBackWindowSize.x--;
		_AttentionBackWindowImage->SetSize(_AttentionBackWindowSize);
	}


	//テキストの位置調整。
	if (KeyBoardInput->isPressed(DIK_J) && KeyBoardInput->isPressed(DIK_UP)) {
		_AttentionTextPos.y -= 1.0f;
		_AttentionText->transform->SetLocalPosition(Vector3(_AttentionTextPos.x, _AttentionTextPos.y, 0.0f));
	}

	if (KeyBoardInput->isPressed(DIK_J) && KeyBoardInput->isPressed(DIK_DOWN)) {
		_AttentionTextPos.y += 1.0f;
		_AttentionText->transform->SetLocalPosition(Vector3(_AttentionTextPos.x, _AttentionTextPos.y, 0.0f));
	}

	if (KeyBoardInput->isPressed(DIK_J) && KeyBoardInput->isPressed(DIK_RIGHT)) {
		_AttentionTextPos.x += 1.0f;
		_AttentionText->transform->SetLocalPosition(Vector3(_AttentionTextPos.x, _AttentionTextPos.y, 0.0f));
	}

	if (KeyBoardInput->isPressed(DIK_J) && KeyBoardInput->isPressed(DIK_LEFT)) {
		_AttentionTextPos.x -= 1.0f;
		_AttentionText->transform->SetLocalPosition(Vector3(_AttentionTextPos.x, _AttentionTextPos.y, 0.0f));
	}

	//テキストのサイズ調整。
	if ((KeyBoardInput->isPressed(DIK_L) && KeyBoardInput->isPressed(DIK_UP))) {
		_AttentionTextFontSize++;
		_AttentionText->SetFontSize(_AttentionTextFontSize);
	}

	if ((KeyBoardInput->isPressed(DIK_L) && KeyBoardInput->isPressed(DIK_DOWN))) {
		_AttentionTextFontSize--;
		_AttentionText->SetFontSize(_AttentionTextFontSize);
	}

	//画像表示用。
	if ((KeyBoardInput->isPush(DIK_O))) {
		wchar_t ErrorText[256];
		wcscpy_s(ErrorText, wcslen(L"所持品が一杯でアイテムを追加出来ませんでした。\n所持品を整理してください。AボタンまはたキーボードのPで閉じる。") + 1, L"所持品が一杯でアイテムを追加出来ませんでした。\n所持品を整理してください。AボタンまはたキーボードのPで閉じる。");
		Create(ErrorText, Vector3(642.0f, 363.0f, 0.0f), Vector2(793.0f, 229.0f), Vector3(1.0f, -20.0f, 0.0), 33.0f, Color::red);
		SetActiveTrue();
	}

	//画像を閉じる。
	if ((VPadInput->IsPush(fbEngine::VPad::ButtonA)||(KeyBoardInput->isPush(DIK_P)))) {
		SetActiveFalse();
	}
}
#endif // DEBUG