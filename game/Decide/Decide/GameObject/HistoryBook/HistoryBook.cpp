#include"stdafx.h"
#include "HistoryBook.h"
#include "fbEngine\_Object\_Component\_3D\SkinModel.h"
#include "fbEngine\_Object\_Component\_3D\Animation.h"
#include "GameObject\Player\Player.h"

namespace
{
	//プレイヤーの半分の高さ。
	const Vector3 PLAYER_HALFHEIGHT(0.0f, 0.75f, 0.0f);
}

HistoryBook::HistoryBook(const char * name) :
	GameObject(name),
	//閉じた状態。
	_CloseState(this),
	//開いている状態。
	_OpeningState(this),
	//開いた状態。
	_Open(this),
	//閉じている状態。
	_Closeing(this)
{

}

void HistoryBook::Awake()
{
	//モデル。
	_Model = AddComponent<SkinModel>();
	//アニメーション
	_Anim = AddComponent<Animation>();
	//モデルデータ。
	SkinModelData* modeldata = new SkinModelData();
	//クローンモデルの作成。
	modeldata->CloneModelData(SkinModelManager::LoadModel("HistoryBook.X"), _Anim);
	_Model->SetModelData(modeldata);
	_Model->SetModelEffect(ModelEffectE::CAST_SHADOW, false);

	//プレイヤーを検索
	_Player = (Player*)INSTANCE(GameObjectManager)->FindObject("Player");
}

void HistoryBook::Start()
{
	transform->SetLocalPosition(Vector3(0.0f, 0.0f, 0.0f) + PLAYER_HALFHEIGHT);
	transform->SetLocalScale(Vector3::one);

	//アニメーションの終了時間設定。
	//-1.0fを設定しているのはアニメーションの再生時間が1秒未満。
	_AnimationEndTime[(int)AnimationNo::AnimationClose] = 3.0f;		//本が閉じた状態のアニメーション。
	_AnimationEndTime[(int)AnimationNo::AnimationOpening] = 3.0f;		//本が開くアニメーション。
	_AnimationEndTime[(int)AnimationNo::AnimationOpen] = 3.0f;			//本が開いた状態のアニメーション。
	_AnimationEndTime[(int)AnimationNo::AnimationCloseing] = 3.3f;	    //本が閉じるアニメーション。

	//各エンドタイムを設定。
	for (int i = 0; i < (int)AnimationNo::AnimationNum; i++)
	{
		_Anim->SetAnimationEndTime(i, _AnimationEndTime[i]);
	}

	//ステートの初期化。
	ChangeState(State::Close);
	//アニメーションの初期化。
	PlayAnimation(AnimationNo::AnimationClose, 0.2f, 0);

	//本は見えないように設定。
	_Model->enable = false;

}

void HistoryBook::Update()
{
	//歴史書を見ているフラグを変える操作。
	ChangeIsLookAtHistoryFlag();

	//現在のステートに何か入っているなら。
	if (_CurrentState != nullptr)
	{
		//ステートの更新。
		_CurrentState->Update();
	}

	_AngleY += 0.9;
	transform->SetLocalAngle(Vector3(0.0f, _AngleY, 0.0f));
	//トランスフォーム更新。
	transform->UpdateTransform();

}

void HistoryBook::PlayAnimation(AnimationNo animno, float interpolatetime, int loopnum)
{
	//現在のアニメーションと違うアニメーション　&& アニメーションナンバーが無効でない。
	if (_Anim->GetPlayAnimNo() != (int)animno && animno != AnimationNo::AnimationInvalid)
	{
		_Anim->PlayAnimation((int)animno, interpolatetime, loopnum);
	}
}


void HistoryBook::ChangeIsLookAtHistoryFlag()
{
	//歴史書を見るフラグの切り替え。
	if (XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_START) || KeyBoardInput->isPush(DIK_E))
	{
		//今_IsLookAtHistoryFlagに設定されている反対のフラグを設定。
		_IsLookAtHistoryFlag = !_IsLookAtHistoryFlag;

		//フラグを見て歴史書の状態を遷移。
		//trueなら歴史書を開く状態にする。
		if (_IsLookAtHistoryFlag == true)
		{
			ChangeState(State::Opening);

		}
		//tureの時に押されたらその時歴史書は開いているので閉じる状態に遷移。
		else if (_IsLookAtHistoryFlag == false)
		{
			ChangeState(State::Closeing);
		}
		
		_PlayerFoward = _Player->transform->GetForward();
		_PlayerFoward = _PlayerFoward*-1;
		transform->SetLocalPosition(_PlayerFoward + PLAYER_HALFHEIGHT);
	}
}

void HistoryBook::ChangeState(State nextstate)
{
	//現在のステートがあるなら現在のステートを抜ける処理をする。
	if (_CurrentState != nullptr)
	{
		_CurrentState->Exit();
	}

	//どのステートが来たかチェック。
	switch (nextstate)
	{
		//閉じた状態。
	case State::Close:
		_CurrentState = &_CloseState;
		break;

		//開いている状態。
	case State::Opening:
		_CurrentState = &_OpeningState;
		break;

		//開いた状態。
	case State::Open:
		_CurrentState = &_Open;
		break;

		//閉じている状態。
	case State::Closeing:
		_CurrentState = &_Closeing;
		break;
	default:
		break;
	}

	//渡されたステートに変更。
	_State = nextstate;

	//ステートが切り替わったので各ステートの入りに呼ばれる処理を呼ぶ。
	_CurrentState->Entry();
}