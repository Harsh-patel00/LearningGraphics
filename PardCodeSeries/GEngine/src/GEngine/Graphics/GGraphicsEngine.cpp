#include "GEngine/Graphics/GGraphicsEngine.h"
#include "GEngine/Graphics/GVertexArrayObject.h"
#include "glad/glad.h"

GVertexArrayObjectPtr GGraphicsEngine::CreateVertexArrayObject(const GVertexBufferData& bufferData)
{
	return std::make_shared<GVertexArrayObject>(bufferData);
}

void GGraphicsEngine::Clear(const GVec4& color)
{
	glClearColor(color.x, color.y, color.z, color.w);
	glClear(GL_COLOR_BUFFER_BIT);
}

void GGraphicsEngine::SetViewport(const GRect& rect)
{
	glViewport(rect.left, rect.top, rect.width, rect.height);
}

void GGraphicsEngine::SetVertexArrayObj(const GVertexArrayObjectPtr& vao)
{
	glBindVertexArray(vao->GetId());
}

void GGraphicsEngine::DrawTriangle(ui32 vertexCount, ui32 offset)
{
	glDrawArrays(GL_TRIANGLES, offset, vertexCount);
}


