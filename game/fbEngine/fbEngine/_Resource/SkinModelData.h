/*!
 *@brief	スキンモデルデータ
 */
#pragma once
#include "_Object/Noncopyable.h"
#include "_Value\AABB.h"

struct D3DXFRAME_DERIVED : public D3DXFRAME {
	D3DXMATRIXA16	CombinedTransformationMatrix;	//合成済み行列。
	D3DXMATRIX* RotationMatrix = nullptr;
	//オリジナルへのポインタ。
	D3DXFRAME_DERIVED* Original;
	//インスタンシングで使う、ワールド行列を積むベクター。
	vector<D3DXMATRIX*> WorldMatrixStack;
};
//メッシュコンテナを継承した拡張メッシュコンテナ
struct D3DXMESHCONTAINER_DERIVED : public D3DXMESHCONTAINER {
	//マテリアル
	Material** material;
	//オリジナルのメッシュ。
	LPD3DXMESH pOrigMesh;
	//
	LPD3DXATTRIBUTERANGE pAttributeTable;
	//
	DWORD NumAttributeGroups;
	//
	DWORD NumInfl;
	//
	LPD3DXBUFFER pBoneCombinationBuf;
	//フレームの合成済み行列への参照
	D3DXMATRIX** ppBoneMatrixPtrs;
	//ボーンのオフセット行列
	D3DXMATRIX* pBoneOffsetMatrices;
	//
	DWORD NumPaletteEntries;
	//
	bool UseSoftwareVP;
	//
	DWORD iAttributeSW;
	//頂点定義の作成。
	IDirect3DVertexDeclaration9* vertexDecl = NULL;
	//ワールド行列用のバッファの作成。
	IDirect3DVertexBuffer9* worldMatrixBuffer = NULL;			//ワールド行列のバッファ。
	
};

//モデルのデータを扱うクラス
class SkinModelData:Noncopyable
{
public:
	/*!
	 *@brief	コンストラクタ。
	 */
	SkinModelData();
	/*!
	 *@brief	デストラクタ。
	 */
	~SkinModelData();
	
	//AABB更新。
	void UpdateAABB()
	{
		Vector3 half = _Size;
		half.Scale(0.5f);
		_AABB.Update(_Center, half);
	}

	//AABBを取得。
	AABB GetAABB()
	{
		return _AABB;
	}
	//モデルの中心座標。
	const Vector3& GetCenterPos() { return _Center; }
	
	//モデルデータをロード。
	//[in]	filePath	ファイルパス。
	//[out] ロードが成功したかどうか？ 
	bool LoadModelData(const char* filePath);
	//モデルデータのクローン作製
	void CloneModelData(const SkinModelData* original, Animation* anim = nullptr);
	

	/*!
	* @brief	リリース。
	*/
	void Release();

	//最初(根元？)のフレーム取得
	LPD3DXFRAME GetFrameRoot()
	{
		return _FrameRoot;
	}

	ID3DXAnimationController* GetAnimationController()
	{
		return m_pAnimationController;
	}
	//マテリアルの追加(エンジン専用)
	void AddMaterial(Material* mat)
	{
		_Materials.push_back(mat);
	}
	Material* FindMaterial(const char* matName);

	//引数として受け取った行列を親として,骨の行列を更新
	//第一引数：D3DXMATRIX&　ワールド行列
	void UpdateBoneMatrix(const D3DXMATRIX& matWorld);
	//一番最初のメッシュを取得
	LPD3DXMESH GetOrgMeshFirst() const;

	//モデルのサイズを測定する。
	void Measurement();
	
	//スキンモデルのメッシュのリストを取得。
	const std::vector<LPD3DXMESH>& GetMeshList() const
	{
		return _MeshList;
	}
	const vector<LPD3DXFRAME>& GetFrameList()const
	{
		return _FrameList;
	}
	const Vector4& GetTerrainSize()
	{
		return _TerrainSize;
	}
	const Vector3& GetAABBSize()
	{
		return _Size;
	}
	//インスタンシングフラグを設定。
	void SetInstancing(bool flg)
	{
		_Instancing = flg;
	}
	//フラグを返す。
	const bool& GetInstancing()
	{
		return _Instancing;
	}

public:
	//インスタンシング様にワールド行列を積む。
	//描画済みフラグ取得。
	bool GetAlreadyDrawn()
	{
		return _Original->_AlreadyDrawn;
	}
	//インスタンシング開始。
	void StartInstancing()
	{
		_Original->_AlreadyDrawn = false;
	}
	//インスタンシング終了。
	void EndInstancing()
	{
		_Original->_AlreadyDrawn = true;
	}
private:
	//最初のメッシュを返す？
	LPD3DXMESH GetOrgMesh(LPD3DXFRAME frame) const;
	//クローン作製
	void CloneSkeleton(LPD3DXFRAME& dstFrame, LPD3DXFRAME srcFrame);
	//クローン削除
	void DeleteCloneSkeleton(LPD3DXFRAME frame);
	//アニメーションのなにかを設定(行列のやつとかだと思う)
	void SetupOutputAnimationRegist(LPD3DXFRAME frame, ID3DXAnimationController* animCtr);
	//メッシュのリストを作成
	void _CreateMeshList();
	//メッシュのリストを作成に使う再帰関数
	void _QueryMeshes(LPD3DXFRAME frame);
private:
	LPD3DXFRAME					_FrameRoot;		//フレームルート(大本)。
	std::vector<Material*>		_Materials;		//マテリアルのリスト
	std::vector<LPD3DXMESH>		_MeshList;		//メッシュのリスト。
	std::vector<LPD3DXFRAME>	_FrameList;
	//アニメーションコントローラー
	ID3DXAnimationController* m_pAnimationController;
	//とりあえずほじさせたかった。いつか消す。
	Vector4 _TerrainSize;
	//
	Vector3 _Size;
	//中心座標
	Vector3 _Center;
	//
	AABB _AABB;
private:
	//インスタンシング描画フラグ。デフォルトはfalse。
	bool _Instancing;
	//描画済みフラグ。インスタンシングで使う。
	bool _AlreadyDrawn;
	//オリジナルモデルへのポインタ。
	SkinModelData* _Original;
};