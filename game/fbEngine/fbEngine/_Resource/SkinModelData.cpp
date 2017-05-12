/*!
 *@brief	スキンモデルデータ。
 */
#include"fbstdafx.h"
#include "SkinModelData.h"

//UINT                        g_NumBoneMatricesMax = 0;
//D3DXMATRIXA16*              g_pBoneMatrices = NULL;

//モデルのFrame更新
void UpdateFrameMatrices(LPD3DXFRAME pFrameBase, const D3DXMATRIX* pParentMatrix)
{
	D3DXFRAME_DERIVED* pFrame = (D3DXFRAME_DERIVED*)pFrameBase;

	if (pParentMatrix != NULL)
		//ワールド変換行列を作成
		D3DXMatrixMultiply(&pFrame->CombinedTransformationMatrix, &pFrame->TransformationMatrix, pParentMatrix);
	else
		//そのまま
		pFrame->CombinedTransformationMatrix = pFrame->TransformationMatrix;

	//兄弟更新
	if (pFrame->pFrameSibling != NULL)
	{
		//同じ行列を渡す
		UpdateFrameMatrices(pFrame->pFrameSibling, pParentMatrix);
	}

	//子更新
	if (pFrame->pFrameFirstChild != NULL)
	{
		//親の行列で更新
		UpdateFrameMatrices(pFrame->pFrameFirstChild, &pFrame->CombinedTransformationMatrix);
	}
}
	
//メッシュ頂点をグループに分ける
HRESULT GenerateSkinnedMesh(
	IDirect3DDevice9* pd3dDevice,
	D3DXMESHCONTAINER_DERIVED* pMeshContainer
	)
{
	HRESULT hr = S_OK;
	D3DCAPS9 d3dCaps;
	pd3dDevice->GetDeviceCaps(&d3dCaps);
	//ボーン情報がないなら終了
	if (pMeshContainer->pSkinInfo == NULL)
		return hr;

	SAFE_RELEASE(pMeshContainer->MeshData.pMesh);
	SAFE_RELEASE(pMeshContainer->pBoneCombinationBuf);

	{
		// Get palette size
		// First 9 constants are used for other data.  Each 4x3 matrix takes up 3 constants.
		// (96 - 9) /3 i.e. Maximum constant count - used constants 
		//骨の最大行列数
		//こっちの数字を変えたならシェーダの方も変えること
		UINT MaxMatrices = 50;
		pMeshContainer->NumPaletteEntries = min(MaxMatrices, pMeshContainer->pSkinInfo->GetNumBones());

		DWORD Flags = D3DXMESHOPT_VERTEXCACHE;
		if (d3dCaps.VertexShaderVersion >= D3DVS_VERSION(1, 1))
		{
			pMeshContainer->UseSoftwareVP = false;
			Flags |= D3DXMESH_MANAGED;
		}
		else
		{
			pMeshContainer->UseSoftwareVP = true;
			Flags |= D3DXMESH_SYSTEMMEM;
		}

		SAFE_RELEASE(pMeshContainer->MeshData.pMesh);

		//ボーンと頂点のブレンドインデックスから新しいメッシュを作成
		hr = pMeshContainer->pSkinInfo->ConvertToIndexedBlendedMesh
			(
			pMeshContainer->pOrigMesh,
			Flags,
			pMeshContainer->NumPaletteEntries,
			pMeshContainer->pAdjacency,
			NULL, NULL, NULL,
			&pMeshContainer->NumInfl,
			&pMeshContainer->NumAttributeGroups,
			&pMeshContainer->pBoneCombinationBuf,
			&pMeshContainer->MeshData.pMesh);

		if (FAILED(hr))
			goto e_Exit;


		// FVF has to match our declarator. Vertex shaders are not as forgiving as FF pipeline
		DWORD NewFVF = (pMeshContainer->MeshData.pMesh->GetFVF() & D3DFVF_POSITION_MASK) | D3DFVF_NORMAL |
			D3DFVF_TEX1 | D3DFVF_LASTBETA_UBYTE4;
		if (NewFVF != pMeshContainer->MeshData.pMesh->GetFVF())
		{
			LPD3DXMESH pMesh;
			hr = pMeshContainer->MeshData.pMesh->CloneMeshFVF(pMeshContainer->MeshData.pMesh->GetOptions(), NewFVF,
				pd3dDevice, &pMesh);
			if (!FAILED(hr))
			{
				pMeshContainer->MeshData.pMesh->Release();
				pMeshContainer->MeshData.pMesh = pMesh;
				pMesh = NULL;
			}
		}

		D3DVERTEXELEMENT9 pDecl[MAX_FVF_DECL_SIZE];
		LPD3DVERTEXELEMENT9 pDeclCur;
		hr = pMeshContainer->MeshData.pMesh->GetDeclaration(pDecl);
		if (FAILED(hr))
			goto e_Exit;

		// the vertex shader is expecting to interpret the UBYTE4 as a D3DCOLOR, so update the type 
		//   NOTE: this cannot be done with CloneMesh, that would convert the UBYTE4 data to float and then to D3DCOLOR
		//          this is more of a "cast" operation
		pDeclCur = pDecl;
		while (pDeclCur->Stream != 0xff)
		{
			if ((pDeclCur->Usage == D3DDECLUSAGE_BLENDINDICES) && (pDeclCur->UsageIndex == 0))
				pDeclCur->Type = D3DDECLTYPE_D3DCOLOR;
			pDeclCur++;
		}

		hr = pMeshContainer->MeshData.pMesh->UpdateSemantics(pDecl);
		if (FAILED(hr))
			goto e_Exit;

		// allocate a buffer for bone matrices, but only if another mesh has not allocated one of the same size or larger
		//if (g_NumBoneMatricesMax < pMeshContainer->pSkinInfo->GetNumBones())
		//{
		//	g_NumBoneMatricesMax = pMeshContainer->pSkinInfo->GetNumBones();

		//	// Allocate space for blend matrices
		//	delete[] g_pBoneMatrices;
		//	g_pBoneMatrices = new D3DXMATRIXA16[g_NumBoneMatricesMax];
		//	if (g_pBoneMatrices == NULL)
		//	{
		//		hr = E_OUTOFMEMORY;
		//		goto e_Exit;
		//	}
		//}

	}
e_Exit:
	return hr;
}

HRESULT AllocateName(LPCSTR Name, LPSTR* pNewName)
{
	UINT cbLength;

	if (Name != NULL)
	{
		cbLength = (UINT)strlen(Name) + 1;
		*pNewName = new CHAR[cbLength];
		if (*pNewName == NULL)
			return E_OUTOFMEMORY;
		memcpy(*pNewName, Name, cbLength * sizeof(CHAR));
	}
	else
	{
		*pNewName = NULL;
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
// Called to setup the pointers for a given bone to its transformation matrix
//--------------------------------------------------------------------------------------
HRESULT SetupBoneMatrixPointersOnMesh(LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME rootFrame)
{
	UINT iBone, cBones;
	D3DXFRAME_DERIVED* pFrame;

	//標準のメッシュコンテナを自分でカスタマイズしたメッシュコンテナにキャスト
	D3DXMESHCONTAINER_DERIVED* pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerBase;

	//スキン情報(ボーンがあるかどうか)確認
	if (pMeshContainer->pSkinInfo != NULL)
	{
		//ボーンの数取得
		cBones = pMeshContainer->pSkinInfo->GetNumBones();

		//ボーンの行列の配列を確保
		pMeshContainer->ppBoneMatrixPtrs = new D3DXMATRIX*[cBones];
		//骨がねぇ！
		if (pMeshContainer->ppBoneMatrixPtrs == NULL)
			return E_OUTOFMEMORY;

		for (iBone = 0; iBone < cBones; iBone++)
		{
			//ルートフレームからボーンの名前で検索し,子フレームを取得
			pFrame = (D3DXFRAME_DERIVED*)D3DXFrameFind(rootFrame,
				pMeshContainer->pSkinInfo->GetBoneName(iBone));
			//骨に対応したフレームが見つからなかった・・・
			if (pFrame == NULL)
				return E_FAIL;
			
			//フレームから行列を取得し,骨の行列とする
			pMeshContainer->ppBoneMatrixPtrs[iBone] = &pFrame->CombinedTransformationMatrix;
		}
	}

	return S_OK;
}


//--------------------------------------------------------------------------------------
// Called to setup the pointers for a given bone to its transformation matrix
//--------------------------------------------------------------------------------------
HRESULT SetupBoneMatrixPointers(LPD3DXFRAME pFrame, LPD3DXFRAME pRootFrame)
{
	HRESULT hr;

	if (pFrame->pMeshContainer != NULL)
	{
		//骨の行列設定
		hr = SetupBoneMatrixPointersOnMesh(pFrame->pMeshContainer, pRootFrame);
		if (FAILED(hr))
			return hr;
	}

	//兄弟初期化(再帰)
	if (pFrame->pFrameSibling != NULL)
	{
		hr = SetupBoneMatrixPointers(pFrame->pFrameSibling, pRootFrame);
		if (FAILED(hr))
			return hr;
	}

	//子初期化(再帰)
	if (pFrame->pFrameFirstChild != NULL)
	{
		hr = SetupBoneMatrixPointers(pFrame->pFrameFirstChild, pRootFrame);
		if (FAILED(hr))
			return hr;
	}

	return S_OK;
}

class CAllocateHierarchy : public ID3DXAllocateHierarchy
{
public:
	//フレーム生成
	STDMETHOD(CreateFrame)(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame);
	//メッシュコンテナを作成
	STDMETHOD(CreateMeshContainer)(THIS_
		LPCSTR Name,
		CONST D3DXMESHDATA *pMeshData,
		CONST D3DXMATERIAL *pMaterials,
		CONST D3DXEFFECTINSTANCE *pEffectInstances,
		DWORD NumMaterials,
		CONST DWORD *pAdjacency,
		LPD3DXSKININFO pSkinInfo,
		LPD3DXMESHCONTAINER *ppNewMeshContainer);
	//フレーム削除
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree);
	//メッシュコンテナを削除
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerBase);

	CAllocateHierarchy(SkinModelData* data):
		_SkinModelData(data)
	{
	}
private:
	SkinModelData* _SkinModelData;
};

//--------------------------------------------------------------------------------------
// Name: CAllocateHierarchy::CreateFrame()
// Desc: 
//--------------------------------------------------------------------------------------
HRESULT CAllocateHierarchy::CreateFrame(LPCSTR Name, LPD3DXFRAME* ppNewFrame)
{
	HRESULT hr = S_OK;
	D3DXFRAME_DERIVED* pFrame;

	*ppNewFrame = NULL;

	pFrame = new D3DXFRAME_DERIVED();
	if (pFrame == NULL)
	{
		hr = E_OUTOFMEMORY;
		goto e_Exit;
	}

	hr = AllocateName(Name, &pFrame->Name);
	if (FAILED(hr))
		goto e_Exit;

	// initialize other data members of the frame
	D3DXMatrixIdentity(&pFrame->TransformationMatrix);
	D3DXMatrixIdentity(&pFrame->CombinedTransformationMatrix);

	pFrame->pMeshContainer = NULL;
	pFrame->pFrameSibling = NULL;
	pFrame->pFrameFirstChild = NULL;

	*ppNewFrame = pFrame;
	pFrame = NULL;

e_Exit:
	delete pFrame;
	return hr;
}

//--------------------------------------------------------------------------------------
// Name: CAllocateHierarchy::CreateMeshContainer()
// Desc: 
//--------------------------------------------------------------------------------------
//メッシュコンテナを作成
HRESULT CAllocateHierarchy::CreateMeshContainer(
	LPCSTR Name,
	CONST D3DXMESHDATA *pMeshData,
	CONST D3DXMATERIAL *pMaterials,
	CONST D3DXEFFECTINSTANCE *pEffectInstances,
	DWORD NumMaterials,
	CONST DWORD *pAdjacency,
	LPD3DXSKININFO pSkinInfo,
	LPD3DXMESHCONTAINER *ppNewMeshContainer)
{
	HRESULT hr;
	D3DXMESHCONTAINER_DERIVED *pMeshContainer = NULL;
	UINT NumFaces;
	UINT iMaterial;
	UINT iBone, cBones;
	LPDIRECT3DDEVICE9 pd3dDevice = NULL;

	LPD3DXMESH pMesh = NULL;

	*ppNewMeshContainer = NULL;

	// this sample does not handle patch meshes, so fail when one is found
	if (pMeshData->Type != D3DXMESHTYPE_MESH)
	{
		hr = E_FAIL;
		goto e_Exit;
	}

	// get the pMesh interface pointer out of the mesh data structure
	pMesh = pMeshData->pMesh;

	// this sample does not FVF compatible meshes, so fail when one is found
	if (pMesh->GetFVF() == 0)
	{
		hr = E_FAIL;
		goto e_Exit;
	}

	// allocate the overloaded structure to return as a D3DXMESHCONTAINER
	pMeshContainer = new D3DXMESHCONTAINER_DERIVED;
	if (pMeshContainer == NULL)
	{
		hr = E_OUTOFMEMORY;
		goto e_Exit;
	}
	memset(pMeshContainer, 0, sizeof(D3DXMESHCONTAINER_DERIVED));

	// make sure and copy the name.  All memory as input belongs to caller, interfaces can be addref'd though
	hr = AllocateName(Name, &pMeshContainer->Name);
	if (FAILED(hr))
		goto e_Exit;

	pMesh->GetDevice(&pd3dDevice);
	NumFaces = pMesh->GetNumFaces();

	// if no normals are in the mesh, add them
	if (!(pMesh->GetFVF() & D3DFVF_NORMAL))
	{
		pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

		// clone the mesh to make room for the normals
		hr = pMesh->CloneMeshFVF(pMesh->GetOptions(),
			pMesh->GetFVF() | D3DFVF_NORMAL,
			pd3dDevice, &pMeshContainer->MeshData.pMesh);
		if (FAILED(hr))
			goto e_Exit;

		// get the new pMesh pointer back out of the mesh container to use
		// NOTE: we do not release pMesh because we do not have a reference to it yet
		pMesh = pMeshContainer->MeshData.pMesh;

		// now generate the normals for the pmesh
		D3DXComputeNormals(pMesh, NULL);
	}
	else  // if no normals, just add a reference to the mesh for the mesh container
	{
		pMeshContainer->MeshData.pMesh = pMesh;
		pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

		pMesh->AddRef();
	}

	// allocate memory to contain the material information.  This sample uses
	//   the D3D9 materials and texture names instead of the EffectInstance style materials
	//マテリアルの数設定
	pMeshContainer->NumMaterials = max(1, NumMaterials);
	pMeshContainer->pMaterials = new D3DXMATERIAL[pMeshContainer->NumMaterials];
	pMeshContainer->material = new Material*[pMeshContainer->NumMaterials];
	pMeshContainer->pAdjacency = new DWORD[NumFaces * 3];
	if ((pMeshContainer->pAdjacency == NULL) || (pMeshContainer->pMaterials == NULL))
	{
		hr = E_OUTOFMEMORY;
		goto e_Exit;
	}

	memcpy(pMeshContainer->pAdjacency, pAdjacency, sizeof(DWORD)* NumFaces * 3);
	//テクスチャーを0で初期化
	//memset(pMeshContainer->ppTextures, 0, sizeof(LPDIRECT3DTEXTURE9)* pMeshContainer->NumMaterials);

	//マテリアルの数だけ画像を読み込み、コピー
	if (NumMaterials > 0)
	{
		memcpy(pMeshContainer->pMaterials, pMaterials, sizeof(D3DXMATERIAL)* NumMaterials);
		for (iMaterial = 0; iMaterial < NumMaterials; iMaterial++)
		{
			pMeshContainer->material[iMaterial] = nullptr;
			//ファイル指定があるなら
			if (pMeshContainer->pMaterials[iMaterial].pTextureFilename != NULL)
			{
				pMeshContainer->pMaterials[iMaterial].pTextureFilename = new CHAR[strlen(pMaterials[iMaterial].pTextureFilename) + 1];
				strcpy(pMeshContainer->pMaterials[iMaterial].pTextureFilename, pMaterials[iMaterial].pTextureFilename);

				//画像のパス追加
				char* baseDir = "Asset/Xfile/";
				char filePath[64];
				strcpy_s(filePath, baseDir);
				strcat_s(filePath, pMeshContainer->pMaterials[iMaterial].pTextureFilename);

				pMeshContainer->material[iMaterial] = new Material(pMeshContainer->pMaterials[iMaterial].pTextureFilename);

				//画像の情報を取得
				D3DXIMAGE_INFO info;
				D3DXGetImageInfoFromFile(
					filePath,	//テクスチャパス
					&info	//情報格納先
				);
				LPDIRECT3DBASETEXTURE9 texture;
				//テクスチャのタイプに合った読み込み方法
				switch (info.ResourceType)
				{
				case _D3DRESOURCETYPE::D3DRTYPE_TEXTURE:
					//通常テクスチャ読み込み
					if (FAILED(D3DXCreateTextureFromFile(
						pd3dDevice,
						filePath,
						(LPDIRECT3DTEXTURE9*)&texture)))
					{
						//なかった
						pMeshContainer->material[iMaterial]->SetTexture(Material::TextureHandleE::DiffuseMap,nullptr);
					}
					else
					{
						//あった
						pMeshContainer->material[iMaterial]->SetTexture(Material::TextureHandleE::DiffuseMap, texture);
						_SkinModelData->AddMaterial(pMeshContainer->material[iMaterial]);
					}
					break;
				case _D3DRESOURCETYPE::D3DRTYPE_CUBETEXTURE:
					//キューブテクスチャ読み込み
					if (FAILED(D3DXCreateCubeTextureFromFile(
						pd3dDevice,
						filePath,
						(LPDIRECT3DCUBETEXTURE9*)&texture)))
					{
						//なかった
						pMeshContainer->material[iMaterial]->SetTexture(Material::TextureHandleE::DiffuseMap, nullptr);
					}
					else
					{
						//あった
						pMeshContainer->material[iMaterial]->SetTexture(Material::TextureHandleE::DiffuseMap, texture);
						_SkinModelData->AddMaterial(pMeshContainer->material[iMaterial]);
					}
					break;
				default:
					break;
				}
				
				
				
					// don't remember a pointer into the dynamic memory, just forget the name after loading
					//よくわからんがファイルパスを消している。
					//pMeshContainer->pMaterials[iMaterial].pTextureFilename = NULL;
			}
		}
	}
	else //マテリアルがないよー
	{
		pMeshContainer->pMaterials[0].pTextureFilename = NULL;
		memset(&pMeshContainer->pMaterials[0].MatD3D, 0, sizeof(D3DMATERIAL9));
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.r = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.g = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.b = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Specular = pMeshContainer->pMaterials[0].MatD3D.Diffuse;
	}
	pMeshContainer->pOrigMesh = pMesh;
	pMesh->AddRef();
	// if there is skinning information, save off the required data and then setup for HW skinning
	if (pSkinInfo != NULL)
	{
		// first save off the SkinInfo and original mesh data
		pMeshContainer->pSkinInfo = pSkinInfo;
		pSkinInfo->AddRef();

		

		//ボーンの数だけボーンのオフセット行列確保
		cBones = pSkinInfo->GetNumBones();
		pMeshContainer->pBoneOffsetMatrices = new D3DXMATRIX[cBones];
		if (pMeshContainer->pBoneOffsetMatrices == NULL)
		{
			hr = E_OUTOFMEMORY;
			goto e_Exit;
		}

		//ボーンのオフセット行列セット
		for (iBone = 0; iBone < cBones; iBone++)
		{
			pMeshContainer->pBoneOffsetMatrices[iBone] = *(pMeshContainer->pSkinInfo->GetBoneOffsetMatrix(iBone));
		}

		//メッシュ頂点をグループに分ける
		hr = GenerateSkinnedMesh(pd3dDevice, pMeshContainer);
		if (FAILED(hr))
			goto e_Exit;
	}

	*ppNewMeshContainer = pMeshContainer;
	pMeshContainer = NULL;

e_Exit:
	pd3dDevice->Release();

	// call Destroy function to properly clean up the memory allocated 
	if (pMeshContainer != NULL)
	{
		DestroyMeshContainer(pMeshContainer);
	}

	return hr;
}

//--------------------------------------------------------------------------------------
// Name: CAllocateHierarchy::DestroyFrame()
// Desc: 
//--------------------------------------------------------------------------------------
HRESULT CAllocateHierarchy::DestroyFrame(LPD3DXFRAME pFrameToFree)
{
	SAFE_DELETE_ARRAY(pFrameToFree->Name);
	SAFE_DELETE(pFrameToFree);
	return S_OK;
}


//--------------------------------------------------------------------------------------
// Name: CAllocateHierarchy::DestroyMeshContainer()
// Desc: 
//--------------------------------------------------------------------------------------
HRESULT CAllocateHierarchy::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase)
{
	UINT iMaterial;
	D3DXMESHCONTAINER_DERIVED* pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerBase;

	SAFE_DELETE_ARRAY(pMeshContainer->Name);
	SAFE_DELETE_ARRAY(pMeshContainer->pAdjacency);


	SAFE_DELETE_ARRAY(pMeshContainer->pBoneOffsetMatrices);

	// release all the allocated textures
	if (pMeshContainer->material != NULL)
	{
		for (iMaterial = 0; iMaterial < pMeshContainer->NumMaterials; iMaterial++)
		{
			//SAFE_RELEASE(pMeshContainer->material[iMaterial]);
			SAFE_DELETE_ARRAY(pMeshContainer->pMaterials[iMaterial].pTextureFilename);
		}
	}

	SAFE_DELETE_ARRAY(pMeshContainer->pMaterials);
	SAFE_DELETE_ARRAY(pMeshContainer->material);
	SAFE_DELETE_ARRAY(pMeshContainer->ppBoneMatrixPtrs);
	SAFE_RELEASE(pMeshContainer->pBoneCombinationBuf);
	SAFE_RELEASE(pMeshContainer->MeshData.pMesh);
	SAFE_RELEASE(pMeshContainer->pSkinInfo);
	SAFE_RELEASE(pMeshContainer->pOrigMesh);
	SAFE_DELETE(pMeshContainer);
	return S_OK;
}

/*!
 *@brief	コンストラクタ。
 */
SkinModelData::SkinModelData():
_FrameRoot(nullptr),
m_pAnimationController(nullptr)
{
	
}

/*!
 *@brief	デストラクタ。
 */
SkinModelData::~SkinModelData()
{
	Release();
}

/*!
* @brief	リリース。
*/
void SkinModelData::Release()
{
	if (_FrameRoot) {
		//クローン
		DeleteCloneSkeleton(_FrameRoot);
		_FrameRoot = nullptr;
	}
	SAFE_RELEASE(m_pAnimationController);
}
	
/*!
 * @brief	モデルデータをロード。
 *@param[in]	filePath	ファイルパス。
 */
bool SkinModelData::LoadModelData(const char* filePath)
{
	CAllocateHierarchy alloc(this);
	HRESULT hr = D3DXLoadMeshHierarchyFromXA(
		filePath,
		D3DXMESH_VB_MANAGED,
		graphicsDevice(),
		&alloc,
		nullptr,
		&_FrameRoot,
		&m_pAnimationController
		);
	
	if (_FrameRoot)
	{
		//骨に行列をセットする
		SetupBoneMatrixPointers(_FrameRoot, _FrameRoot);

		//メッシュリストを作成。
		_CreateMeshList();
		return true;
	}
	return false;
}

//モデルデータのクローンを作成
//すべてクローンで作成する。
void SkinModelData::CloneModelData(const SkinModelData* modelData, Animation* anim)
{
	//フレームを新しく作成
	_FrameRoot = new D3DXFRAME_DERIVED;
	_FrameRoot->pFrameFirstChild = nullptr;
	_FrameRoot->pFrameSibling = nullptr;
	_FrameRoot->pMeshContainer = nullptr;
	//骨のクローン作製
	CloneSkeleton(_FrameRoot, modelData->_FrameRoot);
	//アニメーションコントローラを作成して、スケルトンと関連付けを行う。
	if (modelData->m_pAnimationController) {
		modelData->m_pAnimationController->CloneAnimationController(
			modelData->m_pAnimationController->GetMaxNumAnimationOutputs(),
			modelData->m_pAnimationController->GetMaxNumAnimationSets(),
			modelData->m_pAnimationController->GetMaxNumTracks(),
			modelData->m_pAnimationController->GetMaxNumEvents(),
			&m_pAnimationController
		);
		//ウェイト設定？
		SetupOutputAnimationRegist(_FrameRoot, m_pAnimationController);

		//両方があるなら初期化する
		if (anim && m_pAnimationController) {
			anim->Initialize(m_pAnimationController);
		}
	}
	//マテリアルコピー
	this->_Materials = modelData->_Materials;
	//メッシュリストコピー
	this->_MeshList = modelData->_MeshList;
	//
	this->_TerrainSize = modelData->_TerrainSize;
	
	SetupBoneMatrixPointers(_FrameRoot, _FrameRoot);
}

//骨をコピー
void SkinModelData::CloneSkeleton(LPD3DXFRAME& dstFrame, LPD3DXFRAME srcFrame)
{
	//名前と行列をコピー。
	dstFrame->TransformationMatrix = srcFrame->TransformationMatrix;
	//メッシュコンテナをコピー。メッシュは使いまわす。
	if (srcFrame->pMeshContainer) {
		dstFrame->pMeshContainer = new D3DXMESHCONTAINER_DERIVED;
		memcpy(dstFrame->pMeshContainer, srcFrame->pMeshContainer, sizeof(D3DXMESHCONTAINER_DERIVED));
	}
	else {
		dstFrame->pMeshContainer = NULL;
	}
	AllocateName(srcFrame->Name, &dstFrame->Name);


	if (srcFrame->pFrameSibling != nullptr) {
		//兄弟がいるので、兄弟のためのメモリを確保。
		dstFrame->pFrameSibling = new D3DXFRAME_DERIVED;
		dstFrame->pFrameSibling->pFrameFirstChild = nullptr;
		dstFrame->pFrameSibling->pFrameSibling = nullptr;
		dstFrame->pFrameSibling->pMeshContainer = nullptr;
		CloneSkeleton(dstFrame->pFrameSibling, srcFrame->pFrameSibling);
	}
	if (srcFrame->pFrameFirstChild != nullptr)
	{
		//子供がいるので、子供のためのメモリを確保。
		dstFrame->pFrameFirstChild = new D3DXFRAME_DERIVED;
		dstFrame->pFrameFirstChild->pFrameFirstChild = nullptr;
		dstFrame->pFrameFirstChild->pFrameSibling = nullptr;
		dstFrame->pFrameFirstChild->pMeshContainer = nullptr;

		CloneSkeleton(dstFrame->pFrameFirstChild, srcFrame->pFrameFirstChild);
	}
}

void SkinModelData::DeleteCloneSkeleton(LPD3DXFRAME frame)
{

	if (frame->pFrameSibling != nullptr) {
		//兄弟
		DeleteCloneSkeleton(frame->pFrameSibling);
	}
	if (frame->pFrameFirstChild != nullptr)
	{
		//子供。
		DeleteCloneSkeleton(frame->pFrameFirstChild);
	}
	D3DXMESHCONTAINER_DERIVED* pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)(frame->pMeshContainer);
	if (pMeshContainer) {
		SAFE_DELETE_ARRAY(pMeshContainer->ppBoneMatrixPtrs);
		SAFE_DELETE(pMeshContainer);
	}
	SAFE_DELETE_ARRAY(frame->Name);
	SAFE_DELETE(frame);
}

void SkinModelData::SetupOutputAnimationRegist(LPD3DXFRAME frame, ID3DXAnimationController* animCtr)
{
	if (animCtr == nullptr) {
		return;
	}
	HRESULT hr = animCtr->RegisterAnimationOutput(frame->Name, &frame->TransformationMatrix, nullptr, nullptr, nullptr);
	if (frame->pFrameSibling != nullptr) {
		SetupOutputAnimationRegist(frame->pFrameSibling, animCtr);
	}
	if (frame->pFrameFirstChild != nullptr)
	{
		SetupOutputAnimationRegist(frame->pFrameFirstChild, animCtr);
	}
}

Material * SkinModelData::FindMaterial(const char * matName)
{
	for (Material* mat : _Materials) {
		if (strcmp(mat->GetName(), matName) == 0) {
			return mat;
		}
	}
	return nullptr;
}

/*!
* @brief	ボーン行列を更新。
*/
void SkinModelData::UpdateBoneMatrix(const D3DXMATRIX& matWorld)
{
	UpdateFrameMatrices(_FrameRoot, (const D3DXMATRIX*)&matWorld);
}

LPD3DXMESH SkinModelData::GetOrgMesh(LPD3DXFRAME frame) const
{
	D3DXMESHCONTAINER_DERIVED* pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)(frame->pMeshContainer);
	//メッシュコンテナが見つかったならメッシュを返す
	if (pMeshContainer != nullptr) {
		return pMeshContainer->pOrigMesh;
	}
	//見つからなかった場合兄弟、子供とメッシュコンテナを探す

	if (frame->pFrameSibling != nullptr) {
		//兄弟
		LPD3DXMESH mesh = GetOrgMesh(frame->pFrameSibling);

		if (mesh) {
			return mesh;
		}
	}
	if (frame->pFrameFirstChild != nullptr)
	{
		//子供。
		LPD3DXMESH mesh = GetOrgMesh(frame->pFrameFirstChild);
		if (mesh) {
			return mesh;
		}
	}

	return nullptr;
}
LPD3DXMESH SkinModelData::GetOrgMeshFirst() const
{
	return GetOrgMesh(_FrameRoot);
}

void SkinModelData::CalcWidthAndHeight()
{
	const std::vector<LPD3DXMESH>& meshList = GetMeshList();
	//番兵設定
	float minX = FLT_MAX;
	float minZ = FLT_MAX;
	float maxX = -FLT_MAX;
	float maxZ = -FLT_MAX;
	for (auto& mesh : meshList) {
		//頂点バッファを取得。
		LPDIRECT3DVERTEXBUFFER9 vb;
		mesh->GetVertexBuffer(&vb);
		//頂点定義を取得。
		D3DVERTEXBUFFER_DESC desc;
		vb->GetDesc(&desc);
		//頂点ストライドを取得。
		int stride = mesh->GetNumBytesPerVertex();
		D3DXVECTOR3* vertexPos;
		vb->Lock(0, desc.Size, (void**)&vertexPos, D3DLOCK_READONLY);
		//頂点数ループ
		for (unsigned int i = 0; i < mesh->GetNumVertices(); i++) {
			minX = min(minX, vertexPos->x);
			minZ = min(minZ, vertexPos->z);
			maxX = max(maxX, vertexPos->x);
			maxZ = max(maxZ, vertexPos->z);
			//次の頂点へ。
			char* p = (char*)vertexPos;
			p += stride;
			vertexPos = (D3DXVECTOR3*)p;
		}
		vb->Unlock();
		vb->Release();
	}
	_TerrainSize.x = minX;
	_TerrainSize.y = maxX;
	_TerrainSize.z = minZ;
	_TerrainSize.w = maxZ;
}

void SkinModelData::_CreateMeshList()
{
	//再帰関数呼び出し
	_QueryMeshes(_FrameRoot);
}

void SkinModelData::_QueryMeshes(LPD3DXFRAME frame)
{
	LPD3DXMESHCONTAINER pMeshContainer;
	pMeshContainer = frame->pMeshContainer;
	//全てのメッシュコンテナを見る
	while (pMeshContainer != NULL)
	{
		//メッシュを追加
		_MeshList.push_back(pMeshContainer->MeshData.pMesh);
		//次のメッシュコンテナを取得
		pMeshContainer = pMeshContainer->pNextMeshContainer;
	}
	//再帰処理
	if (frame->pFrameSibling != NULL)
	{
		_QueryMeshes(frame->pFrameSibling);
	}
	if (frame->pFrameFirstChild != NULL)
	{
		_QueryMeshes(frame->pFrameFirstChild);
	}
}