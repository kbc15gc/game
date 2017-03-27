#pragma once
#include "Collider.h"

class SkinModel;

/*!
 * @brief	���b�V���R���C�_�[�B
 */
class MeshCollider : public Collider
{
public:
	/*!
	 * @brief	�R���X�g���N�^�B
	 */
	MeshCollider(GameObject* g, Transform* t);
	/*!
	 * @brief	�f�X�g���N�^�B
	 */
	~MeshCollider();
	/*!
	 * @brief	CSkinModelData���烁�b�V���R���C�_�[�𐶐��B
	 *@param[in]	model		�X�L�����f���B
	 */
	void Create(SkinModel* model);
	btCollisionShape* GetBody() override
	{
		return meshShape;
	}
private:
	typedef std::vector<Vector3>		VertexBuffer;		//���_�o�b�t�@�B
	typedef std::vector<unsigned int>	IndexBuffer;		//�C���f�b�N�X�o�b�t�@�B
	std::vector<VertexBuffer*>		vertexBufferArray;		//���_�o�b�t�@�̔z��B
	std::vector<IndexBuffer*>		indexBufferArray;		//�C���f�b�N�X�o�b�t�@�̔z��B
	btBvhTriangleMeshShape*			meshShape;				//���b�V���`��B
	btTriangleIndexVertexArray*		stridingMeshInterface;
};

