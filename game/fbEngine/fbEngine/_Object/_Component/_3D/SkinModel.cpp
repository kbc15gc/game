#include"fbstdafx.h"
#include "SkinModel.h"

#include "_Effect\EffectManager.h"
#include "_Object\_Component\_3D\Light.h"
#include "_Object\_Component\_3D\Camera.h"
#include "_Object\_Component\_3D\ShadowCamera.h"

#include"_Nature\Sky.h"

//extern UINT                 g_NumBoneMatricesMax;
//extern D3DXMATRIXA16*       g_pBoneMatrices ;

namespace
{
	/** �V���h�E�}�b�v�`��t���O. */
	bool g_ShadowRender = false;
	/** ���}�b�v�`��t���O. */
	bool g_EnvironmentRender = false;

}

SkinModel::SkinModel(GameObject * g, Transform * t) :
	Component(g, t, typeid(this).name(),100),
	_Effect(nullptr),
	_ModelDate(nullptr),
	_Camera(nullptr),
	_Light(nullptr),
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
	
}

void SkinModel::Start()
{
	//main�̂��̂��ݒ肳��Ă���Ȃ�Z�b�g�����B
	_Camera = INSTANCE(GameObjectManager)->mainCamera;
	_Light = INSTANCE(GameObjectManager)->mainLight;
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
	if (_ModelEffect & ModelEffectE::CAST_SHADOW)
	{
		INSTANCE(SceneManager)->GetShadowMap()->EntryModel(this);
	}
	if (_ModelEffect & ModelEffectE::CAST_ENVIRONMENT)
	{
		INSTANCE(SceneManager)->GetEnvironmentMap()->EntryModel(this);
	}
}

void SkinModel::Render()
{
	g_ShadowRender = false;
	g_EnvironmentRender = false;
	//���f���f�[�^������Ȃ�
	if (_ModelDate)
	{
		//�ċA�֐��Ăяo��
		DrawFrame(_ModelDate->GetFrameRoot());
	}
}

/**
* �V���h�E�}�b�v�ɐ[�x����������.
* �V���h�E�}�b�v�N���X����Ă΂�Ă���.
*/
void SkinModel::RenderToShadowMap()
{
	g_ShadowRender = true;
	g_EnvironmentRender = false;

	//���f���f�[�^������Ȃ�
	if (_ModelDate)
	{
		//�ċA�֐��Ăяo��
		DrawFrame(_ModelDate->GetFrameRoot());
	}
}

/**
* ���}�b�v�ɕ`�悷��.
* ���}�b�v�N���X����Ă΂�Ă���.
*/
void SkinModel::RenderToEnvironmentMap()
{
	g_ShadowRender = false;
	g_EnvironmentRender = true;
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
	
	//�e�`��
	if (g_ShadowRender)
	{
		if (_ModelEffect & ModelEffectE::CAST_SHADOW)
		{
			CreateShadow(pMeshContainer, pFrame);
		}
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
		if(terain)
		{
			_Effect->SetTechnique("TerrainRender");
		}
		else
		{
			_Effect->SetTechnique("NormalRender");
		}
		//�J�n�i�K���I�����邱�Ɓj
		_Effect->Begin(NULL, D3DXFX_DONOTSAVESHADERSTATE);
		_Effect->BeginPass(0);

		const int num = INSTANCE(GameObjectManager)->mainLight->GetNum();
		Vector4 dir[System::MAX_LIGHTNUM];
		Color color[System::MAX_LIGHTNUM];
		ZeroMemory(dir, sizeof(Vector4)*System::MAX_LIGHTNUM);
		const vector<DirectionalLight*>& vec = INSTANCE(GameObjectManager)->mainLight->GetLight();
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
		Vector3 ambient = INSTANCE(GameObjectManager)->mainLight->GetAmbientLight();
		_Effect->SetVector("g_ambientLight", &D3DXVECTOR4(ambient.x, ambient.y, ambient.z, 1.0f));

		//�J�����̃|�W�V�����Z�b�g(�X�y�L�������C�g�p)
		Vector3 campos = INSTANCE(GameObjectManager)->mainCamera->transform->GetPosition();
		_Effect->SetValue("g_cameraPos", &D3DXVECTOR4(campos.x, campos.y, campos.z, 1.0f), sizeof(D3DXVECTOR4));
		
		//�e�s��𑗐M
		_Effect->SetMatrix("g_rotationMatrix", transform->GetRotateMatrixAddress());
		
		if (g_EnvironmentRender)
		{
			_Effect->SetMatrix("g_viewMatrix", &(D3DXMATRIX)INSTANCE(SceneManager)->GetEnvironmentMap()->GetViewMatrix());
			_Effect->SetMatrix("g_projectionMatrix", &(D3DXMATRIX)INSTANCE(SceneManager)->GetEnvironmentMap()->GetProjMatrix());
		}
		else
		{
			_Effect->SetMatrix("g_viewMatrix", &(D3DXMATRIX)INSTANCE(GameObjectManager)->mainCamera->GetViewMat());
			_Effect->SetMatrix("g_projectionMatrix", &(D3DXMATRIX)INSTANCE(GameObjectManager)->mainCamera->GetProjectionMat());
		}

		{
			//���V�[�o�[�p�p�����[�^��ݒ�.
			ShadowMap::ShadowReceiverParam* shadowParam = INSTANCE(SceneManager)->GetShadowMap()->GetShadowReceiverParam();
			_Effect->SetValue("g_ShadowReceiverParam", shadowParam, sizeof(ShadowMap::ShadowReceiverParam));
			//�[�x�e�N�X�`��
			_Effect->SetTexture("g_ShadowMap_0", INSTANCE(SceneManager)->GetShadowMap()->GetShadowMapTexture(0));
			_Effect->SetTexture("g_ShadowMap_1", INSTANCE(SceneManager)->GetShadowMap()->GetShadowMapTexture(1));
			_Effect->SetTexture("g_ShadowMap_2", INSTANCE(SceneManager)->GetShadowMap()->GetShadowMapTexture(2));
			//�e���f�����ǂ����̃t���O�Z�b�g
			_Effect->SetBool("ReceiveShadow", (_ModelEffect & ModelEffectE::RECEIVE_SHADOW) > 0);
		}

		Vector2 size = 20;
		Vector2 texel = Vector2(1.0f / size.x, 1.0f / size.y);
		_Effect->SetValue("g_TexelSize", &texel, sizeof(Vector2));

		//�t���O�ݒ�
		Vector4 flg;
		flg.x = (_ModelEffect & ModelEffectE::RECEIVE_SHADOW) > 0;

		_Effect->SetValue("g_EffectFlg", &flg, sizeof(Vector4));

		//�X�y�L�����t���O�Z�b�g
		_Effect->SetBool("Spec", (_ModelEffect & ModelEffectE::SPECULAR) > 0);

		_Effect->SetVector("g_blendcolor", (D3DXVECTOR4*)&_AllBlend);

		AtmosphericScatteringParamS atmos = INSTANCE(SceneManager)->GetSky()->GetAtmosphericScatteringParam();
		_Effect->SetValue("g_atmosParam", &atmos, sizeof(atmos));
		_Effect->SetInt("g_atmosFlag", _AtomosphereFunc);

	
		(*graphicsDevice()).SetRenderState(D3DRS_ZWRITEENABLE, _SkyBox ? FALSE : TRUE);
		(*graphicsDevice()).SetRenderState(D3DRS_ZENABLE, _SkyBox ? FALSE : TRUE);
		(*graphicsDevice()).SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		(*graphicsDevice()).SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		(*graphicsDevice()).SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		//(*graphicsDevice()).SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		//(*graphicsDevice()).SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		(*graphicsDevice()).SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
		(*graphicsDevice()).SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
	

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
				//�}�e���A��
				Material* material = pMeshContainer->material[pBoneComb[iAttrib].AttribId];
				
				//�e�N�X�`�����i�[����Ă���΃Z�b�g
				if (material != NULL)
				{
					// �f�B�t���[�Y�e�N�X�`���B
					_Effect->SetTexture("g_diffuseTexture", material->GetTexture(Material::TextureHandleE::DiffuseMap));
					_Effect->SetVector("g_Textureblendcolor", (D3DXVECTOR4*)&material->GetBlendColor());
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

			_Effect->SetInt("g_isEnvironmentMap", g_EnvironmentRender ? 1 : 0);

			//�}�e���A���̐�
			DWORD MaterialNum = pMeshContainer->NumMaterials;
			//�}�e���A��
			D3DXMATERIAL *mtrl = (D3DXMATERIAL*)(pMeshContainer->pMaterials);

			//���f���`��
			for (DWORD i = 0; i < MaterialNum; i++)
			{
				_Effect->SetBool("SkyBox", _SkyBox);
				//�f�B�t���[�Y�J���[
				D3DXVECTOR4* Diffuse = (D3DXVECTOR4*)&mtrl[i].MatD3D.Diffuse;
				//�}�e���A��
				Material* material = pMeshContainer->material[i];
				
				//�e�N�X�`�����i�[����Ă���΃Z�b�g
				if (material != nullptr)
				{
					_Effect->SetTexture("g_Texture", material->GetTexture(Material::TextureHandleE::DiffuseMap));
					_Effect->SetVector("g_Textureblendcolor", (D3DXVECTOR4*)&material->GetBlendColor());
					_Effect->SetBool("Texflg", true);

					//�X�v���b�g�}�b�v
					IDirect3DBaseTexture9* splat = material->GetTexture(Material::TextureHandleE::SplatMap);
					if (splat)
					{
						_Effect->SetValue("g_terrainRect", &_ModelDate->GetTerrainSize(), sizeof(Vector4));

						_Effect->SetTexture("g_splatMap", splat);
						FOR(i, 4)
						{
							IDirect3DBaseTexture9* tex = material->GetTexture(Material::TextureHandleE::TerrainTex0 + i);
							if (tex)
							{
								char param[20] = "g_terrainTex";
								char idx[2] = { i + 48, 0 };
								strcat(param, idx);
								_Effect->SetTexture(param, tex);
							}
						}
					}

				}
				else
				{
					//�e�N�X�`�����Ȃ��Ȃ�J���[�Z�b�g
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
			(*graphicsDevice()).SetRenderState(D3DRS_ZENABLE, TRUE);
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
	_Effect->SetMatrix("g_viewMatrix", INSTANCE(SceneManager)->GetShadowMap()->GetLVMatrix());
	_Effect->SetMatrix("g_projectionMatrix", INSTANCE(SceneManager)->GetShadowMap()->GetLPMatrix());

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