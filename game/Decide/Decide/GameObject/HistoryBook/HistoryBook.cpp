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
	GameObject(name)
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
	_HistoryBookState = State::Close;
	//アニメーションの初期化。
	PlayAnimation(AnimationNo::AnimationClose, 0.2f, 0);

	//フラグの初期化。
	_IsLookAtHistoryFlag = false;

	//本は見えないように設定。
	_Model->enable = false;

}

void HistoryBook::Update()
{
	//歴史書を見ているフラグを変える操作。
	ChangeIsLookAtHistoryFlag();

	//歴史書の状態を見てアニメーション再生。
	AnimationControl();
	
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

void HistoryBook::AnimationControl()
{
	//歴史書の状態を見る。
	switch (_HistoryBookState)
	{
		//本を閉じた状態。
	case State::Close:

		PlayAnimation(AnimationNo::AnimationClose, 0.2f, 1);

		//本を見えないようにする。
		_Model->enable = false;
		break;
		//本を開いている。
	case State::Opening:
		PlayAnimation(AnimationNo::AnimationOpening, 0.2f, 1);

		//本を開ききったら本を開いている状態で固定。
		if (_Anim->GetPlaying() == false)
		{
			_HistoryBookState = State::Open;
		}
		break;

		//本を開いた状態。
	case State::Open:
		PlayAnimation(AnimationNo::AnimationOpen, 0.2f, 1);
		break;

		//本を閉じている。
	case State::Closeing:
		PlayAnimation(AnimationNo::AnimationCloseing, 0.2f, 1);

		//閉じ終わったら。閉じた状態で固定。
		if (_Anim->GetPlaying() == false)
		{
			_HistoryBookState = State::Close;
		}
		break;
	default:
		break;
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
			//本を可視化。
			_Model->enable = true;

			//本を開いている状態にする。
			_HistoryBookState = State::Opening;

		}
		//tureの時に押されたらその時歴史書は開いているので閉じる状態に遷移。
		else if (_IsLookAtHistoryFlag == false)
		{
			_HistoryBookState = State::Closeing;
		}
		
		_PlayerFoward = _Player->transform->GetForward();
		_PlayerFoward = _PlayerFoward*-1;
		transform->SetLocalPosition(_PlayerFoward + PLAYER_HALFHEIGHT);
	}
}