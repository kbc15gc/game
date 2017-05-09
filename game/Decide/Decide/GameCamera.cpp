#include "GameCamera.h"
#include "fbEngine\_Object\_Component\_3D\Camera.h"
#include "GameObject\Player\Player.h"
#include "GameObject\HistoryBook\HistoryBook.h"

namespace
{
	const Vector3 PLAYER_HEIGHT(0.0f, 1.5f, 0.0f);
	const float CAMERA_SPEED = 1.0f;
}

void GameCamera::Awake()
{
	//�J�����R���|�[�l���g
	_Camera = AddComponent<Camera>();
	_Camera->SetNear(1);
	_Camera->SetFar(1000);
	INSTANCE(GameObjectManager)->mainCamera = _Camera;

	//�J�����̃R���W�����̔��a�ݒ�
	_Radius = 0.5f;
	_Sphere = AddComponent<SphereCollider>();
	_Sphere->Create(_Radius);
	//�����̉����Ə��
	_Dist = 7.0f;
}

void GameCamera::Start()
{
	//�v���C���[������
	_Player = (Player*)INSTANCE(GameObjectManager)->FindObject("Player");
	//�v���C���[�̃|�W�V�����ւ̎Q�Ƃ��擾
	_PlayerPos = &_Player->transform->GetPosition();
	//���K����������������
	D3DXVec3Normalize(&_ToPlayerDir, &D3DXVECTOR3(0.0f, 3.0f, -4.0f));

	//���j���������B
	_HistoryBook = (HistoryBook*)INSTANCE(GameObjectManager)->FindObject("HistoryBook");
}

void GameCamera::Update()
{
	if (true)
	{
		_StandardBehavior();
	}
	else
	{
		_HistoryBehavior();
	}
}

void GameCamera::RotTransversal(float roty)
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

void GameCamera::RotLongitudinal(float rotx)
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

void GameCamera::_Move()
{
	//�v���C���[�ƃJ�����̋���
	Vector3 dist = (Vector3)(_ToPlayerDir * _Dist);
	Vector3 from, to;
	//�����_
	from = (*_PlayerPos) + PLAYER_HEIGHT;
	//�J�����̈ړ���
	to = from + dist;
	//���C���΂�
	fbPhysicsCallback::ClosestConvexResultCallback ray = INSTANCE(PhysicsWorld)->ClosestRayShape(_Sphere,from, to);
	//�ړ���|�W�V����
	Vector3 next;
	//���C�������ɓ����������ǂ����H
	if(ray.hitObject)
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

void GameCamera::_StandardBehavior()
{
	//�E��]
	if (KeyBoardInput->isPressed(DIK_RIGHT) || (XboxInput(0)->GetAnalog(AnalogInputE::R_STICK).x / 32767.0f) > 0.1f)
	{
		RotTransversal(CAMERA_SPEED * Time::DeltaTime());
	}
	//����]
	if (KeyBoardInput->isPressed(DIK_LEFT) || (XboxInput(0)->GetAnalog(AnalogInputE::R_STICK).x / 32767.0f) < -0.1f)
	{
		RotTransversal(-CAMERA_SPEED * Time::DeltaTime());
	}
	//��
	if (KeyBoardInput->isPressed(DIK_UP) || (XboxInput(0)->GetAnalog(AnalogInputE::R_STICK).y / 32767.0f) > 0.1f)
	{
		RotLongitudinal(CAMERA_SPEED * Time::DeltaTime());
	}
	//��
	if (KeyBoardInput->isPressed(DIK_DOWN) || (XboxInput(0)->GetAnalog(AnalogInputE::R_STICK).y / 32767.0f) < -0.1f)
	{
		RotLongitudinal(-CAMERA_SPEED * Time::DeltaTime());
	}

	//�ړ�
	_Move();
	//�v���C���[�̕�������
	transform->LockAt((*_PlayerPos) + PLAYER_HEIGHT);
}

void GameCamera::_HistoryBehavior()
{

	//�J�����̈ʒu���v���C���[�̊炮�炢�ɐݒ�B
	transform->SetLocalPosition((*_PlayerPos) + PLAYER_HEIGHT);

	_HistoryBookPos = &_HistoryBook->transform->GetPosition();

	//�J�����̒����_��HistoryBook�̃��f���ɐݒ�B
	transform->LockAt((*_HistoryBookPos));
}
