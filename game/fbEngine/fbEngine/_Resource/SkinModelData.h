/*!
 *@brief	�X�L�����f���f�[�^
 */
#pragma once
#include "_Object/Noncopyable.h"
#include "_Value\AABB.h"

struct D3DXFRAME_DERIVED : public D3DXFRAME {
	D3DXMATRIXA16	CombinedTransformationMatrix;	//�����ςݍs��B
	D3DXMATRIX* RotationMatrix = nullptr;
	//�I���W�i���ւ̃|�C���^�B
	D3DXFRAME_DERIVED* Original;
	//�C���X�^���V���O�Ŏg���A���[���h�s���ςރx�N�^�[�B
	vector<D3DXMATRIX*> WorldMatrixStack;
};
//���b�V���R���e�i���p�������g�����b�V���R���e�i
struct D3DXMESHCONTAINER_DERIVED : public D3DXMESHCONTAINER {
	//�}�e���A��
	Material** material;
	//�I���W�i���̃��b�V���B
	LPD3DXMESH pOrigMesh;
	//
	LPD3DXATTRIBUTERANGE pAttributeTable;
	//
	DWORD NumAttributeGroups;
	//
	DWORD NumInfl;
	//
	LPD3DXBUFFER pBoneCombinationBuf;
	//�t���[���̍����ςݍs��ւ̎Q��
	D3DXMATRIX** ppBoneMatrixPtrs;
	//�{�[���̃I�t�Z�b�g�s��
	D3DXMATRIX* pBoneOffsetMatrices;
	//
	DWORD NumPaletteEntries;
	//
	bool UseSoftwareVP;
	//
	DWORD iAttributeSW;
	//���_��`�̍쐬�B
	IDirect3DVertexDeclaration9* vertexDecl = NULL;
	//���[���h�s��p�̃o�b�t�@�̍쐬�B
	IDirect3DVertexBuffer9* worldMatrixBuffer = NULL;			//���[���h�s��̃o�b�t�@�B
	
};

//���f���̃f�[�^�������N���X
class SkinModelData:Noncopyable
{
public:
	/*!
	 *@brief	�R���X�g���N�^�B
	 */
	SkinModelData();
	/*!
	 *@brief	�f�X�g���N�^�B
	 */
	~SkinModelData();
	
	//AABB�X�V�B
	void UpdateAABB()
	{
		Vector3 half = _Size;
		half.Scale(0.5f);
		_AABB.Update(_Center, half);
	}

	//AABB���擾�B
	AABB GetAABB()
	{
		return _AABB;
	}
	//���f���̒��S���W�B
	const Vector3& GetCenterPos() { return _Center; }
	
	//���f���f�[�^�����[�h�B
	//[in]	filePath	�t�@�C���p�X�B
	//[out] ���[�h�������������ǂ����H 
	bool LoadModelData(const char* filePath);
	//���f���f�[�^�̃N���[���쐻
	void CloneModelData(const SkinModelData* original, Animation* anim = nullptr);
	

	/*!
	* @brief	�����[�X�B
	*/
	void Release();

	//�ŏ�(�����H)�̃t���[���擾
	LPD3DXFRAME GetFrameRoot()
	{
		return _FrameRoot;
	}

	ID3DXAnimationController* GetAnimationController()
	{
		return m_pAnimationController;
	}
	//�}�e���A���̒ǉ�(�G���W����p)
	void AddMaterial(Material* mat)
	{
		_Materials.push_back(mat);
	}
	Material* FindMaterial(const char* matName);

	//�����Ƃ��Ď󂯎�����s���e�Ƃ���,���̍s����X�V
	//�������FD3DXMATRIX&�@���[���h�s��
	void UpdateBoneMatrix(const D3DXMATRIX& matWorld);
	//��ԍŏ��̃��b�V�����擾
	LPD3DXMESH GetOrgMeshFirst() const;

	//���f���̃T�C�Y�𑪒肷��B
	void Measurement();
	
	//�X�L�����f���̃��b�V���̃��X�g���擾�B
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
	//�C���X�^���V���O�t���O��ݒ�B
	void SetInstancing(bool flg)
	{
		_Instancing = flg;
	}
	//�t���O��Ԃ��B
	const bool& GetInstancing()
	{
		return _Instancing;
	}

public:
	//�C���X�^���V���O�l�Ƀ��[���h�s���ςށB
	//�`��ς݃t���O�擾�B
	bool GetAlreadyDrawn()
	{
		return _Original->_AlreadyDrawn;
	}
	//�C���X�^���V���O�J�n�B
	void StartInstancing()
	{
		_Original->_AlreadyDrawn = false;
	}
	//�C���X�^���V���O�I���B
	void EndInstancing()
	{
		_Original->_AlreadyDrawn = true;
	}
private:
	//�ŏ��̃��b�V����Ԃ��H
	LPD3DXMESH GetOrgMesh(LPD3DXFRAME frame) const;
	//�N���[���쐻
	void CloneSkeleton(LPD3DXFRAME& dstFrame, LPD3DXFRAME srcFrame);
	//�N���[���폜
	void DeleteCloneSkeleton(LPD3DXFRAME frame);
	//�A�j���[�V�����̂Ȃɂ���ݒ�(�s��̂�Ƃ����Ǝv��)
	void SetupOutputAnimationRegist(LPD3DXFRAME frame, ID3DXAnimationController* animCtr);
	//���b�V���̃��X�g���쐬
	void _CreateMeshList();
	//���b�V���̃��X�g���쐬�Ɏg���ċA�֐�
	void _QueryMeshes(LPD3DXFRAME frame);
private:
	LPD3DXFRAME					_FrameRoot;		//�t���[�����[�g(��{)�B
	std::vector<Material*>		_Materials;		//�}�e���A���̃��X�g
	std::vector<LPD3DXMESH>		_MeshList;		//���b�V���̃��X�g�B
	std::vector<LPD3DXFRAME>	_FrameList;
	//�A�j���[�V�����R���g���[���[
	ID3DXAnimationController* m_pAnimationController;
	//�Ƃ肠�����ق��������������B���������B
	Vector4 _TerrainSize;
	//
	Vector3 _Size;
	//���S���W
	Vector3 _Center;
	//
	AABB _AABB;
private:
	//�C���X�^���V���O�`��t���O�B�f�t�H���g��false�B
	bool _Instancing;
	//�`��ς݃t���O�B�C���X�^���V���O�Ŏg���B
	bool _AlreadyDrawn;
	//�I���W�i�����f���ւ̃|�C���^�B
	SkinModelData* _Original;
};