/*!
 *@brief	スキンモデルデータ
 */
#pragma once
#include "_Object/Noncopyable.h"

struct D3DXFRAME_DERIVED : public D3DXFRAME {
	D3DXMATRIXA16	CombinedTransformationMatrix;	//合成済み行列。
	D3DXMATRIX* RotationMatrix = nullptr;
};
//メッシュコンテナを継承した拡張メッシュコンテナ
struct D3DXMESHCONTAINER_DERIVED : public D3DXMESHCONTAINER {
	//マテリアル
	Material** material;
	//
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
	
	//モデルデータをロード。
	//[in]	filePath	ファイルパス。
	//[out] ロードが成功したかどうか？ 
	bool LoadModelData(const char* filePath);
	//モデルデータのクローン作製
	void CloneModelData(const SkinModelData* modelData, Animation* anim = nullptr);
	

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

	//テラインのXZ軸の縦幅と横幅を求める関数
	//いつか消す
	void CalcWidthAndHeight();
	
	//スキンモデルのメッシュのリストを取得。
	const std::vector<LPD3DXMESH>& GetMeshList() const
	{
		return _MeshList;
	}
	const Vector4& GetTerrainSize()
	{
		return _TerrainSize;
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
	//アニメーションコントローラー
	ID3DXAnimationController* m_pAnimationController;
	//とりあえずほじさせたかった。いつか消す。
	Vector4 _TerrainSize;
	//インスタンシング描画フラグ。デフォルトはfalse。
	bool _Instancing;
};