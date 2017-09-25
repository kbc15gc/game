#include"stdafx.h"
#include "Player.h"
#include "fbEngine\_Object\_Component\_3D\SkinModel.h"
#include "fbEngine\_Object\_Component\_3D\Animation.h"
#include <string>
#include <sstream>
#include "GameObject\SplitSpace.h"
#include "GameObject\AttackValue2D.h"
#include "..\History\HistoryManager.h"

namespace
{
	float NormalAnimationSpeed = 1.0f;
	float AttackAnimationSpeed = 1.3f;
	float Oboreru = 1.0f;
}

Player::Player(const char * name) :
	GameObject(name),
	//�L�����N�^�[�R���g���[���[NULL
	_CharacterController(NULL),
	//�d�͐ݒ�
	_Gravity(-55.0f),
	//���݂̃X�e�[�g
	_CurrentState(NULL),
	//����X�e�[�g
	_RunState(this),
	//�A�C�h���X�e�[�g
	_IdolState(this),
	//�U���X�e�[�g
	_AttackState(this),
	//���S�X�e�[�g
	_DeathState(this),
	//�f�o�b�O��
	_Debug(false)
{
	//�o���l�e�[�u�������[�h
	_LoadEXPTable();
}

Player::~Player()
{
	char text[256];
	sprintf(text, "~Player address %x\n", *this);
	OutputDebugString(text);
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
	_Height = 1.3f;
	//���a�ݒ�
	_Radius = 0.1f;
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
	_CharacterController->Init(this, transform, Vector3(0.0f,_Height * 0.5f + _Radius,0.0f), Collision_ID::PLAYER, coll, _Gravity);
	// �ȉ��Փ˂���肽������(������)���w��B
	_CharacterController->AttributeXZ_AllOff();	// �S�Փ˖����B
	_CharacterController->AddAttributeXZ(Collision_ID::GROUND);		// �n�ʃR���W������ǉ��B
	_CharacterController->AddAttributeXZ(Collision_ID::ENEMY);		// �G�̃R���W�����ǉ��B
	_CharacterController->AddAttributeXZ(Collision_ID::BOSS);		// �{�X�̃R���W�����ǉ��B
	_CharacterController->AddAttributeXZ(Collision_ID::BUILDING);	// �����̃R���W�����ǉ��B
	// �ȉ��Փ˂���肽������(�c����)���w��B
	_CharacterController->AttributeY_AllOn();	// �S�ՓˁB
	_CharacterController->SubAttributeY(Collision_ID::ENEMY);	// �G�l�~�[���폜�B
	_CharacterController->SubAttributeY(Collision_ID::BOSS);	// �{�X���폜�B
	_CharacterController->SubAttributeY(Collision_ID::ATTACK);	//�U���R���W�����폜�B
	//�L�����N�^�[�R���g���[���[�̏d�͐ݒ�
	_CharacterController->SetGravity(_Gravity);

	//�v���C���[�̃p�����[�^�[�������B
	_PlayerParam->ParamReset(_ParamTable[0]);
	
	// HP�̃o�[��\���B
	{
		vector<BarColor> Colors;
		Colors.push_back(BarColor::Green);
		_HPBar->Create(Colors, _PlayerParam->GetMaxHP(), _PlayerParam->GetParam(CharacterParameter::HP), true, NULL, Vector3(1110.0f, 660.0f, 0.0f));
	}
	// MP�̃o�[��\���B
	{
		vector<BarColor> Colors;
		Colors.push_back(BarColor::Blue); //175.0f, 21.9f, 0.0f
		_MPBar->Create(Colors, _PlayerParam->GetMaxMP(), _PlayerParam->GetParam(CharacterParameter::MP), true, _HPBar->GetTransform(), Vector3(0.0f, 40.0f, 0.0f), Vector2(1.0f, 1.0f));
	}
	//�_���[�WSE������
	_DamageSE = INSTANCE(GameObjectManager)->AddNew<SoundSource>("DamageSE", 0);
	_DamageSE->Init("Asset/Sound/Damage_01.wav");
	//���x���A�b�v��������
	_LevelUP_SE = INSTANCE(GameObjectManager)->AddNew<SoundSource>("LevelUP", 0);
	_LevelUP_SE->Init("Asset/Sound/levelup.wav");
	_LevelUP_SE->SetVolume(2.0f);
#ifdef _DEBUG

	_outputData = AddComponent<OutputData>();
#endif
	_Equipment = new PlayerEquipment;
}

void Player::Start()
{
	//���f���ɃJ�����ݒ�
	_Model->SetCamera(INSTANCE(GameObjectManager)->mainCamera);
	//���f���Ƀ��C�g�ݒ�
	_Model->SetLight(INSTANCE(GameObjectManager)->mainLight);
	//�A�j���[�V�����̏I�����Ԑݒ�
	_AnimationEndTime[(int)AnimationNo::AnimationIdol] = -1.0f;			//�A�C�h��
	_AnimationEndTime[(int)AnimationNo::AnimationWalk] = -1.0f;			//����
	_AnimationEndTime[(int)AnimationNo::AnimationRun] = 0.68f;			//����
	_AnimationEndTime[(int)AnimationNo::AnimationJump] = -1.0f;			//�W�����v
	_AnimationEndTime[(int)AnimationNo::AnimationAttack01] = -1.0f;		//�U��1
	_AnimationEndTime[(int)AnimationNo::AnimationAttack02] = -1.0f;		//�U��2
	_AnimationEndTime[(int)AnimationNo::AnimationAttack03] = -1.0f;		//�U��3
	_AnimationEndTime[(int)AnimationNo::AnimationAttack04] = -1.0f;		//�U��3
	_AnimationEndTime[(int)AnimationNo::AnimationAttack05] = -1.0f;		//�U��3
	_AnimationEndTime[(int)AnimationNo::AnimationDeath] = -1.0f;		//���S
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
	_StartPos = Vector3(-1056.0f, 69.0f, -1947.0f);
	transform->SetLocalPosition(_StartPos);
	//�ړ����x������
	_MoveSpeed = Vector3::zero;
	//�U���A�j���[�V�����X�e�[�g�̏�����
	_NowAttackAnimNo = AnimationNo::AnimationInvalid;
	_NextAttackAnimNo = AnimationNo::AnimationInvalid;

	//���x���A�b�v���̃X�v���C�g������
	{
		_LevelUpSprite = AddComponent<Sprite>();
		_LevelUpSprite->SetTexture(LOADTEXTURE("levelup.png"));
		_LevelUpSprite->SetEnable(true);
		_LevelUpSprite->SetPivot(Vector2(0.5f, 1.0f));
	}

}

void Player::Update()
{

#ifdef _DEBUG
	
	if (KeyBoardInput->isPressed(DIK_K) && KeyBoardInput->isPush(DIK_1))
	{
		//�������X�g�ɒǉ�.
		INSTANCE(HistoryManager)->AddPossessionChip(ChipID::Fire);
	}
	if (KeyBoardInput->isPressed(DIK_K) && KeyBoardInput->isPush(DIK_2))
	{
		//�������X�g�ɒǉ�.
		INSTANCE(HistoryManager)->AddPossessionChip(ChipID::Iron);
	}
	if (KeyBoardInput->isPressed(DIK_K) && KeyBoardInput->isPush(DIK_3))
	{
		//�������X�g�ɒǉ�.
		INSTANCE(HistoryManager)->AddPossessionChip(ChipID::Oil);
	}
	//�o���l�𑝂₷�B
	if (KeyBoardInput->isPressed(DIK_P) && KeyBoardInput->isPush(DIK_1))
	{
		TakeDrop(100, 0);
	}
#endif // DEBUG


	if (KeyBoardInput->isPush(DIK_L))
	{
		PlayerStopEnable();
	}

	//�J�����g�X�e�[�g��NULL�łȂ� && �X�g�b�v�X�e�[�g����Ȃ��ꍇ�X�V
	if (_CurrentState != nullptr && _State != State::Stop)
	{
		//�X�e�[�g�A�b�v�f�[�g
		_CurrentState->Update();
	}
	if (_HPBar != nullptr)
	{
		//HP�o�[�̍X�V
		_HPBar->Update();
	}
	if (_MPBar != nullptr)
	{
		//MP�o�[�̍X�V
		_MPBar->Update();
	}
	//���x���A�b�v���邩�B
	if (_EXPTable.size() > 0 &&
		_nowEXP >= _EXPTable[_PlayerParam->GetParam(CharacterParameter::LV) - 1])
	{
		_LevelUP();
	}
	//�_���[�W���󂯂鏈���B
	_Damage();
	//�A�j���[�V�����R���g���[��
	AnimationControl();
	// ���g�����X�t�H�[�����X�V����Ɠ����ŃI�C���[�p����N�H�[�^�j�I�����쐬���鏈�����Ă΂��B
	// ���I�C���[�p���g�p�������ڃN�H�[�^�j�I����G��ꍇ�͂��̏������ĂԂƃI�C���[�p�̒l�ō쐬���ꂽ�N�H�[�^�j�I���ŏ㏑�������B
	// ���s���������̂łƂ肠�����R�����g�A�E�g�B
		////�g�����X�t�H�[���X�V
		//transform->UpdateTransform();
}

void Player::ChangeState(State nextstate)
{
	if (_CurrentState != NULL) {
		//���݂̃X�e�[�g�𔲂���Ƃ��̏���
		_CurrentState->Leave();
	}
	switch (nextstate)
	{
		
	case State::Idol:
		//�ҋ@���
		_CurrentState = &_IdolState;
		break;
	case State::Run:
		//������
		_CurrentState = &_RunState;
		break;
	case State::Attack:
		//�U�����
		_CurrentState = &_AttackState;
		break;
	case State::Death:					
		//���S���
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
	//�A�j���[�V�����X�s�[�h�͊�{�P
	_Anim->SetAnimeSpeed(NormalAnimationSpeed);
	//���S�A�j���[�V����
	if (_State == State::Death)
	{
		PlayAnimation(AnimationNo::AnimationDeath, 0.1f, 1);
		return;
	}
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
		else if (_State == State::Idol)
		{
			PlayAnimation(AnimationNo::AnimationIdol, 0.2f);
		}
		//�v���C���[�X�g�b�v�Ȃ�A�C�h���A�j���[�V����
		else if (_State == State::Stop)
		{
			PlayAnimation(AnimationNo::AnimationIdol, 0.2f);
		}
		//�A�^�b�N�A�j���[�V����
		else if (_State == State::Attack)
		{
			//�U���̎��̓X�s�[�h��ύX�B
			_Anim->SetAnimeSpeed(AttackAnimationSpeed);
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
				//Animation::PlayAnimInfo* info = new Animation::PlayAnimInfo((UINT)_NextAttackAnimNo, 0.1f, 0.7f, 1);
				//_Anim->AddAnimationQueue(info);
				//�A�j���[�V�����L���[�ɒǉ��B
				_Anim->AddAnimationQueue(new Animation::PlayAnimInfo((UINT)_NextAttackAnimNo, 0.1f, 0.7f, 1));
				_NowAttackAnimNo = _NextAttackAnimNo;
				_NextAttackAnimNo = AnimationNo::AnimationInvalid;
			}
		}
	}
}

//�U�����󂯂��Ƃ��B
void Player:: HitAttackCollisionEnter(AttackCollision* hitCollision) 
{
	if (hitCollision->GetMaster() == AttackCollision::CollisionMaster::Enemy && _PlayerParam->GetParam(CharacterParameter::HP) > 0)
	{
		int damage;
		if (_Equipment != nullptr&&_Equipment->armor != nullptr)
		damage = _PlayerParam->ReceiveDamageMass(hitCollision->GetDamage(),hitCollision->GetIsMagic(),_Equipment->armor);
		else
		damage = _PlayerParam->ReceiveDamageMass(hitCollision->GetDamage(), hitCollision->GetIsMagic());

		_HPBar->SubValue(damage);
		_DamageSE->Play(false);//�_���[�W���󂯂��Ƃ���SE
		AttackValue2D* attackvalue = INSTANCE(GameObjectManager)->AddNew<AttackValue2D>("AttackValue2D", 5);
		attackvalue->Init(damage, 1.5f, Vector3(0.0f, _Height, 0.0f));
		attackvalue->transform->SetParent(transform);
	}
}

//����B
void Player::Releace()
{
	_CharacterController = nullptr;
	_DamageSE = nullptr;
	_Rotation = nullptr;
	_PlayerParam = nullptr;
	_CurrentState = nullptr;
	_HPBar = nullptr;
	_MPBar = nullptr;
}

//�U�����󂯂��Ƃ�
void Player::_Damage()
{
	//���S�X�e�[�g�ȊO�̎��B
	//���C�t��0�ɂȂ�Ǝ��S����B
	if (_PlayerParam->GetDeathFalg() == true && _State != State::Death)
	{
		ChangeState(State::Death);
	}
	//�C�ɓ���ƃ_���[�W��H�炤�B
	if (transform->GetLocalPosition().y < 48.5f 
		&& _PlayerParam->GetParam(CharacterParameter::HP) > 0 && _Debug == false)
	{
		_HPBar->SubValue(_PlayerParam->ReciveDamage(CharacterParameter::HP, Oboreru * Time::DeltaTime()));
	}
}

//�o���l�e�[�u�������[�h�B
void Player::_LoadEXPTable()
{
	std::vector<std::unique_ptr<ExperiencePointTableInfo>> exptbaleinfo;
	Support::LoadCSVData<ExperiencePointTableInfo>("Asset/Data/PlayerParameter.csv", ExperiencePointTableInfoData, ARRAYSIZE(ExperiencePointTableInfoData), exptbaleinfo);
	
	for (int i = 0; i < MAXLV; i++)
	{
		_EXPTable.push_back(exptbaleinfo[i]->ExperiencePoint);

		// �e�X�g�B
		for (int idx = 0; idx < CharacterParameter::Param::MAX; idx++) {
			_ParamTable[i][idx] = exptbaleinfo[i]->param[idx];
		}

		//{
		//	_ParamTable[i][CharacterParameter::Param::MAXHP] = exptbaleinfo[i]->HP;
		//	_ParamTable[i][CharacterParameter::Param::HP] = exptbaleinfo[i]->HP;
		//	_ParamTable[i][CharacterParameter::Param::MAXMP] = exptbaleinfo[i]->MP;
		//	_ParamTable[i][CharacterParameter::Param::MP] = exptbaleinfo[i]->MP;
		//	_ParamTable[i][CharacterParameter::Param::ATK] = exptbaleinfo[i]->ATK;
		//	_ParamTable[i][CharacterParameter::Param::MAT] = exptbaleinfo[i]->MAT;
		//	_ParamTable[i][CharacterParameter::Param::DEF] = exptbaleinfo[i]->DEF;
		//	_ParamTable[i][CharacterParameter::Param::MDE] = exptbaleinfo[i]->MDE;
		//	_ParamTable[i][CharacterParameter::Param::DEX] = exptbaleinfo[i]->DEX;
		//	_ParamTable[i][CharacterParameter::Param::CRT] = exptbaleinfo[i]->CRT;
		//	_ParamTable[i][CharacterParameter::Param::LV] = i + 1; 
		//}
	}
}

void Player::_LevelUP()
{
	// ���݂̌o���l���Z�b�g�B
	_nowEXP = _nowEXP - _EXPTable[_PlayerParam->GetParam(CharacterParameter::Param::LV) - 1];	// ���x���A�b�v���Ɉ�ꂽ�������݂̌o���l�ɐݒ�B

	// ���̃��x���̃p�����[�^��ݒ�B
	_PlayerParam->ParamReset(_ParamTable[_PlayerParam->GetParam(CharacterParameter::Param::LV)]);

	//HP���オ�����̂�HP�o�[��HP�ݒ肵�Ȃ����B
	_HPBar->Reset(_PlayerParam->GetParam(CharacterParameter::HP), _PlayerParam->GetParam(CharacterParameter::HP));
	//MP���オ�����̂�MP�o�[��MP�ݒ肵�Ȃ����B
	_MPBar->Reset(_PlayerParam->GetParam(CharacterParameter::MP), _PlayerParam->GetParam(CharacterParameter::MP));
	//���x���A�b�v���̉��Đ��B
	_LevelUP_SE->Play(false);
}