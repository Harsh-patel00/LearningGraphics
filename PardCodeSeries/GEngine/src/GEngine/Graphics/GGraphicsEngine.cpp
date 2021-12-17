#include "GEngine/Graphics/GGraphicsEngine.h"
#include "glad/glad.h"

void GGraphicsEngine::Clear(const GVec4& color)
{
	glClearColor(color.x, color.y, color.z, color.w);
	glClear(GL_COLOR_BUFFER_BIT);
}
