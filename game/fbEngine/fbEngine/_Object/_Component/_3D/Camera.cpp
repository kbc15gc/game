#include"fbstdafx.h"
#include "Camera.h"

Camera::~Camera()
{

}

void Camera::Awake()
{
	//�s�񏉊���
	D3DXMatrixIdentity(&_View);
	D3DXMatrixIdentity(&_Projection);

	_ViewAngle = 45.0f;
	_Aspect = (16.0f / 9.0f);
	_near = 1.0f;
	_far = 1000.0f;
}

void Camera::Start()
{
	
}

void Camera::Update()
{
	ViewMatrixUpdate();
	ProjectionMatrixUpdate();
}

void Camera::ViewMatrixUpdate()
{
	if (_Target)
	{
		//�����_���g��
		D3DXMatrixLookAtLH(&_View, (D3DXVECTOR3*)&transform->GetPosition(), (D3DXVECTOR3*)_Target, (D3DXVECTOR3*)&transform->Direction(Vector3::up));
		//�t�s������[���h�s��ɂ���B
		D3DXMATRIX world;
		D3DXMatrixInverse(&world, NULL, &_View);
		//transform->SetWorldMatrix(world);
	}
	else
	{
		//�t�s����g��
		D3DXMatrixInverse(&_View, NULL, &transform->GetWorldMatrix());
	}
}

void Camera::ProjectionMatrixUpdate()
{
	// �v���W�F�N�V�����ϊ�(�ˉe�ϊ�)
	D3DXMatrixPerspectiveFovLH(
		&_Projection,
		D3DXToRadian(_ViewAngle),	//��p
		_Aspect,		//�A�X�y�N�g��
		_near,			//�N���b�v����(Near)
		_far);			//�N���b�v����(far)
}

Vector2 Camera::WorldToScreen(Vector3 wpos)
{
	//�󂯎�������[���h���W�ւ̃x�N�g��(XZ)
	Vector3 ToW = wpos - transform->GetPosition();
	Vector2 ToWXZ = Vector2(ToW.x, ToW.z);
	ToWXZ.Normalize();
	//�����x�N�g��
	//Vector3 Dir = transform->Direction(Vector3::front);
	Vector3 Dir = transform->GetForward();
	Vector2 DirXZ = Vector2(Dir.x, Dir.z);
	DirXZ.Normalize();
	//���ρ@0�`2�͈̔͂��Ԃ�悤�ɒ���
	float dot = fabs(DirXZ.Dot(ToWXZ) - 1);
	//�Ƃ��p�x�ɕϊ�
	float vangle = D3DXToDegree(dot);

	//����
	float len = ToW.Length();

	//�J�����Ɏʂ��Ă��邩�ǂ����H
	if ((_near < len && len < _far) &&	//near�Efar�͈̔͂Ɏ��܂��Ă��邩�H
		(vangle < _ViewAngle))			//��p�̒��Ɏ��܂��Ă��邩�H
	{
		//���[���h���W
		D3DXVECTOR4 v = { wpos.x,wpos.y,wpos.z,1 };
		//�r���[�ϊ�
		D3DXVec4Transform(&v, &v, &_View);
		//�v���W�F�N�V�����ϊ�
		D3DXVec4Transform(&v, &v, &_Projection);
		//�e�v�f��w�Ŋ���
		v.x /= v.w;
		v.y /= v.w;
		v.z /= v.w;
		v.w /= v.w;
		//�r���[�|�[�g�s��쐬
		D3DXMATRIX viewport;
		D3DXMatrixIdentity(&viewport);
		//�X�N���[���T�C�Y�̔���
		viewport._11 = g_WindowSize.x / 2;
		viewport._22 = -g_WindowSize.y / 2;
		viewport._41 = g_WindowSize.x / 2;
		viewport._42 = g_WindowSize.y / 2;
		//�r���[�|�[�g�ϊ�
		D3DXVec4Transform(&v, &v, &viewport);

		return Vector2(v.x, v.y);
	}
	else
	{
		//��΂ɉf��Ȃ��ł��낤�ʒu��Ԃ�
		return Vector2(-99999, -99999);
	}
}

Vector3 Camera::ScreenToWorld(Vector2 spos)
{
	D3DXMATRIX viewportInv, projInv, viewInv;
	D3DXMatrixIdentity(&viewportInv);
	viewportInv._11 = g_WindowSize.x / 2;
	viewportInv._22 = -g_WindowSize.y / 2;
	viewportInv._41 = g_WindowSize.x / 2;
	viewportInv._42 = g_WindowSize.y / 2;
	//�t�s���
	D3DXMatrixInverse(&viewportInv, NULL, &viewportInv);
	D3DXMatrixInverse(&projInv, NULL, &_Projection);
	D3DXMatrixInverse(&viewInv, NULL, &_View);
	//2D���W�ɋt�s��������Ă����B
	D3DXVECTOR4 v = { spos.x,spos.y,0,1 };
	D3DXVec4Transform(&v, &v, &viewportInv);
	D3DXVec4Transform(&v, &v, &projInv);
	D3DXVec4Transform(&v, &v, &viewInv);

	return Vector3(v.x / v.w, v.y / v.w, v.z / v.w);
}

Collision* Camera::GetClickCollision(float rayLength, int attribute)
{
	//���N���b�N
	if (MouseInput->GetValue(MouseInE::L_CLICK))
	{
		//�}�E�X�J�[�\���̃|�W�V���������[���h���W�ɕϊ��B�n�_�Ƃ���B
		Vector3 start = this->ScreenToWorld(MouseInput->GetCursorPosOnWindow(g_MainWindow));
		//���C�̕����v�Z�B
		Vector3 dir = start - this->transform->GetPosition();
		dir.Normalize();
		//�I�_���v�Z�B
		Vector3 end = start + (dir * rayLength);
		//���C���΂�
		fbPhysicsCallback::ClosestRayResultCallback callback = INSTANCE(PhysicsWorld)->ClosestRayTest(start, end, (const int)attribute);

		return callback.hitObject;
	}
	return nullptr;
}
