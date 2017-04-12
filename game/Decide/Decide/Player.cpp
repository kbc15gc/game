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
	_Jump(false),
	_CurrentState(NULL),
	_RunState(this)
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
	//�|�W�V����
	transform->SetLocalPosition(Vector3(0.0f, 10.0f, 0.0f));
	//�ړ����x������
	_MoveSpeed = Vector3::zero;
	//�����v���C���[��ԁi�ҋ@�j
	_State = State::Wait;
	//�v���C���[�̃��x��������
	//�ŏ��͂P����
	_Level = 1;
}
#include "fbEngine/Camera.h"
void Player::Update()
{
	
	_CurrentState = &_RunState;
	_CurrentState->Update();
	
	//�g�����X�t�H�[���X�V
	transform->UpdateTransform();
}

void Player::ChangeState(State nextstate)
{
	if (_CurrentState != NULL) {
		//���݂̃X�e�[�g�𔲂���B
		_CurrentState->Leave();
	}
	switch (_State)
	{
		//�ҋ@���
	case State::Wait:
		break;
		//������
	case State::Run:
		
		break;
		//�W�����v���
	case State::Jump:
		break;
		//�U�����
	case State::Attack:
		break;
	default:
		break;
	}
	_State = nextstate;
	_CurrentState->Enter();
}

void Player::Attack()
{

}