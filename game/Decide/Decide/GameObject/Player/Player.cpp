#include"stdafx.h"
#include "Player.h"
#include "fbEngine\_Object\_Component\_3D\SkinModel.h"
#include "fbEngine\_Object\_Component\_3D\Animation.h"
#include "GameObject\SplitSpace.h"
#include "GameObject\TextImage\AttackValue2D.h"
#include "..\History\HistoryManager.h"
#include "GameObject\Component\ParticleEffect.h"
#include "GameObject\Component\BuffDebuffICon.h"
#include "GameObject\ItemManager\DropItem\DropItem.h"
#include "GameObject\Enemy\EnemyCharacter.h"

#include"../UI/PlayerParameterUI.h"
#include "GameObject\WorldMap\WorldMap.h"

namespace
{
	float SlowAnimationSpeed = 0.7f;
	float NormalAnimationSpeed = 1.0f;
	float AttackAnimationSpeed = 1.3f;
	float Oboreru = 1.0f;
}

Player::Player(const char * name) :
	GameObject(name),
	//�L�����N�^�[�R���g���[���[NULL
	_CharacterController(NULL),
	//�d�͐ݒ�
	_Gravity(-0.98f),
	//���݂̃X�e�[�g
	_CurrentState(NULL),
	//����X�e�[�g
	_RunState(this),
	//�A�C�h���X�e�[�g
	_IdolState(this),
	//�U���X�e�[�g
	_AttackState(this),
	//�_���[�W���󂯂�X�e�[�g
	_ImpactState(this),
	//���S�X�e�[�g
	_DeathState(this),
	//�X�g�b�v�X�e�[�g
	_StopState(this),
	//�f�o�b�O��
	_Debug(false),
	//�W�����v���Ă��邩
	_NoJump(false),
	//�b���Ă��邩
	_IsSpeak(false)
{
	//�o���l�e�[�u�������[�h
	_LoadEXPTable();
}

Player::~Player()
{
	/*char text[256];
	sprintf(text, "~Player address %x\n", *this);
	OutputDebugString(text);*/
	//for (auto& village : _NPC)
	//{
	//	for (auto& npc : village)
	//	{
	//		npc = nullptr;
	//	}
	//}
	//_NPC.clear();
}

void Player::Awake()
{
	
	_NearNPCLen = FLT_MAX;

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
	//_HPBar = AddComponent<ParameterBar>();
	// MP�o�[�B
	//_MPBar = AddComponent<ParameterBar>();
	//�����ݒ�
	_Height = /*1.3f*/0.3f;
	//���a�ݒ�
	_Radius = 0.6f;
	//�J�v�Z���R���C�_�[�쐬
	coll->Create(_Radius, _Height);

	//�X�L�����f���쐬
	SkinModelData* modeldata = new SkinModelData();
	//���f���f�[�^�쐬
	modeldata->CloneModelData(SkinModelManager::LoadModel("Player.X"), _Anim);
	//���f���ݒ�
	_Model->SetModelData(modeldata);
	_Model->SetModelEffect(ModelEffectE::SPECULAR, true);
	_Model->SetModelEffect(ModelEffectE::RECEIVE_SHADOW, true);
	_Model->SetModelEffect(ModelEffectE::LIMLIGHT, true);
	_Model->SetModelEffect(ModelEffectE::FRUSTUM_CULLING, false);
	_Model->SetModelEffect(ModelEffectE::DITHERING, true);
	//_Model->SetAllBlend(Color::white * 13);
	//_Model->SetLuminanceColor(Color(1.0f, 0.0f, 0.0f, 1.0f));

	_Model->SetAtomosphereFunc(AtmosphereFunc::enAtomosphereFuncObjectFromAtomosphere);

	//�A�j���[�V�����C�x���g�ǉ�
	_AnimationEventPlayer = AddComponent<AnimationEventPlayer>();
	_AnimationEventPlayer->Init((int)AnimationNo::AnimationNum);
	AnimationEventControl();

	//�L�����N�^�[�R���g���[���[������
	_CharacterController->Init(Vector3(0.0f,_Height * 0.5f + _Radius,0.0f), Collision_ID::PLAYER, coll, _Gravity);

	// �ȉ��Փ˂���肽������(������)���w��B
	_CharacterController->AttributeXZ_AllOff();	// �S�Փ˖����B
	_CharacterController->AddAttributeXZ(Collision_ID::GROUND);		// �n�ʃR���W������ǉ��B
	_CharacterController->AddAttributeXZ(Collision_ID::ENEMY);		// �G�̃R���W�����ǉ��B
	_CharacterController->AddAttributeXZ(Collision_ID::BOSS);		// �G�̃R���W�����ǉ��B
	_CharacterController->AddAttributeXZ(Collision_ID::BUILDING);	// �����̃R���W�����ǉ��B
	_CharacterController->AddAttributeXZ(Collision_ID::NPC_Collision);		// �����̃R���W�����ǉ��B
	
	// �ȉ��Փ˂���肽������(�c����)���w��B
	_CharacterController->AttributeY_AllOn();	// �S�ՓˁB
	_CharacterController->SubAttributeY(Collision_ID::ENEMY);	// �G�l�~�[���폜�B
	_CharacterController->SubAttributeY(Collision_ID::BOSS);	// �G�l�~�[���폜�B
	_CharacterController->SubAttributeY(Collision_ID::DRARIAN);	// �G�l�~�[���폜�B
	_CharacterController->SubAttributeY(Collision_ID::ATTACK);	//�U���R���W�����폜�B
	_CharacterController->SubAttributeY(Collision_ID::DROPITEM);//�h���b�v�A�C�e���R���W�������폜�B
	_CharacterController->SubAttributeY(Collision_ID::ITEMRANGE);//�A�C�e���R���W�������폜�B
	_CharacterController->SubAttributeY(Collision_ID::SPACE);	//��ԃR���W�������폜�B
	_CharacterController->SubAttributeY(Collision_ID::NPC_Collision);		//NPC���폜�B

	//�L�����N�^�[�R���g���[���[�̏d�͐ݒ�
	_CharacterController->SetGravity(_Gravity);

	//�v���C���[�̃p�����[�^�[�������B
	int lv = 0;


	//int lv = 0;

	// �e�X�g�B
	//int lv = 30;

	if (IS_CONTINUE)
	{
		JsonData PlayerData;
		if (PlayerData.Load("Player"))
		{
			picojson::object player = PlayerData.GetDataObject("Player");
			lv = player["Level"].get<double>() - 1;
			_nowEXP = player["EXP"].get<double>();
		}
	}

	_PlayerParam->ParamReset(_ParamTable[lv]);

	if (!IS_CONTINUE)
	{
		SaveLevel();
	}

	// HP�̃o�[��\���B
	{
		vector<BarColor> Colors;
		Colors.push_back(BarColor::Green);
		//_HPBar->Create(Colors, static_cast<float>(_PlayerParam->GetMaxHP()), static_cast<float>(_PlayerParam->GetParam(CharacterParameter::HP)),true, true, NULL);
	}
	// MP�̃o�[��\���B
	{
		//vector<BarColor> Colors;
		//Colors.push_back(BarColor::Blue); //175.0f, 21.9f, 0.0f
		//_MPBar->Create(Colors, static_cast<float>(_PlayerParam->GetMaxMP()), static_cast<float>(_PlayerParam->GetParam(CharacterParameter::MP)), true, true, _HPBar->GetTransform(), Vector3(0.0f, 40.0f, 0.0f), Vector2(1.0f, 1.0f));
	}
	//�_���[�WSE������
	_DamageSound = INSTANCE(GameObjectManager)->AddNew<SoundSource>("DamageSound", 0);
	_DamageSound->Init("Asset/Sound/Damage_01.wav");
	//���x���A�b�v��������
	_LevelUpSound = INSTANCE(GameObjectManager)->AddNew<SoundSource>("LevelUpSound", 0);
	_LevelUpSound->Init("Asset/Sound/Player/lvup.wav");
	_LevelUpSound->SetVolume(0.3f);
	//���S�{�C�X������
	_DeathSound = INSTANCE(GameObjectManager)->AddNew<SoundSource>("DeathSound", 0);
	_DeathSound->Init("Asset/Sound/Player/death.wav");
	_DeathSound->SetVolume(0.3f);
	//��SE������
	_HeelSound = INSTANCE(GameObjectManager)->AddNew<SoundSource>("HeelSound", 0);
	_HeelSound->Init("Asset/Sound/Player/heal02.wav");
	_HeelSound->SetVolume(2.0f);
	//�X�e�[�^�X�A�b�v�T�E���h������
	_StatusUpSound = INSTANCE(GameObjectManager)->AddNew<SoundSource>("StatusUpSound", 0);
	_StatusUpSound->Init("Asset/Sound/Player/statusup.wav");
	_StatusUpSound->SetVolume(1.0f);
	//�X�e�[�^�X�_�E���T�E���h������
	_StatusDownSound = INSTANCE(GameObjectManager)->AddNew<SoundSource>("StatusDownSound", 0);
	_StatusDownSound->Init("Asset/Sound/Player/statusdown.wav");
	_StatusDownSound->SetVolume(1.0f);
	//�U���T�E���h������
	_AttackSoound = INSTANCE(GameObjectManager)->AddNew<SoundSource>("SE", 0);
	_AttackSoound->Init("Asset/Sound/Player/PlayerAttack_00.wav");
	//�����T�E���h������
	_AsiotoSound = INSTANCE(GameObjectManager)->AddNew<SoundSource>("AsiotoSE", 0);
	_AsiotoSound->Init("Asset/Sound/Player/asioto.wav");
	_AsiotoSound->SetVolume(0.5f);
	//���n�T�E���h������
	_TyakutiSound = INSTANCE(GameObjectManager)->AddNew<SoundSource>("TyakutiSE", 0);
	_TyakutiSound->Init("Asset/Sound/Player/tyakuti.wav");
	_TyakutiSound->SetVolume(0.8f);
	//�U���{�C�X������
	_AttackBoiceSound[static_cast<int>(AttackBoice::Attack1)] = INSTANCE(GameObjectManager)->AddNew<SoundSource>("Attack1", 0);
	_AttackBoiceSound[static_cast<int>(AttackBoice::Attack2)] = INSTANCE(GameObjectManager)->AddNew<SoundSource>("Attack2", 0);
	_AttackBoiceSound[static_cast<int>(AttackBoice::Attack3)] = INSTANCE(GameObjectManager)->AddNew<SoundSource>("Attack3", 0);
	_AttackBoiceSound[static_cast<int>(AttackBoice::Attack1)]->Init("Asset/Sound/Player/attack1.wav");
	_AttackBoiceSound[static_cast<int>(AttackBoice::Attack2)]->Init("Asset/Sound/Player/attack2.wav");
	_AttackBoiceSound[static_cast<int>(AttackBoice::Attack3)]->Init("Asset/Sound/Player/attack3.wav");
#ifdef _DEBUG
	_outputData = AddComponent<OutputData>();
#endif
	//�v���C���[�̑���
	_Equipment = new PlayerEquipment;

	//�p�[�e�B�N���G�t�F�N�g�B
	_ParticleEffect = AddComponent<ParticleEffect>();
	
	_PlayerParameterUI = INSTANCE(GameObjectManager)->AddNew<PlayerParameterUI>("PlayerParameterUI", 9);

	//�o�t�f�o�t�A�C�R���B
	_BuffDebuffICon = AddComponent<BuffDebuffICon>();
	_BuffDebuffICon->SetHpBarTransform(_PlayerParameterUI->transform);

	_Model->SetCharacterLight(&_CharaLight);

	_CharaLight.SetDiffuseLightDirection(0, Vector3(1.0f, 0.0f, 0.0f));
	_CharaLight.SetDiffuseLightDirection(1, Vector3(0.0f, 0.0f, 0.0f));
	_CharaLight.SetDiffuseLightDirection(2, Vector3(0.0f, 0.0f, 0.0f));
	_CharaLight.SetDiffuseLightDirection(3, Vector3(0.0f, 0.0f, 0.0f));
	
	_CharaLight.SetDiffuseLightColor(0, Vector4(0.5f, 0.5f, 0.5f, 30.0f));
	_CharaLight.SetDiffuseLightColor(1, Vector4(0.0f, 0.0f, 0.0f, 1.0f));
	_CharaLight.SetDiffuseLightColor(2, Vector4(0.0f, 0.0f, 0.0f, 1.0f));
	_CharaLight.SetDiffuseLightColor(3, Vector4(0.0f, 0.0f, 0.0f, 1.0f));
	
	_CharaLight.SetAmbientLight(Vector4(0.1f, 0.1f, 0.1f, 1.0f));

	_BloodEffect = INSTANCE(GameObjectManager)->AddNew<BloodEffect>("BloodEffect", 0);

	if (IS_CONTINUE)
	{
		JsonData PlayerData;
		if (PlayerData.Load("Player_Pos"))
		{
			picojson::object player = PlayerData.GetDataObject("Player");
			_RespawnPos.x = player["RespawnPos_X"].get<double>();
			_RespawnPos.y = player["RespawnPos_Y"].get<double>();
			_RespawnPos.z = player["RespawnPos_Z"].get<double>();
			_RespawnRot.x = player["RespawnRot_X"].get<double>();
			_RespawnRot.y = player["RespawnRot_Y"].get<double>();
			_RespawnRot.z = player["RespawnRot_Z"].get<double>();
			_RespawnRot.w = player["RespawnRot_W"].get<double>();
		}
	}
	else
	{
		SetRespawnPos(Vector3(-198.475342,57.9678802,-144.642334),Quaternion(-0.000000000, -0.999639690, -0.000000000, 0.0268424246));
	}

	//�|�W�V����
	transform->SetLocalPosition(_RespawnPos);
	transform->SetLocalRotation(_RespawnRot);

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
	_AnimationEndTime[(int)AnimationNo::AnimationAttack04] = -1.0f;		//�U��4
	_AnimationEndTime[(int)AnimationNo::AnimationAttack05] = -1.0f;		//�U��5
	_AnimationEndTime[(int)AnimationNo::AnimationImpact] = 0.6f;		//�_���[�W	
	_AnimationEndTime[(int)AnimationNo::AnimationDeath] = -1.0f;		//���S
	//�e�G���h�^�C����ݒ�
	for (int i = 0; i < (int)AnimationNo::AnimationNum; i++)
	{
		_Anim->SetAnimationEndTime(i, _AnimationEndTime[i]);
	}
	//�����A�j���[�V����
	PlayAnimation(AnimationNo::AnimationWalk, 0.2f);
	//�����X�e�[�g�ݒ�
	ChangeState(State::Idol);

	//if (IS_CONTINUE)
	//{
	//	JsonData PlayerData;
	//	if (PlayerData.Load("Player_Pos"))
	//	{
	//		picojson::object player = PlayerData.GetDataObject("Player");
	//		_RespawnPos.x = player["RespawnPos_X"].get<double>();
	//		_RespawnPos.y = player["RespawnPos_Y"].get<double>();
	//		_RespawnPos.z = player["RespawnPos_Z"].get<double>();
	//	}
	//}
	//else
	//{
	//	SetRespawnPos(Vector3(-202.0f, 58.0f, -156.0f));
	//}

	////�|�W�V����
	//transform->SetLocalPosition(_RespawnPos);
	//�ړ����x������
	_MoveSpeed = Vector3::zero;
	//�U���A�j���[�V�����X�e�[�g�̏�����
	_NowAttackAnimNo = AnimationNo::AnimationInvalid;
	_NextAttackAnimNo = AnimationNo::AnimationInvalid;

	//���x���A�b�v�C���[�W������
	_LevelUpImage = INSTANCE(GameObjectManager)->AddNew<LevelUpImage>("LevelUP", 9);

	//�Q�[���J�n���ɃC���x���g�����瑕�����Ă��镐���T�������������B
	Re_SetEquipment();

	//// �����ʒu�Ɉړ��B
	//_CharacterController->Execute();

	_AttentionText = static_cast<AttentionTextOnly*>(INSTANCE(GameObjectManager)->FindObject("AttentionTextOnly"));
}

void Player::Update()
{
	//�J�����g�X�e�[�g��NULL�łȂ� && �X�g�b�v�X�e�[�g����Ȃ��ꍇ�X�V
	if (_CurrentState != nullptr && _State != State::Stop)
	{
		//�X�e�[�g�A�b�v�f�[�g
		_CurrentState->Update();
	}

	//if (_HPBar != nullptr)
	//{
	//	//HP�o�[�̍X�V
	//	_HPBar->Update();
	//}

	//if (_MPBar != nullptr)
	//{
	//	//MP�o�[�̍X�V
	//	_MPBar->Update();
	//}

	if (_PlayerParam)
	{	
		//���x���A�b�v���邩�B
		if (_EXPTable.size() > 0 &&
			_nowEXP >= _EXPTable[_PlayerParam->GetParam(CharacterParameter::LV) - 1]
			&& _PlayerParam->GetParam(CharacterParameter::LV) < 100)
		{
			_LevelUP();
		}
		//�_���[�W���󂯂鏈���B
		_Damage();
		//�G�t�F�N�g
		EffectUpdate();

		//@todo for debug
		//@todo for releasedebug
#if defined(_DEBUG) || defined(RELEASEDEBUG)
		_DebugPlayer();
#endif // _DEBUG // RELEASEDEBUG
	}
	
	//�A�j���[�V�����R���g���[��
	AnimationControl();
	
	//NPC�Ƙb��
	if (_State != State::Death)
	{
		Speak();
	}
	
	//���n
	_Tyakuti();
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
	case State::Impact:
		//�_���[�W���󂯂����
		_CurrentState = &_ImpactState;
		break;
	case State::Death:					
		//���S���
		_CurrentState = &_DeathState;
		break;
	case State::Stop:
		//�X�g�b�v���
		_CurrentState = &_StopState;
		break;
	default:
		//�f�t�H���g
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
	if (!_CharacterController)
	{
		return;
	}
	//�A�j���[�V�����X�s�[�h�͊�{�P
	_Anim->SetAnimeSpeed(NormalAnimationSpeed);
	//���S�A�j���[�V����
	if (_State == State::Death)
	{
		_Anim->PopAnimationQueue();
		PlayAnimation(AnimationNo::AnimationDeath, 0.0f, 0);
		return;
	}
	//�W�����v�A�j���[�V����
	//�X�g�b�v����Ȃ��Ȃ�W�����v����B
	if (_CharacterController->IsJump() && _State != State::Stop && !_NoJump)
	{
		PlayAnimation(AnimationNo::AnimationJump, 0.1f, 0);
	}
	else
	{
		//����A�j���[�V����
		if (_State == State::Run)
		{
			PlayAnimation(AnimationNo::AnimationRun, 0.1f);
		}
		//�A�C�h���A�j���[�V����
		else if (_State == State::Idol)
		{
			PlayAnimation(AnimationNo::AnimationIdol, 0.1f);
		}
		//�v���C���[�X�g�b�v�Ȃ�A�C�h���A�j���[�V����
		else if (_State == State::Stop)
		{
			PlayAnimation(AnimationNo::AnimationIdol, 0.2f);
		}
		//�_���[�W���󂯂��A�j���[�V����
		else if (_State == State::Impact)
		{
			_Anim->PopAnimationQueue();
			PlayAnimation(AnimationNo::AnimationImpact, 0.0f,0);
		}
		//�A�^�b�N�A�j���[�V����
		else if (_State == State::Attack)
		{
			//�U���̎��̓X�s�[�h��ύX�B
			_Anim->SetAnimeSpeed(AttackAnimationSpeed);
			if (_NextAttackAnimNo == AnimationNo::AnimationAttackStart)
			{
				//�U���J�n
				PlayAnimation(_NextAttackAnimNo, 0.0f, 0);
				_NowAttackAnimNo = _NextAttackAnimNo;
				_NextAttackAnimNo = AnimationNo::AnimationInvalid;
			}
			else if (_NextAttackAnimNo != AnimationNo::AnimationInvalid)
			{
				//�A��
				//Animation::PlayAnimInfo* info = new Animation::PlayAnimInfo((UINT)_NextAttackAnimNo, 0.1f, 0.7f, 1);
				//_Anim->AddAnimationQueue(info);
				//�A�j���[�V�����L���[�ɒǉ��B
				_Anim->AddAnimationQueue(new Animation::PlayAnimInfo((UINT)_NextAttackAnimNo, 0.0f, 0.7f, 0));
				_NowAttackAnimNo = _NextAttackAnimNo;
				_NextAttackAnimNo = AnimationNo::AnimationInvalid;
			}
		}
	}
}

//�U�����󂯂��Ƃ��B
void Player:: HitAttackCollisionEnter(AttackCollision* hitCollision) 
{
	if (_PlayerParam == nullptr) {
		return;
	}
	if (hitCollision->GetMaster() == AttackCollision::CollisionMaster::Enemy && _PlayerParam->GetParam(CharacterParameter::HP) > 0)
	{
#ifdef _DEBUG
		if (_Equipment == nullptr)
		{
			// �����p�̍\���̂�Null�B
			abort();
		}
#endif
		//�_���[�W���󂯂���ԂɕύX
		if (_State != State::Death && _State != State::Stop && _State != State::Impact && hitCollision->GetReactionType() == AttackCollision::ReactionType::Leans)
		{
			ChangeState(State::Impact);
		}

		// �_���[�W��^���鏈��
		int damage = _PlayerParam->ReciveDamage(*hitCollision->GetDamageInfo(), _Equipment->armor);
		//_HPBar->SubValue(static_cast<float>(damage));
		_DamageSound->Play(false);//�_���[�W���󂯂��Ƃ���SE
		AttackValue2D* attackvalue = INSTANCE(GameObjectManager)->AddNew<AttackValue2D>("AttackValue2D", 5);
		Color c;
		if (damage == 0)
		{
			//�D�F��
			c = Color::white * 0.3f;
		}
		else
		{
			//���F
			c = { 0.0f,0.6f,1.0f,1.0f };

			//_BloodEffect->Damage(_PlayerParam->GetMaxHP(), damage);
		}
		//�_���[�W�ʂ�\������B
		attackvalue->Init(transform, damage, hitCollision->GetDamageInfo()->isCritical, 1.5f, Vector3(0.0f, _Height, 0.0f),c);
		attackvalue->transform->SetParent(transform);
	}
}

//����B
void Player::Releace()
{
	_CharacterController = nullptr;
	_DamageSound = nullptr;
	_Rotation = nullptr;
	_PlayerParam = nullptr;
	_CurrentState = nullptr;
	//_HPBar = nullptr;
	//_MPBar = nullptr;
	for (auto& p : _AttackBoiceSound)
	{
		p = nullptr;
	}
	_DeathSound = nullptr;
	//_AnimationEventPlayer = nullptr;
}

void Player::TakeDrop(int dropexp, int money)
{
	_nowEXP += dropexp;
	SaveLevel();
	// �����̓C���x���g���Ɋi�[�B
	INSTANCE(Inventory)->AddPlayerMoney(money);

	wchar_t WText[256];
	char	CText[256];

	sprintf(CText, "+%d�o���l", dropexp);
	//chatr����wchra_t�ɕϊ��B
	mbstowcs(WText, CText, sizeof(CText));
	_AttentionText->CreateText(
		WText,
		Vector3(580.0f, 460.0f, 0.0f),
		33.0f,
		Color::white,
		AttentionTextOnly::MoveType::Up
	);

	sprintf(CText, "+%d�_���[", money);
	//chatr����wchra_t�ɕϊ��B
	mbstowcs(WText, CText, sizeof(CText));
	_AttentionText->CreateText(
		WText,
		Vector3(580.0f, 460.0f, 0.0f),
		33.0f,
		Color::white,
		AttentionTextOnly::MoveType::Up
	);

}

/**
* �A�C�e�����g�p���ꂽ.
*/
bool Player::ItemEffect(Item::ItemInfo* info)
{
	//�߂�l.
	bool returnValue = false;

	//HP�񕜏���.
	if (_PlayerParam->HeelHP(info->effectValue[CharacterParameter::Param::HP]))
	{
		if (_ParticleEffect)
		{
			_ParticleEffect->HeelHpEffect();
		}
		/*if (_HPBar) {
			_HPBar->SetValue(static_cast<float>(_PlayerParam->GetParam(CharacterParameter::Param::HP)));
		}*/

		_HeelSound->Play(false);
		
		returnValue = true;
	}
	if (_PlayerParam->HeelMP(info->effectValue[CharacterParameter::Param::MP]))
	{
		if (_ParticleEffect)
		{
			_ParticleEffect->HeelMpEffect();
		}
		/*if (_MPBar) {
			_MPBar->SetValue(static_cast<float>(_PlayerParam->GetParam(CharacterParameter::Param::MP)));
		}*/
		
		_HeelSound->Play(false);
		
		returnValue = true;
	}

	returnValue = BuffAndDebuff(info->effectValue,info->time);
//	for (int idx = static_cast<int>(CharacterParameter::Param::ATK); idx < CharacterParameter::MAX; idx++) {
//		int value = info->effectValue[idx];
//		if (value > 0) {
//			// �o�t�B
//			if (_ParticleEffect) {
//				_ParticleEffect->BuffEffect();
//			}
//#ifdef  _DEBUG
//			else {
//				// �G�t�F�N�g�R���|�[�l���g�Ȃ���B
//				abort();
//			}
//#endif //  _DEBUG
//
//			_PlayerParam->Buff(static_cast<CharacterParameter::Param>(idx), static_cast<unsigned short>(value), info->time);
//			_BuffDebuffICon->SetHpBarTransform(_HPBar->GetTransform());
//			_BuffDebuffICon->BuffIconCreate(static_cast<CharacterParameter::Param>(idx));
//
//			_StatusUpSound->Play(false);
//
//			returnValue = true;
//		}
//		else if (value < 0) {
//			// �f�o�t(�f�����b�g)�B
//			if (_ParticleEffect) {
//				_ParticleEffect->DeBuffEffect();
//			}
//#ifdef  _DEBUG
//			else {
//				// �G�t�F�N�g�R���|�[�l���g�Ȃ���B
//				abort();
//			}
//#endif //  _DEBUG
//			_PlayerParam->Debuff(static_cast<CharacterParameter::Param>(idx), static_cast<unsigned short>(abs(value)), info->time);
//			_BuffDebuffICon->SetHpBarTransform(_HPBar->GetTransform());
//			_BuffDebuffICon->DebuffIconCreate(static_cast<CharacterParameter::Param>(idx));
//
//			_StatusDownSound->Play(false);
//
//			returnValue = true;
//		}
//	}
	return returnValue;
}

bool Player::BuffAndDebuff(int effectValue[CharacterParameter::Param::MAX], float time) {
	bool ret = false;
	for (int idx = static_cast<int>(CharacterParameter::Param::ATK); idx < CharacterParameter::MAX; idx++) {
		//���@�U���𖳎��B
		if (idx == CharacterParameter::Param::MAT)
			continue;
		int value = effectValue[idx];
		if (value > 0) {
			// �o�t�B
			if (_ParticleEffect) {
				_ParticleEffect->BuffEffect();
			}
#ifdef  _DEBUG
			else {
				// �G�t�F�N�g�R���|�[�l���g�Ȃ���B
				abort();
			}
#endif //  _DEBUG

			_PlayerParam->Buff(static_cast<CharacterParameter::Param>(idx), value, time);
			_BuffDebuffICon->SetHpBarTransform(_PlayerParameterUI->transform);
			_BuffDebuffICon->BuffIconCreate(static_cast<CharacterParameter::Param>(idx));

			_StatusUpSound->Play(false);

			ret = true;
		}
		else if (value < 0) {
			// �f�o�t(�f�����b�g)�B
			if (_ParticleEffect) {
				_ParticleEffect->DeBuffEffect();
			}
#ifdef  _DEBUG
			else {
				// �G�t�F�N�g�R���|�[�l���g�Ȃ���B
				abort();
			}
#endif //  _DEBUG
			_PlayerParam->Debuff(static_cast<CharacterParameter::Param>(idx),abs(value), time);
			_BuffDebuffICon->SetHpBarTransform(_PlayerParameterUI->transform);
			_BuffDebuffICon->DebuffIconCreate(static_cast<CharacterParameter::Param>(idx));

			_StatusDownSound->Play(false);

			ret = true;
		}
	}
	return ret;
}

/**
* �G�t�F�N�g�p�X�V.
*/
void Player::EffectUpdate()
{
	bool isBuffEffect = false;
	bool isDeBuffEffect = false;
	for (int idx = static_cast<int>(CharacterParameter::Param::ATK); idx <= CharacterParameter::Param::DEX; idx++) {

		if (_PlayerParam->GetBuffParam_Percentage((CharacterParameter::Param)idx) > 0)
		{
			isBuffEffect = true;
		}
		else
		{
			_BuffDebuffICon->DeleteBuffIcon((CharacterParameter::Param)idx);
		}

		if (_PlayerParam->GetDebuffParam_Percentage((CharacterParameter::Param)idx) > 0)
		{
			isDeBuffEffect = true;
		}
		else
		{
			_BuffDebuffICon->DeleteDebuffIcon((CharacterParameter::Param)idx);
		}
	}

	_ParticleEffect->SetBuffEffectFlag(isBuffEffect);
	_ParticleEffect->SetDebuffEffectFlag(isDeBuffEffect);
}

//�U�����󂯂��Ƃ�
void Player::_Damage()
{
	//���S�X�e�[�g�ȊO�̎��B
	//���C�t��0�ɂȂ�Ǝ��S����B

	if (_PlayerParam->GetDeathFlg() == true && _State != State::Death)
	{
		_DeathSound->Play(false);
		ChangeState(State::Death);
		return;
	}

	//�C�ɓ���ƃ_���[�W��H�炤�B
	//�C�̒��̏ꍇ�B
	//HP��0�ȏ�Ȃ�B
	//�f�o�b�O���łȂ��B
	//1�b�Ԋu�ŁB

	if (transform->GetLocalPosition().y < 48.5f && _PlayerParam->GetParam(CharacterParameter::HP) > 0 && _Debug == false)
	{
		static float time = 0.0f;
		time += Time::DeltaTime();
		//if (fmod(time, 1.0f) >= 1.0f)
		if(time >= 0.5f)
		{
			//�ő�HP��1��������B
			_PlayerParam->ReciveDamageThrough(static_cast<int>(_PlayerParam->GetMaxHP() * 0.1f));
			//_HPBar->SubValue(static_cast<float>((_PlayerParam->ReciveDamageThrough(static_cast<int>(_PlayerParam->GetMaxHP() * 0.1f)))));
			time = 0.0f;
		}
	}

}

//�o���l�e�[�u�������[�h�B
void Player::_LoadEXPTable()
{
	std::vector<std::unique_ptr<ExperiencePointTableInfo>> exptbaleinfo;
	Support::LoadCSVData<ExperiencePointTableInfo>("Asset/Data/PlayerParameter.csv", ExperiencePointTableInfoData, ARRAYSIZE(ExperiencePointTableInfoData), exptbaleinfo);
	
	_EXPTable.clear();

	for (int i = 0; i < MAXLV; i++)
	{
		_EXPTable.push_back(exptbaleinfo[i]->ExperiencePoint);

		for (int idx = 0; idx < CharacterParameter::Param::MAX; idx++) {
			_ParamTable[i][idx] = exptbaleinfo[i]->param[idx];
		}
	}
}

void Player::_LevelUP()
{
	// ���݂̌o���l���Z�b�g�B
	_nowEXP = _nowEXP - _EXPTable[_PlayerParam->GetParam(CharacterParameter::Param::LV) - 1];	// ���x���A�b�v���Ɉ�ꂽ�������݂̌o���l�ɐݒ�B

	// ���̃��x���̃p�����[�^��ݒ�B
	_PlayerParam->ParamReset(_ParamTable[_PlayerParam->GetParam(CharacterParameter::Param::LV)]);

	//HP���オ�����̂�HP�o�[��HP�ݒ肵�Ȃ����B
	//_HPBar->Reset(static_cast<float>(_PlayerParam->GetParam(CharacterParameter::HP)), static_cast<float>(_PlayerParam->GetParam(CharacterParameter::HP)),true);
	//MP���オ�����̂�MP�o�[��MP�ݒ肵�Ȃ����B
	//_MPBar->Reset(static_cast<float>(_PlayerParam->GetParam(CharacterParameter::MP)), static_cast<float>(_PlayerParam->GetParam(CharacterParameter::MP)),true);
	//���x���A�b�v���̃C���[�W�\���B
	_LevelUpImage->Init();
	//���x���A�b�v���̉��Đ��B
	_LevelUpSound->Play(false);
	//���x���A�b�v�G�t�F�N�g
	/*_ParticleEffect->LevelUpEffect();
	_ParticleEffect->SetLevelUPEffectFlag(true);*/

	SaveLevel();
}

void Player::Speak()
{
	//�V���b�v�C�x���g�t���O�擾�B
	bool eventflag = INSTANCE(EventManager)->IsEvent();
	//�߂���npc������ꍇ
	if (_NearNPC)
	{
		_NearNPCLen = (_NearNPC->transform->GetPosition() - transform->GetPosition()).Length();
		float lenLimit = (strcmp(_NearNPC->GetName(), "LastBoss.X") == 0) ? 15.0f : 3.0f;
		//��b�\
		if (_NearNPCLen <= lenLimit && !eventflag)
		{
			//�n�ʂɂ��Ă���Θb���������
			//�V���b�v�C�x���g�łȂ��Ƃ��B
			if (_CharacterController->IsOnGround())
			{
				if (_NearNPC->GetActive()) {
					//��b�̂���HP�o�[�Ȃǂ������B
					//_HPBar->RenderDisable();
					//_MPBar->RenderDisable();
					_PlayerParameterUI->SetActive(false, true);
					//�b���t���O�Z�b�g
					_NearNPC->SetIsSpeak(true);
					//�v���C���[�b���t���O�ݒ�
					//�W�����v���Ȃ��Ȃ�
					_NoJump = true;
				}
				else {
					//��b�̂���HP�o�[�Ȃǂ������B
					//_HPBar->RenderEnable();
					_PlayerParameterUI->SetActive(true, true);
					//�b���t���O�Z�b�g
					_NearNPC->SetIsSpeak(false);
					//�v���C���[�b���t���O�ݒ�
					//�W�����v���Ȃ��Ȃ�
					_NoJump = false;
				}
			}
		}
		else
		{
			//�b��NPC�����Ȃ��̂�
			_NearNPC->SetIsSpeak(false);
			//�b���I����
			if (_NoJump)
			{
				//if (_HPBar/* && _MPBar*/)
				{
					//_HPBar->RenderEnable();
					//_MPBar->RenderEnable();
					_PlayerParameterUI->SetActive(true, true);
					_NoJump = false;
				}
			}
		}
	}
	//NPC�Ȃ�
	else
	{
		_NearNPCLen = FLT_MAX;
	}
}

void Player::_Tyakuti()
{
	if (_CharacterController->IsJump() && _IsTyakuti == true)
	{
		_IsTyakuti = false;
	}
	else if (_CharacterController->IsOnGround() && _IsTyakuti == false)
	{
		_TyakutiSound->Play(false);
		_IsTyakuti = true;
	}
}

#if defined(_DEBUG) || defined(RELEASEDEBUG)
void Player::_DebugPlayer()
{
	//����������
	if (KeyBoardInput->isPush(DIK_O))
	{
		INSTANCE(Inventory)->AddPlayerMoney(10000);
	}
	//�o���l������
	
	if (KeyBoardInput->isPressed(DIK_K) && KeyBoardInput->isPush(DIK_1))
	{
		//�������X�g�ɒǉ�.
		INSTANCE(HistoryManager)->AddPossessionChip(ChipID::Fire);
	}
	if (KeyBoardInput->isPressed(DIK_K) && KeyBoardInput->isPush(DIK_2))
	{
		//�������X�g�ɒǉ�.
		INSTANCE(HistoryManager)->AddPossessionChip(ChipID::Tree);
	}
	if (KeyBoardInput->isPressed(DIK_K) && KeyBoardInput->isPush(DIK_3))
	{
		//�������X�g�ɒǉ�.
		INSTANCE(HistoryManager)->AddPossessionChip(ChipID::Stone);
	}
	if (KeyBoardInput->isPressed(DIK_K) && KeyBoardInput->isPush(DIK_4))
	{
		//�������X�g�ɒǉ�.
		INSTANCE(HistoryManager)->AddPossessionChip(ChipID::Copper);
	}
	if (KeyBoardInput->isPressed(DIK_K) && KeyBoardInput->isPush(DIK_5))
	{
		//�������X�g�ɒǉ�.
		INSTANCE(HistoryManager)->AddPossessionChip(ChipID::Agriculture);
	}
	if (KeyBoardInput->isPressed(DIK_K) && KeyBoardInput->isPush(DIK_6))
	{
		//�������X�g�ɒǉ�.
		INSTANCE(HistoryManager)->AddPossessionChip(ChipID::Hunt);
	}
	if (KeyBoardInput->isPressed(DIK_K) && KeyBoardInput->isPush(DIK_7))
	{
		//�������X�g�ɒǉ�.
		INSTANCE(HistoryManager)->AddPossessionChip(ChipID::Iron);
	}
	if (KeyBoardInput->isPressed(DIK_K) && KeyBoardInput->isPush(DIK_8))
	{
		//�������X�g�ɒǉ�.
		INSTANCE(HistoryManager)->AddPossessionChip(ChipID::Oil);
	}
	if (KeyBoardInput->isPressed(DIK_K) && KeyBoardInput->isPush(DIK_9))
	{
		//�������X�g�ɒǉ�.
		INSTANCE(HistoryManager)->AddPossessionChip(ChipID::Medicine);
	}



	
	int level = _PlayerParam->GetParam(CharacterParameter::LV) - 1;
	//�o���l�𑝂₷�B
	if (KeyBoardInput->isPressed(DIK_P) && KeyBoardInput->isPush(DIK_1))
	{
		if (level < 99)
		{
			_nowEXP = 0;
			_DebugLevel(++level);
		}
	}
	if (KeyBoardInput->isPressed(DIK_P) && KeyBoardInput->isPush(DIK_2))
	{
		if (level > 0)
		{
			_nowEXP = 0;
			_DebugLevel(--level);
		}
	}
	//���x�����グ��B
	if (level < 95)
	{
		if (KeyBoardInput->isPressed(DIK_P) && KeyBoardInput->isPush(DIK_3))
		{
			_nowEXP = 0;
			level += 5;
			_DebugLevel(level);
		}
	}
	if (level >= 6)
	{
		//���x��������B
		if (KeyBoardInput->isPressed(DIK_P) && KeyBoardInput->isPush(DIK_4))
		{
			_nowEXP = 0;
			level -= 5;
			_DebugLevel(level);
		}
	}

	//�h���b�v�A�C�e�����o���B
	if (KeyBoardInput->isPressed(DIK_P) && KeyBoardInput->isPush(DIK_5)) {
		DropItem* item = INSTANCE(GameObjectManager)->AddNew<DropItem>("DropItem", 9);
		item->Create(rand() % 10,0, transform->GetPosition(), 20);
	}

	//�h���b�v�A�C�e�����o���B
	if (KeyBoardInput->isPressed(DIK_P) && KeyBoardInput->isPush(DIK_6)) {
		DropItem* item = INSTANCE(GameObjectManager)->AddNew<DropItem>("DropItem", 9);
		item->Create(rand() % 20,1, transform->GetPosition(), 1);
	}

	//�h���b�v�A�C�e�����o���B
	if (KeyBoardInput->isPressed(DIK_P) && KeyBoardInput->isPush(DIK_7)) {
		DropItem* item = INSTANCE(GameObjectManager)->AddNew<DropItem>("DropItem", 9);
		item->Create(rand() % 20,2, transform->GetPosition(), 1);
	}

	//�v���C���[���S
	if (KeyBoardInput->isPressed(DIK_P) && KeyBoardInput->isPush(DIK_D))
	{
		//_HPBar->SubValue(static_cast<float>((_PlayerParam->ReciveDamageThrough(_PlayerParam->GetParam(CharacterParameter::HP)))));
		_PlayerParam->ReciveDamageThrough(_PlayerParam->GetParam(CharacterParameter::HP));
	}

	//�ړ�
	if (KeyBoardInput->isPressed(DIK_P) && KeyBoardInput->isPush(DIK_U)) {
		transform->SetLocalPosition(-387.3f, 58.307f, -75.8f);
		_nowEXP = 0;
		_DebugLevel(5);
	}
	if (KeyBoardInput->isPressed(DIK_P) && KeyBoardInput->isPush(DIK_I)) {
		transform->SetLocalPosition(-108.1f, 55.524f, 533.9f);
		_nowEXP = 0;
		_DebugLevel(14);
	}
	if (KeyBoardInput->isPressed(DIK_P) && KeyBoardInput->isPush(DIK_O)) {
		transform->SetLocalPosition(218.88f, 67.0f, -0.92f);
		_nowEXP = 0;
		_DebugLevel(39);
	}

	if (KeyBoardInput->isPressed(DIK_P) && KeyBoardInput->isPush(DIK_C)) {
		// ��������ӓK���B
		transform->SetLocalPosition(-157.8f, 121.8f, 79.48f);
		_nowEXP = 0;
		_DebugLevel(42);
	}
	if (KeyBoardInput->isPressed(DIK_P) && KeyBoardInput->isPush(DIK_V)) {
		// ������1�w�K���B
		transform->SetLocalPosition(-142.4f, 121.8f, 175.9f);
		_nowEXP = 0;
		_DebugLevel(44);
	}
	if (KeyBoardInput->isPressed(DIK_P) && KeyBoardInput->isPush(DIK_B)) {
		// ������2�w�K���B
		transform->SetLocalPosition(-164.7f, 147.2f, 128.6f);
		_nowEXP = 0;
		_DebugLevel(46);
	}
	if (KeyBoardInput->isPressed(DIK_P) && KeyBoardInput->isPush(DIK_N)) {
		// ������3�w�K���B
		transform->SetLocalPosition(-140.9f, 169.3f, 246.8f);
		_nowEXP = 0;
		_DebugLevel(56);
	}
	if (KeyBoardInput->isPressed(DIK_P) && KeyBoardInput->isPush(DIK_M)) {
		// �����K���B
		transform->SetLocalPosition(-145.69, 190.0f, 264.72f);
		_nowEXP = 0;
		_DebugLevel(61);
	}
}
void Player::_DebugLevel(int lv)
{
	// ���̃��x���̃p�����[�^��ݒ�B
	_PlayerParam->ParamReset(_ParamTable[lv]);
	//HP���オ�����̂�HP�o�[��HP�ݒ肵�Ȃ����B
	//_HPBar->Reset(static_cast<float>(_PlayerParam->GetParam(CharacterParameter::HP)), static_cast<float>(_PlayerParam->GetParam(CharacterParameter::HP)),true);
	//MP���オ�����̂�MP�o�[��MP�ݒ肵�Ȃ����B
	//_MPBar->Reset(static_cast<float>(_PlayerParam->GetParam(CharacterParameter::MP)), static_cast<float>(_PlayerParam->GetParam(CharacterParameter::MP)),true);
}
#endif // _DEBUG

//�v���C���[�ɑ������Z�b�g(���ŃA�C�e���R�[�h�����ĕ��킩�h����Z�b�g)�B
void Player::SetEquipment(HoldItemBase* equi) {
	if (equi == nullptr) {
		return;
	}

	//�h��B
	if (equi->GetInfo()->TypeID == Item::ItemCodeE::Armor) {
		//�������Ă���h��Ƒ������悤�Ƃ��Ă���h������Ȃ�O���B
		if (static_cast<HoldArmor*>(equi) == _Equipment->armor) {
			_Equipment->armor->SetIsEquipFalse();
			_Equipment->armor = nullptr;
			return;
		}
		//�O�ɑ������Ă����h����O���B
		else if (_Equipment->armor != nullptr) {

			_Equipment->armor->SetIsEquipFalse();
			_Equipment->armor = nullptr;
		}
		//�h��B
		_Equipment->armor = static_cast<HoldArmor*>(equi);
		//�����t���O��true�ɂ���B
		_Equipment->armor->SetIsEquipTrue();

		INSTANCE(Inventory)->SaveArmor();
	}
	else
		//����B
	{
		//�������Ă���h��Ƒ������悤�Ƃ��Ă���h������Ȃ�O���B
		if (static_cast<HoldWeapon*>(equi) == _Equipment->weapon) {
			_Equipment->weapon->SetIsEquipFalse();
			_Equipment->weapon = nullptr;
			return;
		}
		else if (_Equipment->weapon != nullptr) {
			//�O�ɑ������Ă���������O���B
			_Equipment->weapon->SetIsEquipFalse();
			_Equipment->weapon = nullptr;
		}
		//����B
		_Equipment->weapon = static_cast<HoldWeapon*>(equi);
		//�����t���O��true�ɂ���B
		_Equipment->weapon->SetIsEquipTrue();

		INSTANCE(Inventory)->SaveWeapon();
	}
}

//�Q�[���J�n���ɃC���x���g�����瑕�����Ă��镐���T�������������B
void Player::Re_SetEquipment() {

	//��������t���O��T���B
	for (int type = static_cast<int>(Item::ItemCodeE::Armor); type <= static_cast<int>(Item::ItemCodeE::Weapon); type++)
	{
		//����̃C���x���g�����X�g���擾�B
		vector<HoldItemBase*> vector = INSTANCE(Inventory)->GetInventoryList(static_cast<Item::ItemCodeE>(type));
		for (auto itr = vector.begin(); itr != vector.end();)
		{
			//��������Ă���B
			if ((*itr) != nullptr&& static_cast<HoldEquipment*>((*itr))->GetIsEquip() == true) {
				//�v���C���[�ɑ����B
				SetEquipment((*itr));
				break;
			}
			else
			{
				itr++;
			}
		}
	}
}


void Player::SetNPC(NPC * npc)
{
	//���̂܂ܐݒ�
	if ((_NearNPC == nullptr) || (npc == nullptr))
	{
		_NearNPC = npc;
	}
	else
	{
		_NearNPC->SetIsSpeak(false);
		_NearNPC = npc;
	}
}

void Player::AnimationEventControl()
{
	/*******************/
	/*		�U��	   */
	/*******************/
	//�U��1
	{
		float eventframe = 0.5f;
		_AnimationEventPlayer->AddAnimationEvent((int)Player::AnimationNo::AnimationAttack01, eventframe, static_cast<AnimationEvent>(&Player::Attack1));
	}
	//�U��2
	{
		float eventframe = 0.5f;
		_AnimationEventPlayer->AddAnimationEvent((int)Player::AnimationNo::AnimationAttack02, eventframe, static_cast<AnimationEvent>(&Player::Attack2));

	}
	//�U��3
	{
		float eventframe = 0.5f;
		_AnimationEventPlayer->AddAnimationEvent((int)Player::AnimationNo::AnimationAttack03, eventframe, static_cast<AnimationEvent>(&Player::Attack3));

	}
	//�U��4
	{
		float eventframe = 0.5f;
		_AnimationEventPlayer->AddAnimationEvent((int)Player::AnimationNo::AnimationAttack04, eventframe, static_cast<AnimationEvent>(&Player::Attack4));
	}
	//�U��5
	{
		float eventframe = 1.1f;
		_AnimationEventPlayer->AddAnimationEvent((int)Player::AnimationNo::AnimationAttack05, eventframe, static_cast<AnimationEvent>(&Player::Attack5));
	}

	/*******************/
	/*		����	   */
	/*******************/
	float eventframe = 0.2f;
	_AnimationEventPlayer->AddAnimationEvent((int)Player::AnimationNo::AnimationRun, eventframe, static_cast<AnimationEvent>(&Player::Asioto1));
	eventframe = 0.6f;
	_AnimationEventPlayer->AddAnimationEvent((int)Player::AnimationNo::AnimationRun, eventframe, static_cast<AnimationEvent>(&Player::Asioto1));
	eventframe = 1.0f;
	_AnimationEventPlayer->AddAnimationEvent((int)Player::AnimationNo::AnimationRun, eventframe, static_cast<AnimationEvent>(&Player::Asioto1));

}

void Player::Attack1()
{
	//�U�����̃T�E���h�Đ��B
	_AttackSoound->Play(false);
	//�U���{�C�X�Đ�
	_AttackBoiceSound[(int)Player::AttackBoice::Attack1]->Play(false);
	//�U���R���W�����쐬
	AttackCollision* attack = INSTANCE(GameObjectManager)->AddNew<AttackCollision>("attack01", 1);
	if (_Equipment) {
		attack->Create(move(_PlayerParam->GiveDamageMass(false, false, _Equipment->weapon, 120)), Vector3(0.0f, 1.0f, 1.5f), Quaternion::Identity, Vector3(1.5f, 1.0f, 2.5f), AttackCollision::CollisionMaster::Player, 0.2f, AttackCollision::ReactionType::Leans, transform);
		attack->RemoveParent();
	}
}

void Player::Attack2()
{
	//�U�����̃T�E���h�Đ��B
	_AttackSoound->Play(false);
	//�U���{�C�X�Đ�
	_AttackBoiceSound[(int)Player::AttackBoice::Attack2]->Play(false);
	//�U���R���W�����쐬
	AttackCollision* attack = INSTANCE(GameObjectManager)->AddNew<AttackCollision>("attack02", 1);
	if (_Equipment) {
		attack->Create(move(_PlayerParam->GiveDamageMass(false, false, _Equipment->weapon, 100)), Vector3(0.0f, 1.0f, 1.5f), Quaternion::Identity, Vector3(1.5f, 1.0f, 2.5f), AttackCollision::CollisionMaster::Player, 0.2f, AttackCollision::ReactionType::Leans, transform);
		attack->RemoveParent();
	}
}

void Player::Attack3()
{
	//�U�����̃T�E���h�Đ��B
	_AttackSoound->Play(false);
	//�U���{�C�X�Đ�
	_AttackBoiceSound[(int)Player::AttackBoice::Attack1]->Play(false);
	//�U���R���W�����쐬
	AttackCollision* attack = INSTANCE(GameObjectManager)->AddNew<AttackCollision>("attack03", 1);
	if (_Equipment) {
		attack->Create(move(_PlayerParam->GiveDamageMass(false, false, _Equipment->weapon, 120)), Vector3(0.0f, 1.0f, 1.5f), Quaternion::Identity, Vector3(1.5f, 1.0f, 2.5f), AttackCollision::CollisionMaster::Player, 0.2f, AttackCollision::ReactionType::Leans, transform);
		attack->RemoveParent();
	}
}

void Player::Attack4()
{
	//�U�����̃T�E���h�Đ��B
	_AttackSoound->Play(false);
	//�U���{�C�X�Đ�
	_AttackBoiceSound[(int)Player::AttackBoice::Attack2]->Play(false);
	//�U���R���W�����쐬
	AttackCollision* attack = INSTANCE(GameObjectManager)->AddNew<AttackCollision>("attack04", 1);
	if (_Equipment) {
		attack->Create(move(_PlayerParam->GiveDamageMass(false, false, _Equipment->weapon, 100)), Vector3(0.0f, 1.0f, 1.5f), Quaternion::Identity, Vector3(1.5f, 1.0f, 2.5f), AttackCollision::CollisionMaster::Player, 0.2f, AttackCollision::ReactionType::Leans, transform);
		attack->RemoveParent();
	}
}

void Player::Attack5()
{
	//�U�����̃T�E���h�Đ��B
	_AttackSoound->Play(false);
	//�U���{�C�X�Đ�
	_AttackBoiceSound[(int)Player::AttackBoice::Attack3]->Play(false);
	//�U���R���W�����쐬
	AttackCollision* attack = INSTANCE(GameObjectManager)->AddNew<AttackCollision>("attack05", 1);
	if (_Equipment) {
		attack->Create(move(_PlayerParam->GiveDamageMass(false, false, _Equipment->weapon, 200)), Vector3(0.0f, 1.0f, 1.5f), Quaternion::Identity, Vector3(2.5f, 1.0f, 2.5f), AttackCollision::CollisionMaster::Player, 0.2f, AttackCollision::ReactionType::Leans, transform);
		attack->RemoveParent();
	}
}

void Player::Asioto1()
{
	//�����̃T�E���h�Đ�
	_AsiotoSound->Play(false);
}
