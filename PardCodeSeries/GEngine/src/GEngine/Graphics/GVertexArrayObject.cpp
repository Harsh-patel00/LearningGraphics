#include <glad/glad.h>

#include "GEngine\Graphics\GVertexArrayObject.h"
#include "GEngine\Console\GConsole.h"


GVertexArrayObject::GVertexArrayObject(const GVertexBufferDesc& data)
{
	if (!data.listSize) GConsole::LOGERROR("GVertxArrayObject::listSize is NULL");
	if (!data.vertexSize) GConsole::LOGERROR("GVertxArrayObject::vertexSize is NULL");
	if (!data.verticesList) GConsole::LOGERROR("GVertxArrayObject::vertexList is NULL");


	glGenVertexArrays(1, &_vertexArrayObjectId);
	glBindVertexArray(_vertexArrayObjectId);

	glGenBuffers(1, &_vertexBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, data.vertexSize * data.listSize, data.verticesList, GL_STATIC_DRAW);

	for (ui32 i = 0; i < data.attributesListSize; i++)
	{
		glVertexAttribPointer(i, data.attributesList[i].numElements, GL_FLOAT, 
			GL_FALSE, data.vertexSize, (void*)((i==0) ? 0 : data.attributesList[i-1].numElements * sizeof(f32)));
		glEnableVertexAttribArray(i);
	}


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
