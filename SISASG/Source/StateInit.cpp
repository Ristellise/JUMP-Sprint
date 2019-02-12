#include "StateInit.h"

void Stateinit::OnRender()
{
    modelStack.PushMatrix();
    modelStack.Translate(0, 0, 0);
    RenderMesh(meshList[GEO_AXES], false);
    modelStack.PopMatrix();
}