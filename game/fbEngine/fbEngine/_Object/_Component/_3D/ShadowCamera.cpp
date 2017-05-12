#include"fbstdafx.h"
#include "ShadowCamera.h"

void ShadowCamera::Update()
{
	ViewMatrixUpdate();
	ParallelProjectionMatrixUpdate();
}
