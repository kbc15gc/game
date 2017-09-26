#include"fbstdafx.h"
#include "FPS.h"

void FPS::Start()
{
	_Text = AddComponent<Text>();
	
	_Text->SetSize(40.0f);
	_Text->SetText(L"FPS:0");
	_Text->SetAnchor(fbText::TextAnchorE::MiddleLeft);
}

void FPS::Update()
{
	wchar_t fps[30];
	wchar_t dnum[10];
	Support::ToString(Time::Fps(), dnum,2);
	wcscpy_s(fps, wcslen(L"FPS:")+1, L"FPS:");
	wcscat_s(fps, wcslen(fps) + wcslen(dnum)+1, dnum);
	_Text->SetText(fps);
}