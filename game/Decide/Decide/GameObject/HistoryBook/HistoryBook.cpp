#include "HistoryBook.h"
#include "fbEngine\_Object\_Component\_3D\SkinModel.h"
#include "fbEngine\_Object\_Component\_3D\Animation.h"
#include "fbEngine\_Object\_Component\_3D\Light.h"

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

}

void HistoryBook::Start()
{
	transform->SetLocalPosition(Vector3(0.0f, 6.0f, 0.0f));
	transform->SetLocalScale(Vector3::one);

	//アニメーションの終了時間設定。
	//-1.0fを設定しているのはアニメーションの再生時間が1秒未満。
	_AnimationEndTime[(int)AnimationNo::AnimationCloseIdol] = 3.0f;	//本が閉じた状態のアニメーション。
	_AnimationEndTime[(int)AnimationNo::AnimationOpen] = 3.0f;			//本が開くアニメーション。
	_AnimationEndTime[(int)AnimationNo::AnimationOpenIdol] = 3.0f;		//本が開いた状態のアニメーション。
	_AnimationEndTime[(int)AnimationNo::AnimationClose] = 3.0f;		//本が閉じるアニメーション。

	//各エンドタイムを設定。
	for (int i = 0; i < (int)AnimationNo::AnimationNum; i++)
	{
		_Anim->SetAnimationEndTime(i, _AnimationEndTime[i]);
	}

	//ステートの初期化。
	_HistoryBookState = State::CloseIdol;
	//初期アニメーションとしてアイドルを再生。
	PlayAnimation(AnimationNo::AnimationCloseIdol, 0.2f);

	//フラグの初期化。
	_IsLookAtHistoryFlag = false;
}

void HistoryBook::Update()
{
	//歴史書を見るフラグの切り替え。
	if (XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_START) || KeyBoardInput->isPush(DIK_E))
	{
		//今_IsLookAtHistoryFlagに設定されている反対のフラグを設定。
		_IsLookAtHistoryFlag = !_IsLookAtHistoryFlag;		
	}

	//フラグを見て歴史書の状態を遷移。
	//trueなら歴史書を開く状態にする。
	if (_IsLookAtHistoryFlag == true)
	{
		_HistoryBookState = State::Open;
	}
	//tureの時に押されたらその時歴史書は開いているので閉じる状態に遷移。
	else if (_IsLookAtHistoryFlag == false)
	{
		_HistoryBookState = State::CloseIdol;
	}

	switch (_HistoryBookState)
	{
	case State::CloseIdol:
		PlayAnimation(AnimationNo::AnimationCloseIdol, 0.2f);
		break;
	case State::Open:
		PlayAnimation(AnimationNo::AnimationOpen, 0.2f, 0);
		if (_Anim->GetPlaying() == false)
		{
			_HistoryBookState = State::OpenIdol;
		}
		break;
	case State::OpenIdol:
		PlayAnimation(AnimationNo::AnimationOpenIdol, 0.2f);
		break;
	case State::Close:
		PlayAnimation(AnimationNo::AnimationClose, 0.2f, 0);
		if (_Anim->GetPlaying() == false)
		{
			_HistoryBookState = State::CloseIdol;
		}
		break;
	default:
		break;
	}
	
	//トランスフォーム更新
	transform->UpdateTransform();
}

void HistoryBook::PlayAnimation(AnimationNo animno, float interpolatetime, int loopnum)
{
	//現在のアニメーションと違うアニメーション　&& アニメーションナンバーが無効でない
	if (_Anim->GetPlayAnimNo() != (int)animno && animno != AnimationNo::AnimationInvalid)
	{
		_Anim->PlayAnimation((int)animno, interpolatetime, loopnum);
	}
}