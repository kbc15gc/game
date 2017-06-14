/**
* 歴史書クラスの実装.
*/
#include"stdafx.h"
#include "HistoryBook.h"

#include "fbEngine\_Object\_Component\_3D\SkinModel.h"
#include "fbEngine\_Object\_Component\_3D\Animation.h"
#include "GameObject\Player\Player.h"

namespace
{

	/** プレイヤーの半分の高さ。*/
	const Vector3 PLAYER_HALFHEIGHT(0.0f, 0.75f, 0.0f);

}

/**
* コンストラクタ後の初期化.
*/
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

	//状態リストを初期化.
	_InitState();

}

/**
* 初期化.
*/
void HistoryBook::Start()
{
	transform->SetLocalPosition(Vector3(0.0f, 0.0f, 0.0f) + PLAYER_HALFHEIGHT);
	transform->SetLocalScale(Vector3::one);

	//アニメーションの終了時間設定。
	//-1.0fを設定しているのはアニメーションの再生時間が1秒未満。
	_AnimationEndTime[(int)AnimationNo::AnimationClose] = 5.0f;		//本が閉じた状態のアニメーション。
	_AnimationEndTime[(int)AnimationNo::AnimationOpening] = 3.0f;		//本が開くアニメーション。
	_AnimationEndTime[(int)AnimationNo::AnimationOpen] = 3.0f;			//本が開いた状態のアニメーション。
	_AnimationEndTime[(int)AnimationNo::AnimationCloseing] = 3.3f;	    //本が閉じるアニメーション。

	//各エンドタイムを設定。
	for (int i = 0; i < (int)AnimationNo::AnimationNum; i++)
	{
		_Anim->SetAnimationEndTime(i, _AnimationEndTime[i]);
	}

	//アニメーションの初期化。
	PlayAnimation(AnimationNo::AnimationClose, 0.2f, 0);

	//本は見えないように設定。
	_Model->enable = false;

}

/**
* 更新.
*/
void HistoryBook::Update()
{
	//歴史書を見ているフラグを変える操作。
	_ChangeIsLookAtHistoryFlag();

	//状態の更新。
	_StateList[_NowState]->Update();

	//_AngleY += 0.9;
	//transform->SetLocalAngle(Vector3(0.0f, _AngleY, 0.0f));

	//トランスフォーム更新。
	transform->UpdateTransform();
}

/**
* アニメーションの再生.
*
* @param animno				アニメーションのナンバー.
* @param interpolatetime	補間時間.
* @param loopnum			ループ回数 (デフォルトは-1).
*/
void HistoryBook::PlayAnimation(AnimationNo animno, float interpolatetime, int loopnum)
{
	//現在のアニメーションと違うアニメーション　&& アニメーションナンバーが無効でない。
	if (_Anim->GetPlayAnimNo() != (int)animno && animno != AnimationNo::AnimationInvalid)
	{
		_Anim->PlayAnimation((int)animno, interpolatetime, loopnum);
	}
}

/**
* 状態リストの初期化.
*/
void HistoryBook::_InitState()
{
	//閉じた状態.
	_StateList.push_back(new HistoryBookStateClose(this));
	//本が開いている状態.
	_StateList.push_back(new HistoryBookStateOpening(this));
	//開いた状態状態.
	_StateList.push_back(new HistoryBookStateOpen(this));
	//本が閉じている.
	_StateList.push_back(new HistoryBookStateCloseing(this));

	//初期値は閉じている.
	ChangeState(StateCodeE::Close);

}

/**
* 歴史書を開いている判定フラグを変更.
*/
void HistoryBook::_ChangeIsLookAtHistoryFlag()
{
	//歴史書を見るフラグの切り替え。
	//スタートボタン又はEキーが押された.
	if (XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_START) || KeyBoardInput->isPush(DIK_E))
	{
		//今_IsLookAtHistoryFlagに設定されている反対のフラグを設定。
		_IsLookAtHistoryFlag = !_IsLookAtHistoryFlag;

		//フラグを見て歴史書の状態を遷移。
		//trueなら歴史書を開く状態にする。
		//tureの時に押されたらその時歴史書は開いているので閉じる状態に遷移。
		ChangeState(_IsLookAtHistoryFlag ? StateCodeE::Opening : StateCodeE::Closeing);
		
		_PlayerFoward = _Player->transform->GetForward();
		_PlayerFoward.Scale(-1);
		transform->SetLocalPosition(_PlayerFoward + PLAYER_HALFHEIGHT);
	}
}

/**
* 状態の変更.
*
* @param nextState	次の状態.
*/
void HistoryBook::ChangeState(StateCodeE nextState)
{
	//現在の状態があるなら現在の状態を抜ける処理をする。
	if (_NowState != (int)StateCodeE::Invalid)
	{
		_StateList[_NowState]->Exit();
	}

	//状態を変更.
	_NowState = (int)nextState;

	//状態が切り替わったので各状態の入りに呼ばれる処理を呼ぶ。
	_StateList[_NowState]->Entry();
}