/**
* 環境マップクラスの定義.
*/
#pragma once

/**
* 環境マップクラス.
*/
class EnvironmentMap : Noncopyable
{
public:

	/**
	* コンストラクタ.
	*/
	EnvironmentMap()
	{
	}

	/**
	* デストラクタ.
	*/
	~EnvironmentMap()
	{
	}

	/**
	* 作成.
	*/
	bool Create();

	/**
	* 更新.
	*/
	void Update();

	/**
	* 描画.
	*/
	void Render();

	/**
	* 環境マップに書き込むモデルの追加.
	*/
	void EntryModel(SkinModel* model)
	{
		_ModelList.push_back(model);
	}

	/**
	* ビュー行列を取得.
	*/
	const D3DXMATRIX& GetViewMatrix()
	{
		return _ViewMatrix;
	}

	/**
	* プロジェクション行列を取得.
	*/
	const D3DXMATRIX& GetProjMatrix()
	{
		return _ProjMatrix;
	}

	/**
	* キューブテクスチャを取得.
	*/
	IDirect3DCubeTexture9* GetCubeTexture()
	{
		return _D3DCubeTexture;
	}

private:

	/** キューブテクスチャ. */
	IDirect3DCubeTexture9* _D3DCubeTexture = nullptr;

	/** モデルリスト. */
	vector<SkinModel*> _ModelList;

	/** ビュー行列. */
	D3DXMATRIX _ViewMatrix;
	/** プロジェクション行列. */
	D3DXMATRIX _ProjMatrix;

	/** 描画フラグ. */
	bool _isRender = true;

	/** 何秒に一度更新するか. */
	float _RenderTime = 5.0f;
	float _LocalTime = 0.0f;

};
