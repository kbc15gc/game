#include"stdafx.h"
#include "HistoryBook.h"
#include "fbEngine\_Object\_Component\_3D\SkinModel.h"
#include "fbEngine\_Object\_Component\_3D\Animation.h"
#include "GameObject\Player\Player.h"

namespace
{
	//�v���C���[�̔����̍����B
	const Vector3 PLAYER_HALFHEIGHT(0.0f, 0.75f, 0.0f);
}

HistoryBook::HistoryBook(const char * name) :
	GameObject(name)
{

}

void HistoryBook::Awake()
{
	//���f���B
	_Model = AddComponent<SkinModel>();
	//�A�j���[�V����
	_Anim = AddComponent<Animation>();
	//���f���f�[�^�B
	SkinModelData* modeldata = new SkinModelData();
	//�N���[�����f���̍쐬�B
	modeldata->CloneModelData(SkinModelManager::LoadModel("HistoryBook.X"), _Anim);
	_Model->SetModelData(modeldata);
	_Model->SetModelEffect(ModelEffectE::CAST_SHADOW, false);

	//�v���C���[������
	_Player = (Player*)INSTANCE(GameObjectManager)->FindObject("Player");
}

void HistoryBook::Start()
{
	transform->SetLocalPosition(Vector3(0.0f, 0.0f, 0.0f) + PLAYER_HALFHEIGHT);
	transform->SetLocalScale(Vector3::one);

	//�A�j���[�V�����̏I�����Ԑݒ�B
	//-1.0f��ݒ肵�Ă���̂̓A�j���[�V�����̍Đ����Ԃ�1�b�����B
	_AnimationEndTime[(int)AnimationNo::AnimationClose] = 3.0f;		//�{��������Ԃ̃A�j���[�V�����B
	_AnimationEndTime[(int)AnimationNo::AnimationOpening] = 3.0f;		//�{���J���A�j���[�V�����B
	_AnimationEndTime[(int)AnimationNo::AnimationOpen] = 3.0f;			//�{���J������Ԃ̃A�j���[�V�����B
	_AnimationEndTime[(int)AnimationNo::AnimationCloseing] = 3.3f;	    //�{������A�j���[�V�����B

	//�e�G���h�^�C����ݒ�B
	for (int i = 0; i < (int)AnimationNo::AnimationNum; i++)
	{
		_Anim->SetAnimationEndTime(i, _AnimationEndTime[i]);
	}

	//�X�e�[�g�̏������B
	_HistoryBookState = State::Close;
	//�A�j���[�V�����̏������B
	PlayAnimation(AnimationNo::AnimationClose, 0.2f, 0);

	//�t���O�̏������B
	_IsLookAtHistoryFlag = false;

	//�{�͌����Ȃ��悤�ɐݒ�B
	_Model->enable = false;

}

void HistoryBook::Update()
{
	//���j�������Ă���t���O��ς��鑀��B
	ChangeIsLookAtHistoryFlag();

	//���j���̏�Ԃ����ăA�j���[�V�����Đ��B
	AnimationControl();
	
	//�g�����X�t�H�[���X�V�B
	transform->UpdateTransform();

}

void HistoryBook::PlayAnimation(AnimationNo animno, float interpolatetime, int loopnum)
{
	//���݂̃A�j���[�V�����ƈႤ�A�j���[�V�����@&& �A�j���[�V�����i���o�[�������łȂ��B
	if (_Anim->GetPlayAnimNo() != (int)animno && animno != AnimationNo::AnimationInvalid)
	{
		_Anim->PlayAnimation((int)animno, interpolatetime, loopnum);
	}
}

void HistoryBook::AnimationControl()
{
	//���j���̏�Ԃ�����B
	switch (_HistoryBookState)
	{
		//�{�������ԁB
	case State::Close:

		PlayAnimation(AnimationNo::AnimationClose, 0.2f, 1);

		//�{�������Ȃ��悤�ɂ���B
		_Model->enable = false;
		break;
		//�{���J���Ă���B
	case State::Opening:
		PlayAnimation(AnimationNo::AnimationOpening, 0.2f, 1);

		//�{���J����������{���J���Ă����ԂŌŒ�B
		if (_Anim->GetPlaying() == false)
		{
			_HistoryBookState = State::Open;
		}
		break;

		//�{���J������ԁB
	case State::Open:
		PlayAnimation(AnimationNo::AnimationOpen, 0.2f, 1);
		break;

		//�{����Ă���B
	case State::Closeing:
		PlayAnimation(AnimationNo::AnimationCloseing, 0.2f, 1);

		//���I�������B������ԂŌŒ�B
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
	//���j��������t���O�̐؂�ւ��B
	if (XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_START) || KeyBoardInput->isPush(DIK_E))
	{
		//��_IsLookAtHistoryFlag�ɐݒ肳��Ă��锽�΂̃t���O��ݒ�B
		_IsLookAtHistoryFlag = !_IsLookAtHistoryFlag;

		//�t���O�����ė��j���̏�Ԃ�J�ځB
		//true�Ȃ���j�����J����Ԃɂ���B
		if (_IsLookAtHistoryFlag == true)
		{
			//�{�������B
			_Model->enable = true;

			//�{���J���Ă����Ԃɂ���B
			_HistoryBookState = State::Opening;

		}
		//ture�̎��ɉ����ꂽ�炻�̎����j���͊J���Ă���̂ŕ����ԂɑJ�ځB
		else if (_IsLookAtHistoryFlag == false)
		{
			_HistoryBookState = State::Closeing;
		}
		
		_PlayerFoward = _Player->transform->GetForward();
		_PlayerFoward = _PlayerFoward*-1;
		transform->SetLocalPosition(_PlayerFoward + PLAYER_HALFHEIGHT);
	}
}