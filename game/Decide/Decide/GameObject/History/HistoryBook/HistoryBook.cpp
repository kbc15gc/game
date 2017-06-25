/**
* 歴史書クラスの実装.
*/
#include"stdafx.h"
#include "HistoryBook.h"

#include "fbEngine\_Object\_Component\_3D\SkinModel.h"
#include "fbEngine\_Object\_Component\_3D\Animation.h"
#include "GameObject\Player\Player.h"

//状態.
#include"HFSM\HistoryBookStateUnused.h"
#include"HFSM\HistoryBookStateIdol.h"
#include"HFSM\HistoryBookStateMove.h"
#include"HFSM\HistoryBookStateOpen.h"
#include"HFSM\HistoryBookStateClose.h"

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

	_HistoryPage = INSTANCE(GameObjectManager)->AddNew<HistoryPage>("HistoryPage", 1);
	_HistoryPage->transform->SetParent(this->transform);

}

/**
* 初期化.
*/
void HistoryBook::Start()
{

	//状態リストを初期化.
	_InitState();

	//アニメーションの終了時間設定.
	//-1.0fを設定しているのはアニメーションの再生時間が1秒未満.
	double animationEndTime[(int)AnimationCodeE::AnimationNum];
	animationEndTime[(int)AnimationCodeE::CloseIdol] = 2.0f;		//待機アニメーション.

	//各エンドタイムを設定.
	for (int i = 0; i < (int)AnimationCodeE::AnimationNum; i++)
	{
		_Anim->SetAnimationEndTime(i, animationEndTime[i]);
	}

	//アニメーションの初期化。
	PlayAnimation(AnimationCodeE::OpenIdol, 0.0f);

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
}

/**
* アニメーションの再生.
*
* @param animno				アニメーションのナンバー.
* @param interpolatetime	補間時間.
* @param loopnum			ループ回数 (デフォルトは-1).
*/
void HistoryBook::PlayAnimation(AnimationCodeE animno, float interpolatetime, int loopnum)
{
	//現在のアニメーションと違うアニメーション　&& アニメーションナンバーが無効でない。
	if (_Anim->GetPlayAnimNo() != (int)animno && animno != AnimationCodeE::Invalid)
	{
		_Anim->PlayAnimation((int)animno, interpolatetime, loopnum);
	}
}

/**
* 状態リストの初期化.
*/
void HistoryBook::_InitState()
{
	//未使用状態.
	_StateList.push_back(unique_ptr<HistoryBookStateUnused>(new HistoryBookStateUnused(this)));
	//待機状態.
	_StateList.push_back(unique_ptr<HistoryBookStateIdol>(new HistoryBookStateIdol(this)));
	//移動状態.
	_StateList.push_back(unique_ptr<HistoryBookStateMove>(new HistoryBookStateMove(this)));
	//開く状態状態.
	_StateList.push_back(unique_ptr<HistoryBookStateOpen>(new HistoryBookStateOpen(this)));
	//閉じる状態状態.
	_StateList.push_back(unique_ptr<HistoryBookStateClose>(new HistoryBookStateClose(this)));

	//初期値は閉じている.
	ChangeState(StateCodeE::Unused);

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

		//未使用なら表示。使用状態なら閉じる.
		if (_NowState == (int)StateCodeE::Unused)
		{
			_IsOpenOrClose = true;
			ChangeState(StateCodeE::Move);
		}
		else if (_NowState == (int)StateCodeE::Idol)
		{
			_IsOpenOrClose = false;
			ChangeState(StateCodeE::Close);
		}
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