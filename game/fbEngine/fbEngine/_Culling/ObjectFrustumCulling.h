#pragma once
#include "_Culling\IObjectCulling.h"

//�t���X�^���J�����O�����B
//�I�u�W�F�N�g������AABB���\������8���_���J�����ɉf��Ȃ��ꍇ�ɃJ�����O����܂��B
class CObjectFrustumCulling : public IObjectCulling {
public:
	CObjectFrustumCulling() {}
	~CObjectFrustumCulling() {}
	
	//�J�����O�A���S���Y���̎��s�B
	//[in] �J�����O���s�������I�u�W�F�N�g������AABB�B
	void Execute(const AABB& aabb, const D3DXMATRIX& rotation, const D3DXMATRIX& view, const D3DXMATRIX& proj) override;
};