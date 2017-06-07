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
	GameObject(name),
	//������ԁB
	_CloseState(this),
	//�J���Ă����ԁB
	_OpeningState(this),
	//�J������ԁB
	_Open(this),
	//���Ă����ԁB
	_Closeing(this)
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
	ChangeState(State::Close);
	//�A�j���[�V�����̏������B
	PlayAnimation(AnimationNo::AnimationClose, 0.2f, 0);

	//�{�͌����Ȃ��悤�ɐݒ�B
	_Model->enable = false;

}

void HistoryBook::Update()
{
	//���j�������Ă���t���O��ς��鑀��B
	ChangeIsLookAtHistoryFlag();

	//���݂̃X�e�[�g�ɉ��������Ă���Ȃ�B
	if (_CurrentState != nullptr)
	{
		//�X�e�[�g�̍X�V�B
		_CurrentState->Update();
	}

	_AngleY += 0.9;
	transform->SetLocalAngle(Vector3(0.0f, _AngleY, 0.0f));
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
			ChangeState(State::Opening);

		}
		//ture�̎��ɉ����ꂽ�炻�̎����j���͊J���Ă���̂ŕ����ԂɑJ�ځB
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
	//���݂̃X�e�[�g������Ȃ猻�݂̃X�e�[�g�𔲂��鏈��������B
	if (_CurrentState != nullptr)
	{
		_CurrentState->Exit();
	}

	//�ǂ̃X�e�[�g���������`�F�b�N�B
	switch (nextstate)
	{
		//������ԁB
	case State::Close:
		_CurrentState = &_CloseState;
		break;

		//�J���Ă����ԁB
	case State::Opening:
		_CurrentState = &_OpeningState;
		break;

		//�J������ԁB
	case State::Open:
		_CurrentState = &_Open;
		break;

		//���Ă����ԁB
	case State::Closeing:
		_CurrentState = &_Closeing;
		break;
	default:
		break;
	}

	//�n���ꂽ�X�e�[�g�ɕύX�B
	_State = nextstate;

	//�X�e�[�g���؂�ւ�����̂Ŋe�X�e�[�g�̓���ɌĂ΂�鏈�����ĂԁB
	_CurrentState->Entry();
}