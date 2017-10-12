#pragma once

#include "_Culling\IObjectCulling.h"
class Camera;


//�t���X�^���J�����O�����B
//�I�u�W�F�N�g������AABB���\������8���_���J�����ɉf��Ȃ��ꍇ�ɃJ�����O����܂��B
class CObjectFrustumCulling : public IObjectCulling {
public:
	CObjectFrustumCulling();
	~CObjectFrustumCulling();
	//�J������ݒ�B
	void SetCamera(const Camera& camera)
	{
		_Camera = &camera;
	}
	//�J�����O�A���S���Y���̎��s�B
	//[in] �J�����O���s�������I�u�W�F�N�g������AABB�B
	void Execute(const AABB& aabb, const D3DXMATRIX& rotation) override;
private:
	const Camera*	_Camera = nullptr;
};