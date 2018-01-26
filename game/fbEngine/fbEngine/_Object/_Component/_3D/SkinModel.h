#pragma once
#include "_Object\_Component\Component.h"

class Effect;
class Camera;
class Light;
class CharacterLight;
class ShadowCamera;
class IObjectCulling;

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
	FRUSTUM_CULLING = BIT(7),	//フラスタムカリングを行うかどうか？
	ALPHA = BIT(8),				//!< アルファ.
	DITHERING = BIT(9),			//!< ディザリング.
	RECEIVE_POINTLIGHT = BIT(10),	//ポイントライトを落とす.
};

/**
* 大気散乱の種類.
*/
enum AtmosphereFunc
{
	enAtomosphereFuncNone = 0,				//大気錯乱シミュレーションなし。
	enAtomosphereFuncObjectFromAtomosphere,	//オブジェクトを大気圏から見た場合の大気錯乱シミュレーション。
	enAtomosphereFuncSkyFromAtomosphere,	//空を大気圏から見た場合の大気錯乱シミュレーション。
	enAtomosphereFuncNum,
};

/**
* フォグパラメータ.
*/
enum class FogFunc
{
	FogFuncNone,	// フォグなし.
	FogFuncDist,	// 距離フォグ.
	FogFuncHeight,	// 高さフォグ.
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
	SkinModelData* GetModelData() {
		return _ModelDate;
	}

	//メッシュコライダー作るときに
	LPD3DXMESH GetOrgMeshFirst() const
	{
		return _ModelDate->GetOrgMeshFirst();
	}
	LPD3DXFRAME GetFrameRoot() const
	{
		return _ModelDate->GetFrameRoot();
	}
	void SetSky()
	{
		_IsSky = true;
		SetAtomosphereFunc(AtmosphereFunc::enAtomosphereFuncSkyFromAtomosphere);
	}
	//void SetTextureBlend(const Color& c)
	//{
	//	_TextureBlend = c;
	//}

	// モデルに乗算するカラー情報を設定。
	void SetAllBlend(const Color& c)
	{
		_AllBlend = c;
	}
	inline const Color& GetAllBlend()const {
		return _AllBlend;
	}

	/**
	* アルファのしきい値を設定(モデルの不透明度ではない点に注意).
	*/
	void SetAlpha(bool flag,float a = 0.0f)
	{
		SetModelEffect(ModelEffectE::ALPHA, flag);
		_Alpha = a;
	}

	/**
	* 大気散乱シミュレーションの種類を設定.
	*/
	void SetAtomosphereFunc(AtmosphereFunc func)
	{
		_AtomosphereFunc = func;
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
	//背面描画モードの設定。
	//[in] 描画する面のフラグ
	void SetCullMode(D3DCULL flg)
	{
		_CullMode = flg;
	}
	bool terain = false;

	/**
	* キャラクターライトを設定.
	*/
	void SetCharacterLight(CharacterLight* cLight)
	{
		_CharaLight = cLight;
	}

	void SetTree()
	{
		_IsTree = true;
		SetModelEffect(ModelEffectE::DITHERING, true);
	}

	/**
	* ディザ係数を設定する.
	* 0~65の値を入れる.
	* 0を入れると消えない.
	* 値が大きくなるほど消えていく.
	* モデルエフェクトの方も設定しないと意味ないよ？
	* 
	* 大川これだよ。これこれ。これですってば。
	*/
	void SetDitherCoefficient(float value)
	{
		_DitherCoefficient = value;
	}

	/**
	* フォグパラメータを設定.
	*
	* @param fogfunc フォグの種類.
	* @param idx0    フォグがかかり始める距離.
	* @param idx1    フォグがかかりきる距離.
	* @param color	　フォグの色
	*/
	void SetFogParam(FogFunc fogFunc, float idx0, float idx1, const Vector4& color, bool isNight = false)
	{
		_FogFunc = fogFunc;
		_FogParam[0] = idx0;
		_FogParam[1] = idx1;
		_FogParam[3] = isNight;
		_FogColor = color;
	}

	/**
	* 溢れ輝度を書き込むかのフラグ.
	*/
	void SetIsLuminance(bool value)
	{
		_IsLuminance = value;
	}

	/**
	* 色々な色の輝度を設定できるよ.
	* aに1を入れると書き込まれるよ.
	*/
	void SetLuminanceColor(Color lumColor)
	{
		_LuminanceColor = lumColor;
	}

	/**
	* フレネル反射のパラメータを設定.
	*/
	void SetFresnelParam(bool is, const Vector4& param = Vector4(1.0f, 1.0f, 1.0f, 1.5f))
	{
		_IsFresnel = is;
		_FresnelParam = param;
	}

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
	//カメラ。
	Camera* _Camera;
	//ライト。
	Light* _Light;
	//ブレンドする色
	Color /*_TextureBlend,*/ _AllBlend;	// モデルの最終的なピクセルカラーに乗算する値(透明にする場合はこの値のアルファ成分を0にする)。

	/** キャラクターライト. */
	CharacterLight* _CharaLight = nullptr;

	//エフェクトを描けるかどうかのフラグ
	ModelEffectE _ModelEffect;

	bool _IsSky = false;

	//最大数
	static const int MAX_MATRIX_PALLET = 50;
	//一時的にボーン行列を格納する作業用変数？
	D3DXMATRIX _BoneMatrixPallets[MAX_MATRIX_PALLET];

	/** 大気散乱. */
	AtmosphereFunc _AtomosphereFunc = AtmosphereFunc::enAtomosphereFuncObjectFromAtomosphere;

	//描画する面
	D3DCULL _CullMode;

	//オブジェクトカリング。
	IObjectCulling* _Culling;

	/** アルファの閾値. */
	float _Alpha = 0.0f;

	/** 木. */
	bool _IsTree = false;

	/** ディザ係数. */
	float _DitherCoefficient = 0;

	/** フォグの種類. */
	FogFunc _FogFunc = FogFunc::FogFuncNone;
	/** フォグのパラメータ. */
	float _FogParam[2];
	/** フォグの色. */
	Vector4 _FogColor = Vector4(0, 0, 0, 1);

	/** 輝度の色. */
	Color _LuminanceColor = Color(0.0f, 0.0f, 0.0f, 0.0f);

	/** 溢れ輝度を書き込むかフラグ. */
	bool _IsLuminance = true;

	/** フレネル反射を行うフラグ. */
	bool _IsFresnel = false;
	/** フレネル反射のパラメータ. */
	Vector4 _FresnelParam = Vector4(1.0f, 1.0f, 1.0f, 1.5f);

};