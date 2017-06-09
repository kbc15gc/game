#include"stdafx.h"
#include "Player.h"
#include "fbEngine\_Object\_Component\_3D\SkinModel.h"
#include "fbEngine\_Object\_Component\_3D\Animation.h"
#include <string>
#include <sstream>

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
	_AttackState(this),
	//���S�X�e�[�g
	_DeathState(this)
{
}

Player::~Player()
{
}

void Player::Awake()
{
	//���f��
	_Model = AddComponent<SkinModel>();
	//�A�j���[�V����
	_Anim = AddComponent<Animation>();
	//�J�v�Z���R���C�_�[
	CCapsuleCollider* coll = AddComponent<CCapsuleCollider>();
	//�L�����N�^�[�R���g���[���[
	_CharacterController = AddComponent<CCharacterController>();
	//�L�����N�^�[�p�����[�^�[
	_PlayerParam = AddComponent<CharacterParameter>();
	//�����ݒ�
	_Height = 1.5f;
	//���a�ݒ�
	_Radius = 0.5f;
	//�J�v�Z���R���C�_�[�쐬
	coll->Create(_Radius, _Height);
	//�X�L�����f���쐬
	SkinModelData* modeldata = new SkinModelData();
	//���f���f�[�^�쐬
	modeldata->CloneModelData(SkinModelManager::LoadModel("Player.X"), _Anim);
	//���f���ݒ�
	_Model->SetModelData(modeldata);
	_Model->SetModelEffect(ModelEffectE::SPECULAR, true);
	_Model->SetAllBlend(Color::white * 13);
	//�L�����N�^�[�R���g���[���[������
	{
		_CharacterController->Init(this, transform, _Radius, _Height, Vector3(0.0f, _Height / 2, 0.0f), Collision_ID::PLAYER, coll, _Gravity);
		// �ȉ��Փ˂���肽������(������)���w��B
		{
			// ���e�X�g�p(��Œ����Ă�)�B
			_CharacterController->AttributeXZ_AllOff();	// �S�Փ˖����B
			//_CharacterController->AddAttributeXZ(Collision_ID::ATTACK);	// �U���R���W������ǉ��B
		}
		// �ȉ��Փ˂���肽������(�c����)���w��B
		{
			// ���e�X�g�p(��Œ����Ă�)�B
			_CharacterController->AttributeY_AllOn();	// �S�ՓˁB
			_CharacterController->SubAttributeY(Collision_ID::ENEMY);	// �G�l�~�[���폜�B
			_CharacterController->SubAttributeY(Collision_ID::ATTACK);
		}
		//�L�����N�^�[�R���g���[���[�̏d�͐ݒ�
		_CharacterController->SetGravity(_Gravity);
	}
	//HP�̃e�L�X�g�\��
	{
		_HPText = INSTANCE(GameObjectManager)->AddNew<TextObject>("HPText", _Priority);
		_HPText->Initialize(L"", 70.0f);
		_HPText->SetFormat((int)fbText::TextFormatE::CENTER | (int)fbText::TextFormatE::UP);
		_HPText->transform->SetLocalPosition(Vector3(1150, 630,0));
	}
	//MP�̃e�L�X�g�\��
	{
		_MPText = INSTANCE(GameObjectManager)->AddNew<TextObject>("MPText", _Priority);
		_MPText->Initialize(L"", 70.0f);
		_MPText->SetFormat((int)fbText::TextFormatE::CENTER | (int)fbText::TextFormatE::UP);
		_MPText->transform->SetLocalPosition(Vector3(1150, 680, 0));
	}
	//�_���[�WSE������
	_DamageSE = INSTANCE(GameObjectManager)->AddNew<SoundSource>("DamageSE", 0);
	_DamageSE->Init("Asset/Sound/Damage_01.wav");
}

void Player::Start()
{
	//���f���ɃJ�����ݒ�
	_Model->SetCamera(INSTANCE(GameObjectManager)->mainCamera);
	//���f���Ƀ��C�g�ݒ�
	_Model->SetLight(INSTANCE(GameObjectManager)->mainLight);
	//�A�j���[�V�����̏I�����Ԑݒ�
	_AnimationEndTime[(int)AnimationNo::AnimationIdol] = -1.0;			//�A�C�h��
	_AnimationEndTime[(int)AnimationNo::AnimationWalk] = -1.0;			//����
	_AnimationEndTime[(int)AnimationNo::AnimationRun] = 0.68;			//����
	_AnimationEndTime[(int)AnimationNo::AnimationAttack01] = -1.0f;		//�U���P
	_AnimationEndTime[(int)AnimationNo::AnimationAttack02] = -1.0;		//�U���Q
	_AnimationEndTime[(int)AnimationNo::AnimationDeath] = -1.0;			//���S
	//�e�G���h�^�C����ݒ�
	for (int i = 0; i < (int)AnimationNo::AnimationNum; i++)
	{
		_Anim->SetAnimationEndTime(i, _AnimationEndTime[i]);
	}
	//�����A�j���[�V�����Ƃ��ăA�C�h�����Đ�
	PlayAnimation(AnimationNo::AnimationIdol, 0.2f);
	//�����X�e�[�g�ݒ�
	ChangeState(State::Idol);
	//�|�W�V����
	transform->SetLocalPosition(Vector3(374, 69, -1275));
	//�ړ����x������
	_MoveSpeed = Vector3::zero;
	//�����v���C���[��ԁi�ҋ@�j
	_State = State::Idol;
	//�U���A�j���[�V�����X�e�[�g�̏�����
	_NowAttackAnimNo = AnimationNo::AnimationInvalid;
	_NextAttackAnimNo = AnimationNo::AnimationInvalid;
	//�v���C���[�̃p�����[�^�[�������B
	_PlayerParam->ParamInit(100, 50, 5, 4, 3, 1);
	//���x��������
	_Level = 1;
}

void Player::Update()
{
	if (_CurrentState != NULL)
	{
		//�X�e�[�g�A�b�v�f�[�g
		_CurrentState->Update();
	}
	//HP�̃e�L�X�g��\���X�V
	string hp = to_string(_PlayerParam->_HP);
	string mp = to_string(_PlayerParam->_MP);
	_HPText->SetString(hp.data());
	_MPText->SetString(mp.data());
	//���C�t��0�ɂȂ�Ǝ��S����B

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
		//���S���
	case State::Death:
		_CurrentState = &_DeathState;
		//�f�t�H���g
	default:
		break;
	}
	//���̃X�e�[�g�ɕύX
	_State = nextstate;
	//�e�X�e�[�g�̓���ɌĂ΂�鏈��
	_CurrentState->Enter();
}

void Player::PlayAnimation(AnimationNo animno, float interpolatetime , int loopnum)
{
	//���݂̃A�j���[�V�����ƈႤ�A�j���[�V�����@&& �A�j���[�V�����i���o�[�������łȂ�
	if (_Anim->GetPlayAnimNo() != (int)animno && animno != AnimationNo::AnimationInvalid)
	{
		_Anim->PlayAnimation((int)animno, interpolatetime , loopnum);
	}
}

void Player::AnimationControl()
{
	//�W�����v�A�j���[�V����
	if (_CharacterController->IsJump())
	{
		PlayAnimation(AnimationNo::AnimationJump, 0.1f);
	}
	else
	{
		//����A�j���[�V����
		if (_State == State::Run)
		{
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
			if (_NextAttackAnimNo == AnimationNo::AnimationAttackStart)
			{
				//�U���J�n
				PlayAnimation(_NextAttackAnimNo, 0.1f, 1);
				_NowAttackAnimNo = _NextAttackAnimNo;
				_NextAttackAnimNo = AnimationNo::AnimationInvalid;
			}
			else if (_NextAttackAnimNo != AnimationNo::AnimationInvalid)
			{
				//�A��
				PlayAnimation(_NextAttackAnimNo, 0.1f, 1);
				_NowAttackAnimNo = _NextAttackAnimNo;
				_NextAttackAnimNo = AnimationNo::AnimationInvalid;
			}

			
		}
		//���S�A�j���[�V����
		else if (_State == State::Death)
		{
			PlayAnimation(AnimationNo::AnimationDeath, 0.1f, 1);
		}
	}
}
