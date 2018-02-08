#include "CommonMeshes.h"
#include <nclgl\NCLDebug.h>
#include <nclgl\OBJMesh.h>
#include <SOIL.h>

Mesh* CommonMeshes::m_pCube		= NULL;
Mesh* CommonMeshes::m_pSphere	= NULL;

void CommonMeshes::InitializeMeshes()
{
	if (m_pCube == NULL)
	{
		m_pCube = new OBJMesh(MESHDIR"cube.obj");

		m_pSphere = new OBJMesh(MESHDIR"sphere.obj");
	}
}

void CommonMeshes::ReleaseMeshes()
{
	SAFE_DELETE(m_pCube);
	SAFE_DELETE(m_pSphere);
}