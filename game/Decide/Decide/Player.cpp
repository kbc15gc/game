#include "Player.h"
#include "fbEngine/Skinmodel.h"
#include "fbEngine/SkinmodelData.h"
#include "fbEngine/Animation.h"
#include "fbEngine/RigidBody.h"
#include "fbEngine/BoxCollider.h"
#include "fbEngine/CapsuleCollider.h"

namespace 
{
	const float SPEED = 30.0f;
	const float JUMP_POWER = 15.0f;
}

Player::Player(const char * name) :
	GameObject(name),
	//�L�����N�^�[�R���g���[���[NULL
	_CharacterController(NULL),
	//�d�͐ݒ�
	_Gravity(-50.0f),
	//���݂̃X�e�[�g
	_CurrentState(NULL),
	//����X�e�[�g
	_RunState(this),
	//�A�C�h���X�e�[�g
	_IdolState(this),
	//�U���X�e�[�g
	_AttackState(this)
{
}

void Player::Awake()
{
	//���f��
	_Model = AddComponent<SkinModel>();
	//�A�j���[�V����
	_Anim = AddComponent<Animation>();
	////���W�b�h�{�f�B
	//_Rigid = AddComponent<RigidBody>();
	//�J�v�Z���R���C�_�[
	CCapsuleCollider* coll = AddComponent<CCapsuleCollider>();
	//�L�����N�^�[�R���g���[���[
	_CharacterController = AddComponent<CCharacterController>();
	//�����ݒ�
	_Height = 1.5f;
	//���a�ݒ�
	_Radius = 0.5f;
	//�J�v�Z���R���C�_�[�쐬
	coll->Create(_Radius, _Height);
	////���W�b�h�{�f�B�쐬
	//_Rigid->Create(0.0f, coll, Collision_ID::PLAYER, Vector3::zero, Vector3(0, _Height/2, 0));
	//_RB = (btRigidBody*)_Rigid->GetCollisonObj();
	////�X���[�v�����Ȃ�(�K�v���ǂ����킩��Ȃ��B)
	//_RB->setSleepingThresholds(0, 0);

	//�X�L�����f���쐬
	SkinModelData* modeldata = new SkinModelData();
	//���f���f�[�^�쐬
	modeldata->CloneModelData(SkinModelManager::LoadModel("Player.X"), _Anim);
	//���f���ݒ�
	_Model->SetModelData(modeldata);
	_Model->SetModelEffect(ModelEffectE::SPECULAR, false);
	//�L�����N�^�[�R���g���[���[������
	_CharacterController->Init(this,transform,_Radius,_Height, Vector3(0.0f,_Height / 2, 0.0f) , Collision_ID::PLAYER,coll, _Gravity);
	//�L�����N�^�[�R���g���[���[�̏d�͐ݒ�
	_CharacterController->SetGravity(_Gravity);
}

void Player::Start()
{
	//���f���ɃJ�����ݒ�
	_Model->SetCamera(GameObjectManager::mainCamera);
	//���f���Ƀ��C�g�ݒ�
	_Model->SetLight(GameObjectManager::mainLight);
	//�A�j���[�V����
	PlayAnimation(AnimationNo::AnimationIdol, 0.2f);
	//�����X�e�[�g�ݒ�
	ChangeState(State::Idol);
	//�|�W�V����
	transform->SetLocalPosition(Vector3(0.0f, 10.0f, 0.0f));
	//�ړ����x������
	_MoveSpeed = Vector3::zero;
	//�����v���C���[��ԁi�ҋ@�j
	_State = State::Idol;
	//�v���C���[�̃��x��������
	//�ŏ��͂P����
	_Level = 1;
	//�A�j���[�V�����̏I�����Ԑݒ�
	//��{��-1.0f��
	FOR((int)AnimationNo::AnimationNum)
	{
		_AnimationEndTime[i] = -1.0f;
	}
	//����A�j���[�V����
	_AnimationEndTime[(int)AnimationNo::AnimationRun] = 0.88;
}
#include "fbEngine/Camera.h"
void Player::Update()
{
	if (_CurrentState != NULL)
	{
		//�X�e�[�g�A�b�v�f�[�g
		_CurrentState->Update();
	}
	//�A�j���[�V�����R���g���[��
	AnimationControl();
	//�g�����X�t�H�[���X�V
	transform->UpdateTransform();
}

void Player::ChangeState(State nextstate)
{
	if (_CurrentState != NULL) {
		//���݂̃X�e�[�g�𔲂���Ƃ��̏���
		_CurrentState->Leave();
	}
	switch (nextstate)
	{
		//�ҋ@���
	case State::Idol:
		_CurrentState = &_IdolState;
		break;
		//������
	case State::Run:
		_CurrentState = &_RunState;
		break;
		//�U�����
	case State::Attack:
		_CurrentState = &_AttackState;
		break;
	default:
		break;
	}
	_State = nextstate;
	//�e�X�e�[�g�̓���ɌĂ΂�鏈��
	_CurrentState->Enter();
}

void Player::PlayAnimation(AnimationNo animno, float interpolatetime , int loopnum)
{
	//���݂̃A�j���[�V�����ƈႤ�A�j���[�V�����@&& �A�j���[�V�����i���o�[�������łȂ�
	if (_Anim->GetPlayAnimNo() != (int)animno && animno != AnimationNo::AnimationInvalid)
	{
		_Anim->SetAnimationEndTime(_AnimationEndTime[(int)animno]);
		_Anim->PlayAnimation((int)animno, interpolatetime , loopnum);
	}
}

void Player::AnimationControl()
{
	//�W�����v�A�j���[�V����
	if (_CharacterController->IsJump())
	{
		PlayAnimation(AnimationNo::AnimationJump, 0.2f);
	}
	else
	{
		//����A�j���[�V����
		if (_State == State::Run)
		{
			//_Anim->SetAnimationEndTime(0.33);
			PlayAnimation(AnimationNo::AnimationRun, 0.2f);
		}
		//�A�C�h���A�j���[�V����
		else if(_State == State::Idol)
		{
			PlayAnimation(AnimationNo::AnimationIdol, 0.2f);
		}
		//�A�^�b�N�A�j���[�V����
		else if (_State == State::Attack)
		{
			PlayAnimation(AnimationNo::AnimationAttack01, 0.2, 1);
		}
	}
}

const bool Player::GetAnimIsPlay() const 
{
	return _Anim->GetPlaying();
}