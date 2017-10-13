#include"fbstdafx.h"
#include "Camera.h"

Camera::~Camera()
{

}

void Camera::Awake()
{
	//行列初期化
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
		//注視点を使う
		D3DXMatrixLookAtLH(&_View, (D3DXVECTOR3*)&transform->GetPosition(), (D3DXVECTOR3*)_Target, (D3DXVECTOR3*)&transform->Direction(Vector3::up));
		//逆行列をワールド行列にする。
		D3DXMATRIX world;
		D3DXMatrixInverse(&world, NULL, &_View);
		//transform->SetWorldMatrix(world);
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

Vector2 Camera::WorldToScreen(Vector3 wpos)
{
	//受け取ったワールド座標へのベクトル(XZ)
	Vector3 ToW = wpos - transform->GetPosition();
	Vector2 ToWXZ = Vector2(ToW.x, ToW.z);
	ToWXZ.Normalize();
	//向きベクトル
	//Vector3 Dir = transform->Direction(Vector3::front);
	Vector3 Dir = transform->GetForward();
	Vector2 DirXZ = Vector2(Dir.x, Dir.z);
	DirXZ.Normalize();
	//内積　0～2の範囲が返るように調整
	float dot = fabs(DirXZ.Dot(ToWXZ) - 1);
	//θを角度に変換
	float vangle = D3DXToDegree(dot);

	//長さ
	float len = ToW.Length();

	//カメラに写っているかどうか？
	if ((_near < len && len < _far) &&	//near・farの範囲に収まっているか？
		(vangle < _ViewAngle))			//画角の中に収まっているか？
	{
		//ワールド座標
		D3DXVECTOR4 v = { wpos.x,wpos.y,wpos.z,1 };
		//ビュー変換
		D3DXVec4Transform(&v, &v, &_View);
		//プロジェクション変換
		D3DXVec4Transform(&v, &v, &_Projection);
		//各要素をwで割る
		v.x /= v.w;
		v.y /= v.w;
		v.z /= v.w;
		v.w /= v.w;
		//ビューポート行列作成
		D3DXMATRIX viewport;
		D3DXMatrixIdentity(&viewport);
		//スクリーンサイズの半分
		viewport._11 = g_WindowSize.x / 2;
		viewport._22 = -g_WindowSize.y / 2;
		viewport._41 = g_WindowSize.x / 2;
		viewport._42 = g_WindowSize.y / 2;
		//ビューポート変換
		D3DXVec4Transform(&v, &v, &viewport);

		return Vector2(v.x, v.y);
	}
	else
	{
		//絶対に映らないであろう位置を返す
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
	//逆行列に
	D3DXMatrixInverse(&viewportInv, NULL, &viewportInv);
	D3DXMatrixInverse(&projInv, NULL, &_Projection);
	D3DXMatrixInverse(&viewInv, NULL, &_View);
	//2D座標に逆行列をかけていく。
	D3DXVECTOR4 v = { spos.x,spos.y,0,1 };
	D3DXVec4Transform(&v, &v, &viewportInv);
	D3DXVec4Transform(&v, &v, &projInv);
	D3DXVec4Transform(&v, &v, &viewInv);

	return Vector3(v.x / v.w, v.y / v.w, v.z / v.w);
}

Collision* Camera::GetClickCollision(float rayLength, int attribute)
{
	//左クリック
	if (MouseInput->GetValue(MouseInE::L_CLICK))
	{
		//マウスカーソルのポジションをワールド座標に変換。始点とする。
		Vector3 start = this->ScreenToWorld(MouseInput->GetCursorPosOnWindow(g_MainWindow));
		//レイの方向計算。
		Vector3 dir = start - this->transform->GetPosition();
		dir.Normalize();
		//終点を計算。
		Vector3 end = start + (dir * rayLength);
		//レイを飛ばす
		fbPhysicsCallback::ClosestRayResultCallback callback = INSTANCE(PhysicsWorld)->ClosestRayTest(start, end, (const int)attribute);

		return callback.hitObject;
	}
	return nullptr;
}
