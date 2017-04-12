#include "Camera.h"
#include "EffectManager.h"

Camera::~Camera()
{
	SetViewPoint(nullptr);
}

void Camera::Awake()
{
	//行列初期化
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
		//注視点を使う
		D3DXMatrixLookAtLH(&_View, (D3DXVECTOR3*)&transform->GetPosition(), (D3DXVECTOR3*)_ViewPoint, (D3DXVECTOR3*)&transform->Direction(Vector3::up));
	}
	else
	{
		//逆行列を使う
		D3DXMatrixInverse(&_View, NULL, &transform->GetWorldMatrix());
	}
}

void Camera::ProjectionMatrixUpdate()
{
	// プロジェクション変換(射影変換)
	D3DXMatrixPerspectiveFovLH(
		&_Projection,
		D3DXToRadian(_ViewAngle),	//画角
		_Aspect,		//アスペクト比
		_near,			//クリップ距離(Near)
		_far);			//クリップ距離(far)
}