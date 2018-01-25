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
	/** シャドウマップ描画フラグ. */
	bool g_ShadowRender = false;
	/** 環境マップ描画フラグ. */
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

//再帰関数
void SkinModel::DrawFrame(LPD3DXFRAME pFrame)
{
	//カリングするならスキップ。
	if (_Culling->IsCulling())
		return;

	//描画済みなのでスキップ。
	if ((_ModelDate->GetInstancing() == true) && (_ModelDate->GetAlreadyDrawn() == true))
		return;

	D3DXMESHCONTAINER_DERIVED* pMeshContainer;
	pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pFrame->pMeshContainer;
	//繋がっているメッシュコンテナがなくなるまで続ける
	while (pMeshContainer != NULL)
	{
		//メッシュ描画
		DrawMeshContainer(
			pMeshContainer,
			pFrame
			);
		//次のメッシュコンテナ
		//一つのフレームにコンテナは複数ある？
		pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainer->pNextMeshContainer;
	}

	//兄弟が居るならを兄弟を読み込む
	if (pFrame->pFrameSibling != NULL)
	{
		DrawFrame(pFrame->pFrameSibling);
	}

	//子が居るなら子を読み込む
	if (pFrame->pFrameFirstChild != NULL)
	{
		DrawFrame(pFrame->pFrameFirstChild);
	}
}

void SkinModel::Awake()
{
	//mainのものが設定されているならセットされる。
	_Camera = INSTANCE(GameObjectManager)->mainCamera;
	_Light = INSTANCE(GameObjectManager)->mainLight;
}

void SkinModel::Start()
{
	
}

//モデルデータの行列更新
void SkinModel::LateUpdate()
{
	//モデルデータがあるなら
	if (_ModelDate)
	{
		//掛けるワールド行列
		D3DXMATRIX wolrd;
		D3DXMatrixIdentity(&wolrd);
		wolrd = transform->GetWorldMatrix();
		_ModelDate->UpdateBoneMatrix(wolrd);	//行列を更新。

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
		//インスタンシングフラグをチェック。
		if (_ModelDate->GetInstancing())
		{
			//開始。
			_ModelDate->StartInstancing();
		}
	}
}

void SkinModel::Render()
{
	g_ShadowRender = false;
	g_EnvironmentRender = false;
	//モデルデータがあるなら
	if (_ModelDate)
	{
		if (_ModelEffect & ModelEffectE::FRUSTUM_CULLING)
		{
			if (strcmp("MaouSiro.X", gameObject->GetName()) == 0)
				int a = 0;

			_Camera = INSTANCE(GameObjectManager)->mainCamera;
			if (_Camera != nullptr)
				//カリングするかどうか判定。
				_Culling->Execute(_ModelDate->GetAABB(), transform->GetWorldMatrix(), _Camera->GetViewMat(), _Camera->GetProjectionMat());
		}
		//再帰関数呼び出し
		DrawFrame(_ModelDate->GetFrameRoot());
		//開始。
		_ModelDate->EndInstancing();
	}
}

/**
* シャドウマップに深度を書き込む.
* シャドウマップクラスから呼ばれている.
*/
void SkinModel::RenderToShadowMap()
{
	g_ShadowRender = true;
	g_EnvironmentRender = false;

	//モデルデータがあるなら
	if (_ModelDate)
	{
		if (_ModelEffect & ModelEffectE::FRUSTUM_CULLING)
		{
			//シャドウマップ・
			auto shadowmap = INSTANCE(SceneManager)->GetShadowMap();
			//カリングするかどうか判定。
			_Culling->Execute(_ModelDate->GetAABB(), transform->GetWorldMatrix(), *shadowmap->GetLVMatrix(), *shadowmap->GetLPMatrix());
		}
		//再帰関数呼び出し
		DrawFrame(_ModelDate->GetFrameRoot());
	}
}

/**
* 環境マップに描画する.
* 環境マップクラスから呼ばれている.
*/
void SkinModel::RenderToEnvironmentMap()
{
	g_ShadowRender = false;
	g_EnvironmentRender = true;
	//モデルデータがあるなら
	if (_ModelDate)
	{
		//再帰関数呼び出し
		DrawFrame(_ModelDate->GetFrameRoot());
	}
}

void SkinModel::DrawMeshContainer(
	D3DXMESHCONTAINER_DERIVED* pMeshContainer,
	LPD3DXFRAME pFrameBase
)
{
	D3DXFRAME_DERIVED* pFrame = (D3DXFRAME_DERIVED*)pFrameBase;
	
	//影描画
	if (g_ShadowRender)
	{
		if (_ModelEffect & ModelEffectE::CAST_SHADOW)
		{
			CreateShadow(pMeshContainer, pFrame);
		}
	}
	//モデル描画
	else
	{
		//エフェクト読み込み
		if (pMeshContainer->pSkinInfo != NULL)
		{
			_Effect = EffectManager::LoadEffect("AnimationModel.fx");
		}
		else
		{
			_Effect = EffectManager::LoadEffect("3Dmodel.fx");
		}

		//テクニックをセット
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

		//開始（必ず終了すること）
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
		
		//ライトの向きを転送。
		_Effect->SetValue("g_diffuseLightDirection", &dir, sizeof(Vector4)*System::MAX_LIGHTNUM);
		//ライトのカラーを転送。
		_Effect->SetValue("g_diffuseLightColor", &color, sizeof(Color)*System::MAX_LIGHTNUM);
		//ライト数セット
		_Effect->SetInt("g_LightNum", num);
		//環境光
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

		//カメラのポジションセット(スペキュラライト用)
		Vector3 campos = INSTANCE(GameObjectManager)->mainCamera->transform->GetPosition();
		_Effect->SetValue("g_cameraPos", &D3DXVECTOR4(campos.x, campos.y, campos.z, 1.0f), sizeof(D3DXVECTOR4));
		Vector3 camTar = INSTANCE(GameObjectManager)->mainCamera->GetTarget();
		Vector3 camDir = camTar - campos;
		camDir.Normalize();
		_Effect->SetVector("g_cameraDir", &D3DXVECTOR4(camDir.x, camDir.y, camDir.z, 1.0f));

		//各行列を送信
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
			//レシーバー用パラメータを設定.
			ShadowMap::ShadowReceiverParam shadowParam = INSTANCE(SceneManager)->GetShadowMap()->GetShadowReceiverParam();
			if (terain)
			{
				shadowParam._IsVSM = false;
			}
			_Effect->SetValue("g_ShadowReceiverParam", &shadowParam, sizeof(ShadowMap::ShadowReceiverParam));
			//深度テクスチャ
			_Effect->SetTexture("g_ShadowMap_0", INSTANCE(SceneManager)->GetShadowMap()->GetShadowMapTexture(0, shadowParam._IsVSM));
			_Effect->SetTexture("g_ShadowMap_1", INSTANCE(SceneManager)->GetShadowMap()->GetShadowMapTexture(1, shadowParam._IsVSM));
			_Effect->SetTexture("g_ShadowMap_2", INSTANCE(SceneManager)->GetShadowMap()->GetShadowMapTexture(2, shadowParam._IsVSM));
			//影を映すかどうかのフラグセット
			_Effect->SetInt("ReceiveShadow", (_ModelEffect & ModelEffectE::RECEIVE_SHADOW) > 0);
		}

		Vector2 size = 20;
		Vector2 texel = Vector2(1.0f / size.x, 1.0f / size.y);
		_Effect->SetValue("g_TexelSize", &texel, sizeof(Vector2));

		//フラグ設定
		Vector4 flg;
		flg.x = (_ModelEffect & ModelEffectE::RECEIVE_SHADOW) > 0;
		flg.y = (_ModelEffect & ModelEffectE::RECEIVE_POINTLIGHT) > 0;
		flg.z = _IsFresnel;

		_Effect->SetValue("g_EffectFlg", &flg, sizeof(Vector4));

		//フレネル反射のパラメータを設定.
		_Effect->SetVector("g_FresnelParam", (D3DXVECTOR4*)&_FresnelParam);
		
		//スペキュラフラグセット
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
		
		//アルファブレンド.
		(*graphicsDevice()).SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

		(*graphicsDevice()).SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		(*graphicsDevice()).SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		//if ((_ModelEffect & ModelEffectE::ALPHA) > 0)
		//{
		//	//アルファテストを行う.
		//	(*graphicsDevice()).SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		//	(*graphicsDevice()).SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
		//	// 不透明にする値の設定
		//	(*graphicsDevice()).SetRenderState(D3DRS_ALPHAREF, 0x66);
		//}

		_Effect->SetFloat("g_Alpha", _Alpha);

		D3DXVECTOR4 ditherParam = D3DXVECTOR4(0, 0, 0, 0);
		if ((_ModelEffect & ModelEffectE::DITHERING) > 0)
		{
			//掛かりきる最低値.
			const float MinLen = 1.5f;
			//掛かり始める最高値.
			const float MaxLen = 4.0f;

			//カメラから座標へのベクトル.
			Vector3 CameraToPos = gameObject->transform->GetPosition() - campos;
			float CameraToPosLen = CameraToPos.Length();

			CameraToPosLen -= MinLen;
			//最低値~最高値にクランプ.
			CameraToPosLen = min(MaxLen - MinLen, CameraToPosLen);
			CameraToPosLen - max(0.0f, CameraToPosLen);
			//正規化.
			CameraToPosLen /= (MaxLen - MinLen);

			// ディザ係数.
			// 0 ~ 65.
			// ディザ係数よりも大きい値のところが残る.
			ditherParam.y = (1.0f - CameraToPosLen) * 65.0f;

			if (ditherParam.y < _DitherCoefficient)
			{
				ditherParam.y = _DitherCoefficient;
			}

			if (_IsTree)
			{
				//フラグを設定.
				ditherParam.x = 2.0f;
			}
			else
			{
				//フラグを設定.
				ditherParam.x = 1.0f;
			}
		}
		ditherParam.z = g_WindowSize.x;
		ditherParam.w = g_WindowSize.y;
		_Effect->SetVector("g_DitherParam", &ditherParam);

		Vector4 fogParam = Vector4(1, 1, 1, 1);
		if (_FogFunc == FogFunc::FogFuncDist)
		{
			//距離フォグ
			fogParam.x = _FogParam[0];
			fogParam.y = _FogParam[1];
			fogParam.z = 1.0f;
		}
		else if (_FogFunc == FogFunc::FogFuncHeight)
		{
			//高さフォグ
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

		//アニメーションの有無で分岐
		if (pMeshContainer->pSkinInfo != NULL)
		{
			UINT iAttrib;
			//バッファー
			LPD3DXBONECOMBINATION pBoneComb = LPD3DXBONECOMBINATION(pMeshContainer->pBoneCombinationBuf->GetBufferPointer());
			//各マテリアル
			for (iAttrib = 0; iAttrib < pMeshContainer->NumAttributeGroups; iAttrib++)
			{
				//ボーン
				for (DWORD iPaletteEntry = 0; iPaletteEntry < pMeshContainer->NumPaletteEntries; ++iPaletteEntry)
				{
					DWORD iMatrixIndex = pBoneComb[iAttrib].BoneId[iPaletteEntry];
					if (iMatrixIndex != UINT_MAX)
					{
						//骨の最終的な行列計算
						D3DXMatrixMultiply(
							&_BoneMatrixPallets[iPaletteEntry],
							//骨のオフセット(移動)行列
							&pMeshContainer->pBoneOffsetMatrices[iMatrixIndex],
							//フレームのワールド行列
							pMeshContainer->ppBoneMatrixPtrs[iMatrixIndex]
						);
					}
				}

				//骨のワールド行列配列
				_Effect->SetMatrixArray("g_mWorldMatrixArray", _BoneMatrixPallets, pMeshContainer->NumPaletteEntries);
				//骨の数
				_Effect->SetFloat("g_numBone", (float)pMeshContainer->NumInfl);

				//ディフューズカラー取得
				D3DXVECTOR4* Diffuse = (D3DXVECTOR4*)&pMeshContainer->pMaterials[pBoneComb[iAttrib].AttribId].MatD3D.Diffuse;
				//マテリアル
				Material* material = pMeshContainer->material[pBoneComb[iAttrib].AttribId];
				
				//テクスチャが格納されていればセット
				if (material != NULL)
				{
					// ディフューズテクスチャ。
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
				//テクスチャがないならカラーセット
				else if (Diffuse != NULL)
				{
					_Effect->SetVector("g_diffuseMaterial", Diffuse);
					_Effect->SetInt("Texflg", false);
				}

				//この関数を呼び出すことで、データの転送が確定する。
				_Effect->CommitChanges();
				//メッシュ描画
				pMeshContainer->MeshData.pMesh->DrawSubset(iAttrib);
			}
		}
		//アニメーションしない方
		else
		{
			_Effect->SetMatrix("g_worldMatrix", &pFrame->CombinedTransformationMatrix);

			_Effect->SetInt("g_isEnvironmentMap", g_EnvironmentRender ? 1 : 0);

			//マテリアルの数
			DWORD MaterialNum = pMeshContainer->NumMaterials;
			//マテリアル
			D3DXMATERIAL *mtrl = (D3DXMATERIAL*)(pMeshContainer->pMaterials);

			if (_ModelDate->GetInstancing())
			{
				//いろいろ設定。

				//マテリアルの数
				auto MaterialNum = pMeshContainer->NumMaterials;
				//マテリアル
				D3DXMATERIAL *mtrl = (D3DXMATERIAL*)(pMeshContainer->pMaterials);
				//メッシュ。
				auto Mesh = pMeshContainer->MeshData.pMesh;
				//ワールド行列のスタック。
				auto Stack = pFrame->Original->WorldMatrixStack;

				for (auto i = 0; i < MaterialNum; i++)
				{
					//ディフューズカラー
					D3DXVECTOR4* Diffuse = (D3DXVECTOR4*)&mtrl[i].MatD3D.Diffuse;
					//マテリアル
					Material* material = pMeshContainer->material[i];

					//テクスチャが格納されていればセット
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
						//テクスチャがないならカラーセット
						_Effect->SetVector("g_diffuseMaterial", Diffuse);
						_Effect->SetBool("Texflg", false);
					}

					

					//DrawSubsetを使用するとインスタンシング描画が行えないので
					//g_pMeshから頂点バッファ、インデックスバッファを引っ張ってきて、直接描画する。
					LPDIRECT3DVERTEXBUFFER9 vb;
					LPDIRECT3DINDEXBUFFER9 ib;
					Mesh->GetVertexBuffer(&vb);
					Mesh->GetIndexBuffer(&ib);

					DWORD fvf = Mesh->GetFVF();
					DWORD stride = D3DXGetFVFVertexSize(fvf);
					//周波数パラメータなるものを設定。
					(*graphicsDevice()).SetStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA | Stack.size());
					(*graphicsDevice()).SetStreamSourceFreq(1, D3DSTREAMSOURCE_INSTANCEDATA | 1);
					//デコレーション設定。
					(*graphicsDevice()).SetVertexDeclaration(pMeshContainer->vertexDecl);

					auto matrixBuffer = pMeshContainer->worldMatrixBuffer;
					(*graphicsDevice()).SetStreamSource(0, vb, 0,48);							//頂点バッファをストリーム0番目に設定
					(*graphicsDevice()).SetStreamSource(1, matrixBuffer, 0, sizeof(D3DXMATRIX));	//ワールド行列用のバッファをストリーム1番目に設定。
																									//ワールド行列を頂点バッファにコピー。
					CopyWorldMatrixToVertexBuffer(matrixBuffer, Stack);

					//インデックスバッファ設定。
					(*graphicsDevice()).SetIndices(ib);
					//この関数を呼び出すことで、データの転送が確定する。描画を行う前に一回だけ呼び出す。
					_Effect->CommitChanges();
					//描画
					(*graphicsDevice()).DrawIndexedPrimitive(
						D3DPT_TRIANGLELIST,
						0,
						0,
						Mesh->GetNumVertices(),
						0,
						Mesh->GetNumFaces());	

					// 後始末
					(*graphicsDevice()).SetStreamSourceFreq(0, 1);
					(*graphicsDevice()).SetStreamSourceFreq(1, 1);

					vb->Release();
					ib->Release();
				}
			}
			else
			{

				//モデル描画
				for (DWORD i = 0; i < MaterialNum; i++)
				{
					_Effect->SetInt("SkyBox", _IsSky);

					//ディフューズカラー
					D3DXVECTOR4* Diffuse = (D3DXVECTOR4*)&mtrl[i].MatD3D.Diffuse;
					//マテリアル
					Material* material = pMeshContainer->material[i];

					//テクスチャが格納されていればセット
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

						//スプラットマップ
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
						//テクスチャがないならカラーセット
						_Effect->SetVector("g_diffuseMaterial", Diffuse);
						_Effect->SetInt("Texflg", false);
					}

					//この関数を呼び出すことで、データの転送が確定する。
					_Effect->CommitChanges();
					//メッシュ描画
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
	//エフェクト読み込み
	if (pMeshContainer->pSkinInfo != NULL)
	{
		_Effect = EffectManager::LoadEffect("AnimationModel.fx");
	}
	else
	{
		_Effect = EffectManager::LoadEffect("3Dmodel.fx");
	}

	//テクニック設定
	if (!_IsTree)
	{
		_Effect->SetTechnique("Shadow");
	}
	else
	{
		_Effect->SetTechnique("ShadowTree");
	}

	//開始
	_Effect->Begin(0, D3DXFX_DONOTSAVESTATE);
	_Effect->BeginPass(0);

	//影カメラのビュープロジェクション行列を送る
	_Effect->SetMatrix("g_viewMatrix", INSTANCE(SceneManager)->GetShadowMap()->GetLVMatrix());
	_Effect->SetMatrix("g_projectionMatrix", INSTANCE(SceneManager)->GetShadowMap()->GetLPMatrix());

	(*graphicsDevice()).SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	//if ((_ModelEffect & ModelEffectE::ALPHA) > 0)
	//{
	//	//アルファテストを行う.
	//	(*graphicsDevice()).SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//	(*graphicsDevice()).SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	//	// 不透明にする値の設定
	//	(*graphicsDevice()).SetRenderState(D3DRS_ALPHAREF, 0x66);
	//}

	_Effect->SetFloat("g_Alpha", _Alpha);

	//アニメーションの有無で分岐
	if (pMeshContainer->pSkinInfo != NULL)
	{
		UINT iAttrib;
		//バッファー
		LPD3DXBONECOMBINATION pBoneComb = LPD3DXBONECOMBINATION(pMeshContainer->pBoneCombinationBuf->GetBufferPointer());
		//各マテリアル
		for (iAttrib = 0; iAttrib < pMeshContainer->NumMaterials; iAttrib++)
		{
			//ボーン
			for (DWORD iPaletteEntry = 0; iPaletteEntry < pMeshContainer->NumPaletteEntries; ++iPaletteEntry)
			{
				DWORD iMatrixIndex = pBoneComb[iAttrib].BoneId[iPaletteEntry];
				if (iMatrixIndex != UINT_MAX)
				{
					//骨の最終的な行列計算
					D3DXMatrixMultiply(
						&_BoneMatrixPallets[iPaletteEntry],
						//骨のオフセット(移動)行列
						&pMeshContainer->pBoneOffsetMatrices[iMatrixIndex],
						//フレームのワールド行列
						pMeshContainer->ppBoneMatrixPtrs[iMatrixIndex]
					);
				}
			}

			//骨のワールド行列配列
			_Effect->SetMatrixArray("g_mWorldMatrixArray", _BoneMatrixPallets, pMeshContainer->NumPaletteEntries);
			//骨の数
			_Effect->SetFloat("g_numBone", (float)pMeshContainer->NumInfl);
	

			//この関数を呼び出すことで、データの転送が確定する。
			_Effect->CommitChanges();
			//メッシュ描画
			pMeshContainer->MeshData.pMesh->DrawSubset(iAttrib);
		}
	}
	else
	{
		_Effect->SetMatrix("g_worldMatrix", &pFrame->CombinedTransformationMatrix);

		//モデル描画
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

			//この関数を呼び出すことで、データの転送が確定する。
			_Effect->CommitChanges();
			//メッシュ描画
			pMeshContainer->MeshData.pMesh->DrawSubset(i);
		}
	}

	//終了
	_Effect->EndPass();
	_Effect->End();

	(*graphicsDevice()).SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}

void CopyWorldMatrixToVertexBuffer(IDirect3DVertexBuffer9* buffer, vector<D3DXMATRIX*> stack)
//ワールド行列を頂点バッファにコピー。
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