#include "stdafx.h"
#include "GameObject\Component\OutputData.h"
#include "fbEngine\_Support\Support.h"
#include "GameObject\Component\CharacterParameter.h"
#include "fbEngine\_Object\_GameObject\GameObjectManager.h"
#include "fbEngine\_Object\_GameObject\TextObject.h"
#ifdef _DEBUG

void OutputData::Start()
{
	_lastChildren = nullptr;
	if (gameObject) {
		char text[FILENAME_MAX];
		strcpy_s(text, gameObject->GetName());
		strcat_s(text, strlen(text) + strlen(":") + 1, ":");		// 名前に文字列を追加。

		_CreateTextObject(&_textName, "ObjectNameView",Vector3::zero,text);
	}
	if (gameObject->transform) {
		_CreateTextObject(&_textTransform, "TransformView", Vector3(0.0f,35.0f,0.0f), "",25.0f, Color::red);
		OutputTextTrans();
		_textTransform->SetSpacing(50.0f);
	}
	if (gameObject->GetComponent<CharacterParameter>()) {
		_CreateTextObject(&_textParam, "ParamterView", Vector3(0.0f, 20.0f, 0.0f), "", 25.0f, Color::yellow);
		OutputTextParam();
	}

}

void OutputData::Update() {
	OutputTextTrans();
	OutputTextParam();
}


TextObject* OutputData::_CreateTextObject(TextObject** obj, const char* name, const Vector3& localPos, const char* initText, const float textSize ,Color textColor, fbText::TextFormatE format) {
	if (*obj == nullptr) {
		*obj = INSTANCE(GameObjectManager)->AddNew<TextObject>("ObjectNameView", 9);
		wchar_t text[FILENAME_MAX];
		if (initText) {
			mbstowcs_s(nullptr, text, initText, strlen(initText));	// char*をwchar_t*に変換。
			(*obj)->Initialize(text, textSize, textColor);

		}
		else {
			(*obj)->Initialize(nullptr, textSize, textColor);
		}
		(*obj)->SetFormat(static_cast<int>(format));
		if (_lastChildren) {
			(*obj)->transform->SetParent(_lastChildren->transform);
			(*obj)->transform->SetLocalPosition(localPos + Vector3(0.0f, _lastChildren->GetLength().y, 0.0f));
		}
		else {
			(*obj)->transform->SetPosition(_rootPos);
		}

		_lastChildren = *obj;

		return *obj;
	}
	return nullptr;
}

wchar_t* OutputData::_TuckText(wchar_t* left, wchar_t* text, wchar_t* right) {
	wchar_t work[FILENAME_MAX];
	wcscpy_s(work, left);
	wcscat_s(work, text);
	wcscat_s(work, right);
	wcscpy_s(text,wcslen(work) + 1 ,work);
	return text;
}

void OutputData::OutputTextParam() {
	if (_textParam) {
		wchar_t outPut[FILENAME_MAX];
		wcscpy_s(outPut, wcslen(L"\n\n<Parameter>\n") + 1, L"\n\n<Parameter>\n");
		vector<int> Param = gameObject->GetComponent<CharacterParameter>()->GetParams();

		for (int idx = 0; idx < CharacterParameter::Param::MAX; idx++) {
			wchar_t strNum[10];
			wchar_t strType[FILENAME_MAX];

			wcscpy_s(strType, wcslen(CharacterParameter::ENUM_NAME[idx]) + 1, CharacterParameter::ENUM_NAME[idx]);	// char*をwchar_t*に変換。
			Support::ToString(Param[idx], strNum);
			wcscat_s(outPut, wcslen(outPut) + wcslen(strType) + 1, strType);
			wcscat_s(outPut, wcslen(outPut) + wcslen(L": ") + 1, L": ");
			wcscat_s(outPut, wcslen(outPut) + wcslen(_TuckText(L" [ ",strNum,L" ]")) + 1, strNum);
			wcscat_s(outPut, wcslen(outPut) + wcslen(L"\n") + 1, L"\n");
		}
		_textParam->SetString(outPut);
	}
}

void OutputData::OutputTextTrans() {
	if (_textTransform) {
		wchar_t strTrans[FILENAME_MAX];
		wchar_t strNum[FILENAME_MAX];

		wcscpy_s(strTrans, wcslen(L"<Transform>\n") + 1, L"<Transform>\n");
		wcscat_s(strTrans, wcslen(strTrans) + wcslen(L"Position : ") + 1, L"Position : ");
		Support::ToString(Vector4(gameObject->transform->GetPosition(), 0.0f), strNum, 2);
		wcscat_s(strTrans, wcslen(strTrans) + wcslen(_TuckText(L" [ ", strNum, L" ]")) + 1, strNum);
		wcscat_s(strTrans, wcslen(strTrans) + wcslen(L"\nRotation : ") + 1, L"\nRotation : ");
		Support::ToString(Vector4(gameObject->transform->GetRotation()), strNum, 2);
		wcscat_s(strTrans, wcslen(strTrans) + wcslen(_TuckText(L" [ ", strNum, L" ]")) + 1, strNum);
		wcscat_s(strTrans, wcslen(strTrans) + wcslen(L"\nScale   : ") + 1, L"\nScale   : ");
		Support::ToString(Vector4(gameObject->transform->GetScale(),0.0f), strNum, 2);
		wcscat_s(strTrans, wcslen(strTrans) + wcslen(_TuckText(L" [ ", strNum, L" ]")) + 1, strNum);

		_textTransform->SetString(strTrans);
	}
}

#endif