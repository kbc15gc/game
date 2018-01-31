#include "stdafx.h"
#include "VillageName.h"
#include "fbEngine\_Object\_GameObject\TextObject.h"
#include "GameObject\History\HistoryInfo.h"

static const string LocationNames[] =
{
	AllLocationNameList[static_cast<int>(LocationCodeAll::Drarian)],
	AllLocationNameList[static_cast<int>(LocationCodeAll::Gremrin)],
	AllLocationNameList[static_cast<int>(LocationCodeAll::BossD)],
	AllLocationNameList[static_cast<int>(LocationCodeAll::Begin)],
	AllLocationNameList[static_cast<int>(LocationCodeAll::Hunting)],
	AllLocationNameList[static_cast<int>(LocationCodeAll::Prosperity)],
	AllLocationNameList[static_cast<int>(LocationCodeAll::Kuni)],
	AllLocationNameList[static_cast<int>(LocationCodeAll::DevilKingdom)],
	AllLocationNameList[static_cast<int>(LocationCodeAll::Sinden)],
	AllLocationNameList[static_cast<int>(LocationCodeAll::World)],
	"そと"		// 未使用？。
};

void VillageName::Awake()
{
	_Text = INSTANCE(GameObjectManager)->AddNew<TextObject>("VillageName", 10);
	_Text->Initialize(L"", 50.0f, Color::white, fbSprite::SpriteEffectE::OUTLINE, STRING(fbText::TextStyleE::ＭＳ_明朝));
	
	//座標を設定.
	_Text->transform->SetLocalPosition(Vector3(g_WindowSize.x / 2.0f, 50.0f, 0));
	_Text->SetAnchor(fbText::TextAnchorE::UpperCenter);
	
}

void VillageName::Update()
{
	_Timer += Time::DeltaTime();
	float in = 1.0f, stay = 1.5f, out = 1.0f;
	if (_Timer < 1.0f)
		_Text->SetBlendColor(Color(1.0f, 1.0f, 1.0f, min(1.0f, _Timer / in)));
	else
		_Text->SetBlendColor(Color(1.0f, 1.0f, 1.0f, min(1.0f, max(0.0, (in + stay - _Timer) / out))));
}

void VillageName::Excute(const int location)
{
	if (now != location && location < ARRAY_SIZE(LocationNames))
	{
		//表示名を設定.
		_Text->SetText(LocationNames[location].c_str());
		_Timer = 0.0f;
	}
	now = location;
}
