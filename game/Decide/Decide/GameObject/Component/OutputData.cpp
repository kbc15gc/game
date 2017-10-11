#include "stdafx.h"
#include "GameObject\Component\OutputData.h"
#include "fbEngine\_Support\Support.h"
#include "GameObject\Component\CharacterParameter.h"
#include "fbEngine\_Object\_GameObject\GameObjectManager.h"
#include "fbEngine\_Object\_GameObject\TextObject.h"
#include <typeinfo>
#include <algorithm>

#ifdef _DEBUG

void OutputData::Start()
{
	_lastChildren = nullptr;
	_rootPos = Vector3(750.0f, 20.0f, 0.0f);
	if (gameObject) {
		char text[FILENAME_MAX];
		strcpy_s(text, gameObject->GetName());
		strcat_s(text, strlen(text) + strlen(":") + 1, ":");		// 名前に文字列を追加。

		_CreateTextObject(OutputInfo::Name, "ObjectNameView", Vector3::zero, text,40.0f,Color::white,fbText::TextAnchorE::UpperRight);
		_outputTexts[static_cast<int>(OutputInfo::Name)]->transform->SetPosition(Vector3(700.0f, 20.0f, 0.0f));
		_root = nullptr;	// 名前は親にふくまない。
		_lastChildren = nullptr;	// 名前箱に含まない。
	}
	else {
		// ゲームオブジェクトがないので無効化。
		this->SetEnable(false);
		return;
	}

	_rootPos = Vector3(750.0f, 20.0f, 0.0f);

	if (gameObject->transform) {
		_CreateTextObject(OutputInfo::Transform, "TransformView", Vector3(0.0f, 15.0f, 0.0f), "", 25.0f, Color::red);
		OutputTextTrans();
	}
	if (gameObject->GetComponent<CharacterParameter>()) {
		_CreateTextObject(OutputInfo::Parameter, "ParamterView", Vector3(0.0f, 15.0f, 0.0f), "", 25.0f, Color::yellow);
		OutputTextParam();
	}
	if (gameObject->GetAttachCollision()) {
		_CreateTextObject(OutputInfo::Collision, "CollisionView", Vector3(0.0f, 15.0f, 0.0f), "", 25.0f, Color::green);
		OutputTextCollision();
	}
	_CreateTextObject(OutputInfo::Attach, "ComponentsView", Vector3(0.0f, 15.0f, 0.0f), "", 25.0f, Color::blue);
	OutputTextAttach();

	// 最初は非アクティブにする。
	if (this->GetEnable()) {
		this->SetEnable(false);
		for (auto t : _outputTexts) {
			t->SetActive(false);
		}
	}
}

void OutputData::Update() {
	if (_root) {
		float speed = 6.0f;
		if (KeyBoardInput->isPressed(DIK_UP))
		{
			_root->transform->SetPosition(_root->transform->GetPosition() + (Vector3::down * speed));
		}
		else if (KeyBoardInput->isPressed(DIK_DOWN)) {
			_root->transform->SetPosition(_root->transform->GetPosition() + (Vector3::up * speed));
		}
	}

	OutputTextTrans();
	OutputTextParam();
	OutputTextCollision();
	OutputTextAttach();
}

#ifdef _DEBUG
void OutputData::Debug() {
	if (KeyBoardInput->isPush(DIK_T)) {
		if (this->GetEnable()) {
			this->SetEnable(false);
			for (auto t : _outputTexts) {
				t->SetActive(false);
			}
		}
		else {
			this->SetEnable(true);
			for (auto t : _outputTexts) {
				t->SetActive(true);
			}
		}
	}
}
#endif

TextObject* OutputData::_CreateTextObject(OutputInfo infoIdx, const char* name, const Vector3& localPos, const char* initText, const float textSize ,Color textColor, fbText::TextAnchorE anchor) {
	if (infoIdx <= OutputInfo::None) {
		// 0より小さい値が設定された。
		abort();
	}
	if (_outputTexts[static_cast<int>(infoIdx)] == nullptr) {
		_outputTexts[static_cast<int>(infoIdx)] = INSTANCE(GameObjectManager)->AddNew<TextObject>("ObjectNameView", 9);
		wchar_t text[FILENAME_MAX];
		if (initText) {
			mbstowcs_s(nullptr, text, initText, strlen(initText));	// char*をwchar_t*に変換。
			_outputTexts[static_cast<int>(infoIdx)]->Initialize(text, textSize, textColor, fbSprite::SpriteEffectE::NONE, "ＭＳ ゴシック");

		}
		else {
			_outputTexts[static_cast<int>(infoIdx)]->Initialize(nullptr, textSize, textColor,fbSprite::SpriteEffectE::NONE,"ＭＳ ゴシック");
		}
		_outputTexts[static_cast<int>(infoIdx)]->SetAnchor((anchor));

		if (_lastChildren) {
			_outputTexts[static_cast<int>(infoIdx)]->transform->SetParent(_lastChildren->transform);
			_outputTexts[static_cast<int>(infoIdx)]->transform->SetLocalPosition(localPos + Vector3(0.0f, _lastChildren->GetLength().y, 0.0f));
		}
		else {
			_outputTexts[static_cast<int>(infoIdx)]->transform->SetPosition(_rootPos);
			_root = _outputTexts[static_cast<int>(infoIdx)];
		}

		_lastChildren = _outputTexts[static_cast<int>(infoIdx)];
		return _outputTexts[static_cast<int>(infoIdx)];
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
	if (_outputTexts[static_cast<int>(OutputInfo::Parameter)]) {
		wchar_t outPut[FILENAME_MAX];
		wchar_t strNum[10];

		wcscpy_s(outPut, wcslen(L"-Parameter-\n") + 1, L"-Parameter-\n");
		CharacterParameter* Param = gameObject->GetComponent<CharacterParameter>();

		wcscat_s(outPut, wcslen(outPut) + wcslen(L"MaxHP : ") + 1, L"MaxHP : ");	// char*をwchar_t*に変換。
		Support::ToString(Param->GetMaxHP(), strNum);
		wcscat_s(outPut, wcslen(outPut) + wcslen(_TuckText(L" [ ", strNum, L" ]")) + 1, strNum);
		wcscat_s(outPut, wcslen(outPut) + wcslen(L"\n") + 1, L"\n");

		wcscat_s(outPut, wcslen(outPut) + wcslen(L"MaxMP : ") + 1, L"MaxMP : ");	// char*をwchar_t*に変換。
		Support::ToString(Param->GetMaxMP(), strNum);
		wcscat_s(outPut, wcslen(outPut) + wcslen(_TuckText(L" [ ", strNum, L" ]")) + 1, strNum);
		wcscat_s(outPut, wcslen(outPut) + wcslen(L"\n") + 1, L"\n");

		for (int idx = 0; idx < CharacterParameter::Param::MAX; idx++) {
			wchar_t strType[FILENAME_MAX];
			wcscpy_s(strType, wcslen(CharacterParameter::ENUM_NAME[idx]) + 1, CharacterParameter::ENUM_NAME[idx]);	// char*をwchar_t*に変換。
			Support::ToString(Param->GetParam(static_cast<CharacterParameter::Param>(idx)), strNum);
			wcscat_s(outPut, wcslen(outPut) + wcslen(strType) + 1, strType);
			wcscat_s(outPut, wcslen(outPut) + wcslen(L": ") + 1, L": ");
			wcscat_s(outPut, wcslen(outPut) + wcslen(_TuckText(L" [ ",strNum,L" ]")) + 1, strNum);
			wcscat_s(outPut, wcslen(outPut) + wcslen(L"\n") + 1, L"\n");
		}
		_outputTexts[static_cast<int>(OutputInfo::Parameter)]->SetText(outPut);
	}
}

void OutputData::OutputTextTrans() {
	if (_outputTexts[static_cast<int>(OutputInfo::Transform)]) {
		wchar_t strTrans[FILENAME_MAX];
		wchar_t strNum[FILENAME_MAX];

		wcscpy_s(strTrans, wcslen(L"-Transform-\n") + 1, L"-Transform-\n");
		wcscat_s(strTrans, wcslen(strTrans) + wcslen(L"Position : ") + 1, L"Position : ");
		Support::ToString(Vector4(gameObject->transform->GetPosition(), 0.0f), strNum, 2);
		wcscat_s(strTrans, wcslen(strTrans) + wcslen(_TuckText(L" [ ", strNum, L" ]")) + 1, strNum);
		wcscat_s(strTrans, wcslen(strTrans) + wcslen(L"\nRotation : ") + 1, L"\nRotation : ");
		Support::ToString(Vector4(gameObject->transform->GetRotation()), strNum, 2);
		wcscat_s(strTrans, wcslen(strTrans) + wcslen(_TuckText(L" [ ", strNum, L" ]")) + 1, strNum);
		wcscat_s(strTrans, wcslen(strTrans) + wcslen(L"\nScale   : ") + 1, L"\nScale   : ");
		Support::ToString(Vector4(gameObject->transform->GetScale(),0.0f), strNum, 2);
		wcscat_s(strTrans, wcslen(strTrans) + wcslen(_TuckText(L" [ ", strNum, L" ]\n")) + 1, strNum);

		_outputTexts[static_cast<int>(OutputInfo::Transform)]->SetText(strTrans);
	}
}

void OutputData::OutputTextCollision() {
	if (_outputTexts[static_cast<int>(OutputInfo::Collision)]) {
		Collision* coll = gameObject->GetAttachCollision();
		btTransform tr = coll->GetCollisionObj()->getWorldTransform();
		btVector3 scale = coll->GetCollisionObj()->getCollisionShape()->getLocalScaling();
		Vector3 halfSize = coll->GetShape()->GetHalfSize();

		wchar_t out[FILENAME_MAX * 5];
		wchar_t num[FILENAME_MAX];
		wcscpy_s(out,wcslen(L"-Collision-\n") + 1 ,L"-Collision-\n");
		wcscat_s(out, wcslen(out) + wcslen(L"Type         : ") + 1, L"Type         : ");
		wcscpy_s(num,wcslen(Collision::TypeName[static_cast<int>(coll->GetCollisionType())]) + 1, Collision::TypeName[static_cast<int>(coll->GetCollisionType())]);
		wcscat_s(out, wcslen(out) + wcslen(_TuckText(L" [ ", num, L" ]")) + 1, num);

		wcscat_s(out, wcslen(out) + wcslen(L"\nShape        : ") + 1, L"\nShape        : ");
		wcscpy_s(num, wcslen(Collider::ShapeName[static_cast<int>(coll->GetShape()->GetType())]) + 1, Collider::ShapeName[static_cast<int>(coll->GetShape()->GetType())]);
		wcscat_s(out, wcslen(out) + wcslen(_TuckText(L" [ ", num, L" ]")) + 1, num);


		wcscat_s(out, wcslen(out) + wcslen(L"\nIsAddWorld   : ") + 1, L"\nIsAddWorld   : ");
		if (coll->GetIsAddWorld()) {
			wcscpy_s(num, wcslen(L"true") + 1, L"true");
		}
		else {
			wcscpy_s(num, wcslen(L"false") + 1, L"false");
		}
		wcscat_s(out, wcslen(out) + wcslen(_TuckText(L" [ ", num, L" ]")) + 1, num);

		wcscat_s(out, wcslen(out) + wcslen(L"\nPhysics     : ") + 1, L"\nPhysics     : ");
		if (coll->GetPhysicsType() == Collision::PhysicsType::Kinematick) {
			wcscpy_s(num, wcslen(L"kinematick") + 1,L"kinematick");
		}
		else if(coll->GetPhysicsType() == Collision::PhysicsType::Dynamic){
			wcscpy_s(num, wcslen(L"Dynamic") + 1,L"Dynamic");
		}
		else {
			wcscpy_s(num,wcslen(L"Static") + 1, L"Static");
		}
		wcscat_s(out, wcslen(out) + wcslen(_TuckText(L" [ ", num, L" ]")) + 1, num);

		wcscat_s(out, wcslen(out) + wcslen(L"\nState       : ") + 1, L"\nState       : ");
		if (coll->GetIsActive()) {
			wcscpy_s(num,wcslen(L"Active") + 1, L"Active");
		}
		else {
			wcscpy_s(num, wcslen(L"Sleep") + 1, L"Sleep");
		}
		wcscat_s(out, wcslen(out) + wcslen(_TuckText(L" [ ", num, L" ]")) + 1, num);


		wcscat_s(out, wcslen(out) + wcslen(L"\nCollisionID   : ") + 1, L"\nCollisionID   : ");
		Support::ToString(coll->GetID(), num);
		wcscat_s(out, wcslen(out) + wcslen(_TuckText(L" [ ", num, L" ]")) + 1, num);

		wcscat_s(out, wcslen(out) + wcslen(L"\nPosition     : ") + 1, L"\nPosition     : ");
		Support::ToString(Vector4(tr.getOrigin().getX(), tr.getOrigin().getY(), tr.getOrigin().getZ() , 0.0f), num, 2);
		wcscat_s(out, wcslen(out) + wcslen(_TuckText(L" [ ", num, L" ]")) + 1, num);
		wcscat_s(out, wcslen(out) + wcslen(L"\nOffsetPosition : ") + 1, L"\nOffsetPosition : ");
		Support::ToString(Vector4(coll->GetOffsetPos(), 0.0f), num, 2);
		wcscat_s(out, wcslen(out) + wcslen(_TuckText(L" [ ", num, L" ]")) + 1, num);
		wcscat_s(out, wcslen(out) + wcslen(L"\nRotation     : ") + 1, L"\nRotation     : ");
		Support::ToString(Vector4(tr.getRotation().getX(), tr.getRotation().getY(), tr.getRotation().getZ(), tr.getRotation().getW()), num, 2);
		wcscat_s(out, wcslen(out) + wcslen(_TuckText(L" [ ", num, L" ]")) + 1, num);
		wcscat_s(out, wcslen(out) + wcslen(L"\nLocalScale   : ") + 1, L"\nLocalScale   : ");
		Support::ToString(Vector4(scale.getX(), scale.getY(), scale.getZ(), 0.0f), num, 2);
		wcscat_s(out, wcslen(out) + wcslen(_TuckText(L" [ ", num, L" ]")) + 1, num);
		wcscat_s(out, wcslen(out) + wcslen(L"\nHalfSize   : ") + 1, L"\nHalfSize   : ");
		Support::ToString(Vector4(halfSize, 0.0f), num, 2);
		wcscat_s(out, wcslen(out) + wcslen(_TuckText(L" [ ", num, L" ]")) + 1, num);

		_outputTexts[static_cast<int>(OutputInfo::Collision)]->SetText(out);
	}
}

void OutputData::OutputTextAttach() {
	if (_outputTexts[static_cast<int>(OutputInfo::Attach)]) {
		wchar_t out[FILENAME_MAX * 5];
		wcscpy_s(out, wcslen(L"-AttachComponents-\n") + 1, L"-AttachComponents-\n");
		for (auto component : gameObject->GetComponentAll()) {
			wcscat_s(out, wcslen(out) + wcslen(component->GetDebugName()) + 1, component->GetDebugName());
			char work[FILENAME_MAX];
			sprintf(work, "0x%p", component);
			string workStr(work);
			std::transform(workStr.cbegin(), workStr.cend(), workStr.begin(), tolower);
			wchar_t text[FILENAME_MAX];
			mbstowcs_s(nullptr, text, workStr.c_str(), strlen(workStr.c_str()));

			wcscat_s(out, wcslen(out) + wcslen(_TuckText(L" : [ ", text, L" ]\n")) + 1, text);

		}
		_outputTexts[static_cast<int>(OutputInfo::Attach)]->SetText(out);
	}
}
#endif