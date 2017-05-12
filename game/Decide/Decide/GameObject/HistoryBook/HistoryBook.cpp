#include"stdafx.h"
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

	//�A�j���[�V�����̏I�����Ԑݒ�B
	//-1.0f��ݒ肵�Ă���̂̓A�j���[�V�����̍Đ����Ԃ�1�b�����B
	//_AnimationEndTime[(int)AnimationNo::AnimationCloseIdol] = -1.0f;	//�{��������Ԃ̃A�j���[�V�����B
	//_AnimationEndTime[(int)AnimationNo::AnimationOpen] = 0.9f;			//�{���J���A�j���[�V�����B
	//_AnimationEndTime[(int)AnimationNo::AnimationOpenIdol] = -1.0f;		//�{���J������Ԃ̃A�j���[�V�����B
	//_AnimationEndTime[(int)AnimationNo::AnimationClose] = 1.0f;		//�{������A�j���[�V�����B

	//�e�G���h�^�C����ݒ�B
	//for (int i = 0; i < (int)AnimationNo::AnimationNum; i++)
	//{
	//	_Anim->SetAnimationEndTime(i, _AnimationEndTime[i]);
	//}

	//�X�e�[�g�̏������B
	_HistoryBookState = State::Open;
	//�����A�j���[�V�����Ƃ��ăA�C�h�����Đ��B
	//PlayAnimation(AnimationNo::AnimationOpen, 0.2f);
	_Anim->PlayAnimation((int)AnimationNo::AnimationCloseIdol, 0.2f);
}

void HistoryBook::Update()
{
	if (XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_START) || KeyBoardInput->isPush(DIK_E))
	{
		//��_IsLookAtHistoryFlag�ɐݒ肳��Ă��锽�΂̃t���O��ݒ�B
		_IsLookAtHistoryFlag = !_IsLookAtHistoryFlag;
	}

	/*switch (_HistoryBookState)
	{
	case State::IdolClose:
		_Anim->PlayAnimation((int)AnimationNo::AnimationCloseIdol, 0.2f);
		break;
	case State::Open:
		_Anim->PlayAnimation((int)AnimationNo::AnimationOpen, 0.2f);
		break;
	default:
		break;
	}*/
	
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