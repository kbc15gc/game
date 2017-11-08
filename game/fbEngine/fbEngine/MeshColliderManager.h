#pragma once

class MeshCollider;
class SkinModel;

//���b�V���R���C�_�[���Ǘ�����}�l�[�W���[�N���X�B
class MeshColliderManager
{
public:
	~MeshColliderManager();

	//���f�����󂯎��A���̃��f���̃R���C�_�[��Ԃ��B
	static MeshCollider* CloneMeshCollider(const char* filename);
private:
	//���b�V���R���C�_�[��ۑ�����map
	static map<UINT64, MeshCollider*> _Colliders;
};