#pragma once
#include "_Object\_Component\Component.h"

class Camera;

//トランスフォームクラス
class Transform : public Component
{
public:
	Transform(GameObject* g, Transform* t);
	~Transform();

	void Awake()override;
	void Start()override;
	void Update()override;

	//子供を検索する関数
	//戻り値：Transform* 一番最初にヒットした子のアドレス
	//第一引数：char* 子供の名前
	Transform* FindChild(char* name);
	//添え字で取得
	Transform* FindChild(unsigned int idx);
	//子供を検索する関数(未実装)
	//戻り値：Transform** ヒットした子達のアドレスの配列
	//第一引数：char* 子供の名前
	Transform** FindChilds(char* name);

	//ローカルからトランスフォームを更新
	void UpdateTransform();

	//トランスフォームからワールド行列更新
	void UpdateWolrdMatrix();

	// 右方向のベクトル取得。
	const Vector3& GetRight();
	// 上方向のベクトル取得。
	const Vector3& GetUp();
	//前向きのベクトル取得。
	const Vector3& GetForward();
	//受け取ったベクトルをこいつから見た向きに変換
	Vector3 Direction(const Vector3& v);
	//ローカルな位置に変換(回転込み)
	Vector3 Local(const Vector3& v);
	//ローカルな位置に変換(回転抜き)
	Vector3 LocalPos(const Vector3& v);
	//受け取ったオブジェクトの向きを見る
	void LockAt(const Vector3& wpos);
	void LockAt(GameObject* obj);

	//解放
	void Release();

	// 子を追加。
	void AddChild(Transform* t);
	//子を外す
	void RemoveChild(Transform* t);

	//親を返す関数
	//戻り値：Transform* 親のアドレス
	Transform* Transform::GetParent()
	{
		return _Parent;
	};

	//引数のトランスフォームを自分の親に登録する
	//第一引数：Transform
	void Transform::SetParent(Transform* _Parent);

	//子供たち取得
	const vector<Transform*>& GetChildren()
	{
		return _Children;
	};

	//各セッター・ゲッター

	void SetLocalPosition(const Vector3& v);
	inline void SetLocalPosition(float x, float y, float z) {
		SetLocalPosition(Vector3(x, y, z));
	}
	void SetPosition(const Vector3& v);
	inline void SetPosition(float x, float y, float z) {
		SetPosition(Vector3(x, y, z));
	}
	void SetLocalScale(const Vector3& v);
	inline void SetLocalScale(float x, float y, float z) {
		SetLocalScale(Vector3(x, y, z));
	}
	void SetScale(const Vector3& v);
	inline void SetScale(float x, float y, float z) {
		SetScale(Vector3(x, y, z));
	}
	void SetLocalAngle(const Vector3& v, bool update = true);
	inline void SetLocalAngle(float x, float y, float z, bool update = true) {
		SetLocalAngle(Vector3(x, y, z),update);
	}
	void SetAngle(const Vector3& v, bool update = true);
	inline void SetAngle(float x, float y, float z, bool update = true) {
		SetAngle(Vector3(x, y, z),update);
	}
	void SetLocalRotation(const Quaternion& q, bool update = true);
	inline void SetLocalRotation(float x, float y, float z, float w, bool update = true) {
		SetLocalRotation(Quaternion(x, y, z, w),update);
	}
	void SetRotation(const Quaternion& q, bool update = true);
	inline void SetRotation(float x, float y, float z, float w, bool update = true) {
		SetRotation(Quaternion(x, y, z, w),update);
	}
	void SetRotateMatrix(const D3DXMATRIX r);
	void SetWorldMatrix(D3DXMATRIX w);

	const Vector3&	GetLocalPosition()const;
	const Vector3& GetPosition()const;
	const Vector3& GetLocalScale()const;
	const Vector3& GetScale()const;
	const Vector3&	GetLocalAngle()const;
	const Vector3& GetAngle()const;
	const Quaternion& GetLocalRotation()const;
	const Quaternion& GetRotation()const;
	D3DXMATRIX* GetRotateMatrixAddress();
	const D3DXMATRIX& GetRotateMatrix()const;
	const D3DXMATRIX& GetWorldMatrix()const;
private:
	Transform* _Parent;		//親のアドレス
	vector<Transform*> _Children;		//子供達のアドレスを格納した
	
	Vector3	_Position;		//最終ポジション
	Vector3	_LocalPosition;	//ローカルポジション
	Vector3	_Scale;			//最終スケール
	Vector3	_LocalScale;	//ローカルスケール
	Vector3	_Angle;			//角度		（あてにしない方がいいかも）
	Vector3	_LocalAngle;	//ローカル角度
	Quaternion _Rotation;		//回転（クウォータニオン）
	Quaternion _LocalRotation;	//ローカル回転（クウォータニオン）
	D3DXMATRIX _RotateMatrix;	//回転行列
	D3DXMATRIX _WorldMatrix;	//ワールド行列
	Vector3 _Right;				//ワールド座標での右方向。
	Vector3 _Up;				//ワールド座標での上方向。
	Vector3 _Forward;			//ワールド座標での前方向。
};