#include "fbstdafx.h"
#include "ObjectFrustumCulling.h"
#include "_Object\_Component\_3D\Camera.h"
#include "_Value\AABB.h"

CObjectFrustumCulling::CObjectFrustumCulling()
{
}

CObjectFrustumCulling::~CObjectFrustumCulling()
{
}

void CObjectFrustumCulling::Execute(const AABB & aabb)
{
	if (_Camera != nullptr) {
		//�r���[�v���W�F�N�V�����s��쐬�B
		auto viewProjMatrix = _Camera->GetViewMat() * _Camera->GetProjectionMat();
		SetCullingFlag(true);
		//AABB�̂W���_���X�N���[����Ԃ̐��K�����W�n�ɕϊ�����B
		//x�Ay��1.0�`-1.0�Az��0.0�`1.0�͈͓̔��ɂ������ʓ��ɂ���Ƃ������ƂɂȂ�B
		//8���_���ׂĂ���ʂ̊O�Ȃ�J�����O����B
		for (int i = 0; i < 8; i++) {
			D3DXVECTOR4 vertPos;
			auto v = aabb.GetVertexPosition(i);
			vertPos.x = v.x;
			vertPos.y = v.y;
			vertPos.z = v.z;
			vertPos.w = 1.0f;
			//�s��������āA�X�N���[�����W�ɕϊ��B
			D3DXVec4Transform(&vertPos, &vertPos, &viewProjMatrix);
			//���K�����W�n�ɕϊ��B
			vertPos.x /= vertPos.w;
			vertPos.y /= vertPos.w;
			vertPos.z /= vertPos.w;

			if (vertPos.x >= -1.0f
				&& vertPos.x <= 1.0f
				&& vertPos.y >= -1.0f
				&& vertPos.y <= 1.0f
				&& vertPos.z >= 0.0f
				&& vertPos.z <= 1.0f
				) {
				//��ʓ��ɂ���
				SetCullingFlag(false);
				break;
			}
		}
	}
}
