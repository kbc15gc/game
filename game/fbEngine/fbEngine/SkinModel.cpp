#include "SkinModel.h"
#include "Effect.h"
#include "Camera.h"
#include "Light.h"
#include "ShadowCamera.h"


//extern UINT                 g_NumBoneMatricesMax;
//extern D3DXMATRIXA16*       g_pBoneMatrices ;
bool g_PreRender = false;

SkinModel::SkinModel(GameObject * g, Transform * t) :
	Component(g, t, typeid(this).name()),
	_Effect(nullptr),
	_ModelDate(nullptr),
	_Camera(nullptr),
	_Light(nullptr),
	_ShadowCamera(nullptr),
	_TextureBlend(Color::white),
	_AllBlend(Color::white),
	_ModelEffect(ModelEffectE(ModelEffectE::CAST_SHADOW | ModelEffectE::RECEIVE_SHADOW)),
	_SkyBox(false)
{
	
}

SkinModel::~SkinModel()
{
	SAFE_DELETE(_ModelDate)
}

//�ċA�֐�
void SkinModel::DrawFrame(LPD3DXFRAME pFrame)
{
	D3DXMESHCONTAINER_DERIVED* pMeshContainer;

	pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pFrame->pMeshContainer;
	//�q�����Ă��郁�b�V���R���e�i���Ȃ��Ȃ�܂ő�����
	while (pMeshContainer != NULL)
	{
		//���b�V���`��
		DrawMeshContainer(
			pMeshContainer,
			pFrame
			);
		//���̃��b�V���R���e�i
		//��̃t���[���ɃR���e�i�͕�������H
		pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainer->pNextMeshContainer;
	}

	//�Z�킪����Ȃ���Z���ǂݍ���
	if (pFrame->pFrameSibling != NULL)
	{
		DrawFrame(pFrame->pFrameSibling);
	}

	//�q������Ȃ�q��ǂݍ���
	if (pFrame->pFrameFirstChild != NULL)
	{
		DrawFrame(pFrame->pFrameFirstChild);
	}
}

void SkinModel::Awake()
{
	//main�̂��̂��ݒ肳��Ă���Ȃ�Z�b�g�����B
	_Camera = GameObjectManager::mainCamera;
	_Light = GameObjectManager::mainLight;
	_ShadowCamera = GameObjectManager::mainShadowCamera;
}

//���f���f�[�^�̍s��X�V
void SkinModel::LateUpdate()
{
	//���f���f�[�^������Ȃ�
	if (_ModelDate)
	{
		//�|���郏�[���h�s��
		D3DXMATRIX wolrd;
		D3DXMatrixIdentity(&wolrd);
		wolrd = transform->GetWorldMatrix();
		
		_ModelDate->UpdateBoneMatrix(wolrd);	//�s����X�V�B
	}
}

void SkinModel::PreRender()
{
	g_PreRender = true;
	//���f���f�[�^������Ȃ�
	if (_ModelDate)
	{
		//�ċA�֐��Ăяo��
		DrawFrame(_ModelDate->GetFrameRoot());
	}
}

void SkinModel::Render()
{
	g_PreRender = false;
	//���f���f�[�^������Ȃ�
	if (_ModelDate)
	{
		//�ċA�֐��Ăяo��
		DrawFrame(_ModelDate->GetFrameRoot());
	}
}

void SkinModel::DrawMeshContainer(
	D3DXMESHCONTAINER_DERIVED* pMeshContainer,
	LPD3DXFRAME pFrameBase
)
{
	D3DXFRAME_DERIVED* pFrame = (D3DXFRAME_DERIVED*)pFrameBase;
	
	//���O�`��
	if(g_PreRender)
	{
		//�e�`��
		if (_ModelEffect & ModelEffectE::CAST_SHADOW)
			CreateShadow(pMeshContainer, pFrame);
	}
	//���f���`��
	else
	{
		//�G�t�F�N�g�ǂݍ���
		if (pMeshContainer->pSkinInfo != NULL)
			_Effect = EffectManager::LoadEffect("AnimationModel.fx");
		else
			_Effect = EffectManager::LoadEffect("3Dmodel.fx");

		//�e�N�j�b�N���Z�b�g
		_Effect->SetTechnique("NormalRender");
		//�J�n�i�K���I�����邱�Ɓj
		_Effect->Begin(NULL, D3DXFX_DONOTSAVESHADERSTATE);
		_Effect->BeginPass(0);

		const int num = GameObjectManager::mainLight->GetNum();
		Vector4 dir[System::MAX_LIGHTNUM];
		Color color[System::MAX_LIGHTNUM];
		ZeroMemory(dir, sizeof(Vector4)*System::MAX_LIGHTNUM);
		const vector<DirectionalLight*>& vec = GameObjectManager::mainLight->GetLight();
		FOR(i, num)
		{
			dir[i] = vec[i]->Direction();
			color[i] = vec[i]->GetColor();
		}
		
		//���C�g�̌�����]���B
		_Effect->SetValue("g_diffuseLightDirection", &dir, sizeof(Vector4)*System::MAX_LIGHTNUM);
		//���C�g�̃J���[��]���B
		_Effect->SetValue("g_diffuseLightColor", &color, sizeof(Color)*System::MAX_LIGHTNUM);
		//���C�g���Z�b�g
		_Effect->SetInt("g_LightNum", num);
		//����
		_Effect->SetVector("g_ambientLight", &D3DXVECTOR4(0.4, 0.4, 0.4, 1.0f));

		//�J�����̃|�W�V�����Z�b�g(�X�y�L�������C�g�p)
		Vector3 campos = GameObjectManager::mainCamera->transform->GetPosition();
		_Effect->SetValue("g_cameraPos", &D3DXVECTOR4(campos.x, campos.y, campos.z, 1.0f), sizeof(D3DXVECTOR4));
		
		//�e�s��𑗐M
		_Effect->SetMatrix("g_rotationMatrix", transform->GetRotateMatrixAddress());
		_Effect->SetMatrix("g_viewMatrix", &(D3DXMATRIX)_Camera->GetViewMat());
		_Effect->SetMatrix("g_projectionMatrix", &(D3DXMATRIX)_Camera->GetProjectionMat());
		
		//�e�J�����̃r���[�v���W�F�N�V�����s�������đ��M
		D3DXMATRIX LVP = GameObjectManager::mainShadowCamera->GetViewMat() * GameObjectManager::mainShadowCamera->GetProjectionMat();
		_Effect->SetMatrix("g_LVP", &LVP);

		//�[�x�e�N�X�`��
		TEXTURE* shadow = INSTANCE(RenderTargetManager)->GetRTTextureFromList(RTIdxE::SHADOWDEPTH);
		_Effect->SetTexture("g_Shadow", shadow->pTexture);
		Vector2 texel = Vector2(1.0f / shadow->Size.x, 1.0f / shadow->Size.y);
		_Effect->SetValue("g_TexelSize", &texel,sizeof(Vector2));
		//�e���f�����ǂ����̃t���O�Z�b�g
		_Effect->SetBool("ReceiveShadow", (_ModelEffect & ModelEffectE::RECEIVE_SHADOW) > 0);
		//�X�y�L�����t���O�Z�b�g
		_Effect->SetBool("Spec", (_ModelEffect & ModelEffectE::SPECULAR) > 0);

		_Effect->SetVector("g_blendcolor", (D3DXVECTOR4*)&_AllBlend);

		if(_SkyBox)
		{
			(*graphicsDevice()).SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		}

		(*graphicsDevice()).SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		(*graphicsDevice()).SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		(*graphicsDevice()).SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		//�A�j���[�V�����̗L���ŕ���
		if (pMeshContainer->pSkinInfo != NULL)
		{
			UINT iAttrib;
			//�o�b�t�@�[
			LPD3DXBONECOMBINATION pBoneComb = LPD3DXBONECOMBINATION(pMeshContainer->pBoneCombinationBuf->GetBufferPointer());
			//�e�}�e���A��
			for (iAttrib = 0; iAttrib < pMeshContainer->NumAttributeGroups; iAttrib++)
			{
				//�{�[��
				for (DWORD iPaletteEntry = 0; iPaletteEntry < pMeshContainer->NumPaletteEntries; ++iPaletteEntry)
				{
					DWORD iMatrixIndex = pBoneComb[iAttrib].BoneId[iPaletteEntry];
					if (iMatrixIndex != UINT_MAX)
					{
						//���̍ŏI�I�ȍs��v�Z
						D3DXMatrixMultiply(
							&_BoneMatrixPallets[iPaletteEntry],
							//���̃I�t�Z�b�g(�ړ�)�s��
							&pMeshContainer->pBoneOffsetMatrices[iMatrixIndex],
							//�t���[���̃��[���h�s��
							pMeshContainer->ppBoneMatrixPtrs[iMatrixIndex]
						);
					}
				}

				//���̃��[���h�s��z��
				_Effect->SetMatrixArray("g_mWorldMatrixArray", _BoneMatrixPallets, pMeshContainer->NumPaletteEntries);
				//���̐�
				_Effect->SetFloat("g_numBone", (float)pMeshContainer->NumInfl);

				//�f�B�t���[�Y�J���[�擾
				D3DXVECTOR4* Diffuse = (D3DXVECTOR4*)&pMeshContainer->pMaterials[pBoneComb[iAttrib].AttribId].MatD3D.Diffuse;
				//�e�N�X�`���[
				LPDIRECT3DTEXTURE9 texture = pMeshContainer->ppTextures[pBoneComb[iAttrib].AttribId];
				//�e�N�X�`�����i�[����Ă���΃Z�b�g
				if (texture != NULL)
				{
					// �f�B�t���[�Y�e�N�X�`���B
					_Effect->SetTexture("g_diffuseTexture", texture);
					_Effect->SetVector("g_Textureblendcolor", (D3DXVECTOR4*)&Color::white);
					//�Ƃ肠�����A����̓e�N�X�`���̖��O������
					
					if (strcmp(pMeshContainer->pMaterials[pBoneComb[iAttrib].AttribId].pTextureFilename, "TV_Head.png") == 0)
						_Effect->SetVector("g_Textureblendcolor", (D3DXVECTOR4*)&_TextureBlend);
					_Effect->SetBool("Texflg", true);
				}
				//�e�N�X�`�����Ȃ��Ȃ�J���[�Z�b�g
				else if (Diffuse != NULL)
				{
					_Effect->SetVector("g_diffuseMaterial", Diffuse);
					_Effect->SetBool("Texflg", false);
				}

				//���̊֐����Ăяo�����ƂŁA�f�[�^�̓]�����m�肷��B
				_Effect->CommitChanges();
				//���b�V���`��
				pMeshContainer->MeshData.pMesh->DrawSubset(iAttrib);
			}
		}
		//�A�j���[�V�������Ȃ���
		else
		{
			_Effect->SetMatrix("g_worldMatrix", &pFrame->CombinedTransformationMatrix);

			//�}�e���A���̐�
			DWORD MaterialNum = pMeshContainer->NumMaterials;
			//�}�e���A��
			D3DXMATERIAL *mtrl = (D3DXMATERIAL*)(pMeshContainer->pMaterials);

			//���f���`��
			for (DWORD i = 0; i < MaterialNum; i++)
			{
				//�f�B�t���[�Y�J���[
				D3DXVECTOR4* Diffuse = (D3DXVECTOR4*)&mtrl[i].MatD3D.Diffuse;
				//�e�N�X�`���[
				LPDIRECT3DBASETEXTURE9 texture;
				_Effect->SetBool("SkyBox", _SkyBox);
				if(_SkyBox)
				{
					texture = pMeshContainer->ppCubeTextures[i];
				}
				else
				{
					texture = pMeshContainer->ppTextures[i];
				}

				//�e�N�X�`�����i�[����Ă���΃Z�b�g
				if (texture != NULL)
				{
					_Effect->SetTexture("g_Texture", texture);
					_Effect->SetVector("g_Textureblendcolor", (D3DXVECTOR4*)&Color::white);
					_Effect->SetBool("Texflg", true);
				}
				//�e�N�X�`�����Ȃ��Ȃ�J���[�Z�b�g
				else
				{
					_Effect->SetVector("g_diffuseMaterial", Diffuse);
					_Effect->SetBool("Texflg", false);
				}

				//���̊֐����Ăяo�����ƂŁA�f�[�^�̓]�����m�肷��B
				_Effect->CommitChanges();
				//���b�V���`��
				pMeshContainer->MeshData.pMesh->DrawSubset(i);
			}
		}

		_Effect->EndPass();
		_Effect->End();

		if (_SkyBox)
		{
			(*graphicsDevice()).SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		}

		(*graphicsDevice()).SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		(*graphicsDevice()).SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
		(*graphicsDevice()).SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
	}
}

void SkinModel::CreateShadow(D3DXMESHCONTAINER_DERIVED * pMeshContainer, D3DXFRAME_DERIVED * pFrame)
{
	//�G�t�F�N�g�ǂݍ���
	if (pMeshContainer->pSkinInfo != NULL)
		_Effect = EffectManager::LoadEffect("AnimationModel.fx");
	else
		_Effect = EffectManager::LoadEffect("3Dmodel.fx");
	//�e�N�j�b�N�ݒ�
	_Effect->SetTechnique("Shadow");

	//�J�n
	_Effect->Begin(0, D3DXFX_DONOTSAVESTATE);
	_Effect->BeginPass(0);

	//�e�J�����̃r���[�v���W�F�N�V�����s��𑗂�
	_Effect->SetMatrix("g_viewMatrix", &(D3DXMATRIX)GameObjectManager::mainShadowCamera->GetViewMat());
	_Effect->SetMatrix("g_projectionMatrix", &(D3DXMATRIX)GameObjectManager::mainShadowCamera->GetProjectionMat());

	//�A�j���[�V�����̗L���ŕ���
	if (pMeshContainer->pSkinInfo != NULL)
	{
		UINT iAttrib;
		//�o�b�t�@�[
		LPD3DXBONECOMBINATION pBoneComb = LPD3DXBONECOMBINATION(pMeshContainer->pBoneCombinationBuf->GetBufferPointer());
		//�e�}�e���A��
		for (iAttrib = 0; iAttrib < pMeshContainer->NumMaterials; iAttrib++)
		{
			//�{�[��
			for (DWORD iPaletteEntry = 0; iPaletteEntry < pMeshContainer->NumPaletteEntries; ++iPaletteEntry)
			{
				DWORD iMatrixIndex = pBoneComb[iAttrib].BoneId[iPaletteEntry];
				if (iMatrixIndex != UINT_MAX)
				{
					//���̍ŏI�I�ȍs��v�Z
					D3DXMatrixMultiply(
						&_BoneMatrixPallets[iPaletteEntry],
						//���̃I�t�Z�b�g(�ړ�)�s��
						&pMeshContainer->pBoneOffsetMatrices[iMatrixIndex],
						//�t���[���̃��[���h�s��
						pMeshContainer->ppBoneMatrixPtrs[iMatrixIndex]
					);
				}
			}

			//���̃��[���h�s��z��
			_Effect->SetMatrixArray("g_mWorldMatrixArray", _BoneMatrixPallets, pMeshContainer->NumPaletteEntries);
			//���̐�
			_Effect->SetFloat("g_numBone", (float)pMeshContainer->NumInfl);
	

			//���̊֐����Ăяo�����ƂŁA�f�[�^�̓]�����m�肷��B
			_Effect->CommitChanges();
			//���b�V���`��
			pMeshContainer->MeshData.pMesh->DrawSubset(iAttrib);
		}
	}
	else
	{
		_Effect->SetMatrix("g_worldMatrix", &pFrame->CombinedTransformationMatrix);

		//���f���`��
		for (DWORD i = 0; i < pMeshContainer->NumMaterials; i++)
		{
			//���̊֐����Ăяo�����ƂŁA�f�[�^�̓]�����m�肷��B
			_Effect->CommitChanges();
			//���b�V���`��
			pMeshContainer->MeshData.pMesh->DrawSubset(i);
		}
	}

	//�I��
	_Effect->EndPass();
	_Effect->End();
}