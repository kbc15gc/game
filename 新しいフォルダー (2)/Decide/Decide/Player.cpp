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
	const float JUMP_POWER = 5.0f;
}

Player::Player(const char * name) :
	GameObject(name),
	_Gravity(-30.0f),
	_Jump(false)
{

}

void Player::Awake()
{
	_Model = AddComponent<SkinModel>();
	_Anim = AddComponent<Animation>();
	_Rigid = AddComponent<RigidBody>();
	_CharacterController = AddComponent<CCharacterController>();
	CCapsuleCollider* coll = AddComponent<CCapsuleCollider>();
	_Height = 1.5f;
	_Radius = 0.5f;
	coll->Create(_Radius, _Height);
	_Rigid->Create(0.0f, coll, Collision_ID::PLAYER, Vector3::zero, Vector3(0, _Height/2, 0));
	_RB = (btRigidBody*)_Rigid->GetCollisonObj();
	//RigidBody�̏㉺�̈ړ��ʂ�����
	//_Rigid->SetGravity(btVector3(0.0f, 0.0f, 0.0f));
	//�X���[�v�����Ȃ�(�K�v���ǂ����킩��Ȃ��B)
	_RB->setSleepingThresholds(0, 0);

	SkinModelData* modeldata = new SkinModelData();
	modeldata->CloneModelData(SkinModelManager::LoadModel("Unity.X"), _Anim);
	_Model->SetModelData(modeldata);
	_Model->SetModelEffect(ModelEffectE::SPECULAR, false);

	_CharacterController->Init(_Radius,_Height,_Rigid,coll);
	_CharacterController->SetGravity(_Gravity);
}

void Player::Start()
{
	_Model->SetCamera(GameObjectManager::mainCamera);
	_Model->SetLight(GameObjectManager::mainLight);

	transform->SetLocalPosition(Vector3(0.0f, 10.0f, 0.0f));
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
	case PlayerState::Wait:
		break;
	case PlayerState::Run:
		break;
	case PlayerState::Jump:
		break;
	case PlayerState::Attack:
		break;
	default:
		break;
	}
	Move();
	Jump();
	//Y���̈ړ��ʕۑ�
	//Vector3 move = _MoveSpeed;
	//�L�����N�^�[�R���g���[���[�X�V
	_CharacterController->SetMoveSpeed(_MoveSpeed);
	_CharacterController->Execute();
	/*transform->position = _CharacterController->GetPosition();	//���W���
	_CharacterController->SetPosition(transform->position);*/
	//XZ���̈ړ�
//	transform->localPosition.Add(_MoveSpeed);
	transform->UpdateTransform();
	//if (_MoveSpeed.Length() > 0)
	//{
	//	
	//}
	//Y���ړ��͂��Ȃ�
	//_MoveSpeed.y = 0.0f;
	//�ǃ`�F�b�N
	//_WallCheck(move);
	//�d�̓`�F�b�N
	//_GravityCheck(move.y);
}

void Player::Move()
{
	//������
	_MoveSpeed = _CharacterController->GetMoveSpeed();
	_MoveSpeed.x = 0.0f;
	_MoveSpeed.z = 0.0f;
	//����
	_Dir = Vector3::zero;
	static Vector3 dirX = Vector3::axisX;
	static Vector3 dirZ = Vector3::axisZ;
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
#endif // _DEBUG

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
		Vector3 ang = transform->GetLocalAngle();
		ang.y = D3DXToDegree(rot + D3DXToRadian(90));
		//��]
		transform->SetLocalAngle(ang);
	}
}

void Player::Jump()
{
	if (KeyBoardInput->isPush(DIK_J) || XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_A))
	{
		_MoveSpeed.y = JUMP_POWER;
		_CharacterController->Jump();
	}
}

void Player::Attack()
{

}