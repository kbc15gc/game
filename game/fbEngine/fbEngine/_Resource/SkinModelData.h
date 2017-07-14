/*!
 *@brief	�X�L�����f���f�[�^
 */
#pragma once
#include "_Object/Noncopyable.h"

struct D3DXFRAME_DERIVED : public D3DXFRAME {
	D3DXMATRIXA16	CombinedTransformationMatrix;	//�����ςݍs��B
	D3DXMATRIX* RotationMatrix = nullptr;
};
//���b�V���R���e�i���p�������g�����b�V���R���e�i
struct D3DXMESHCONTAINER_DERIVED : public D3DXMESHCONTAINER {
	//�}�e���A��
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
	
	//���f���f�[�^�����[�h�B
	//[in]	filePath	�t�@�C���p�X�B
	//[out] ���[�h�������������ǂ����H 
	bool LoadModelData(const char* filePath);
	//���f���f�[�^�̃N���[���쐻
	void CloneModelData(const SkinModelData* modelData, Animation* anim = nullptr);
	

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

	//�e���C����XZ���̏c���Ɖ��������߂�֐�
	//��������
	void CalcWidthAndHeight();
	
	//�X�L�����f���̃��b�V���̃��X�g���擾�B
	const std::vector<LPD3DXMESH>& GetMeshList() const
	{
		return _MeshList;
	}
	const Vector4& GetTerrainSize()
	{
		return _TerrainSize;
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
	//�A�j���[�V�����R���g���[���[
	ID3DXAnimationController* m_pAnimationController;
	//�Ƃ肠�����ق��������������B���������B
	Vector4 _TerrainSize;
	//�C���X�^���V���O�`��t���O�B�f�t�H���g��false�B
	bool _Instancing;
};