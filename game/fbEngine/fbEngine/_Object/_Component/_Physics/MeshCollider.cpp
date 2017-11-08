#include"fbstdafx.h"
#include "_Object\_Component\_Physics\MeshCollider.h"

MeshCollider::MeshCollider(GameObject* g, Transform* t) :
	Collider(g,t, typeid(this).name()),
	stridingMeshInterface(NULL)
{
	_Type = ShapeType::Mesh;
#ifdef _DEBUG
	mbstowcs_s(nullptr, name, typeid(*this).name(), strlen(typeid(*this).name()));
#endif
}


MeshCollider::~MeshCollider()
{
	/*for (auto& vb : vertexBufferArray) {
		delete vb;
	}
	for (auto& ib : indexBufferArray) {
		delete ib;
	}
	SAFE_DELETE(stridingMeshInterface);*/
}

/*!
 * @brief	CSkinModelからメッシュコライダーを生成。
 *@param[in]	model		スキンモデル。
 */
void MeshCollider::Create(SkinModel* model)
{
	Create(model->GetModelData());
}

void MeshCollider::Create(SkinModelData * model)
{
	//初期化された行列。
	D3DXMATRIX iden;
	D3DXMatrixIdentity(&iden);
	//移動量0の行列で計算して作った。
	//model->UpdateBoneMatrix(iden);

	stridingMeshInterface = new btTriangleIndexVertexArray;
	////番兵設定
	//Vector3 Min(FLT_MAX, FLT_MAX, FLT_MAX), Max(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	auto& frames = model->GetFrameList();
	//
	for (auto& frame : frames)
	{
		D3DXFRAME_DERIVED* pFrame = (D3DXFRAME_DERIVED*)frame;
		D3DXMESHCONTAINER_DERIVED* container = (D3DXMESHCONTAINER_DERIVED*)pFrame->pMeshContainer;
		//コンテナが存在する限り。
		while (container != nullptr)
		{
			//CSkinModelからコリジョンで使用する、頂点バッファとインデックスバッファを作成する。
			auto mesh = container->pOrigMesh;

			{
				//頂点ストライドを取得。
				DWORD stride = D3DXGetFVFVertexSize(mesh->GetFVF());
				//頂点バッファを取得。
				LPDIRECT3DVERTEXBUFFER9 vb;
				mesh->GetVertexBuffer(&vb);
				//頂点バッファの定義を取得する。
				D3DVERTEXBUFFER_DESC desc;
				vb->GetDesc(&desc);
				//頂点バッファをロックする。
				Vector3* pos;
				vb->Lock(0, 0, (void**)&pos, D3DLOCK_READONLY);
				VertexBuffer* vertexBuffer = new VertexBuffer;
				int numVertex = mesh->GetNumVertices();
				//当たりデータで使用する頂点バッファを作成。
				for (int v = 0; v < numVertex; v++) {
					Vector3 posTmp = *pos;
					//行列変換。
					posTmp.Transform(pFrame->CombinedTransformationMatrix);

					////最小。
					//Min.x = min(Min.x, posTmp.x);
					//Min.y = min(Min.y, posTmp.y);
					//Min.z = min(Min.z, posTmp.z);
					////最大。
					//Max.x = max(Max.x, posTmp.x);
					//Max.y = max(Max.y, posTmp.y);
					//Max.z = max(Max.z, posTmp.z);

					vertexBuffer->push_back(posTmp);
					char* p = (char*)pos;
					p += stride;
					pos = (Vector3*)p;
				}
				vb->Unlock();
				vb->Release();
				vertexBufferArray.push_back(vertexBuffer);
			}
			{
				//続いてインデックスバッファを作成。
				LPDIRECT3DINDEXBUFFER9 ib;
				mesh->GetIndexBuffer(&ib);
				D3DINDEXBUFFER_DESC desc;
				ib->GetDesc(&desc);
				int stride = 0;
				if (desc.Format == D3DFMT_INDEX16) {
					//インデックスが16bit
					stride = 2;
				}
				else if (desc.Format == D3DFMT_INDEX32) {
					//インデックスが32bit
					stride = 4;
				}
				//インデックスバッファをロック。
				char* p;
				HRESULT hr = ib->Lock(0, 0, (void**)&p, D3DLOCK_READONLY);
				IndexBuffer* indexBuffer = new IndexBuffer;
				for (unsigned int i = 0; i < desc.Size / stride; i++) {
					unsigned int index;
					if (desc.Format == D3DFMT_INDEX16) {
						unsigned short* pIndex = (unsigned short*)p;
						index = (unsigned int)*pIndex;
					}
					else {
						unsigned int* pIndex = (unsigned int*)p;
						index = *pIndex;
					}

					indexBuffer->push_back(index);
					p += stride;
				}
				ib->Unlock();
				//ib->Release();
				indexBufferArray.push_back(indexBuffer);
			}
			//インデックスメッシュを作成。
			btIndexedMesh indexedMesh;
			IndexBuffer* ib = indexBufferArray.back();
			VertexBuffer* vb = vertexBufferArray.back();
			indexedMesh.m_numTriangles = ib->size() / 3;
			indexedMesh.m_triangleIndexBase = (unsigned char*)(&ib->front());
			indexedMesh.m_triangleIndexStride = 12;
			indexedMesh.m_numVertices = vb->size();
			indexedMesh.m_vertexBase = (unsigned char*)(&vb->front());
			indexedMesh.m_vertexStride = sizeof(Vector3);
			stridingMeshInterface->addIndexedMesh(indexedMesh);

			//次のコンテナ
			container = (D3DXMESHCONTAINER_DERIVED*)container->pNextMeshContainer;
		}
	}
	meshShape = new btBvhTriangleMeshShape(stridingMeshInterface, true);
}
