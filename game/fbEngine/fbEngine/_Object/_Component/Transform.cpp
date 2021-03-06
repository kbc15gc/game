#include"fbstdafx.h"
#include "Transform.h"

Transform::Transform(GameObject * g, Transform * t) :
	Component(g, t, typeid(this).name(),0),
	_Parent(nullptr),
	_LocalPosition(Vector3::zero),
	_Position(Vector3::zero),
	_LocalScale(Vector3::one),
	_Scale(Vector3::one),
	_LocalAngle(Vector3::zero),
	_Angle(Vector3::zero),
	_LocalRotation(Quaternion::Identity),
	_Rotation(Quaternion::Identity)
{
	_Children.clear();
	D3DXMatrixIdentity(&_RotateMatrix);
	D3DXMatrixIdentity(&_WorldMatrix);
#ifdef _DEBUG
	mbstowcs_s(nullptr, name, typeid(*this).name(), strlen(typeid(*this).name()));
#endif
}

Transform::~Transform()
{
	//親が居るなら
	if(_Parent)
	{
		//親の子から自分を外す
		SetParent(nullptr);
	}
	//子がいるなら
	if(_Children.size() > 0)
	{
		//子の親をnullに
		vector<Transform*>::iterator it = _Children.begin();
		while (it != _Children.end())
		{
			(*it)->_LocalPosition = (*it)->_Position;
			(*it)->_LocalScale = (*it)->_Scale;
			(*it)->_LocalAngle = (*it)->_Angle + (*it)->_Parent->_Angle;
			(*it)->_LocalRotation = (*it)->_Rotation;
			(*it)->_Parent = nullptr;
			(*it)->UpdateTransform();

			it = _Children.erase(it);
		}
	}
}

void Transform::Awake()
{
	
}

void Transform::Start()
{
	
}

void Transform::Update()
{
	
}

//子供を検索する関数
//戻り値：Transform* ヒットした子のアドレス
//第一引数：char* 子供の名前
Transform* Transform::FindChild(char* name)
{
	//イテレータ取得
	vector<Transform*>::iterator it = _Children.begin();
	//最後まで見る
	while (it != _Children.end())
	{
		//子の名前取得
		const char* childname = (*it)->gameObject->GetName();
		//名前比較
		if(strcmp(childname,name) == 0)
		{
			return (*it);
		}
		it++;
	}

	return nullptr;
}
Transform * Transform::FindChild(unsigned int idx)
{
	//範囲外アクセスをはじく
	if (_Children.size() <= idx)
	{
		return nullptr;
	}
	//添え字番目を返す
	return _Children[idx];
}
//未実装
Transform ** Transform::FindChilds(char * name)
{
	vector<Transform*> v;
	//イテレータ取得
	vector<Transform*>::iterator it = _Children.begin();
	//最後まで見る
	while (it != _Children.end())
	{
		//子の名前取得
		const char* childname = (*it)->gameObject->GetName();
		//名前比較
		if (strcmp(childname, name) == 0)
		{
			
		}
		it++;
	}

	return nullptr;
}

//ローカルからトランスフォームを更新
//親から更新していくべき
//戻り値：なし
//引数：なし
void Transform::UpdateTransform()
{
	//親が居るなら
	if (_Parent) {
		//親のワールド行列を取得。
		D3DXMATRIX PWorld = _Parent->GetWorldMatrix();
		//子のワールド座標。
		D3DXVECTOR4 pos;
		//子のローカル座標。
		D3DXVECTOR3 lpos;
		//ローカルをコピー。
		_LocalPosition.CopyFrom(lpos);
		//親のワールド行列を乗算して、ローカル座標をワールド座標に変換する。
		//子のローカル座標×親のワールド行列。
		D3DXVec3Transform(&pos, &lpos, &PWorld);
		//ワールド座標。
		_Position = Vector3(pos.x, pos.y, pos.z);
		//スケール値
		SetLocalScale(_LocalScale);
		//オイラー角
		_Angle = _LocalAngle - _Parent->_Angle;
		//クォータニオン計算。
		_Rotation = _LocalRotation * _Parent->GetRotation();
	}
	else {
		//ローカルをそのまま
		_Position = _LocalPosition;
		_Scale = _LocalScale;
		_Angle = _LocalAngle;
		_Rotation = _LocalRotation;
	}
	//回転行列取得
	_RotateMatrix = _Rotation.GetRotationMatrix();

	UpdateWolrdMatrix();
}

//トランスフォームからワールド行列更新
//戻り値：なし
//引数：なし
void Transform::UpdateWolrdMatrix()
{
	//初期化
	D3DXMATRIX Scale, Pos;
	D3DXMatrixIdentity(&Scale);
	D3DXMatrixIdentity(&Pos);
	//スケール
	D3DXMatrixScaling(&Scale, _Scale.x, _Scale.y, _Scale.z);
	//ポジション
	D3DXMatrixTranslation(&Pos, _Position.x, _Position.y, _Position.z);
	//ワールド行列設定
	_WorldMatrix = Scale * _RotateMatrix * Pos;

	_Right = Vector3(_WorldMatrix.m[0][0], _WorldMatrix.m[0][1], _WorldMatrix.m[0][2]);
	_Right.Normalize();
	_Up = Vector3(_WorldMatrix.m[1][0], _WorldMatrix.m[1][1], _WorldMatrix.m[1][2]);
	_Up.Normalize();
	_Forward = Vector3(_WorldMatrix.m[2][0], _WorldMatrix.m[2][1], _WorldMatrix.m[2][2]);
	_Forward.Normalize();

	//子も更新する
	for each (Transform* t in _Children)
	{
		t->UpdateTransform();
	}
}

const Vector3& Transform::GetRight()
{
	return _Right;	// ワールド行列のX成分を返す。
}

const Vector3& Transform::GetUp()
{
	return _Up;	// ワールド行列のY成分を返す。
}

const Vector3& Transform::GetForward()
{
	return _Forward;;	// ワールド行列のZ成分を返す。
}

Vector3 Transform::Direction(const Vector3& v)
{
	D3DXVECTOR3 in;
	D3DXVECTOR3 out;
	v.CopyFrom(in);
	//受け取ったベクトルを回転行列を使って回転させる。
	D3DXVec3TransformCoord(&out, &in, &_RotateMatrix);

	Vector3 r = out;
	return r;
}

Vector3 Transform::Local(const Vector3& v)
{
	D3DXVECTOR4 pos;	//ポジション
	D3DXVECTOR3 lpos;	//ローカルポジション
	//vをコピー
	v.CopyFrom(lpos);
	//ワールド行列から見たローカルポジションをposに格納
	//ワールド行列を使って移動させる。
	D3DXVec3Transform(&pos, &lpos, &_WorldMatrix);
	return Vector3(pos.x, pos.y, pos.z);
}

Vector3 Transform::LocalPos(const Vector3& v)
{
	D3DXVECTOR4 pos;
	D3DXVECTOR3 lpos;
	v.CopyFrom(lpos);
	D3DXMATRIX offset;
	D3DXMatrixIdentity(&offset);
	offset._41 = _WorldMatrix._41;
	offset._42 = _WorldMatrix._42;
	offset._43 = _WorldMatrix._43;
	//移動行列を使って移動させる
	D3DXVec3Transform(&pos, &lpos, &offset);
	return Vector3(pos.x, pos.y, pos.z);
}

void Transform::LockAt(const Vector3 & wpos)
{
	//注視点と視点
	D3DXVECTOR3 target, eye;
	D3DXMATRIX view;
	wpos.CopyFrom(target);
	this->_Position.CopyFrom(eye);
	//ビュー行列初期化
	D3DXMatrixIdentity(&view);
	//ビュー行列を作成
	D3DXMatrixLookAtLH(&view, &eye, &target, (D3DXVECTOR3*)&Vector3::up);
	//ビュー行列を逆行列にしてワールド行列に
	D3DXMatrixInverse(&_RotateMatrix, NULL, &view);
	// オフセットを切って回転行列だけにしてしまう
	_RotateMatrix._41 = 0.0f;
	_RotateMatrix._42 = 0.0f;
	_RotateMatrix._43 = 0.0f;

	//回転行列からクォータニオン生成
	D3DXQUATERNION q;
	D3DXQuaternionRotationMatrix(&q, &_RotateMatrix);
	//メンバ変数のクォータニオンにコピー
	_Rotation = q;
	SetRotation(_Rotation);
	//クォータニオンからオイラー角を求める
	//SetAngle(_Rotation.GetAngle());
	//ワールド行列を求める。
	UpdateWolrdMatrix();
}

void Transform::LockAt(GameObject * obj)
{
	LockAt(obj->transform->_Position);
}

//トランスフォーム解放
void Transform::Release()
{
	vector<Transform*>::iterator it = _Children.begin();
	while (it != _Children.end())
	{
		//親を削除
		(*it)->_Parent = nullptr;
		it++;
	}
	_Children.clear();
}

void Transform::AddChild(Transform* t) {
	bool isRegistered = false;	// すでに子ども登録されているか。
	for (auto child : _Children) {
		if (child == t) {
			// 登録済み。
			return;
		}
	}
	
	// まだ登録されていない。
	_Children.push_back(t);	//子に登録。
}

void Transform::RemoveChild(Transform * t)
{
	vector<Transform*>::iterator it = _Children.begin();
	while (it != _Children.end())
	{
		//アドレス比較
		if((*it) == t)
		{
			(*it)->_Parent = nullptr;
			_Children.erase(it);
			return;
		}
		it++;
	}
}

void Transform::SetParent(Transform * parent)
{
	if(_Parent)
	{
		// 元々親がいた。
		if (parent == nullptr)
		{
			// 親が外された。
			_LocalPosition = _Position;
			_LocalScale = _Scale;
			_LocalAngle = _Angle;// +_Parent->_Angle;
			_LocalRotation = _Rotation;
		}
		// 親を外すので、現在の親の子供リストから自分を外す。
		this->_Parent->RemoveChild(this);
	}

	// 作業用。
	Transform* oldParent = this->_Parent;

	//親に登録。
	// ※下記の処理で更新後の親を登録している必要があるのでここで登録。
	this->_Parent = parent;

	if (parent && oldParent != parent) {
		// 親が設定された。
		//新しい親から
		//新しいローカルを計算する。
		SetPosition(_Position);
		SetAngle(_Angle);
		SetScale(_Scale);
		SetRotation(_Rotation);


		// 親の子供に自分を追加。
		parent->AddChild(this);

		//親の設定を取得
		if (gameObject) {
			if (parent->gameObject) {
				gameObject->SetDiscard(parent->gameObject->GetDiscard());
			}
		}
	}

	UpdateTransform();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//セッター・ゲッター

void Transform::SetLocalPosition(const Vector3& v)
{
	_LocalPosition = v;

	//ローカルから最終的なポジションを計算
	if (_Parent)
	{
		D3DXMATRIX pworld = _Parent->GetWorldMatrix();

		D3DXVECTOR3 lpos;
		D3DXVECTOR4 pos;
		//ローカルをコピー
		_LocalPosition.CopyFrom(lpos);
		//親のワールド行列を乗算して、ローカル座標をワールド座標に変換する。
		D3DXVec3Transform(&pos, &lpos, &pworld);
		//ポジション
		_Position.x = pos.x;
		_Position.y = pos.y;
		_Position.z = pos.z;
	}
	else
	{
		//ローカルそのまま
		_Position = _LocalPosition;
	}
	//ワールド行列更新
	UpdateWolrdMatrix();
}

void Transform::SetPosition(const Vector3 & v)
{
	_Position = v;
	//最終的なポジションからローカルを逆算
	if (_Parent)
	{
		D3DXMATRIX pworldInv = _Parent->GetWorldMatrix();
		//逆行列に
		D3DXMatrixInverse(&pworldInv, NULL, &pworldInv);
		D3DXVECTOR4 lpos;
		D3DXVECTOR3 pos;
		//コピー
		_Position.CopyFrom(pos);
		//逆行列を乗算して、ローカルを求める。
		D3DXVec3Transform(&lpos, &pos, &pworldInv);
		
		_LocalPosition.x = lpos.x;
		_LocalPosition.y = lpos.y;
		_LocalPosition.z = lpos.z;
	}
	else
	{
		//そのままローカルに
		_LocalPosition = _Position;
	}
	//ワールド行列更新
	UpdateWolrdMatrix();
}

void Transform::SetLocalScale(const Vector3& v)
{
	_LocalScale = v;
	//ローカルから最終的なスケール計算
	if (_Parent) {
		//ローカルスケール×親のスケール
		_Scale = _LocalScale * _Parent->_Scale;
	}
	else
	{
		//ローカルそのまま
		_Scale = _LocalScale;
	}
	//ワールド行列更新
	UpdateWolrdMatrix();
}

void Transform::SetScale(const Vector3& v)
{
	_Scale = v;
	//最終的なスケールからローカルを逆算
	if (_Parent)
	{
		//ローカルスケールを求める
		_LocalScale = _Scale / _Parent->_Scale;
	}
	else
	{
		//そのままローカルに
		_LocalScale = _Scale;
	}
	//ワールド行列更新
	UpdateWolrdMatrix();
}

void Transform::SetLocalAngle(const Vector3& v, bool update)
{
	//ローカルアングル決定。
	_LocalAngle = v;
	//ローカルから最終的な角度計算
	if (_Parent) {
		//親のクォータニオン取得
		Quaternion prot = _Parent->GetRotation();
		//各軸の回転量を取得
		Vector3 pang = prot.GetAngle();
		//足す
		_Angle = _LocalAngle + pang;
	}
	else
	{
		//ローカルそのまま
		_Angle = _LocalAngle;
	}

	//クォータニオン計算。
	if (update)
	{
		//クォータニオン更新
		_LocalRotation.SetEuler(_LocalAngle);
		//クォータニオン設定
		SetLocalRotation(_LocalRotation,false);
	}
}

void Transform::SetAngle(const Vector3& v, bool update)
{
	_Angle = v;
	//ローカルの角度逆算。
	if (_Parent) {
		//親のクォータニオン取得
		Quaternion prot = _Parent->GetRotation();
		//各軸の回転量を取得
		Vector3 pang = prot.GetAngle();
		//引く
		_LocalAngle = _Angle - pang;
	}
	else
	{
		//ローカルそのまま
		_LocalAngle = _Angle;
	}

	//クォータニオン計算。
	if (update)
	{
		//クォータニオン更新
		_Rotation.SetEuler(_Angle);
		//クォータニオン設定
		SetRotation(_Rotation,false);
	}
}

void Transform::SetLocalRotation(const Quaternion & q, bool update)
{
	_LocalRotation = q;
	//親がいるかどうか？
	if (_Parent) {
		//親のクォータニオン取得
		Quaternion prot = _Parent->GetRotation();
		//ローカルと掛ける
		//_Rotation = _LocalRotation * prot;
		_Rotation = prot * _LocalRotation;
	}
	else
	{
		//ローカルそのまま
		_Rotation = _LocalRotation;
	}
	//回転行列更新
	_RotateMatrix = _Rotation.GetRotationMatrix();

	//オイラー角の更新。
	if (update)
	{
		SetLocalAngle(_LocalRotation.GetAngle(), false);
	}

	//ワールド行列更新
	UpdateWolrdMatrix();
}

void Transform::SetRotation(const Quaternion& q, bool update)
{
	_Rotation = q;
	//親がいるかどうか？
	if (_Parent) {
		//親のクォータニオン取得
		Quaternion protInv = _Parent->GetRotation();
		//逆クォータニオンにする。
		protInv.Inverse();
		//ローカルを逆算
		_LocalRotation = _Rotation * protInv;
		_LocalRotation.GetAngle();
	}
	else
	{
		//ローカルそのまま
		_LocalRotation = _Rotation;
	}
	//回転行列更新
	_RotateMatrix = _Rotation.GetRotationMatrix();
	//オイラー角の更新。
	if (update)
	{
		SetAngle(_Rotation.GetAngle(), false);
	}
	//ワールド行列更新
	UpdateWolrdMatrix();
}

void Transform::SetRotateMatrix(const D3DXMATRIX r)
{
	_RotateMatrix = r;
	//回転行列からクォータニオン生成
	D3DXQUATERNION q;
	D3DXQuaternionRotationMatrix(&q, &_RotateMatrix);
	//メンバ変数のクォータニオンに
	SetRotation(q);
	UpdateWolrdMatrix();
}

void Transform::SetWorldMatrix(D3DXMATRIX w)
{
	_WorldMatrix = w;
}
//ゲッター
const Vector3& Transform::GetLocalPosition()const
{
	return _LocalPosition;
}

const Vector3& Transform::GetPosition()const
{
	return _Position;
}

const Vector3& Transform::GetLocalScale()const
{
	return _LocalScale;
}

const Vector3& Transform::GetScale()const
{
	return _Scale;
}

const Vector3& Transform::GetLocalAngle()const
{
	return _LocalAngle;
}

const Vector3& Transform::GetAngle()const
{
	return _Angle;
}

const Quaternion& Transform::GetLocalRotation()const
{
	return _LocalRotation;
}

const Quaternion& Transform::GetRotation()const
{
	return _Rotation;
}

const D3DXMATRIX & Transform::GetWorldMatrix()const
{
	return _WorldMatrix;
}

const D3DXMATRIX & Transform::GetRotateMatrix()const
{
	return _RotateMatrix;
}

D3DXMATRIX * Transform::GetRotateMatrixAddress()
{
	return &_RotateMatrix;
}