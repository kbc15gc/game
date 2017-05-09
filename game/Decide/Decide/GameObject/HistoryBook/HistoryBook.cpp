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
	SkinModel* model = AddComponent<SkinModel>();
	//アニメーション
	_Anim = AddComponent<Animation>();
	//モデルデータ。
	SkinModelData* modeldata = new SkinModelData();
	//クローンモデルの作成。
	modeldata->CloneModelData(SkinModelManager::LoadModel("HistoryBook.X"), _Anim);
	model->SetModelData(modeldata);
	model->SetModelEffect(ModelEffectE::CAST_SHADOW, false);

}

void HistoryBook::Start()
{
	transform->SetLocalPosition(Vector3(0.0f, 6.0f, 0.0f));
	transform->SetLocalScale(Vector3::one);

	////アニメーションの終了時間設定。
	//_AnimationEndTime[(int)AnimationNo::AnimationOpen] = -1.0f;	//本が開くアニメーション。

	////各エンドタイムを設定。
	//for (int i = 0; i < (int)AnimationNo::AnimationNum; i++)
	//{
	//	_Anim->SetAnimationEndTime(i, _AnimationEndTime[i]);
	//}

	//ステートの初期化。
	HistoryBookState = State::Open;
	//初期アニメーションとしてアイドルを再生。
	//PlayAnimation(AnimationNo::AnimationOpen, 0.2f);
	_Anim->PlayAnimation((int)AnimationNo::AnimationOpen, 0.2f);
}

void HistoryBook::Update()
{
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