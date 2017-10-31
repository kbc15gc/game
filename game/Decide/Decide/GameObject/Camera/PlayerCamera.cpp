#include"stdafx.h"
#include "PlayerCamera.h"

#include "GameObject\Player\Player.h"
#include "fbEngine\_Object\_Component\_3D\Camera.h"
#include "GameObject\History\HistoryBook\HistoryBook.h"

namespace
{
	/** �v���C���[�̍���. */
	const Vector3 PLAYER_HEIGHT(0.0f, 1.5f, 0.0f);
	/** ��]���x. */
	const float CAMERA_SPEED = 2.0f;
}

PlayerCamera::PlayerCamera(const char * name) :
	GameCamera(name),
	_Spring(50.0f),
	_Damping(12.0f)
{
}

/**
* �f�X�g���N�^.
*/
PlayerCamera::~PlayerCamera()
{
	//�V���h�E�}�b�v�ɐݒ肵���J�������O��
	INSTANCE(SceneManager)->GetShadowMap()->SetCamera(nullptr);
}

void PlayerCamera::Awake()
{
	GameCamera::Awake();

	//�J�����R���|�[�l���g
	_Camera = AddComponent<Camera>();
	_Camera->SetNear(0.01f);
	_Camera->SetFar(10000.0f);
	INSTANCE(GameObjectManager)->mainCamera = _Camera;

	//�J�����̃R���W�����̔��a�ݒ�
	_Radius = 0.001f;
	_Sphere = AddComponent<SphereCollider>();
	_Sphere->Create(_Radius);
	//�����̉����Ə��
	_Dist = 5.0f;

}

void PlayerCamera::Start()
{
	//�v���C���[�̃|�W�V�����ւ̎Q�Ƃ��擾
	_PlayerPos = &_Player->transform->GetPosition();
	// �����l�ݒ�̂��ߏ������ĂԁB
	// �������ȁB
	{
		_Camera->SetTarget(_GetPlayerPos());
		_DestinationPos = _GetPlayerPos() + (_Player->transform->GetForward() * _Dist);
		transform->SetPosition(_DestinationPos);
		_Camera->Update();

		_ToCameraDir = (_Player->transform->GetForward());
	}

	//���j���������B
	_HistoryBook = (HistoryBook*)INSTANCE(GameObjectManager)->FindObject("HistoryBook");

	//�J�������V���h�E�}�b�v�ɐݒ�.
	INSTANCE(SceneManager)->GetShadowMap()->SetCamera(_Camera);
}

void PlayerCamera::UpdateSubClass()
{
	float Pint = 3.0f;

	//���j�������Ă��邩�ǂ����B
	if (_IsMove)
	{
		_StandardBehavior();
		Pint = 3.0f;
	}
	else
	{
		Vector3 CameraToHistory;
		CameraToHistory.Subtract(_HistoryBook->transform->GetPosition(), transform->GetPosition());
		Pint = min(3.0f, CameraToHistory.Length());
	}

	INSTANCE(SceneManager)->GetDepthofField().SetPint(Pint * 1000);
	INSTANCE(SceneManager)->GetDepthofField().SetFParam(5.6f);
	INSTANCE(SceneManager)->GetDepthofField().SetFocalLength(24.0f);
}

Vector3 PlayerCamera::_GetPlayerPos()
{
	return (*_PlayerPos) + PLAYER_HEIGHT;
}

/**
* �ʏ펞�̃J��������.
*/
void PlayerCamera::_StandardBehavior()
{
	//�v���C���[������B
	_LookAtPlayer();

	//�E��]
	if (KeyBoardInput->isPressed(DIK_RIGHT) || (XboxInput(0)->GetAnalog(AnalogE::R_STICK).x / 32767.0f) > 0.1f)
	{
		_RotateHorizon(CAMERA_SPEED * Time::DeltaTime());
	}
	//����]
	if (KeyBoardInput->isPressed(DIK_LEFT) || (XboxInput(0)->GetAnalog(AnalogE::R_STICK).x / 32767.0f) < -0.1f)
	{
		_RotateHorizon(-CAMERA_SPEED * Time::DeltaTime());
	}
	//��
	if (KeyBoardInput->isPressed(DIK_UP) || (XboxInput(0)->GetAnalog(AnalogE::R_STICK).y / 32767.0f) > 0.1f)
	{
		_RotateVertical(CAMERA_SPEED * Time::DeltaTime());
	}
	//��
	if (KeyBoardInput->isPressed(DIK_DOWN) || (XboxInput(0)->GetAnalog(AnalogE::R_STICK).y / 32767.0f) < -0.1f)
	{
		_RotateVertical(-CAMERA_SPEED * Time::DeltaTime());
	}

	//�ړ���|�W�V�������擾�B
	_DestinationPos = _ClosetRay();

	//�J�������ړ�������B
	_Move();
}

void PlayerCamera::_LookAtPlayer()
{
	auto trg = _GetPlayerPos();
	_Camera->SetTarget(trg);
	transform->LockAt(trg);
}

void PlayerCamera::_RotateHorizon(float roty)
{
	D3DXQUATERNION mAxisY;
	D3DXMATRIX rot;
	//Y�������roty�x��]����N�H�[�^�j�I�����쐬�B
	D3DXQuaternionRotationAxis(&mAxisY, &(const D3DXVECTOR3&)Vector3::up, roty);
	//�N�H�[�^�j�I�������]�s����쐬�B
	D3DXMatrixRotationQuaternion(&rot, &mAxisY);

	//�x�N�g������]�s����g����]������B
	D3DXVECTOR4 v;
	D3DXVec3Transform(&v, &(D3DXVECTOR3)_ToCameraDir, &rot);
	_ToCameraDir = v;
	auto nextPos = _GetPlayerPos() + (Vector3(v.x, v.y, v.z) * _Dist);
	//transform->SetPosition(nextPos);
	//_DestinationPos = nextPos;
}

void PlayerCamera::_RotateVertical(float rotx)
{
	D3DXVECTOR3 Cross;
	D3DXQUATERNION zAxis;
	D3DXVECTOR4 v;
	D3DXMATRIX rot;

	//�O�ςŒ�������x�N�g�����擾
	D3DXVec3Cross(&Cross, &(const D3DXVECTOR3&)Vector3::up, &(D3DXVECTOR3)_ToCameraDir);
	//�N�H�[�^�j�I���쐬�B
	D3DXQuaternionRotationAxis(&zAxis, &Cross, rotx);
	D3DXMatrixRotationQuaternion(&rot, &zAxis);
	//��]�s����g���ĉ�]������B
	D3DXVec3Transform(&v, &(D3DXVECTOR3)_ToCameraDir, &rot);
	
	//���K������B
	D3DXVECTOR3 toPosDir;
	D3DXVec3Normalize(&toPosDir, &D3DXVECTOR3(v.x, v.y, v.z));

	bool notlimit = true;
	//�J�����̏㉺�̏��
	if (rotx > 0.0f && toPosDir.y < -0.5f)
	{
		notlimit = false;
	}
	else if (rotx < 0.0f && toPosDir.y > 0.8f)
	{
		notlimit = false;
	}

	if (notlimit)
	{
		_ToCameraDir = v;
		auto nextPos = _GetPlayerPos() + (Vector3(v.x, v.y, v.z) * _Dist);
		//transform->SetPosition(nextPos);
		//_DestinationPos = nextPos;
	}
}

Vector3 PlayerCamera::_ClosetRay()
{
	//�v���C���[�ƃJ�����̋���
	Vector3 dist = (Vector3)(_ToCameraDir * _Dist);
	Vector3 from, to;
	//�����_
	from = _Camera->GetTarget();
	//�J�����̈ړ���
	to = from + dist;

	// �Փ˂𖳎����鑮����ݒ�B
	int attri = (Collision_ID::ATTACK) | (Collision_ID::PLAYER) | (Collision_ID::ENEMY) | (Collision_ID::BOSS) | (Collision_ID::NOTHITCAMERA) | (Collision_ID::CHARACTER_GHOST);
	//���C���΂�
	auto ray = INSTANCE(PhysicsWorld)->ClosestRayShape(_Sphere, from, to, attri);

	//���C�������ɓ����������ǂ����H
	if (ray.hitObject)
	{
		//�Փ˓_�̖@�������ɔ��a���ړ�����x�N�g���B
		Vector3 normal = ray.hitNormal;
		normal.Scale(_Radius);
		//�Փ˓_����@���̕����ɉ����o���A�ړ�����Z�o�B
		return ray.hitPos + normal;
	}
	else
	{
		//�ǂ��ɂ�������Ȃ������̂ŁA���C�̏I�_��Ԃ��B
		return to;
	}
}

void PlayerCamera::_Move()
{
	//�o�l�J����
	_SpringChaseMove(Time::DeltaTime());
}

void PlayerCamera::_SpringChaseMove(float time)
{
	auto& CameraPos = transform->GetPosition();
	auto& TargetPos = _Camera->GetTarget();
	//�ڕW�ɑ΂���J�����̌��݂̈ʒu����ɗ��z���ʊp���X�V�B
	//auto ang = atan2f(pos.y - trg.y, pos.x - trg.x);
	//���[���h�s��ł̃J�����̗��z�ʒu�B
	auto vIdealPos = _DestinationPos;

	//���̗��z�ʒu�ւ̃o�l�ɂ������x���v�Z�B
	auto vDisplace = CameraPos - vIdealPos;
	auto vSpringAccel = (-_Spring * vDisplace) - (_Damping * _Velocity);
	//�I�C���[�ϕ����g���ăJ�����̑��x�ƈʒu���X�V�B
	_Velocity += vSpringAccel * time * CAMERA_SPEED;
	transform->SetPosition(CameraPos + (_Velocity * time));
}