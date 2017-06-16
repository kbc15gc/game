#include"stdafx.h"
#include "Player.h"
#include "fbEngine\_Object\_Component\_3D\SkinModel.h"
#include "fbEngine\_Object\_Component\_3D\Animation.h"
#include <string>
#include <sstream>
#include "GameObject\Component\ParameterBar.h"

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
	//��]�R���|�[�l���g
	_Rotation = AddComponent<ObjectRotation>();
	// HP�o�[�B
	_HPBar = AddComponent<ParameterBar>();
	// MP�o�[�B
	_MPBar = AddComponent<ParameterBar>();
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
			_CharacterController->AttributeXZ_AllOff();	// �S�Փ˖����B
			_CharacterController->AddAttributeXZ(Collision_ID::GROUND);	// �n�ʃR���W������ǉ��B
			_CharacterController->AddAttributeXZ(Collision_ID::ENEMY);	// �G�̃R���W�����ǉ��B
			_CharacterController->AddAttributeXZ(Collision_ID::BOSS);	// �{�X�̃R���W�����ǉ��B
		}
		// �ȉ��Փ˂���肽������(�c����)���w��B
		{
			_CharacterController->AttributeY_AllOn();	// �S�ՓˁB
			_CharacterController->SubAttributeY(Collision_ID::ENEMY);	// �G�l�~�[���폜�B
			_CharacterController->SubAttributeY(Collision_ID::BOSS);	// �{�X���폜�B
			_CharacterController->SubAttributeY(Collision_ID::ATTACK);	//�U���R���W�����B


		}
		//�L�����N�^�[�R���g���[���[�̏d�͐ݒ�
		_CharacterController->SetGravity(_Gravity);
	}
	//�v���C���[�̃p�����[�^�[�������B
	_PlayerParam->ParamInit(100, 100, 50, 50, 5, 3, 3, 1);
	// HP�̃o�[��\���B
	{
		vector<BarColor> Colors;
		Colors.push_back(BarColor::Green);
		_HPBar->Create(Colors, _PlayerParam->GetParam(CharacterParameter::MAXHP), _PlayerParam->GetParam(CharacterParameter::HP),true,NULL, Vector3(1110,660,0));
	}
	// MP�̃o�[��\���B
	{
		vector<BarColor> Colors;
		Colors.push_back(BarColor::Blue); //175.0f, 21.9f, 0.0f
		_MPBar->Create(Colors, _PlayerParam->GetParam(CharacterParameter::MAXMP), _PlayerParam->GetParam(CharacterParameter::MP), true, _HPBar->GetTransform(), Vector3(0.0f, 40.0f, 0.0f), Vector2(1.0f, 1.0f));
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
	_AnimationEndTime[(int)AnimationNo::AnimationJump] = -1.0;			//�W�����v
	_AnimationEndTime[(int)AnimationNo::AnimationAttack01] = -1.0f;		//�U��1
	_AnimationEndTime[(int)AnimationNo::AnimationAttack02] = -1.0;		//�U��2
	_AnimationEndTime[(int)AnimationNo::AnimationAttack03] = -1.0;		//�U��3
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
	transform->SetLocalPosition(Vector3(560, 69, -1000));
	//�ړ����x������
	_MoveSpeed = Vector3::zero;
	//�����v���C���[��ԁi�ҋ@�j
	_State = State::Idol;
	//�U���A�j���[�V�����X�e�[�g�̏�����
	_NowAttackAnimNo = AnimationNo::AnimationInvalid;
	_NextAttackAnimNo = AnimationNo::AnimationInvalid;
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
	//���C�t��0�ɂȂ�Ǝ��S����B
	if (_PlayerParam->GetParam(CharacterParameter::HP) <= 0)
	{
		ChangeState(State::Death);
	}
	//HP�o�[�̍X�V
	_HPBar->Update();
	//MP�o�[�̍X�V
	_MPBar->Update();
	//�A�j���[�V�����R���g���[��
	AnimationControl();

	// ���g�����X�t�H�[�����X�V����Ɠ����ŃI�C���[�p����N�H�[�^�j�I�����쐬���鏈�����Ă΂��B
	// ���I�C���[�p���g�p�������ڃN�H�[�^�j�I����G��ꍇ�͂��̏������ĂԂƃI�C���[�p�̒l�ō쐬���ꂽ�N�H�[�^�j�I���ŏ㏑�������B
	// ���s���������̂łƂ肠�����R�����g�A�E�g�B
	{
		////�g�����X�t�H�[���X�V
		//transform->UpdateTransform();
	}
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
				PlayAnimation(_NextAttackAnimNo, 0.1f,1);
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

//�U�����󂯂��Ƃ��B
void Player:: HitAttackCollisionEnter(AttackCollision* hitCollision) 
{
	if (hitCollision->GetMaster() == AttackCollision::CollisionMaster::Enemy && _PlayerParam->GetParam(CharacterParameter::HP) > 0)
	{
		_HPBar->SubValue(_PlayerParam->DamageMass(hitCollision->GetDamage(), _PlayerParam->GetParam(CharacterParameter::DEF)));
		//_PlayerParam->SubParam(CharacterParameter::Param::HP,1);	//�_���[�W���󂯂�(�Ƃ肠�����A���C�t���P�����炷)
		_DamageSE->Play(false);//�_���[�W���󂯂��Ƃ���SE
	}
}