#include "stdafx.h"
#include "VillageName.h"
#include "fbEngine\_Object\_GameObject\TextObject.h"

static const string LocationNames[] =
{
	"ƒhƒ‰ƒŠƒAƒ“‚ÌZˆ",
	"‚Í‚¶‚Ü‚è‚ÌW—Ž",
	"Žë—Â‚Ì‘º",
	"”É‰h‚ÌŠX",
	"‚»‚Æ"
};

void VillageName::Awake()
{
	_Text = INSTANCE(GameObjectManager)->AddNew<TextObject>("VillageName", 10);
	_Text->Initialize(L"", 50.0f, Color::white, fbSprite::SpriteEffectE::OUTLINE, STRING(fbText::TextStyleE::‚l‚r_–¾’©));
	
	//À•W‚ðÝ’è.
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
		//•\Ž¦–¼‚ðÝ’è.
		_Text->SetText(LocationNames[location].c_str());
		_Timer = 0.0f;
	}
	now = location;
}
