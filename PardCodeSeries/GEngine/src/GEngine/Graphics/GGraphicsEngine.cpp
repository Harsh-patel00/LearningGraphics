#include "GEngine/Graphics/GGraphicsEngine.h"
#include "GEngine/Graphics/GVertexArrayObject.h"
#include "GEngine/Graphics/GShaderProgram.h"
#include "glad/glad.h"

GVertexArrayObjectPtr GGraphicsEngine::CreateVertexArrayObject(const GVertexBufferDesc& bufferDesc)
{
	return std::make_shared<GVertexArrayObject>(bufferDesc);
}

GShaderProgramPtr GGraphicsEngine::CreateShader(const GShaderProgramDesc& desc)
{
	return std::make_shared<GShaderProgram>(desc);
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

void GGraphicsEngine::SetShader(const GShaderProgramPtr& shader)
{
	glUseProgram(shader->GetId());
}

void GGraphicsEngine::DrawTriangle(ui32 vertexCount, ui32 offset)
{
	glDrawArrays(GL_TRIANGLES, offset, vertexCount);
}


