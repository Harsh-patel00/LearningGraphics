#include <glad/glad.h>

#include "GEngine\Graphics\GVertexArrayObject.h"


GVertexArrayObject::GVertexArrayObject(const GVertexBufferData& data)
{
	glGenBuffers(1, &_vertexBufferId);
	

	glGenVertexArrays(1, &_vertexArrayObjectId);
	glBindVertexArray(_vertexArrayObjectId);

	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, data.vertexSize * data.listSize, data.verticesList, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, data.vertexSize, 0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	_vertexBufferData = data;
}

GVertexArrayObject::~GVertexArrayObject()
{
	glDeleteBuffers(1, &_vertexBufferId);
	glDeleteVertexArrays(1, &_vertexArrayObjectId);
}

ui32 GVertexArrayObject::GetId()
{
	return _vertexArrayObjectId;
}

ui32 GVertexArrayObject::GetVertexBufferSize()
{
	return _vertexBufferData.listSize;
}

ui32 GVertexArrayObject::GetVertexSize()
{
	return _vertexBufferData.vertexSize;
}
