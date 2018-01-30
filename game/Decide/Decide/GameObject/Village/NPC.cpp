#include "stdafx.h"
#include "NPC.h"

#include "GameObject\Player\Player.h"
#include "GameObject\Village\TextBox.h"
#include "fbEngine\_Object\_Component\_3D\Camera.h"
#include "GameObject\StatusWindow\StatusWindow.h"


NPC::NPC(const char * name):
	ContinentObject(name),
	_Height(1.0f),
	_Radius(3.0f)
{
}

NPC::~NPC()
{
	INSTANCE(GameObjectManager)->AddRemoveList(_TextBox);
}

void NPC::Awake()
{
	ContinentObject::Awake();

	_Rotation = AddComponent<ObjectRotation>();
	_Player = (Player*)INSTANCE(GameObjectManager)->FindObject("Player");

	//テキストボックスを出す。
	_TextBox = INSTANCE(GameObjectManager)->AddNew<TextBox>("TextBox",StatusWindow::WindowBackPriorty - 1);
	_TextBox->SetTextSpeed(12.0f);
	_IsSpeak = false;
	_State = State::Idol;
}

/**
* 初期化.
*/
void NPC::Start()
{
	if (!strcmp(_Model->GetName(),"villager1.X"))
	{
		_Anim->SetAnimationEndTime((int)AnimationCodeE::Idol, -1.0f);
		_Anim->SetAnimationEndTime((int)AnimationCodeE::Speak, -1.0f);
	}
	else
	{
		_Anim->SetAnimationEndTime((int)AnimationCodeE::Idol, -1.0f);
		_Anim->SetAnimationEndTime((int)AnimationCodeE::Speak, -1.0f);
	}
	PlayAnimation(AnimationCodeE::Idol, 0.2f);

	//足元を地面に合わせる.
	_FitGround();

	ContinentObject::Start();
}

void NPC::Update()
{
	//話す。
	_Speak();
}

void NPC::LateUpdate()
{
	//頭の場所計算
	Vector3 headPos = transform->GetPosition();
	headPos.y += _Height;
	Vector2 screemPos = INSTANCE(GameObjectManager)->mainCamera->WorldToScreen(headPos);
	//テキストの場所設定。
	_TextBox->transform->SetPosition(Vector3(screemPos, 0));
}

void NPC::CreateNPC(const npc::NPCInfo* info)
{
	SetMesseage(info->MesseageID, info->ShowTitle);
	_TextBox->SetEventNo(info->EventNo);
	transform->SetLocalPosition(info->pos);
	transform->SetRotation(info->ang);
	transform->SetLocalScale(info->sca);

	//初期回転保存のため
	_Rot = info->ang;

	LoadModel(info->filename, false);

	_Model->SetFresnelParam(true, Vector4(1.3f, 1.3f, 1.3f, 3.0f));
	_Model->SetModelEffect(ModelEffectE::RECEIVE_POINTLIGHT, false);
}

void NPC::SetMesseage(const int & id, const bool show)
{
	_TextBox->SetMessageID(id);
	_ShowTitle = show;
}

bool NPC::GetIsSpeakEnd()const {
	return _TextBox->IsMessageEnd();
}

bool NPC::GetisSpeakEndLastMessage()const {
	return _TextBox->IsLastMessageEnd();
}

/**
* 足元を地面に合わせる.
*/
void NPC::_FitGround()
{
	//レイを作成する.
	btTransform start, end;
	start.setIdentity();
	end.setIdentity();

	//開始位置と足元の差分.
	float startOffset = 2;

	//if (!strcmp(_Model->GetName(), "NPC_Otoko.X"))
	//{
	//	startOffset = 2.5f;
	//}

	Vector3 pos = transform->GetPosition();
	Quaternion rot = transform->GetRotation();
	//開始地点を設定.
	start.setOrigin(btVector3(pos.x, pos.y + startOffset, pos.z));

	//終了地点を設定.
	//2メートル下を見る.
	end.setOrigin(start.getOrigin() - btVector3(0, startOffset + 2, 0));

	fbPhysicsCallback::SweepResultGround callback;
	callback.me = this;
	callback.startPos.Set(start.getOrigin().x(), start.getOrigin().y(), start.getOrigin().z());
	callback._attribute = Collision_ID::GROUND | Collision_ID::BUILDING;

	INSTANCE(PhysicsWorld)->ConvexSweepTest((const btConvexShape*)GetComponent<MeshCollider>()->GetBody(), start, end, callback);

	if (callback.isHit)
	{
		pos = callback.hitPos; 

		pos.y += (startOffset + _Model->GetModelData()->GetAABBSize().y) * transform->GetScale().y;
		
		transform->SetLocalPosition(pos);
	}
}

void NPC::_Speak()
{
	//死亡時は話せない。
	if (_Player->GetState() == Player::State::Death)
	{
		return;
	}
	//プレイヤーとの距離を計算。
	float len = (transform->GetPosition() - _Player->transform->GetPosition()).Length();
	if (_Player->GetNearNPCLen() > len)
	{
		_Player->SetNPC(this);
	}
	//会話可能な距離か？
	if (_IsSpeak)
	{
		//タイトル表示
		if (_ShowTitle)
		{
			_TextBox->Title(true);
		}
		//本来は外部から呼び出す。
		if (VPadInput->IsPush(fbEngine::VPad::ButtonA))
		{
			//会話する。

			_TextBox->Speak();
			_Rotation->RotationToObject_XZ(_Player);
			if (_State != State::Speak && _IsAnimation)
			{
				_Player->SetSpeakFlag(true);
				_State = State::Speak;
				PlayAnimation(AnimationCodeE::Speak, 0.2f);
			}
		}
	}
	
	//ショップの場合。買い物中
	bool eventflag = INSTANCE(EventManager)->ShopEvent();

	if(!_IsSpeak || GetIsSpeakEnd())
	{
		_TextBox->CloseMessage();
		if (!eventflag)
		{
			transform->SetRotation(_Rot);
		}
		if (_State != State::Idol && _IsAnimation)
		{
			_Player->SetSpeakFlag(false);
			_State = State::Idol;
			PlayAnimation(AnimationCodeE::Idol, 0.2f);
		}
	}
}