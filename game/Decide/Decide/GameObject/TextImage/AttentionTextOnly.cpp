#include "stdafx.h"
#include "AttentionTextOnly.h"
#include "fbEngine\_Object\_GameObject\TextObject.h"

//コンストラクタ。
AttentionTextOnly::AttentionTextOnly(const char* name) :
GameObject(name)
{
}

//デストラクタ。
AttentionTextOnly::~AttentionTextOnly()
{
	DeleteList();
}

//テキストを作成。
void AttentionTextOnly::CreateText(const wchar_t * string,
	const Vector3& textPos,
	float size,
	const Color& color,
	MoveType type)
{

	TextInfo* info = new TextInfo;

	//テキストの初期化。
	TextObject*	Text = INSTANCE(GameObjectManager)->AddNew<TextObject>("AttentionText", 10);
	Text->Initialize(L"", size);
	Text->SetText(string);
	Text->SetAnchor(fbText::TextAnchorE::Center);
	Text->SetBlendColor(color);
	Text->transform->SetPosition(textPos);

	//情報をまとめる。
	info->_Text = Text;
	info->_Color = color;
	info->_Dir = MoveTypeS[static_cast<int>(type)];

	//リストに追加。
	_TextList.push_back(info);
}

//更新。
void AttentionTextOnly::Update() {
	for (auto itr = _TextList.begin(); itr != _TextList.end();)
	{
		//テキストの移動。
		(*itr)->_Text->transform->SetPosition(
			(*itr)->_Text->transform->GetPosition().x + ((*itr)->_Dir.x/**_MoveSpeed*Time::DeltaTime()*/),
			(*itr)->_Text->transform->GetPosition().y + ((*itr)->_Dir.y/**_MoveSpeed*Time::DeltaTime()*/),
			(*itr)->_Text->transform->GetPosition().z + ((*itr)->_Dir.z/**_MoveSpeed*Time::DeltaTime()*/));

		//各テキストが少しずつ透明にしていく。
		(*itr)->_Text->SetBlendColor(
			Color((*itr)->_Color.r,
			(*itr)->_Color.g,
				(*itr)->_Color.b,
				(*itr)->_Color.a-= 1.5f*Time::DeltaTime()));

		//テキストが透明になりきるとリストから削除。
		if ((*itr)->_Color.a < 0.0f) {
			INSTANCE(GameObjectManager)->AddRemoveList((*itr)->_Text);
			itr = _TextList.erase(itr);
		}
		else
		{
			itr++;
		}
	}
}

//テキストリストの全削除。
void AttentionTextOnly::DeleteList() {
	for (auto itr = _TextList.begin(); itr != _TextList.end();)
	{
		INSTANCE(GameObjectManager)->AddRemoveList((*itr)->_Text);
		itr = _TextList.erase(itr);
	}
}