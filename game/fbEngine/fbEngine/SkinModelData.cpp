/*!
 *@brief	�X�L�����f���f�[�^�B
 */

#include "SkinModelData.h"
#include "Animation.h"

//UINT                        g_NumBoneMatricesMax = 0;
//D3DXMATRIXA16*              g_pBoneMatrices = NULL;

//���f����Frame�X�V
void UpdateFrameMatrices(LPD3DXFRAME pFrameBase, const D3DXMATRIX* pParentMatrix)
{
	D3DXFRAME_DERIVED* pFrame = (D3DXFRAME_DERIVED*)pFrameBase;

	if (pParentMatrix != NULL)
		//���[���h�ϊ��s����쐬
		D3DXMatrixMultiply(&pFrame->CombinedTransformationMatrix, &pFrame->TransformationMatrix, pParentMatrix);
	else
		//���̂܂�
		pFrame->CombinedTransformationMatrix = pFrame->TransformationMatrix;

	//�Z��X�V
	if (pFrame->pFrameSibling != NULL)
	{
		//�����s���n��
		UpdateFrameMatrices(pFrame->pFrameSibling, pParentMatrix);
	}

	//�q�X�V
	if (pFrame->pFrameFirstChild != NULL)
	{
		//�e�̍s��ōX�V
		UpdateFrameMatrices(pFrame->pFrameFirstChild, &pFrame->CombinedTransformationMatrix);
	}
}
	
//���b�V�����_���O���[�v�ɕ�����
HRESULT GenerateSkinnedMesh(
	IDirect3DDevice9* pd3dDevice,
	D3DXMESHCONTAINER_DERIVED* pMeshContainer
	)
{
	HRESULT hr = S_OK;
	D3DCAPS9 d3dCaps;
	pd3dDevice->GetDeviceCaps(&d3dCaps);
	//�{�[����񂪂Ȃ��Ȃ�I��
	if (pMeshContainer->pSkinInfo == NULL)
		return hr;

	SAFE_RELEASE(pMeshContainer->MeshData.pMesh);
	SAFE_RELEASE(pMeshContainer->pBoneCombinationBuf);

	{
		// Get palette size
		// First 9 constants are used for other data.  Each 4x3 matrix takes up 3 constants.
		// (96 - 9) /3 i.e. Maximum constant count - used constants 
		//���̍ő�s��
		//�������̐�����ς����Ȃ�V�F�[�_�̕����ς��邱��
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

		//�{�[���ƒ��_�̃u�����h�C���f�b�N�X����V�������b�V�����쐬
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

	//�W���̃��b�V���R���e�i�������ŃJ�X�^�}�C�Y�������b�V���R���e�i�ɃL���X�g
	D3DXMESHCONTAINER_DERIVED* pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerBase;

	//�X�L�����(�{�[�������邩�ǂ���)�m�F
	if (pMeshContainer->pSkinInfo != NULL)
	{
		//�{�[���̐��擾
		cBones = pMeshContainer->pSkinInfo->GetNumBones();

		//�{�[���̍s��̔z����m��
		pMeshContainer->ppBoneMatrixPtrs = new D3DXMATRIX*[cBones];
		//�����˂��I
		if (pMeshContainer->ppBoneMatrixPtrs == NULL)
			return E_OUTOFMEMORY;

		for (iBone = 0; iBone < cBones; iBone++)
		{
			//���[�g�t���[������{�[���̖��O�Ō�����,�q�t���[�����擾
			pFrame = (D3DXFRAME_DERIVED*)D3DXFrameFind(rootFrame,
				pMeshContainer->pSkinInfo->GetBoneName(iBone));
			//���ɑΉ������t���[����������Ȃ������E�E�E
			if (pFrame == NULL)
				return E_FAIL;
			
			//�t���[������s����擾��,���̍s��Ƃ���
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
		//���̍s��ݒ�
		hr = SetupBoneMatrixPointersOnMesh(pFrame->pMeshContainer, pRootFrame);
		if (FAILED(hr))
			return hr;
	}

	//�Z�평����(�ċA)
	if (pFrame->pFrameSibling != NULL)
	{
		hr = SetupBoneMatrixPointers(pFrame->pFrameSibling, pRootFrame);
		if (FAILED(hr))
			return hr;
	}

	//�q������(�ċA)
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
	//�t���[������
	STDMETHOD(CreateFrame)(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame);
	//���b�V���R���e�i���쐬
	STDMETHOD(CreateMeshContainer)(THIS_
		LPCSTR Name,
		CONST D3DXMESHDATA *pMeshData,
		CONST D3DXMATERIAL *pMaterials,
		CONST D3DXEFFECTINSTANCE *pEffectInstances,
		DWORD NumMaterials,
		CONST DWORD *pAdjacency,
		LPD3DXSKININFO pSkinInfo,
		LPD3DXMESHCONTAINER *ppNewMeshContainer);
	//�t���[���폜
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree);
	//���b�V���R���e�i���폜
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerBase);

	CAllocateHierarchy()
	{
	}
	
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
//���b�V���R���e�i���쐬
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
	//�}�e���A���̐��ݒ�
	pMeshContainer->NumMaterials = max(1, NumMaterials);
	pMeshContainer->pMaterials = new D3DXMATERIAL[pMeshContainer->NumMaterials];
	pMeshContainer->ppTextures = new LPDIRECT3DTEXTURE9[pMeshContainer->NumMaterials];
	pMeshContainer->ppCubeTextures = new LPDIRECT3DCUBETEXTURE9[pMeshContainer->NumMaterials];
	pMeshContainer->pAdjacency = new DWORD[NumFaces * 3];
	if ((pMeshContainer->pAdjacency == NULL) || (pMeshContainer->pMaterials == NULL))
	{
		hr = E_OUTOFMEMORY;
		goto e_Exit;
	}

	memcpy(pMeshContainer->pAdjacency, pAdjacency, sizeof(DWORD)* NumFaces * 3);
	//�e�N�X�`���[��0�ŏ�����
	memset(pMeshContainer->ppTextures, 0, sizeof(LPDIRECT3DTEXTURE9)* pMeshContainer->NumMaterials);
	memset(pMeshContainer->ppCubeTextures, 0, sizeof(LPDIRECT3DCUBETEXTURE9)* pMeshContainer->NumMaterials);

	//�}�e���A���̐������摜��ǂݍ��݁A�R�s�[
	if (NumMaterials > 0)
	{
		memcpy(pMeshContainer->pMaterials, pMaterials, sizeof(D3DXMATERIAL)* NumMaterials);
		for (iMaterial = 0; iMaterial < NumMaterials; iMaterial++)
		{
			
			//�t�@�C���w�肪����Ȃ�
			if (pMeshContainer->pMaterials[iMaterial].pTextureFilename != NULL)
			{
				pMeshContainer->pMaterials[iMaterial].pTextureFilename = new CHAR[strlen(pMaterials[iMaterial].pTextureFilename) + 1];
				strcpy(pMeshContainer->pMaterials[iMaterial].pTextureFilename, pMaterials[iMaterial].pTextureFilename);

				//�摜�̃p�X�ǉ�
				char* baseDir = "Asset/Xfile/";
				char filePath[64];
				strcpy_s(filePath, baseDir);
				strcat_s(filePath, pMeshContainer->pMaterials[iMaterial].pTextureFilename);
				if (FAILED(D3DXCreateTextureFromFile(
					pd3dDevice,
					filePath,
					&pMeshContainer->ppTextures[iMaterial]))
					) 
				{
					//�Ȃ�����
					pMeshContainer->ppTextures[iMaterial] = NULL;
				}
				//�L���[�u�e�N�X�`��
				if( FAILED(D3DXCreateCubeTextureFromFile(
					pd3dDevice,
					filePath,
					&pMeshContainer->ppCubeTextures[iMaterial])))
				{
					pMeshContainer->ppCubeTextures[iMaterial] = NULL;
				}
					// don't remember a pointer into the dynamic memory, just forget the name after loading
					//�悭�킩��񂪃t�@�C���p�X�������Ă���B
					//pMeshContainer->pMaterials[iMaterial].pTextureFilename = NULL;
			}
		}
	}
	else //�}�e���A�����Ȃ���[
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

		

		//�{�[���̐������{�[���̃I�t�Z�b�g�s��m��
		cBones = pSkinInfo->GetNumBones();
		pMeshContainer->pBoneOffsetMatrices = new D3DXMATRIX[cBones];
		if (pMeshContainer->pBoneOffsetMatrices == NULL)
		{
			hr = E_OUTOFMEMORY;
			goto e_Exit;
		}

		//�{�[���̃I�t�Z�b�g�s��Z�b�g
		for (iBone = 0; iBone < cBones; iBone++)
		{
			pMeshContainer->pBoneOffsetMatrices[iBone] = *(pMeshContainer->pSkinInfo->GetBoneOffsetMatrix(iBone));
		}

		//���b�V�����_���O���[�v�ɕ�����
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
	if (pMeshContainer->ppTextures != NULL)
	{
		for (iMaterial = 0; iMaterial < pMeshContainer->NumMaterials; iMaterial++)
		{
			SAFE_RELEASE(pMeshContainer->ppTextures[iMaterial]);
			SAFE_DELETE_ARRAY(pMeshContainer->pMaterials[iMaterial].pTextureFilename);
			SAFE_RELEASE(pMeshContainer->ppCubeTextures[iMaterial]);
		}
	}

	SAFE_DELETE_ARRAY(pMeshContainer->pMaterials);
	SAFE_DELETE_ARRAY(pMeshContainer->ppTextures);
	SAFE_DELETE_ARRAY(pMeshContainer->ppCubeTextures);
	SAFE_DELETE_ARRAY(pMeshContainer->ppBoneMatrixPtrs);
	SAFE_RELEASE(pMeshContainer->pBoneCombinationBuf);
	SAFE_RELEASE(pMeshContainer->MeshData.pMesh);
	SAFE_RELEASE(pMeshContainer->pSkinInfo);
	SAFE_RELEASE(pMeshContainer->pOrigMesh);
	SAFE_DELETE(pMeshContainer);
	return S_OK;
}

/*!
 *@brief	�R���X�g���N�^�B
 */
SkinModelData::SkinModelData():
m_frameRoot(nullptr),
m_pAnimationController(nullptr)
{
	
}

/*!
 *@brief	�f�X�g���N�^�B
 */
SkinModelData::~SkinModelData()
{
	Release();
}

/*!
* @brief	�����[�X�B
*/
void SkinModelData::Release()
{
	if (m_frameRoot) {
		//�N���[��
		DeleteCloneSkeleton(m_frameRoot);
		m_frameRoot = nullptr;
	}
	SAFE_RELEASE(m_pAnimationController);
}
	
/*!
 * @brief	���f���f�[�^�����[�h�B
 *@param[in]	filePath	�t�@�C���p�X�B
 */
void SkinModelData::LoadModelData(const char* filePath)
{
	CAllocateHierarchy alloc;
	HRESULT hr = D3DXLoadMeshHierarchyFromXA(
		filePath,
		D3DXMESH_VB_MANAGED,
		graphicsDevice(),
		&alloc,
		nullptr,
		&m_frameRoot,
		&m_pAnimationController
		);
	
	//���ɍs����Z�b�g����
	SetupBoneMatrixPointers(m_frameRoot, m_frameRoot);
}

//���f���f�[�^�̃N���[�����쐬
//���ׂăN���[���ō쐬����B
void SkinModelData::CloneModelData(const SkinModelData* modelData, Animation* anim)
{
	//�t���[����V�����쐬
	m_frameRoot = new D3DXFRAME_DERIVED;
	m_frameRoot->pFrameFirstChild = nullptr;
	m_frameRoot->pFrameSibling = nullptr;
	m_frameRoot->pMeshContainer = nullptr;
	//���̃N���[���쐻
	CloneSkeleton(m_frameRoot, modelData->m_frameRoot);
	//�A�j���[�V�����R���g���[�����쐬���āA�X�P���g���Ɗ֘A�t�����s���B
	if (modelData->m_pAnimationController) {
		modelData->m_pAnimationController->CloneAnimationController(
			modelData->m_pAnimationController->GetMaxNumAnimationOutputs(),
			modelData->m_pAnimationController->GetMaxNumAnimationSets(),
			modelData->m_pAnimationController->GetMaxNumTracks(),
			modelData->m_pAnimationController->GetMaxNumEvents(),
			&m_pAnimationController
		);
		//�E�F�C�g�ݒ�H
		SetupOutputAnimationRegist(m_frameRoot, m_pAnimationController);

		//����������Ȃ珉��������
		if (anim && m_pAnimationController) {
			anim->Initialize(m_pAnimationController);
		}
	}
	SetupBoneMatrixPointers(m_frameRoot, m_frameRoot);
}

//�����R�s�[
void SkinModelData::CloneSkeleton(LPD3DXFRAME& dstFrame, LPD3DXFRAME srcFrame)
{
	//���O�ƍs����R�s�[�B
	dstFrame->TransformationMatrix = srcFrame->TransformationMatrix;
	//���b�V���R���e�i���R�s�[�B���b�V���͎g���܂킷�B
	if (srcFrame->pMeshContainer) {
		dstFrame->pMeshContainer = new D3DXMESHCONTAINER_DERIVED;
		memcpy(dstFrame->pMeshContainer, srcFrame->pMeshContainer, sizeof(D3DXMESHCONTAINER_DERIVED));
	}
	else {
		dstFrame->pMeshContainer = NULL;
	}
	AllocateName(srcFrame->Name, &dstFrame->Name);


	if (srcFrame->pFrameSibling != nullptr) {
		//�Z�킪����̂ŁA�Z��̂��߂̃��������m�ہB
		dstFrame->pFrameSibling = new D3DXFRAME_DERIVED;
		dstFrame->pFrameSibling->pFrameFirstChild = nullptr;
		dstFrame->pFrameSibling->pFrameSibling = nullptr;
		dstFrame->pFrameSibling->pMeshContainer = nullptr;
		CloneSkeleton(dstFrame->pFrameSibling, srcFrame->pFrameSibling);
	}
	if (srcFrame->pFrameFirstChild != nullptr)
	{
		//�q��������̂ŁA�q���̂��߂̃��������m�ہB
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
		//�Z��
		DeleteCloneSkeleton(frame->pFrameSibling);
	}
	if (frame->pFrameFirstChild != nullptr)
	{
		//�q���B
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

/*!
* @brief	�{�[���s����X�V�B
*/
void SkinModelData::UpdateBoneMatrix(const D3DXMATRIX& matWorld)
{
	UpdateFrameMatrices(m_frameRoot, (const D3DXMATRIX*)&matWorld);
}

LPD3DXMESH SkinModelData::GetOrgMesh(LPD3DXFRAME frame) const
{
	D3DXMESHCONTAINER_DERIVED* pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)(frame->pMeshContainer);
	//���b�V���R���e�i�����������Ȃ烁�b�V����Ԃ�
	if (pMeshContainer != nullptr) {
		return pMeshContainer->pOrigMesh;
	}
	//������Ȃ������ꍇ�Z��A�q���ƃ��b�V���R���e�i��T��

	if (frame->pFrameSibling != nullptr) {
		//�Z��
		LPD3DXMESH mesh = GetOrgMesh(frame->pFrameSibling);

		if (mesh) {
			return mesh;
		}
	}
	if (frame->pFrameFirstChild != nullptr)
	{
		//�q���B
		LPD3DXMESH mesh = GetOrgMesh(frame->pFrameFirstChild);
		if (mesh) {
			return mesh;
		}
	}

	return nullptr;
}
LPD3DXMESH SkinModelData::GetOrgMeshFirst() const
{
	return GetOrgMesh(m_frameRoot);
}