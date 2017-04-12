/*!
 *@brief	�X�L�����f���f�[�^
 */
#pragma once
class Animation;

struct D3DXFRAME_DERIVED : public D3DXFRAME {
	D3DXMATRIXA16	CombinedTransformationMatrix;	//�����ςݍs��B
};
//���b�V���R���e�i���p�������g�����b�V���R���e�i
struct D3DXMESHCONTAINER_DERIVED : public D3DXMESHCONTAINER {
	//�e�N�X�`��
	LPDIRECT3DTEXTURE9* ppTextures;
	//�L���[�u
	LPDIRECT3DCUBETEXTURE9* ppCubeTextures;
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
	/*!
	 * @brief	���f���f�[�^�����[�h�B
	 *@param[in]	filePath	�t�@�C���p�X�B
	 */
	void LoadModelData(const char* filePath);
	//���f���f�[�^�̃N���[���쐻
	void CloneModelData(const SkinModelData* modelData, Animation* anim = nullptr);
	

	/*!
	* @brief	�����[�X�B
	*/
	void Release();

	//�ŏ�(�����H)�̃t���[���擾
	LPD3DXFRAME GetFrameRoot()
	{
		return m_frameRoot;
	}

	ID3DXAnimationController* GetAnimationController()
	{
		return m_pAnimationController;
	}

	//�����Ƃ��Ď󂯎�����s���e�Ƃ���,���̍s����X�V
	//�������FD3DXMATRIX&�@���[���h�s��
	void UpdateBoneMatrix(const D3DXMATRIX& matWorld);
	LPD3DXMESH GetOrgMeshFirst() const;
private:
	LPD3DXFRAME					m_frameRoot;		//�t���[�����[�g�B
	//�A�j���[�V�����R���g���[���[
	ID3DXAnimationController* m_pAnimationController;

	LPD3DXMESH GetOrgMesh(LPD3DXFRAME frame) const;

	void CloneSkeleton(LPD3DXFRAME& dstFrame, LPD3DXFRAME srcFrame);
	void DeleteCloneSkeleton(LPD3DXFRAME frame);
	void SetupOutputAnimationRegist(LPD3DXFRAME frame, ID3DXAnimationController* animCtr);
};