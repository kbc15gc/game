/**
* シャドウマップクラスの定義.
*/
#pragma once

#include"_Effect\Blur.h"
#include"_Object\_Component\_3D\Camera.h"

/**
* シャドウマップクラス.
*/
class ShadowMap : Noncopyable
{
public:

	/** シャドウマップテクスチャの数. */
	static const int SHADOWMAP_NUM = 3;

	/**
	* シャドウレシーバー用パラメータ.
	*/
	struct ShadowReceiverParam
	{
	public:

		/** ライトビュープロジェクション行列. */
		D3DXMATRIX _LVPMatrix[SHADOWMAP_NUM];
		/** シャドウマップテクスチャの数. */
		int _ShadowMapNum = SHADOWMAP_NUM;
		/** バリアンスシャドウマップをするか. */
		int _IsVSM = true;
	};

public:

	/**
	* コンストラクタ.
	*/
	ShadowMap();

	/**
	* デストラクタ.
	*/
	~ShadowMap()
	{
	}

	/**
	* 作成.
	*/
	void Create();

	/**
	* 更新.
	*/
	void Update();

	/**
	* 描画.
	*/
	void Render();

	/**
	* 解放.
	*/
	void Release();

	/**
	* キャスターを登録する.
	*
	* @param model	スキンモデルデータ.
	*/
	void EntryModel(SkinModel* model)
	{
		_CasterModelList.push_back(model);
	}

	/**
	* ライトの視点を設定.
	*/
	void SetLightPosition(const Vector3& pos)
	{
		_LightPosition = pos;
	}

	/**
	* ライトの注視点を設定.
	*/
	void SetLightTarget(const Vector3& target)
	{
		_LightTarget = target;
	}

	/** 
	* ライトビュー行列の取得.
	*/
	D3DXMATRIX* GetLVMatrix()
	{
		return &_LVMatrix[_NowRenderShadowMap];
	}

	/**
	* ライトプロジェクション行列の取得.
	*/
	D3DXMATRIX* GetLPMatrix()
	{
		return &_LPMatrix[_NowRenderShadowMap];
	}

	/**
	* シャドウレシーバー用パラメータの取得.
	*/
	ShadowReceiverParam& GetShadowReceiverParam()
	{
		return _ShadowReceiverParam;
	}

	/**
	* 深度書き込みテクスチャの取得.
	*/
	IDirect3DTexture9* GetShadowMapTexture(int idx,bool isVSM = true)
	{
		if (isVSM)
		{
			return _Blur[idx].GetTexture()->pTexture;
		}
		return _ShadowMapRT[idx].texture->pTexture;
	}

	TEXTURE* GetTexture(int idx, bool isVSM = true)
	{
		if (isVSM)
		{
			return _Blur[idx].GetTexture();
		}
		return _ShadowMapRT[idx].texture;
	}

	///**
	//* 一枚目のテクスチャのサイズを取得.
	//*/
	//const Vector2& GetSize()
	//{
	//	return _ShadowArea[0];
	//}

	/**
	* カメラのポインタを設定.
	*/
	void SetCamera(Camera* camera)
	{
		_Camera = camera;
	}

	void SetNear(float n)
	{
		_Near = n;
	}

	void SetFar(float f)
	{
		_Far = f;
	}

private:

	/**
	* ライトビュープロジェクション行列を計算.
	*/
	void CalcLVPMatrixFromCamera();

private:

	/** 深度用レンダリングターゲット. */
	RenderTarget _ShadowMapRT[SHADOWMAP_NUM];

	/** 現在描画中のレンダリングターゲット. */
	int _NowRenderShadowMap = -1;

	/** シャドウキャスターのリスト. */
	vector<SkinModel*> _CasterModelList;

	/** ライトの視点. */
	Vector3 _LightPosition = Vector3::up;
	/** ライトの方向. */
	Vector3 _LightDirection = Vector3::down;
	/** ライトの注視点. */
	Vector3 _LightTarget = Vector3::zero;

	/** 近平面. */
	float _Near = 2.0f;
	/** 遠平面. */
	float _Far = 100.0f;
	/** アスペクト比. */
	float _Aspect = 1.0f;

	/** ライトビュー行列. */
	D3DXMATRIX _LVMatrix[SHADOWMAP_NUM];
	/** ライトプロジェクション行列. */
	D3DXMATRIX _LPMatrix[SHADOWMAP_NUM];

	/** シャドウレシーバー用のパラメータ. */
	ShadowReceiverParam _ShadowReceiverParam;

	/** ブラークラス. */
	Blur _Blur[SHADOWMAP_NUM];

	/** カメラクラスのポインタ. */
	Camera* _Camera = nullptr;

};