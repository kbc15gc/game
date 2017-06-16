#pragma once
#include "_Object\_Component\Component.h"

//方向ライト
struct DirectionalLight
{
public:
	DirectionalLight()
	{
		_Rotation = Vector3::zero;
		_Color = Color::white;
	}
	//オイラー角で設定
	void SetEulerAngles(const Vector3& angle)
	{
		//奥方向ベクトル
		D3DXVECTOR3 in = { 0.0f,0.0f,1.0f };
		D3DXVECTOR3 out;
		//回転行列作成
		D3DXMATRIX rotateMatrix;
		D3DXMatrixRotationYawPitchRoll(&rotateMatrix, D3DXToRadian(angle.y), D3DXToRadian(angle.x), D3DXToRadian(angle.z));
		//ベクトルを回転行列を使って回転
		D3DXVec3TransformCoord(&out, &in, &rotateMatrix);
		//セットして正規化
		_Rotation.Set(out);
		_Rotation.Normalize();
	}
	//向きで設定
	void SetDirection(const Vector3& dir)
	{
		_Rotation = dir;
	}
	//ライトの色を設定
	void SetColor(const Color& color)
	{
		_Color = color;
	}
	const Color& GetColor()
	{
		return _Color;
	}
	//方向を返す
	Vector4 Direction() const
	{
		return Vector4(_Rotation.x, _Rotation.y, _Rotation.z, 1.0f);
	}
private:
	Vector3 _Rotation;	//回転
	Color _Color;	//色
};

class Light :public Component
{
public:
	Light::Light(GameObject* g, Transform* t) :Component(g, t, typeid(this).name())
	{
		
	}
	Light::~Light();
	void Awake()override;
	//ライトの追加
	void AddLight(DirectionalLight* l);
	//ベクター取得
	const vector<DirectionalLight*>& GetLight();
	//ライトの数取得
	const int GetNum()const;

	/**
	* 環境光を取得.
	*/
	const Vector3& GetAmbientLight()
	{
		return _AmbientLight;
	}

	/**
	* 環境光を設定.
	*/
	void SetAmbientLight(const Vector3& amb)
	{
		_AmbientLight = amb;
	}

private:
	//ライトのポインタ達
	vector<DirectionalLight*> _LightVec;
	
	/** 環境光. */
	Vector3 _AmbientLight = Vector3(0.3f, 0.3f, 0.3f);

};