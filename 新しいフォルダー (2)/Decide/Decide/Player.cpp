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
	_Gravity(-30.0f),
	//�W�����v�t���O�ݒ�
	_Jump(false)
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
	_CharacterController->Init(this,transform,_Radius,_Height, Collision_ID::PLAYER,coll);
	//�L�����N�^�[�R���g���[���[�̏d�͐ݒ�
	_CharacterController->SetGravity(_Gravity);
}

void Player::Start()
{
	//���f���ɃJ�����ݒ�
	_Model->SetCamera(GameObjectManager::mainCamera);
	//���f���Ƀ��C�g�ݒ�
	_Model->SetLight(GameObjectManager::mainLight);
	//�|�W�V����
	transform->localPosition = Vector3(0.0f, 10.0f, 0.0f);
	//�ړ����x������
	_MoveSpeed = Vector3::zero;
	//�����v���C���[��ԁi�ҋ@�j
	_State = PlayerState::Wait;
	//�v���C���[�̃��x��������
	//�ŏ��͂P����
	_Level = 1;
}
#include "fbEngine/Camera.h"
void Player::Update()
{
	switch (_State)
	{
	//�ҋ@���
	case PlayerState::Wait:
		break;
	//������
	case PlayerState::Run:
		break;
	//�W�����v���
	case PlayerState::Jump:
		break;
	//�U�����
	case PlayerState::Attack:
		break;
	default:
		break;
	}
	//�ړ�
	Move();
	//�W�����v
	Jump();
	//�L�����N�^�[�R���g���[���[�X�V
	_CharacterController->SetMoveSpeed(_MoveSpeed);
	_CharacterController->Execute();
	//�g�����X�t�H�[���X�V
	transform->UpdateTransform();
}

void Player::Move()
{
	//������
	//�ړ����x
	_MoveSpeed = _CharacterController->GetMoveSpeed();
	_MoveSpeed.x = 0.0f;
	_MoveSpeed.z = 0.0f;
	//����
	_Dir = Vector3::zero;

	//�L�[�{�[�h��J�@or�@�p�b�h��A�{�^���ŃW�����v
	if (KeyBoardInput->isPush(DIK_J) || XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_A))
	{
		//�n�ʏ�ɂ���ꍇ
		if (_CharacterController->IsOnGround())
		{
			//�W�����v�p���[��ݒ�
			_MoveSpeed.y = JUMP_POWER;
			//�L�����N�^�[�R���g���[���[���W�����v��
			_CharacterController->Jump();
		}
	}

	//�R���g���[���[�ړ�
	_Dir.x += (XboxInput(0)->GetAnalog(AnalogInputE::L_STICK).x / 32767.0f);
	_Dir.z += (XboxInput(0)->GetAnalog(AnalogInputE::L_STICK).y / 32767.0f);
#ifdef _DEBUG
	//�L�[�{�[�h(�f�o�b�O�p)
	if (KeyBoardInput->isPressed(DIK_W))
	{
		_Dir.z += SPEED;
	}
	if (KeyBoardInput->isPressed(DIK_S))
	{
		_Dir.z -= SPEED;
	}
	if (KeyBoardInput->isPressed(DIK_A))
	{
		_Dir.x -= SPEED;
	}
	if (KeyBoardInput->isPressed(DIK_D))
	{
		_Dir.x += SPEED;
	}
#endif

	//�ړ�������
	if (_Dir.Length() != 0)
	{
		//�J��������݂������ɕϊ�
		Camera* c = GameObjectManager::mainCamera;
		//_Dir = c->transform->Direction(_Dir)*Time::DeltaTime();
		//Y�̈ړ��ʂ�����
		_MoveSpeed = Vector3(_Dir.x, 0.0f, _Dir.z);

		Vector3 vec = _MoveSpeed;
		//���K��
		vec.Normalize();
		//�x�N�g������p�x�����߂�
		float rot = D3DXToRadian(360) - atan2f(vec.z, vec.x);
		//��]
		transform->localAngle.y = D3DXToDegree(rot + D3DXToRadian(-90));
	}
}

void Player::Jump()
{
	
}

void Player::Attack()
{

}