/**
* ���j���N���X�̎���.
*/
#include"stdafx.h"
#include "HistoryBook.h"

#include "fbEngine\_Object\_Component\_3D\SkinModel.h"
#include "fbEngine\_Object\_Component\_3D\Animation.h"
#include "GameObject\Player\Player.h"

namespace
{

	/** �v���C���[�̔����̍����B*/
	const Vector3 PLAYER_HALFHEIGHT(0.0f, 0.75f, 0.0f);

}

/**
* �R���X�g���N�^��̏�����.
*/
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

/**
* ������.
*/
void HistoryBook::Start()
{

	//��ԃ��X�g��������.
	_InitState();

	transform->SetLocalPosition(Vector3(0.0f, 0.0f, 0.0f) + PLAYER_HALFHEIGHT);
	transform->SetLocalScale(Vector3::one);

	//�A�j���[�V�����̏I�����Ԑݒ�.
	//-1.0f��ݒ肵�Ă���̂̓A�j���[�V�����̍Đ����Ԃ�1�b����.
	double animationEndTime[(int)AnimationCodeE::AnimationNum];
	animationEndTime[(int)AnimationCodeE::Idol] = 2.0f;		//�ҋ@�A�j���[�V����.

	//�e�G���h�^�C����ݒ�.
	for (int i = 0; i < (int)AnimationCodeE::AnimationNum; i++)
	{
		_Anim->SetAnimationEndTime(i, animationEndTime[i]);
	}

	//�A�j���[�V�����̏������B
	PlayAnimation(AnimationCodeE::Idol, 0.2f, 0);

	//�{�͌����Ȃ��悤�ɐݒ�B
	_Model->enable = false;

}

/**
* �X�V.
*/
void HistoryBook::Update()
{
	//���j�������Ă���t���O��ς��鑀��B
	_ChangeIsLookAtHistoryFlag();

	//��Ԃ̍X�V�B
	_StateList[_NowState]->Update();

	//�g�����X�t�H�[���X�V�B
	transform->UpdateTransform();
}

/**
* �A�j���[�V�����̍Đ�.
*
* @param animno				�A�j���[�V�����̃i���o�[.
* @param interpolatetime	��Ԏ���.
* @param loopnum			���[�v�� (�f�t�H���g��-1).
*/
void HistoryBook::PlayAnimation(AnimationCodeE animno, float interpolatetime, int loopnum)
{
	//���݂̃A�j���[�V�����ƈႤ�A�j���[�V�����@&& �A�j���[�V�����i���o�[�������łȂ��B
	if (_Anim->GetPlayAnimNo() != (int)animno && animno != AnimationCodeE::Invalid)
	{
		_Anim->PlayAnimation((int)animno, interpolatetime, loopnum);
	}
}

/**
* ��ԃ��X�g�̏�����.
*/
void HistoryBook::_InitState()
{
	//���g�p���.
	_StateList.push_back(new HistoryBookStateUnused(this));
	//�ҋ@���.
	_StateList.push_back(new HistoryBookStateIdol(this));
	//�J����ԏ��.
	_StateList.push_back(new HistoryBookStateOpen(this));


	//�����l�͕��Ă���.
	ChangeState(StateCodeE::Unused);

}

/**
* ���j�����J���Ă��锻��t���O��ύX.
*/
void HistoryBook::_ChangeIsLookAtHistoryFlag()
{
	//���j��������t���O�̐؂�ւ��B
	//�X�^�[�g�{�^������E�L�[�������ꂽ.
	if (XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_START) || KeyBoardInput->isPush(DIK_E))
	{
		//���g�p�Ȃ�\���B�g�p��ԂȂ����.
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
* ��Ԃ̕ύX.
*
* @param nextState	���̏��.
*/
void HistoryBook::ChangeState(StateCodeE nextState)
{
	//���݂̏�Ԃ�����Ȃ猻�݂̏�Ԃ𔲂��鏈��������B
	if (_NowState != (int)StateCodeE::Invalid)
	{
		_StateList[_NowState]->Exit();
	}

	//��Ԃ�ύX.
	_NowState = (int)nextState;

	//��Ԃ��؂�ւ�����̂Ŋe��Ԃ̓���ɌĂ΂�鏈�����ĂԁB
	_StateList[_NowState]->Entry();
}