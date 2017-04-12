#include "Camera.h"
#include "EffectManager.h"

Camera::~Camera()
{
	SetViewPoint(nullptr);
}

void Camera::Awake()
{
	//�s�񏉊���
	D3DXMatrixIdentity(&_View);
	D3DXMatrixIdentity(&_Projection);

	_ViewPoint = nullptr;
	_ViewAngle = 45;
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
	if (_ViewPoint)
	{
		//�����_���g��
		D3DXMatrixLookAtLH(&_View, (D3DXVECTOR3*)&transform->GetPosition(), (D3DXVECTOR3*)_ViewPoint, (D3DXVECTOR3*)&transform->Direction(Vector3::up));
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