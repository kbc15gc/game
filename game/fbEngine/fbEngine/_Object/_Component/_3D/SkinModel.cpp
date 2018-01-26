#include"fbstdafx.h"
#include "SkinModel.h"

#include "_Effect\EffectManager.h"
#include "_Object\_Component\_3D\Light.h"
#include "_Object\_Component\_3D\Camera.h"
#include "_Object\_Component\_3D\ShadowCamera.h"

#include "_Culling\ObjectFrustumCulling.h"

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

void CopyWorldMatrixToVertexBuffer(IDirect3DVertexBuffer9* buffer, vector<D3DXMATRIX*> stack);

SkinModel::SkinModel(GameObject * g, Transform * t) :
	Component(g, t, typeid(this).name(),100),
	_Effect(nullptr),
	_ModelDate(nullptr),
	_Camera(nullptr),
	_Light(nullptr),
	//_TextureBlend(Color::white),
	_AllBlend(Color::white),
	_ModelEffect(ModelEffectE(ModelEffectE::CAST_SHADOW | ModelEffectE::RECEIVE_SHADOW | ModelEffectE::FRUSTUM_CULLING)),
	_CullMode(D3DCULL_CCW),
	_Culling(new CObjectFrustumCulling)
{
#ifdef _DEBUG
	mbstowcs_s(nullptr, name, typeid(*this).name(), strlen(typeid(*this).name()));
#endif
}

SkinModel::~SkinModel()
{
	SAFE_DELETE(_ModelDate);
}

//�ċA�֐�
void SkinModel::DrawFrame(LPD3DXFRAME pFrame)
{
	//�J�����O����Ȃ�X�L�b�v�B
	if (_Culling->IsCulling())
		return;

	//�`��ς݂Ȃ̂ŃX�L�b�v�B
	if ((_ModelDate->GetInstancing() == true) && (_ModelDate->GetAlreadyDrawn() == true))
		return;

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
	_Camera = INSTANCE(GameObjectManager)->mainCamera;
	_Light = INSTANCE(GameObjectManager)->mainLight;
}

void SkinModel::Start()
{
	
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

		_ModelDate->UpdateAABB();
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

	if (_ModelDate) {
		//�C���X�^���V���O�t���O���`�F�b�N�B
		if (_ModelDate->GetInstancing())
		{
			//�J�n�B
			_ModelDate->StartInstancing();
		}
	}
}

void SkinModel::Render()
{
	g_ShadowRender = false;
	g_EnvironmentRender = false;
	//���f���f�[�^������Ȃ�
	if (_ModelDate)
	{
		if (_ModelEffect & ModelEffectE::FRUSTUM_CULLING)
		{
			if (strcmp("MaouSiro.X", gameObject->GetName()) == 0)
				int a = 0;

			_Camera = INSTANCE(GameObjectManager)->mainCamera;
			if (_Camera != nullptr)
				//�J�����O���邩�ǂ�������B
				_Culling->Execute(_ModelDate->GetAABB(), transform->GetWorldMatrix(), _Camera->GetViewMat(), _Camera->GetProjectionMat());
		}
		//�ċA�֐��Ăяo��
		DrawFrame(_ModelDate->GetFrameRoot());
		//�J�n�B
		_ModelDate->EndInstancing();
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
		if (_ModelEffect & ModelEffectE::FRUSTUM_CULLING)
		{
			//�V���h�E�}�b�v�E
			auto shadowmap = INSTANCE(SceneManager)->GetShadowMap();
			//�J�����O���邩�ǂ�������B
			_Culling->Execute(_ModelDate->GetAABB(), transform->GetWorldMatrix(), *shadowmap->GetLVMatrix(), *shadowmap->GetLPMatrix());
		}
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
		{
			_Effect = EffectManager::LoadEffect("AnimationModel.fx");
		}
		else
		{
			_Effect = EffectManager::LoadEffect("3Dmodel.fx");
		}

		//�e�N�j�b�N���Z�b�g
		if(terain)
		{
			_Effect->SetTechnique("TerrainRender");
		}
		else
		{
			if (_ModelDate->GetInstancing())
			{
				_Effect->SetTechnique("InstancingRender");
			}
			else if (_IsSky)
			{
				_Effect->SetTechnique("SkySphereRender");
			}
			else
			{
				_Effect->SetTechnique("NormalRender");
			}
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

		Vector4 PLparam = INSTANCE(GameObjectManager)->mainLight->GetPointLightParam();
		Vector3 PLpos = INSTANCE(GameObjectManager)->mainLight->GetPointLightPosition();
		_Effect->SetVector("g_PointLightParam", (D3DXVECTOR4*)&PLparam);
		_Effect->SetVector("g_PointLightPos", &D3DXVECTOR4(PLpos.x, PLpos.y, PLpos.z, 1.0f));

		CharacterLight cl;
		if (_CharaLight != nullptr)
		{
			cl = *_CharaLight;
			for (int i = 0; i < cl.LIGHT_NUM; i++)
			{
				Vector4 dir4 = cl.GetDiffuseLightDirection(i);
				Vector3 dir = Vector3(dir4.x, dir4.y, dir4.z);
				dir.Transform(transform->GetRotateMatrix());
				cl.SetDiffuseLightDirection(i, dir);
			}
		}
		_Effect->SetValue("g_CharaLight", &cl, sizeof(CharacterLight));

		//�J�����̃|�W�V�����Z�b�g(�X�y�L�������C�g�p)
		Vector3 campos = INSTANCE(GameObjectManager)->mainCamera->transform->GetPosition();
		_Effect->SetValue("g_cameraPos", &D3DXVECTOR4(campos.x, campos.y, campos.z, 1.0f), sizeof(D3DXVECTOR4));
		Vector3 camTar = INSTANCE(GameObjectManager)->mainCamera->GetTarget();
		Vector3 camDir = camTar - campos;
		camDir.Normalize();
		_Effect->SetVector("g_cameraDir", &D3DXVECTOR4(camDir.x, camDir.y, camDir.z, 1.0f));

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
			ShadowMap::ShadowReceiverParam shadowParam = INSTANCE(SceneManager)->GetShadowMap()->GetShadowReceiverParam();
			if (terain)
			{
				shadowParam._IsVSM = false;
			}
			_Effect->SetValue("g_ShadowReceiverParam", &shadowParam, sizeof(ShadowMap::ShadowReceiverParam));
			//�[�x�e�N�X�`��
			_Effect->SetTexture("g_ShadowMap_0", INSTANCE(SceneManager)->GetShadowMap()->GetShadowMapTexture(0, shadowParam._IsVSM));
			_Effect->SetTexture("g_ShadowMap_1", INSTANCE(SceneManager)->GetShadowMap()->GetShadowMapTexture(1, shadowParam._IsVSM));
			_Effect->SetTexture("g_ShadowMap_2", INSTANCE(SceneManager)->GetShadowMap()->GetShadowMapTexture(2, shadowParam._IsVSM));
			//�e���f�����ǂ����̃t���O�Z�b�g
			_Effect->SetInt("ReceiveShadow", (_ModelEffect & ModelEffectE::RECEIVE_SHADOW) > 0);
		}

		Vector2 size = 20;
		Vector2 texel = Vector2(1.0f / size.x, 1.0f / size.y);
		_Effect->SetValue("g_TexelSize", &texel, sizeof(Vector2));

		//�t���O�ݒ�
		Vector4 flg;
		flg.x = (_ModelEffect & ModelEffectE::RECEIVE_SHADOW) > 0;
		flg.y = (_ModelEffect & ModelEffectE::RECEIVE_POINTLIGHT) > 0;
		flg.z = _IsFresnel;

		_Effect->SetValue("g_EffectFlg", &flg, sizeof(Vector4));

		//�t���l�����˂̃p�����[�^��ݒ�.
		_Effect->SetVector("g_FresnelParam", (D3DXVECTOR4*)&_FresnelParam);
		
		//�X�y�L�����t���O�Z�b�g
		//_Effect->SetInt("Spec", (_ModelEffect & ModelEffectE::SPECULAR) > 0);

		_Effect->SetVector("g_blendcolor", (D3DXVECTOR4*)&_AllBlend);

		AtmosphericScatteringParamS atmos = INSTANCE(SceneManager)->GetSky()->GetAtmosphericScatteringParam();
		_Effect->SetValue("g_atmosParam", &atmos, sizeof(AtmosphericScatteringParamS));
		_Effect->SetInt("g_atmosFlag", _AtomosphereFunc);

		if (_IsSky)
		{
			_Effect->SetTexture("g_NightTexture", INSTANCE(SceneManager)->GetSky()->GetNightTexture());
			(*graphicsDevice()).SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
			(*graphicsDevice()).SetRenderState(D3DRS_ZENABLE, FALSE);
		}
		else
		{
			(*graphicsDevice()).SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
			(*graphicsDevice()).SetRenderState(D3DRS_ZENABLE, TRUE);
		}


		(*graphicsDevice()).SetRenderState(D3DRS_CULLMODE, _CullMode);
		
		//�A���t�@�u�����h.
		(*graphicsDevice()).SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

		(*graphicsDevice()).SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		(*graphicsDevice()).SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		//if ((_ModelEffect & ModelEffectE::ALPHA) > 0)
		//{
		//	//�A���t�@�e�X�g���s��.
		//	(*graphicsDevice()).SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		//	(*graphicsDevice()).SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
		//	// �s�����ɂ���l�̐ݒ�
		//	(*graphicsDevice()).SetRenderState(D3DRS_ALPHAREF, 0x66);
		//}

		_Effect->SetFloat("g_Alpha", _Alpha);

		D3DXVECTOR4 ditherParam = D3DXVECTOR4(0, 0, 0, 0);
		if ((_ModelEffect & ModelEffectE::DITHERING) > 0)
		{
			//�|���肫��Œ�l.
			const float MinLen = 1.5f;
			//�|����n�߂�ō��l.
			const float MaxLen = 4.0f;

			//�J����������W�ւ̃x�N�g��.
			Vector3 CameraToPos = gameObject->transform->GetPosition() - campos;
			float CameraToPosLen = CameraToPos.Length();

			CameraToPosLen -= MinLen;
			//�Œ�l~�ō��l�ɃN�����v.
			CameraToPosLen = min(MaxLen - MinLen, CameraToPosLen);
			CameraToPosLen - max(0.0f, CameraToPosLen);
			//���K��.
			CameraToPosLen /= (MaxLen - MinLen);

			// �f�B�U�W��.
			// 0 ~ 65.
			// �f�B�U�W�������傫���l�̂Ƃ��낪�c��.
			ditherParam.y = (1.0f - CameraToPosLen) * 65.0f;

			if (ditherParam.y < _DitherCoefficient)
			{
				ditherParam.y = _DitherCoefficient;
			}

			if (_IsTree)
			{
				//�t���O��ݒ�.
				ditherParam.x = 2.0f;
			}
			else
			{
				//�t���O��ݒ�.
				ditherParam.x = 1.0f;
			}
		}
		ditherParam.z = g_WindowSize.x;
		ditherParam.w = g_WindowSize.y;
		_Effect->SetVector("g_DitherParam", &ditherParam);

		Vector4 fogParam = Vector4(1, 1, 1, 1);
		if (_FogFunc == FogFunc::FogFuncDist)
		{
			//�����t�H�O
			fogParam.x = _FogParam[0];
			fogParam.y = _FogParam[1];
			fogParam.z = 1.0f;
		}
		else if (_FogFunc == FogFunc::FogFuncHeight)
		{
			//�����t�H�O
			fogParam.x = _FogParam[0];
			fogParam.y = _FogParam[1];
			fogParam.z = 2.0f;
		}
		else
		{
			fogParam.z = 0.0f;
		}
		_Effect->SetVector("g_fogParam", (D3DXVECTOR4*)&fogParam);
		_Effect->SetVector("g_fogColor", (D3DXVECTOR4*)&_FogColor);

		_Effect->SetVector("g_LuminanceColor", (D3DXVECTOR4*)&_LuminanceColor);
		_Effect->SetInt("g_IsLuminance", _IsLuminance);

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
					_Effect->SetInt("Texflg", true);


					D3DXVECTOR4 flag = D3DXVECTOR4(0, 0, 0, 0);
					if (material->GetTexture(Material::TextureHandleE::NormalMap))
					{
						_Effect->SetTexture("g_NormalMap", material->GetTexture(Material::TextureHandleE::NormalMap));
						flag.x = 1;
					}
					if (material->GetTexture(Material::TextureHandleE::SpecularMap))
					{
						_Effect->SetTexture("g_speculerMap", material->GetTexture(Material::TextureHandleE::SpecularMap));
						flag.y = 1;
					}
					_Effect->SetVector("g_MapFlg", &flag);

				}
				//�e�N�X�`�����Ȃ��Ȃ�J���[�Z�b�g
				else if (Diffuse != NULL)
				{
					_Effect->SetVector("g_diffuseMaterial", Diffuse);
					_Effect->SetInt("Texflg", false);
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

			if (_ModelDate->GetInstancing())
			{
				//���낢��ݒ�B

				//�}�e���A���̐�
				auto MaterialNum = pMeshContainer->NumMaterials;
				//�}�e���A��
				D3DXMATERIAL *mtrl = (D3DXMATERIAL*)(pMeshContainer->pMaterials);
				//���b�V���B
				auto Mesh = pMeshContainer->MeshData.pMesh;
				//���[���h�s��̃X�^�b�N�B
				auto Stack = pFrame->Original->WorldMatrixStack;

				for (auto i = 0; i < MaterialNum; i++)
				{
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

						D3DXVECTOR4 flag = D3DXVECTOR4(0, 0, 0, 0);
						if (material->GetTexture(Material::TextureHandleE::NormalMap))
						{
							_Effect->SetTexture("g_NormalMap", material->GetTexture(Material::TextureHandleE::NormalMap));
							flag.x = 1;
						}
						if (material->GetTexture(Material::TextureHandleE::SpecularMap))
						{
							_Effect->SetTexture("g_speculerMap", material->GetTexture(Material::TextureHandleE::SpecularMap));
							flag.y = 1;
						}
						_Effect->SetVector("g_MapFlg", &flag);
					}
					else
					{
						//�e�N�X�`�����Ȃ��Ȃ�J���[�Z�b�g
						_Effect->SetVector("g_diffuseMaterial", Diffuse);
						_Effect->SetBool("Texflg", false);
					}

					

					//DrawSubset���g�p����ƃC���X�^���V���O�`�悪�s���Ȃ��̂�
					//g_pMesh���璸�_�o�b�t�@�A�C���f�b�N�X�o�b�t�@�����������Ă��āA���ڕ`�悷��B
					LPDIRECT3DVERTEXBUFFER9 vb;
					LPDIRECT3DINDEXBUFFER9 ib;
					Mesh->GetVertexBuffer(&vb);
					Mesh->GetIndexBuffer(&ib);

					DWORD fvf = Mesh->GetFVF();
					DWORD stride = D3DXGetFVFVertexSize(fvf);
					//���g���p�����[�^�Ȃ���̂�ݒ�B
					(*graphicsDevice()).SetStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA | Stack.size());
					(*graphicsDevice()).SetStreamSourceFreq(1, D3DSTREAMSOURCE_INSTANCEDATA | 1);
					//�f�R���[�V�����ݒ�B
					(*graphicsDevice()).SetVertexDeclaration(pMeshContainer->vertexDecl);

					auto matrixBuffer = pMeshContainer->worldMatrixBuffer;
					(*graphicsDevice()).SetStreamSource(0, vb, 0,48);							//���_�o�b�t�@���X�g���[��0�Ԗڂɐݒ�
					(*graphicsDevice()).SetStreamSource(1, matrixBuffer, 0, sizeof(D3DXMATRIX));	//���[���h�s��p�̃o�b�t�@���X�g���[��1�Ԗڂɐݒ�B
																									//���[���h�s��𒸓_�o�b�t�@�ɃR�s�[�B
					CopyWorldMatrixToVertexBuffer(matrixBuffer, Stack);

					//�C���f�b�N�X�o�b�t�@�ݒ�B
					(*graphicsDevice()).SetIndices(ib);
					//���̊֐����Ăяo�����ƂŁA�f�[�^�̓]�����m�肷��B�`����s���O�Ɉ�񂾂��Ăяo���B
					_Effect->CommitChanges();
					//�`��
					(*graphicsDevice()).DrawIndexedPrimitive(
						D3DPT_TRIANGLELIST,
						0,
						0,
						Mesh->GetNumVertices(),
						0,
						Mesh->GetNumFaces());	

					// ��n��
					(*graphicsDevice()).SetStreamSourceFreq(0, 1);
					(*graphicsDevice()).SetStreamSourceFreq(1, 1);

					vb->Release();
					ib->Release();
				}
			}
			else
			{

				//���f���`��
				for (DWORD i = 0; i < MaterialNum; i++)
				{
					_Effect->SetInt("SkyBox", _IsSky);

					//�f�B�t���[�Y�J���[
					D3DXVECTOR4* Diffuse = (D3DXVECTOR4*)&mtrl[i].MatD3D.Diffuse;
					//�}�e���A��
					Material* material = pMeshContainer->material[i];

					//�e�N�X�`�����i�[����Ă���΃Z�b�g
					if (material != nullptr)
					{
						
						_Effect->SetTexture("g_Texture", material->GetTexture(Material::TextureHandleE::DiffuseMap));
						_Effect->SetVector("g_Textureblendcolor", (D3DXVECTOR4*)&material->GetBlendColor());
						_Effect->SetInt("Texflg", true);

						D3DXVECTOR4 flag = D3DXVECTOR4(0, 0, 0, 0);
						if (material->GetTexture(Material::TextureHandleE::NormalMap))
						{
							_Effect->SetTexture("g_NormalMap", material->GetTexture(Material::TextureHandleE::NormalMap));
							flag.x = 1;
						}
						if (material->GetTexture(Material::TextureHandleE::SpecularMap))
						{
							_Effect->SetTexture("g_speculerMap", material->GetTexture(Material::TextureHandleE::SpecularMap));
							flag.y = 1;
						}
						_Effect->SetVector("g_MapFlg", &flag);

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
						_Effect->SetInt("Texflg", false);
					}

					//���̊֐����Ăяo�����ƂŁA�f�[�^�̓]�����m�肷��B
					_Effect->CommitChanges();
					//���b�V���`��
					pMeshContainer->MeshData.pMesh->DrawSubset(i);
				}
			}
		}

		_Effect->EndPass();
		_Effect->End();

		(*graphicsDevice()).SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		(*graphicsDevice()).SetRenderState(D3DRS_ZENABLE, TRUE);
		(*graphicsDevice()).SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		(*graphicsDevice()).SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		(*graphicsDevice()).SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
		(*graphicsDevice()).SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
	}
}

void SkinModel::CreateShadow(D3DXMESHCONTAINER_DERIVED * pMeshContainer, D3DXFRAME_DERIVED * pFrame)
{
	//�G�t�F�N�g�ǂݍ���
	if (pMeshContainer->pSkinInfo != NULL)
	{
		_Effect = EffectManager::LoadEffect("AnimationModel.fx");
	}
	else
	{
		_Effect = EffectManager::LoadEffect("3Dmodel.fx");
	}

	//�e�N�j�b�N�ݒ�
	if (!_IsTree)
	{
		_Effect->SetTechnique("Shadow");
	}
	else
	{
		_Effect->SetTechnique("ShadowTree");
	}

	//�J�n
	_Effect->Begin(0, D3DXFX_DONOTSAVESTATE);
	_Effect->BeginPass(0);

	//�e�J�����̃r���[�v���W�F�N�V�����s��𑗂�
	_Effect->SetMatrix("g_viewMatrix", INSTANCE(SceneManager)->GetShadowMap()->GetLVMatrix());
	_Effect->SetMatrix("g_projectionMatrix", INSTANCE(SceneManager)->GetShadowMap()->GetLPMatrix());

	(*graphicsDevice()).SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	//if ((_ModelEffect & ModelEffectE::ALPHA) > 0)
	//{
	//	//�A���t�@�e�X�g���s��.
	//	(*graphicsDevice()).SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//	(*graphicsDevice()).SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	//	// �s�����ɂ���l�̐ݒ�
	//	(*graphicsDevice()).SetRenderState(D3DRS_ALPHAREF, 0x66);
	//}

	_Effect->SetFloat("g_Alpha", _Alpha);

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
			
			Material* material = pMeshContainer->material[i];
			if (material)
			{
				_Effect->SetTexture("g_Texture", material->GetTexture(Material::TextureHandleE::DiffuseMap));
			}
			else
			{
				_Effect->SetTexture("g_Texture", nullptr);
			}

			//���̊֐����Ăяo�����ƂŁA�f�[�^�̓]�����m�肷��B
			_Effect->CommitChanges();
			//���b�V���`��
			pMeshContainer->MeshData.pMesh->DrawSubset(i);
		}
	}

	//�I��
	_Effect->EndPass();
	_Effect->End();

	(*graphicsDevice()).SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}

void CopyWorldMatrixToVertexBuffer(IDirect3DVertexBuffer9* buffer, vector<D3DXMATRIX*> stack)
//���[���h�s��𒸓_�o�b�t�@�ɃR�s�[�B
{
	D3DVERTEXBUFFER_DESC desc;
	buffer->GetDesc(&desc);
	D3DXMATRIX* pData;
	buffer->Lock(0, desc.Size, (void**)&pData, D3DLOCK_DISCARD);

	for (int i = 0; i < stack.size(); i++) {

		*pData = *stack[i];
		pData++;
	}
	buffer->Unlock();
}