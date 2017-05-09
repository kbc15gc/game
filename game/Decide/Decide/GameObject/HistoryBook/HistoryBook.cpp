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
	//���f���B
	SkinModel* model = AddComponent<SkinModel>();
	//�A�j���[�V����
	_Anim = AddComponent<Animation>();
	//���f���f�[�^�B
	SkinModelData* modeldata = new SkinModelData();
	//�N���[�����f���̍쐬�B
	modeldata->CloneModelData(SkinModelManager::LoadModel("HistoryBook.X"), _Anim);
	model->SetModelData(modeldata);
	model->SetModelEffect(ModelEffectE::CAST_SHADOW, false);

}

void HistoryBook::Start()
{
	transform->SetLocalPosition(Vector3(0.0f, 6.0f, 0.0f));
	transform->SetLocalScale(Vector3::one);

	////�A�j���[�V�����̏I�����Ԑݒ�B
	//_AnimationEndTime[(int)AnimationNo::AnimationOpen] = -1.0f;	//�{���J���A�j���[�V�����B

	////�e�G���h�^�C����ݒ�B
	//for (int i = 0; i < (int)AnimationNo::AnimationNum; i++)
	//{
	//	_Anim->SetAnimationEndTime(i, _AnimationEndTime[i]);
	//}

	//�X�e�[�g�̏������B
	HistoryBookState = State::Open;
	//�����A�j���[�V�����Ƃ��ăA�C�h�����Đ��B
	//PlayAnimation(AnimationNo::AnimationOpen, 0.2f);
	_Anim->PlayAnimation((int)AnimationNo::AnimationOpen, 0.2f);
}

void HistoryBook::Update()
{
	//�g�����X�t�H�[���X�V
	transform->UpdateTransform();
}

void HistoryBook::PlayAnimation(AnimationNo animno, float interpolatetime, int loopnum)
{
	//���݂̃A�j���[�V�����ƈႤ�A�j���[�V�����@&& �A�j���[�V�����i���o�[�������łȂ�
	if (_Anim->GetPlayAnimNo() != (int)animno && animno != AnimationNo::AnimationInvalid)
	{
		_Anim->PlayAnimation((int)animno, interpolatetime, loopnum);
	}
}