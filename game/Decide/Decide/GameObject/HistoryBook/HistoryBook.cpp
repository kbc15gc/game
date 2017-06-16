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

}

/**
* 初期化.
*/
void HistoryBook::Start()
{

	//状態リストを初期化.
	_InitState();

	transform->SetLocalPosition(Vector3(0.0f, 0.0f, 0.0f) + PLAYER_HALFHEIGHT);
	transform->SetLocalScale(Vector3::one);

	//アニメーションの終了時間設定.
	//-1.0fを設定しているのはアニメーションの再生時間が1秒未満.
	double animationEndTime[(int)AnimationCodeE::AnimationNum];
	animationEndTime[(int)AnimationCodeE::Idol] = 2.0f;		//待機アニメーション.

	//各エンドタイムを設定.
	for (int i = 0; i < (int)AnimationCodeE::AnimationNum; i++)
	{
		_Anim->SetAnimationEndTime(i, animationEndTime[i]);
	}

	//アニメーションの初期化。
	PlayAnimation(AnimationCodeE::Idol, 0.2f, 0);

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
	_StateList.push_back(new HistoryBookStateUnused(this));
	//待機状態.
	_StateList.push_back(new HistoryBookStateIdol(this));
	//開く状態状態.
	_StateList.push_back(new HistoryBookStateOpen(this));


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
			ChangeState(StateCodeE::Idol);
		}
		else
		{
			ChangeState(StateCodeE::Unused);
			_Player->SetEnable(true);
		}

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