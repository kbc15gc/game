#include"stdafx.h"
#include "Player.h"
#include "fbEngine\_Object\_Component\_3D\SkinModel.h"
#include "fbEngine\_Object\_Component\_3D\Animation.h"
#include <string>
#include <sstream>
#include "GameObject\SplitSpace.h"
#include "GameObject\AttackValue2D.h"
#include "..\History\HistoryManager.h"
#include "GameObject\Component\ParticleEffect.h"
#include "GameObject\Component\BuffDebuffICon.h"

namespace
{
	float NormalAnimationSpeed = 1.0f;
	float AttackAnimationSpeed = 1.3f;
	float Oboreru = 1.0f;
	const float _Speed = 20.0f;
	const float _DashSpeed = 40.0f;
	const float _JumpSpeed = 15.0f;
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
	//���S�X�e�[�g
	_DeathState(this),
	//�X�g�b�v�X�e�[�g
	_StopState(this),
	//�X�s�[�N�X�e�[�g
	_SpeakState(this),
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
	_Radius = 0.2f;
	//�J�v�Z���R���C�_�[�쐬
	coll->Create(_Radius, _Height);
	//�X�L�����f���쐬
	SkinModelData* modeldata = new SkinModelData();
	//���f���f�[�^�쐬
	modeldata->CloneModelData(SkinModelManager::LoadModel("Player.X"), _Anim);
	//���f���ݒ�
	_Model->SetModelData(modeldata);
	_Model->SetModelEffect(ModelEffectE::SPECULAR, true);
	//_Model->SetAllBlend(Color::white * 13);
	
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
		_HPBar->Create(Colors, _PlayerParam->GetMaxHP(), _PlayerParam->GetParam(CharacterParameter::HP),true, true, NULL);
	}
	// MP�̃o�[��\���B
	{
		vector<BarColor> Colors;
		Colors.push_back(BarColor::Blue); //175.0f, 21.9f, 0.0f
		_MPBar->Create(Colors, _PlayerParam->GetMaxMP(), _PlayerParam->GetParam(CharacterParameter::MP), true, true, _HPBar->GetTransform(), Vector3(0.0f, 40.0f, 0.0f), Vector2(1.0f, 1.0f));
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

	//�p�[�e�B�N���G�t�F�N�g�B
	_ParticleEffect = AddComponent<ParticleEffect>();

	//�o�t�f�o�t�A�C�R���B
	_BuffDebuffICon = AddComponent<BuffDebuffICon>();
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
	_StartPos = Vector3(-148.0f, 68.5f, -34.0f);
	transform->SetLocalPosition(_StartPos);
	//�ړ����x������
	_MoveSpeed = Vector3::zero;
	//�U���A�j���[�V�����X�e�[�g�̏�����
	_NowAttackAnimNo = AnimationNo::AnimationInvalid;
	_NextAttackAnimNo = AnimationNo::AnimationInvalid;

	//���x���A�b�v���̃X�v���C�g������
	/*{
		_LevelUpSprite = AddComponent<Sprite>();
		_LevelUpSprite->SetTexture(LOADTEXTURE("levelup.png"));
		_LevelUpSprite->SetEnable(true);
		_LevelUpSprite->SetPivot(Vector2(0.5f, 1.0f));
	}*/

	//�Q�[���J�n���ɃC���x���g�����瑕�����Ă��镐���T�������������B
	Re_SetEquipment();
}

void Player::Update()
{
	//@todo for debug
#ifdef _DEBUG
	_DebugPlayer();
#endif // _DEBUG

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

	EffectUpdate();
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
		break;
	case State::Stop:
		//�X�g�b�v���
		_CurrentState = &_StopState;
		break;
	case State::Speak:
		_CurrentState = &_SpeakState;
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
	//�A�j���[�V�����X�s�[�h�͊�{�P
	_Anim->SetAnimeSpeed(NormalAnimationSpeed);
	//���S�A�j���[�V����
	if (_State == State::Death)
	{
		PlayAnimation(AnimationNo::AnimationDeath, 0.1f, 1);
		return;
	}
	//�W�����v�A�j���[�V����
	//�X�g�b�v����Ȃ��Ȃ�W�����v����B
	if (_CharacterController->IsJump() && _State != State::Stop)
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
		//�b���邩�B
		else if (_State == State::Speak)
		{
			PlayAnimation(AnimationNo::AnimationRun, 0.2f);
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
#ifdef _DEBUG
		if (_Equipment == nullptr)
		{
			// �����p�̍\���̂�Null�B
			abort();
		}
#endif
		// �_���[�W��^���鏈��
		int damage = _PlayerParam->ReciveDamage(hitCollision->GetDamage(), hitCollision->GetIsMagic(), _Equipment->armor);
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

void Player::Run()
{
	//�ړ����x
	Vector3 movespeed = Vector3::zero;
	movespeed.y = _CharacterController->GetMoveSpeed().y;

	//�L�[�{�[�h��J�@or�@�p�b�h��A�{�^���ŃW�����v
	if (KeyBoardInput->isPush(DIK_J) || XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_A))
	{
		//�n�ʏ�ɂ���ꍇ
		if (_CharacterController->IsOnGround() == true)
		{
			//�W�����v�p���[��ݒ�
			movespeed.y = _JumpSpeed;
			//�L�����N�^�[�R���g���[���[���W�����v��
			_CharacterController->Jump();
		}
	}

	//�Q�[���p�b�h����擾��������
	Vector3 dir = Vector3::zero;
	//�R���g���[���[�ړ�
	dir.x += (XboxInput(0)->GetAnalog(AnalogE::L_STICK).x / 32767.0f);
	dir.z += (XboxInput(0)->GetAnalog(AnalogE::L_STICK).y / 32767.0f);
#ifdef _DEBUG
	//�L�[�{�[�h(�f�o�b�O�p)
	if (KeyBoardInput->isPressed(DIK_W))
	{
		dir.z++;
	}
	if (KeyBoardInput->isPressed(DIK_S))
	{
		dir.z--;
	}
	if (KeyBoardInput->isPressed(DIK_A))
	{
		dir.x--;
	}
	if (KeyBoardInput->isPressed(DIK_D))
	{
		dir.x++;
	}
#endif
	//�ړ�������
	if (dir.Length() != 0)
	{
		//�J��������݂������ɕϊ�
		Camera* camera = INSTANCE(GameObjectManager)->mainCamera;
		//�J�����̃r���[�s����Q�b�g
		D3DXMATRIX view = camera->GetViewMat();
		//�r���[�s��̋t�s��
		D3DXMATRIX viewinv;
		D3DXMatrixInverse(&viewinv, NULL, &view);
		//�J������Ԃ��猩���������̃x�N�g�����擾�B
		Vector3 cameraZ;
		cameraZ.x = viewinv.m[2][0];
		cameraZ.y = 0.0f;		//Y������Ȃ��B
		cameraZ.z = viewinv.m[2][2];
		cameraZ.Normalize();	//Y����ł������Ă���̂Ő��K������B
								//�J�������猩���������̃x�N�g�����擾�B
		Vector3 cameraX;
		cameraX.x = viewinv.m[0][0];
		cameraX.y = 0.0f;		//Y���͂���Ȃ��B
		cameraX.z = viewinv.m[0][2];
		cameraX.Normalize();	//Y����ł������Ă���̂Ő��K������B

								// �����x�N�g���Ɉړ��ʂ�ώZ�B
								//�_�b�V���{�^���̏ꍇ
		if (VPadInput->IsPush(fbEngine::VPad::ButtonRB1))
		{
			//�_�b�V���X�s�[�h��K�p
			dir = dir * _DashSpeed;
		}
		else
		{
			//�ʏ�̃X�s�[�h
			dir = dir * _Speed;
		}
		//�J��������݂������Ɏˉe�B
		movespeed = movespeed + cameraX * dir.x;
		movespeed.y = movespeed.y;	//������͌Œ�Ȃ̂ł��̂܂܁B
		movespeed = movespeed + cameraZ * dir.z;

		//�ړ������������̃x�N�g��
		Vector3 vec = movespeed;
		//���K��
		vec.Normalize();
		//�x�N�g������p�x�����߂�
		//��]
		_Rotation->RotationToDirection_XZ(vec);
	}
	//�ړ����Ă��Ȃ�
	if (dir.Length() < 0.0001f)
	{
		ChangeState(Player::State::Idol);
	}

	//�L�����N�^�[�R���g���[���[�X�V
	_CharacterController->SetMoveSpeed(movespeed);
	_CharacterController->Execute();

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

		returnValue = true;
	}
	if (_PlayerParam->HeelMP(info->effectValue[CharacterParameter::Param::MP]))
	{
		if (_ParticleEffect)
		{
			_ParticleEffect->HeelMpEffect();
		}

		returnValue = true;
	}

	for (int idx = static_cast<int>(CharacterParameter::Param::ATK); idx < CharacterParameter::MAX; idx++) {
		int value = info->effectValue[idx];
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

			_PlayerParam->Buff(static_cast<CharacterParameter::Param>(idx), static_cast<unsigned short>(value), info->time);
			_BuffDebuffICon->SetHpBarTransform(_HPBar->GetTransform());
			_BuffDebuffICon->BuffIconCreate(static_cast<BuffDebuffICon::Param>(idx));
			returnValue = true;
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
			_PlayerParam->Debuff(static_cast<CharacterParameter::Param>(idx), static_cast<unsigned short>(abs(value)), info->time);
			_BuffDebuffICon->SetHpBarTransform(_HPBar->GetTransform());
			_BuffDebuffICon->DebuffIconCreate(static_cast<BuffDebuffICon::Param>(idx));
			returnValue = true;
		}
	}
	return returnValue;
}

/**
* �G�t�F�N�g�p�X�V.
*/
void Player::EffectUpdate()
{
	bool isBuffEffect = false;
	bool isDeBuffEffect = false;
	for (int idx = static_cast<int>(CharacterParameter::Param::ATK); idx < CharacterParameter::Param::DEX; idx++) {

		if (_PlayerParam->GetBuffParam((CharacterParameter::Param)idx) > 0.0f)
		{
			isBuffEffect = true;
		}
		else
		{
			_BuffDebuffICon->DeleteBuffIcon(static_cast<BuffDebuffICon::Param>(idx));
		}

		if (_PlayerParam->GetDebuffParam((CharacterParameter::Param)idx) > 0.0f)
		{
			isDeBuffEffect = true;
		}
		else
		{
			_BuffDebuffICon->DeleteDebuffIcon(static_cast<BuffDebuffICon::Param>(idx));
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
		ChangeState(State::Death);
	}
	//�C�ɓ���ƃ_���[�W��H�炤�B

	static float time = 0.0f;
	time += Time::DeltaTime();
	//�C�̒��̏ꍇ�B
	//HP��0�ȏ�Ȃ�B
	//�f�o�b�O���łȂ��B
	//2�b�Ԋu�ŁB
	if (transform->GetLocalPosition().y < 48.5f && _PlayerParam->GetParam(CharacterParameter::HP) > 0 && _Debug == false)
	{
		if (fmod(time, 2.0f) >= 1.0f)
		{
			_HPBar->SubValue(_PlayerParam->ReciveDamageThrough(Oboreru));
			time = 0.0f;
		}
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
	_HPBar->Reset(_PlayerParam->GetParam(CharacterParameter::HP), _PlayerParam->GetParam(CharacterParameter::HP),true);
	//MP���オ�����̂�MP�o�[��MP�ݒ肵�Ȃ����B
	_MPBar->Reset(_PlayerParam->GetParam(CharacterParameter::MP), _PlayerParam->GetParam(CharacterParameter::MP),true);
	//���x���A�b�v���̉��Đ��B
	_LevelUP_SE->Play(false);
}

#ifdef _DEBUG
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
		TakeDrop(100, 100);
	}
	int level = _PlayerParam->GetParam(CharacterParameter::LV);
	//���x�����グ��B
	if (level <= 95)
	{
		if (KeyBoardInput->isPressed(DIK_P) && KeyBoardInput->isPush(DIK_2))
		{
			level += 5;
			_DebugLevel(level);
		}
	}
	if (level >= 6)
	{
		//���x��������B
		if (KeyBoardInput->isPressed(DIK_P) && KeyBoardInput->isPush(DIK_3))
		{
			level -= 5;
			_DebugLevel(level);
		}
	}
}
void Player::_DebugLevel(int lv)
{
	// ���̃��x���̃p�����[�^��ݒ�B
	_PlayerParam->ParamReset(_ParamTable[lv]);
	//HP���オ�����̂�HP�o�[��HP�ݒ肵�Ȃ����B
	_HPBar->Reset(_PlayerParam->GetParam(CharacterParameter::HP), _PlayerParam->GetParam(CharacterParameter::HP),true);
	//MP���オ�����̂�MP�o�[��MP�ݒ肵�Ȃ����B
	_MPBar->Reset(_PlayerParam->GetParam(CharacterParameter::MP), _PlayerParam->GetParam(CharacterParameter::MP),true);
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