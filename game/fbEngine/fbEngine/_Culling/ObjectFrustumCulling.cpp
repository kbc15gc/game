#include "fbstdafx.h"
#include "ObjectFrustumCulling.h"
#include "_Object\_Component\_3D\Camera.h"

#include "_Value\AABB.h"
#include "_Value\AABB2D.h"

CObjectFrustumCulling::CObjectFrustumCulling()
{
}

CObjectFrustumCulling::~CObjectFrustumCulling()
{
}

void CObjectFrustumCulling::Execute(const AABB & aabb, const D3DXMATRIX& rotation)
{
	if (_Camera != nullptr) {
		//�r���[�v���W�F�N�V�����s��쐬�B
		auto viewProjMatrix = _Camera->GetViewMat() * _Camera->GetProjectionMat();
		SetCullingFlag(true);
		const float sikii = 1.5f;
		Vector2 ru = Vector2(-FLT_MAX, -FLT_MAX), ld = Vector2(FLT_MAX, FLT_MAX);

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
			//��]���l���B
			//D3DXVec4Transform(&vertPos, &vertPos, &rotation);
			//�s��������āA�X�N���[�����W�ɕϊ��B
			D3DXVec4Transform(&vertPos, &vertPos, &viewProjMatrix);
			//���K�����W�n�ɕϊ��B
			vertPos.x /= vertPos.w;
			vertPos.y /= vertPos.w;
			vertPos.z /= vertPos.w;

			ru.x = max(vertPos.x, ru.x);
			ru.y = max(vertPos.y, ru.y);

			ld.x = min(vertPos.x, ld.x);
			ld.y = min(vertPos.y, ld.y);

			
			if (vertPos.x >= -sikii
				&& vertPos.x <= sikii
				&& vertPos.y >= -sikii
				&& vertPos.y <= sikii
				&& vertPos.z >= 0.0f
				&& vertPos.z <= sikii
				) {
				//��ʓ��ɂ���
				SetCullingFlag(false);
				break;
			}
		}


		AABB2D obj, screen;
		obj.SetUpVertex(ru, ld);
		screen.SetUpVertex(Vector2(1, -1), Vector2(-1, 1));
		//AABB�̏Փ˔���B
		if (screen.IsHit(obj))
		{
			SetCullingFlag(false);
			return;
		}else
		{
			int a = 0;
		}
	}
}
