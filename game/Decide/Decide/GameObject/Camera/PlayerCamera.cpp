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
	const float CAMERA_SPEED = 2.5f;

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
	_Radius = 0.5f;
	_Sphere = AddComponent<SphereCollider>();
	_Sphere->Create(_Radius);
	//�����̉����Ə��
	_Dist = 5.0f;

}

void PlayerCamera::Start()
{
	//�v���C���[�̃|�W�V�����ւ̎Q�Ƃ��擾
	_PlayerPos = &_Player->transform->GetPosition();
	//���K������������
	static D3DXVECTOR3 baseDir(0.0f, 1.5f, -1.5f);
	D3DXVec3Normalize(&_ToPlayerDir, &baseDir);
	// �����l�ݒ�̂��ߏ������ĂԁB
	// �������ȁB
	{
		Move();
		_Camera->Update();
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

void PlayerCamera::_RotTransversal(float roty)
{
	D3DXQUATERNION mAxisY;
	D3DXVECTOR4 v;
	D3DXMATRIX rot;
	D3DXQuaternionRotationAxis(&mAxisY, &(const D3DXVECTOR3&)Vector3::up, roty);
	D3DXMatrixRotationQuaternion(&rot, &mAxisY);
	D3DXVec3Transform(&v, &_ToPlayerDir, &rot);
	_ToPlayerDir.x = v.x;
	_ToPlayerDir.y = v.y;
	_ToPlayerDir.z = v.z;
}

void PlayerCamera::_RotLongitudinal(float rotx)
{
	D3DXVECTOR3 Cross;
	D3DXQUATERNION zAxis;
	D3DXVECTOR4 v;
	D3DXMATRIX rot;
	//�O�ςŒ�������x�N�g�����擾
	D3DXVec3Cross(&Cross, &(const D3DXVECTOR3&)Vector3::up, &_ToPlayerDir);
	//
	D3DXQuaternionRotationAxis(&zAxis, &Cross, rotx);
	D3DXMatrixRotationQuaternion(&rot, &zAxis);
	D3DXVec3Transform(&v, &_ToPlayerDir, &rot);
	D3DXVECTOR3 oldDir = _ToPlayerDir;
	_ToPlayerDir.x = v.x;
	_ToPlayerDir.y = v.y;
	_ToPlayerDir.z = v.z;

	D3DXVECTOR3 toPosDir;
	D3DXVec3Normalize(&toPosDir, &_ToPlayerDir);
	//�J�����̏㉺�̏��
	if (toPosDir.y < -0.5f)
	{
		_ToPlayerDir = oldDir;
	}
	else if (toPosDir.y > 0.8f)
	{
		_ToPlayerDir = oldDir;
	}
}

void PlayerCamera::Move()
{
	//�v���C���[�ƃJ�����̋���
	Vector3 dist = (Vector3)(_ToPlayerDir * _Dist);
	Vector3 from, to;
	//�����_
	from = (*_PlayerPos) + PLAYER_HEIGHT;
	//�J�����̈ړ���
	to = from + dist;
	//���C���΂�
	// �Փ˂𖳎����鑮�������Z�B
	int attri = static_cast<int>(fbCollisionAttributeE::ALL) & ~(Collision_ID::ATTACK) & ~(Collision_ID::PLAYER) & ~(Collision_ID::ENEMY) & ~(Collision_ID::NOTHITCAMERA);
	fbPhysicsCallback::ClosestConvexResultCallback ray = INSTANCE(PhysicsWorld)->ClosestRayShape(_Sphere, from, to, attri);
	//�ړ���|�W�V����
	Vector3 next;
	//���C�������ɓ����������ǂ����H
	if (ray.hitObject)
	{
		//�Փ˓_�̖@�������ɔ��a���ړ�����x�N�g���B
		Vector3 normal = ray.hitNormal;
		normal.Scale(_Radius);
		//�Փ˓_����@���̕����ɉ����o���A�ړ�����Z�o�B
		next = ray.hitPos + normal;
	}
	else
	{
		//�ǂ��ɂ�������Ȃ������̂ŁA���C�̏I�_�Ɉړ�����B
		next = to;
	}
	//�ړ�
	transform->SetPosition(next);
}

/**
* �ʏ펞�̃J��������.
*/
void PlayerCamera::_StandardBehavior()
{
	//�E��]
	if (KeyBoardInput->isPressed(DIK_RIGHT) || (XboxInput(0)->GetAnalog(AnalogE::R_STICK).x / 32767.0f) > 0.1f)
	{
		_RotTransversal(CAMERA_SPEED * Time::DeltaTime());
	}
	//����]
	if (KeyBoardInput->isPressed(DIK_LEFT) || (XboxInput(0)->GetAnalog(AnalogE::R_STICK).x / 32767.0f) < -0.1f)
	{
		_RotTransversal(-CAMERA_SPEED * Time::DeltaTime());
	}
	//��
	if (KeyBoardInput->isPressed(DIK_UP) || (XboxInput(0)->GetAnalog(AnalogE::R_STICK).y / 32767.0f) > 0.1f)
	{
		_RotLongitudinal(CAMERA_SPEED * Time::DeltaTime());
	}
	//��
	if (KeyBoardInput->isPressed(DIK_DOWN) || (XboxInput(0)->GetAnalog(AnalogE::R_STICK).y / 32767.0f) < -0.1f)
	{
		_RotLongitudinal(-CAMERA_SPEED * Time::DeltaTime());
	}

	//�ړ�
	Move();

	//�v���C���[�̕�������
	transform->LockAt((*_PlayerPos) + PLAYER_HEIGHT);
	_Camera->SetTarget((*_PlayerPos) + PLAYER_HEIGHT);

}
