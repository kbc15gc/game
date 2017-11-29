#include "fbstdafx.h"
#include "ObjectFrustumCulling.h"
#include "_Object\_Component\_3D\Camera.h"

#include "_Value\AABB.h"
#include "_Value\AABB2D.h"

void CObjectFrustumCulling::Execute(const AABB & aabb, const D3DXMATRIX& world, const D3DXMATRIX& view, const D3DXMATRIX& proj)
{
	SetCullingFlag(true);
	Vector3 Max = Vector3(-FLT_MAX, -FLT_MAX, -FLT_MAX), Min = Vector3(FLT_MAX, FLT_MAX, FLT_MAX);

	//AABB�̂W���_���X�N���[����Ԃ̐��K�����W�n�ɕϊ�����B
	//x�Ay��1.0�`-1.0�Az��0.0�`1.0�͈͓̔��ɂ������ʓ��ɂ���Ƃ������ƂɂȂ�B
	//8���_���ׂĂ���ʂ̊O�Ȃ�J�����O����B

	for (int i = 0; i < 8; i++) {

		auto v = aabb.GetVertexPosition(i);
		//���_���W
		D3DXVECTOR4 vertPos = { v.x,v.y,v.z,1.0f };
		//���[���h���W�ϊ��B
		D3DXVec4Transform(&vertPos, &vertPos, &world);
		//�r���[���W�ϊ��B
		D3DXVec4Transform(&vertPos, &vertPos, &view);
		//�v���W�F�N�V�������W�ϊ��B
		D3DXVec4Transform(&vertPos, &vertPos, &proj);

		//�X�N���[�����W�ɕϊ��B
		vertPos.x /= vertPos.w;
		vertPos.y /= vertPos.w;
		vertPos.z /= vertPos.w;

		Max.x = max(vertPos.x, Max.x);
		Max.y = max(vertPos.y, Max.y);
		Max.z = max(vertPos.z, Max.z);

		Min.x = min(vertPos.x, Min.x);
		Min.y = min(vertPos.y, Min.y);
		Min.z = min(vertPos.z, Min.z);

		if (vertPos.x >= -1.0f
			&& vertPos.x <= 1.0f
			&& vertPos.y >= -1.0f
			&& vertPos.y <= 1.0f
			&& vertPos.z >= 0.0f
			&& vertPos.z <= 1.0f
			) {
			//��ʓ��ɂ���
			SetCullingFlag(false);
			return;
		}
	}

	AABB2D screen, front, right, under;
	screen.SetUpVertex(Vector2(1, 1), Vector2(-1, -1));
	//�O��B
	front.SetUpVertex(Vector2(Max.x, Max.y), Vector2(Min.x, Min.y));
	//���E
	right.SetUpVertex(Vector2(Max.z, Max.y), Vector2(Min.z, Min.y));
	//�㉺�B
	under.SetUpVertex(Vector2(Max.x, Max.z), Vector2(Min.x, Min.z));
	//2D��AABB�̏Փ˔���B
	if (Min.z <= 1.0f &&
		(screen.IsHit(front) ||
		screen.IsHit(right) ||
		screen.IsHit(under)))
	{
			SetCullingFlag(false);
			return;
	}
}
