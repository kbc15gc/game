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
 * @brief	CSkinModel���烁�b�V���R���C�_�[�𐶐��B
 *@param[in]	model		�X�L�����f���B
 */
void MeshCollider::Create(SkinModel* model)
{
	Create(model->GetModelData());
}

void MeshCollider::Create(SkinModelData * model)
{
	//���������ꂽ�s��B
	D3DXMATRIX iden;
	D3DXMatrixIdentity(&iden);
	//�ړ���0�̍s��Ōv�Z���č�����B
	//model->UpdateBoneMatrix(iden);

	stridingMeshInterface = new btTriangleIndexVertexArray;
	////�ԕ��ݒ�
	//Vector3 Min(FLT_MAX, FLT_MAX, FLT_MAX), Max(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	auto& frames = model->GetFrameList();
	//
	for (auto& frame : frames)
	{
		D3DXFRAME_DERIVED* pFrame = (D3DXFRAME_DERIVED*)frame;
		D3DXMESHCONTAINER_DERIVED* container = (D3DXMESHCONTAINER_DERIVED*)pFrame->pMeshContainer;
		//�R���e�i�����݂������B
		while (container != nullptr)
		{
			//CSkinModel����R���W�����Ŏg�p����A���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@���쐬����B
			auto mesh = container->pOrigMesh;

			{
				//���_�X�g���C�h���擾�B
				DWORD stride = D3DXGetFVFVertexSize(mesh->GetFVF());
				//���_�o�b�t�@���擾�B
				LPDIRECT3DVERTEXBUFFER9 vb;
				mesh->GetVertexBuffer(&vb);
				//���_�o�b�t�@�̒�`���擾����B
				D3DVERTEXBUFFER_DESC desc;
				vb->GetDesc(&desc);
				//���_�o�b�t�@�����b�N����B
				Vector3* pos;
				vb->Lock(0, 0, (void**)&pos, D3DLOCK_READONLY);
				VertexBuffer* vertexBuffer = new VertexBuffer;
				int numVertex = mesh->GetNumVertices();
				//������f�[�^�Ŏg�p���钸�_�o�b�t�@���쐬�B
				for (int v = 0; v < numVertex; v++) {
					Vector3 posTmp = *pos;
					//�s��ϊ��B
					posTmp.Transform(pFrame->CombinedTransformationMatrix);

					////�ŏ��B
					//Min.x = min(Min.x, posTmp.x);
					//Min.y = min(Min.y, posTmp.y);
					//Min.z = min(Min.z, posTmp.z);
					////�ő�B
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
				//�����ăC���f�b�N�X�o�b�t�@���쐬�B
				LPDIRECT3DINDEXBUFFER9 ib;
				mesh->GetIndexBuffer(&ib);
				D3DINDEXBUFFER_DESC desc;
				ib->GetDesc(&desc);
				int stride = 0;
				if (desc.Format == D3DFMT_INDEX16) {
					//�C���f�b�N�X��16bit
					stride = 2;
				}
				else if (desc.Format == D3DFMT_INDEX32) {
					//�C���f�b�N�X��32bit
					stride = 4;
				}
				//�C���f�b�N�X�o�b�t�@�����b�N�B
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
			//�C���f�b�N�X���b�V�����쐬�B
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

			//���̃R���e�i
			container = (D3DXMESHCONTAINER_DERIVED*)container->pNextMeshContainer;
		}
	}
	meshShape = new btBvhTriangleMeshShape(stridingMeshInterface, true);
}
