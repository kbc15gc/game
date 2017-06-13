#pragma once
#include "_Object\_Component\Component.h"

class Effect;
class Camera;
class Light;
class ShadowCamera;

//エフェクトをかけるか？
enum ModelEffectE
{
	NONE = BIT(0),				//なし
	CAST_SHADOW = BIT(1),		//影を作る
	RECEIVE_SHADOW = BIT(2),	//影を落とす
	SPECULAR = BIT(3),			//鏡面反射
	TOON = BIT(4),				//トゥーン
	LIMLIGHT = BIT(5),			//リムライト
	CAST_ENVIRONMENT = BIT(6),	//環境マップを作る.
};

//モデルの描画を行うクラス
class SkinModel:public Component{
public:
	SkinModel::SkinModel(GameObject* g, Transform* t);
	~SkinModel();

	void Awake()override;
	void Start()override;
	void LateUpdate()override;
	void PreRender()override;
	void Render()override;

	/**
	* シャドウマップに深度を書き込む.
	* シャドウマップクラスから呼ばれている.
	*/
	void RenderToShadowMap();

	/**
	* 環境マップに描画する.
	* 環境マップクラスから呼ばれている.
	*/
	void RenderToEnvironmentMap();

	void SetCamera(Camera* c) 
	{
		_Camera = c;
	}
	void SetLight(Light* l)
	{
		_Light = l;
	}

	//モデルデータへアクセスするためのポインタ設定
	void SetModelData(SkinModelData* pD)
	{
		_ModelDate = pD;
	}
	//メッシュコライダー作るときに
	LPD3DXMESH GetOrgMeshFirst() const
	{
		return _ModelDate->GetOrgMeshFirst();
	}
	void SetSky(bool f)
	{
		_SkyBox = f;
	}
	void SetTextureBlend(const Color& c)
	{
		_TextureBlend = c;
	}
	void SetAllBlend(const Color& c)
	{
		_AllBlend = c;
	}
	
	//上書きセット
	void SkinModel::SetModelEffect(const ModelEffectE& e)
	{
		_ModelEffect = e;
	}
	//今の状態に加算、減算
	void SkinModel::SetModelEffect(const ModelEffectE& e, const bool& f)
	{
		//既に有効かどうか？
		if ((_ModelEffect & e) > 0 && f == false)
		{
			//無効に
			_ModelEffect = ModelEffectE(_ModelEffect - e);
		}
		else if ((_ModelEffect & e) == 0 && f == true)
		{
			//有効に
			_ModelEffect = ModelEffectE(_ModelEffect + e);
		}
	}
	bool terain = false;
private:
	//子とか兄弟も一括で描画するための再帰関数
	void DrawFrame(LPD3DXFRAME pFrame);

	//モデル描画
	void SkinModel::DrawMeshContainer(
		D3DXMESHCONTAINER_DERIVED* pMeshContainerBase,
		LPD3DXFRAME pFrameBase
	);

	//影作成
	void CreateShadow(D3DXMESHCONTAINER_DERIVED* pMeshContainer, D3DXFRAME_DERIVED* pFrame);
private:
	//エフェクトへの参照
	Effect* _Effect;
	//モデルデータへアクセスするためのポインタ保持
	SkinModelData* _ModelDate;
	Camera* _Camera;
	Light* _Light;
	//ブレンドする色
	Color _TextureBlend, _AllBlend;

	//エフェクトを描けるかどうかのフラグ
	ModelEffectE _ModelEffect;
	//スカイボックスかどうか(いつか直す)
	bool _SkyBox;

	//最大数
	static const int MAX_MATRIX_PALLET = 50;
	//一時的にボーン行列を格納する作業用変数？
	D3DXMATRIX _BoneMatrixPallets[MAX_MATRIX_PALLET];
};