#include "Mesh.h"

Mesh::Mesh(void)	{
	glGenVertexArrays(1, &arrayObject);
	
	for(int i = 0; i < MAX_BUFFER; ++i) {
		bufferObject[i] = 0;
	}

	texture		 = 0;
	numVertices  = 0;
	type		 = GL_TRIANGLES;

	//Later tutorial stuff
	numIndices    = 0;
	bumpTexture	  = 0;
	vertices	  = NULL;
	textureCoords = NULL;
	normals		  = NULL;
	tangents	  = NULL;
	indices		  = NULL;
	colours		  = NULL;
}

Mesh::Mesh(const Mesh& rhs)
{
	glGenVertexArrays(1, &arrayObject);

	for (int i = 0; i < MAX_BUFFER; ++i) {
		bufferObject[i] = 0;
	}

	texture			= rhs.texture;
	bumpTexture		= rhs.bumpTexture;

	numVertices		= rhs.numVertices;
	numIndices		= rhs.numIndices;
	type			= rhs.type;

	//Later tutorial stuff
	vertices		= rhs.vertices		? new Vector3[numVertices]	: NULL;
	textureCoords	= rhs.textureCoords	? new Vector2[numVertices]	: NULL;
	normals			= rhs.normals		? new Vector3[numVertices]	: NULL;
	tangents		= rhs.tangents		? new Vector3[numVertices]	: NULL;
	colours			= rhs.colours		? new Vector4[numVertices]	: NULL;
	indices			= rhs.indices		? new uint[numIndices]		: NULL;

	if (vertices)		memcpy(vertices		, rhs.vertices		, numVertices * sizeof(Vector3));
	if (textureCoords)	memcpy(textureCoords, rhs.textureCoords	, numVertices * sizeof(Vector2));
	if (normals)		memcpy(normals		, rhs.normals		, numVertices * sizeof(Vector3));
	if (tangents)		memcpy(tangents		, rhs.tangents		, numVertices * sizeof(Vector3));
	if (colours)		memcpy(colours		, rhs.colours		, numVertices * sizeof(Vector4));
	if (indices)		memcpy(indices		, rhs.indices		, numIndices  * sizeof(GLuint));

	BufferData();
}

Mesh::~Mesh(void)	{
	glDeleteVertexArrays(1, &arrayObject);			//Delete our VAO
	glDeleteBuffers(MAX_BUFFER, bufferObject);		//Delete our VBOs

	glDeleteTextures(1,&texture);					//We'll be nice and delete our texture when we're done with it
	glDeleteTextures(1,&bumpTexture);				//We'll be nice and delete our texture when we're done with it

	//Later tutorial stuff
	delete[]vertices;
	delete[]indices;
	delete[]textureCoords;
	delete[]tangents;
	delete[]normals;
	delete[]colours;

	Vector3 triPos[3] = {
		Vector3(0.5f, -0.5f, 0),
		Vector3(0.0f, 0.5f, 0),
		Vector3(-0.5f, -0.5f, 0)
	};

	Vector4 triCol[3] =  {
		Vector4(1,0,0,1),
		Vector4(0,1,0,1),
		Vector4(0,0,1,1)
	};
}


GLuint activeArrayHack = 0;
GLuint activeTexture1Hack = 0;
GLuint activeTexture2Hack = 0;
void Mesh::Draw()	{

	//if (activeArrayHack != arrayObject)
	{
		glBindVertexArray(arrayObject);
		activeArrayHack = arrayObject;
	}

	if (bufferObject[INDEX_BUFFER]) {
		glDrawElements(type, numIndices, GL_UNSIGNED_INT, 0);
	}
	else {
		glDrawArrays(type, 0, numVertices);	//Draw the triangle!
	}
		glBindVertexArray(0);
}

Mesh* Mesh::GenerateTriangle()	{
	Mesh*m = new Mesh();
	m->numVertices = 3;

	m->vertices = new Vector3[m->numVertices];
	m->vertices[0] = Vector3(0.0f,	0.5f,	0.0f);
	m->vertices[1] = Vector3(0.5f,  -0.5f,	0.0f);
	m->vertices[2] = Vector3(-0.5f, -0.5f,	0.0f);

	m->textureCoords = new Vector2[m->numVertices];
	m->textureCoords[0] = Vector2(0.5f,	0.0f);
	m->textureCoords[1] = Vector2(1.0f,	1.0f);
	m->textureCoords[2] = Vector2(0.0f,	1.0f);

	m->colours = new Vector4[m->numVertices];
	m->colours[0] = Vector4(1.0f, 0.0f, 0.0f,1.0f);
	m->colours[1] = Vector4(0.0f, 1.0f, 0.0f,1.0f);
	m->colours[2] = Vector4(0.0f, 0.0f, 1.0f,1.0f);

	m->GenerateNormals();
	m->GenerateTangents();
	m->BufferData();

	return m;
}

Mesh* Mesh::TestQuad(float r)	{
	Mesh*m = new Mesh();
	m->numVertices = 6;
	m->type = GL_TRIANGLES;

	m->vertices			= new Vector3[m->numVertices];
	m->textureCoords	= new Vector2[m->numVertices];
	//m->colours			= new Vector4[m->numVertices];
	//m->normals			= new Vector3[m->numVertices];
	//m->tangents			= new Vector3[m->numVertices];

	m->vertices[0] = 	Vector3(-1.0f, -1.0f, 0.0f);
	m->vertices[1] = 	Vector3(1.0f,	-1.0f, 0.0f);
	m->vertices[2] = 	Vector3(-1.0f, 1.0f, 0.0f);

	m->vertices[3] = 	Vector3(1.0f,	-1.0f, 0.0f);
	m->vertices[4] = 	Vector3(1.0f, 1.0f, 0.0f);
	m->vertices[5] = 	Vector3(-1.0f,  1.0f, 0.0f);

	m->textureCoords[0] = Vector2(0.0f,	0.0f);
	m->textureCoords[1] = Vector2(1.0f,	0.0f);
	m->textureCoords[2] = Vector2(0.0f,	1.0f);

	m->textureCoords[3] = Vector2(1.0f,	0.0f);
	m->textureCoords[4] = Vector2(1.0f,	1.0f);
	m->textureCoords[5] = Vector2(0.0f,	1.0f);

	Matrix4 pushPos  = Matrix4::Translation(Vector3(0.5,0.5,0));
	Matrix4 popPos	 = Matrix4::Translation(Vector3(-0.5,-0.5,0));
	Matrix4 rotation = Matrix4::Rotation(r,Vector3(0,0,1));
	Matrix4 concat   = pushPos * rotation * popPos;

	for(int i = 0; i < 4; ++i) {
		Vector4 rotvec = concat * Vector4(m->textureCoords[i].x,m->textureCoords[i].y,0,1);
		m->textureCoords[i].x = rotvec.x;
		m->textureCoords[i].y = rotvec.y;
	}

	m->GenerateNormals();
	m->GenerateTangents();
	m->BufferData();

	return m;
}

Mesh*	Mesh::GenerateCone(float segments) {
	Mesh*m = new Mesh();

	m->numVertices = 16;
	m->numIndices  = 8 * 6;
	m->type = GL_TRIANGLES;

	m->vertices			= new Vector3[m->numVertices];
	m->normals			= new Vector3[m->numVertices];
	m->indices			= new unsigned int[m->numIndices];

	const int stepCount = 8; //Modify this to make the circle more / less circular looking!
	const float divisor = 360.0f / stepCount;

	float radius = 1.0f;

	for(int i = 0; i < 8; ++i) {
		float startx = radius * (float)cos(DegToRad(i * divisor));
		float starty = radius * (float)sin(DegToRad(i * divisor));

		m->vertices[i] = Vector3(startx, starty, 1.0f);
	}

	for(int i = 0; i < 8; ++i) {
		float startx = 0.001f * (float)cos(DegToRad(i * divisor));
		float starty = 0.001f * (float)sin(DegToRad(i * divisor));

		//m->vertices[8+i] = Vector3(startx, starty, 0.0f);

		m->vertices[8+i] = Vector3();
	}

	int currentIndex = 0;
	for(int i = 0; i < 7; ++i) {
		int start = i;

		m->indices[currentIndex++] = start+0;
		m->indices[currentIndex++] = start+1+8;
		m->indices[currentIndex++] = start+1;
		
		m->indices[currentIndex++] = start+1+8;
		m->indices[currentIndex++] = start+0;
		m->indices[currentIndex++] = start+0+8;	
	}

	m->indices[currentIndex++] = 7;
	m->indices[currentIndex++] = 8;
	m->indices[currentIndex++] = 0;
	
	m->indices[currentIndex++] = 8;
	m->indices[currentIndex++] = 7;	
	m->indices[currentIndex++] = 15;

	m->GenerateNormals();

	for(int i = 0; i < 8; ++i) {
		//m->normals[i+8] = Vector3(0,1,0);
	}

	m->BufferData();

	return m;
}

Mesh*	Mesh::TestTriangle(float r) {
	Mesh*m = new Mesh();
	m->numVertices = 3;

	m->vertices		= new Vector3[m->numVertices];
	m->vertices[0]	= Vector3(0.0f,	0.0f,	0.0f);
	m->vertices[1]	= Vector3(1.0f,  0.0f,	0.0f);
	m->vertices[2]	= Vector3(0.0f,  1.0f,	0.0f);

	m->textureCoords	= new Vector2[m->numVertices];
	m->textureCoords[0] = Vector2(0.0f,	0.0f);
	m->textureCoords[1] = Vector2(1.0f,	0.0f);
	m->textureCoords[2] = Vector2(0.0f,	1.0f);

	Matrix4 pushPos  = Matrix4::Translation(Vector3(0.5,0.5,0));
	Matrix4 popPos	 = Matrix4::Translation(Vector3(-0.5,-0.5,0));
	Matrix4 rotation = Matrix4::Rotation(r,Vector3(0,0,1));
	Matrix4 concat   = pushPos * rotation * popPos;

	for(int i = 0; i < 3; ++i) {
		Vector4 rotvec = concat * Vector4(m->textureCoords[i].x,m->textureCoords[i].y,0,1);
		m->textureCoords[i].x = rotvec.x;
		m->textureCoords[i].y = rotvec.y;
	}

	m->GenerateNormals();
	m->GenerateTangents();
	m->BufferData();

	return m;
}

Mesh* Mesh::GenerateQuad()	{
	Mesh* m = new Mesh();

	m->numVertices = 4;
	m->type = GL_TRIANGLE_STRIP;

	m->vertices			= new Vector3[m->numVertices];
	m->textureCoords	= new Vector2[m->numVertices];
	m->colours			= new Vector4[m->numVertices];
	m->normals			= new Vector3[m->numVertices];
	m->tangents			= new Vector3[m->numVertices];

	m->vertices[0] = 	Vector3(-1.0f, -1.0f, 0.0f);
	m->vertices[1] = 	Vector3(-1.0f,	1.0f, 0.0f);
	m->vertices[2] = 	Vector3(1.0f, -1.0f, 0.0f);
	m->vertices[3] = 	Vector3(1.0f,  1.0f, 0.0f);

	m->textureCoords[0] = Vector2(0.0f,	1.0f);
	m->textureCoords[1] = Vector2(0.0f,	0.0f);
	m->textureCoords[2] = Vector2(1.0f,	1.0f);
	m->textureCoords[3] = Vector2(1.0f,	0.0f);

	for(int i = 0; i < 4; ++i) {
		m->colours[i] = Vector4(1.0f, 1.0f,1.0f,1.0f);
		m->normals[i] = Vector3(0.0f, 0.0f,-1.0f);
		m->tangents[i] = Vector3(1.0f, 0.0f,0.0f);
	}

	//m->GenerateNormals();
	//m->GenerateTangents();
	m->BufferData();

	return m;
}


Mesh* Mesh::GenerateQuadAlt()	{
	Mesh* m = new Mesh();

	m->numVertices = 4;
	m->type = GL_TRIANGLE_STRIP;

	m->vertices			= new Vector3[m->numVertices];
	m->textureCoords	= new Vector2[m->numVertices];
	m->colours			= new Vector4[m->numVertices];
	m->normals			= new Vector3[m->numVertices];
	m->tangents			= new Vector3[m->numVertices];

	m->vertices[0] = 	Vector3(0.0f, 0.0f, 0.0f);
	m->vertices[1] = 	Vector3(0.0f, 1.0f, 0.0f);
	m->vertices[2] = 	Vector3(1.0f, 0.0f, 0.0f);
	m->vertices[3] = 	Vector3(1.0f,  1.0f, 0.0f);

	m->textureCoords[0] = Vector2(0.0f,	0.0f);
	m->textureCoords[1] = Vector2(0.0f,	1.0f);
	m->textureCoords[2] = Vector2(1.0f,	0.0f);
	m->textureCoords[3] = Vector2(1.0f,	1.0f);

	for(int i = 0; i < 4; ++i) {
		m->colours[i] = Vector4(1.0f, 1.0f,1.0f,1.0f);
		m->normals[i] = Vector3(0.0f, 0.0f,-1.0f);
		m->tangents[i] = Vector3(1.0f, 0.0f,0.0f);
	}

	m->BufferData();

	return m;
}

Mesh* Mesh::GenerateMesh(int w, int h, float s) {
	Mesh* m = new Mesh();
	if (w == 0) { w = h; }
	m->numVertices = 6 * w * h;//m->numVertices = 4 * w * h;
	//m->type = GL_TRIANGLE_STRIP;

	m->vertices = new Vector3[m->numVertices];
	m->textureCoords = new Vector2[m->numVertices];
	m->colours = new Vector4[m->numVertices];
	m->normals = new Vector3[m->numVertices];
	m->tangents = new Vector3[m->numVertices];

	int c = 0;

	for (int i = 0; i < w; ++i) {
			for (int j = 0; j < h; ++j) {
				m->vertices[c] = Vector3(s*i,s*j, 0.0f);
				m->vertices[c + 1] = Vector3(s*i,s*(j+1), 0.0f);
				m->vertices[c + 2] = Vector3(s*(i + 1),s*j, 0.0f);

				m->vertices[c + 3] = Vector3(s*i, s*(j + 1), 0.0f);
				m->vertices[c + 4] = Vector3(s*(i + 1),s*(j + 1), 0.0f);
				m->vertices[c + 5] = Vector3(s*(i + 1), s*j, 0.0f);


				m->textureCoords[c]		= Vector2((float)i / (float)w		, (float)j / (float)h);
				m->textureCoords[c + 1] = Vector2((float)i / (float)w		, (float)(j + 1) / (float)h);
				m->textureCoords[c + 2] = Vector2((float)(i + 1) / (float)w	, (float)j / (float)h);

				m->textureCoords[c + 3] = Vector2((float)i / (float)w		, (float)(j+1) / (float)h);
				m->textureCoords[c + 4] = Vector2((float)(i + 1) / (float)w	, (float)(j + 1) / (float)h);
				m->textureCoords[c + 5] = Vector2((float)(i + 1) / (float)w	, (float)j / (float)h);

				c += 6;
			}
		}

	//for (int i = 0; i < w; ++i) {
	//	for (int j = 0; j < h; ++j) {
	//		m->vertices[c] = Vector3(s*i,s*j, 0.0f);
	//		m->vertices[c + 1] = Vector3(s*i,s*(j+1), 0.0f);
	//		m->vertices[c + 2] = Vector3(s*(i+1),s*j, 0.0f);
	//		m->vertices[c + 3] = Vector3(s*(i+1),s*(j+1), 0.0f);

	//		m->textureCoords[c] = Vector2(0.0f, 1.0f);
	//		m->textureCoords[c + 1] = Vector2(0.0f, 0.0f);
	//		m->textureCoords[c + 2] = Vector2(1.0f, 1.0f);
	//		m->textureCoords[c + 3] = Vector2(1.0f, 0.0f);

	//		c += 4;
	//	}
	//}

	for (int i = 0; i < m->numVertices; ++i) {
		m->colours[i] = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		m->normals[i] = Vector3(0.0f, 0.0f, -1.0f);
		m->tangents[i] = Vector3(1.0f, 0.0f, 0.0f);
	}

	m->BufferData();

	return m;
}

void Mesh::ClearBuffers() {
	glDeleteBuffers(MAX_BUFFER, bufferObject);		//Delete our VBOs
}

void	Mesh::BufferData()	{
	//GenerateNormals();
	//GenerateTangents();

	glBindVertexArray(arrayObject);
	
	//Buffer vertex data
	glGenBuffers(1, &bufferObject[VERTEX_BUFFER]);
	glBindBuffer(GL_ARRAY_BUFFER, bufferObject[VERTEX_BUFFER]);
	glBufferData(GL_ARRAY_BUFFER, numVertices*sizeof(Vector3), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(VERTEX_BUFFER, 3, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(VERTEX_BUFFER);

	//Buffer texture data
	if(textureCoords) {
		glGenBuffers(1, &bufferObject[TEXTURE_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, bufferObject[TEXTURE_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, numVertices*sizeof(Vector2), textureCoords, GL_STATIC_DRAW);
		glVertexAttribPointer(TEXTURE_BUFFER, 2, GL_FLOAT, GL_FALSE, 0, 0); 
		glEnableVertexAttribArray(TEXTURE_BUFFER);
	}

	//buffer colour data
	if (colours)	{
		glGenBuffers(1, &bufferObject[COLOUR_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, bufferObject[COLOUR_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, numVertices*sizeof(Vector4), colours, GL_STATIC_DRAW);
		glVertexAttribPointer(COLOUR_BUFFER, 4, GL_FLOAT, GL_FALSE, 0, 0); 
		glEnableVertexAttribArray(COLOUR_BUFFER);
	}

	//Buffer normal data
	if(normals) {
		glGenBuffers(1, &bufferObject[NORMAL_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, bufferObject[NORMAL_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, numVertices*sizeof(Vector3), normals, GL_STATIC_DRAW);
		glVertexAttribPointer(NORMAL_BUFFER, 3, GL_FLOAT, GL_FALSE, 0, 0); 
		glEnableVertexAttribArray(NORMAL_BUFFER);
	}

	//Buffer tangent data
	if(tangents) {
		glGenBuffers(1, &bufferObject[TANGENT_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, bufferObject[TANGENT_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, numVertices*sizeof(Vector3), tangents, GL_STATIC_DRAW);
		glVertexAttribPointer(TANGENT_BUFFER, 3, GL_FLOAT, GL_FALSE, 0, 0); 
		glEnableVertexAttribArray(TANGENT_BUFFER);
	}

	//buffer index data
	if(indices) {
		glGenBuffers(1, &bufferObject[INDEX_BUFFER]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferObject[INDEX_BUFFER]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices*sizeof(GLuint), indices, GL_STATIC_DRAW);
	}

	glBindVertexArray(0);
}

/*
Stuff for later tutorials...
*/

void	Mesh::GenerateNormals()	{
	if(!normals) {
		normals = new Vector3[numVertices];
	}
	for(GLuint i = 0; i < numVertices; ++i){
		normals[i] = Vector3();
	}

	if(indices) {
		GLuint i = 0;

		int test = 0;
		for(i = 0; i < numIndices; i+=3){
			int a = indices[i];
			int b = indices[i+1];
			int c = indices[i+2];

			Vector3 normal = Vector3::Cross((vertices[b]-vertices[a]),(vertices[c]-vertices[a]));

			normals[a] += normal;
			normals[b] += normal;
			normals[c] += normal;

			test+=3;
		}
		bool asdf = true;
	}
	else{
		//It's just a list of triangles, so generate face normals
		if (type != GL_TRIANGLE_STRIP)
		{
			for (GLuint i = 0; i < numVertices; i += 3) {
				Vector3 &a = vertices[i];
				Vector3 &b = vertices[i + 1];
				Vector3 &c = vertices[i + 2];

				Vector3 normal = Vector3::Cross(b - a, c - a);

				normals[i] = normal;
				normals[i + 1] = normal;
				normals[i + 2] = normal;
			}
		}
		else {
			for (GLuint i = 0; i < numVertices; i += 4) {
				Vector3 &a = vertices[i];
				Vector3 &b = vertices[i + 1];
				Vector3 &c = vertices[i + 2];

				Vector3 normal = Vector3::Cross(b - a, c - a);

				normals[i] = normal;
				normals[i + 1] = normal;
				normals[i + 2] = normal;
				normals[i + 3] = normal;
			}

		}

	}

	for(GLuint i = 0; i < numVertices; ++i){
		normals[i].Normalise();
	}
}

void Mesh::GenerateTangents() {
	//Extra! stops rare occurrence of this function being called
	//on a mesh without tex coords, which would break quite badly!
	if(!textureCoords) {
		return;
	}

	if(!tangents) {
		tangents = new Vector3[numVertices];
	}
	for(GLuint i = 0; i < numVertices; ++i){
		tangents[i] = Vector3();
	}

	if(indices) {
		for(GLuint i = 0; i < numIndices; i+=3){
			int a = indices[i];
			int b = indices[i+1];
			int c = indices[i+2];

			Vector3 tangent = GenerateTangent(vertices[a],vertices[b],vertices[c],textureCoords[a],textureCoords[b],textureCoords[c]);

			tangents[a] += tangent;
			tangents[b] += tangent;
			tangents[c] += tangent;
		}
	}
	else{
		if (type != GL_TRIANGLE_STRIP)
		{
			for (GLuint i = 0; i < numVertices; i += 3) {
				Vector3 tangent = GenerateTangent(vertices[i], vertices[i + 1], vertices[i + 2], textureCoords[i], textureCoords[i + 1], textureCoords[i + 2]);

				tangents[i] += tangent;
				tangents[i + 1] += tangent;
				tangents[i + 2] += tangent;
			}
		}
		else {
			for (GLuint i = 0; i < numVertices; i += 4) {
				Vector3 tangent = GenerateTangent(vertices[i], vertices[i + 1], vertices[i + 2], textureCoords[i], textureCoords[i + 1], textureCoords[i + 2]);

				tangents[i] += tangent;
				tangents[i + 1] += tangent;
				tangents[i + 2] += tangent;
				tangents[i + 3] += tangent;
			}
		}
	}
	for(GLuint i = 0; i < numVertices; ++i){
		tangents[i].Normalise();
	}
}

Vector3 Mesh::GenerateTangent(const Vector3 &a,const Vector3 &b,const Vector3 &c,const Vector2 &ta,const Vector2 &tb,const Vector2 &tc)	 {
	Vector2 coord1  = tb-ta;
	Vector2 coord2  = tc-ta;

	Vector3 vertex1 = b-a;
	Vector3 vertex2 = c-a;
	 
	Vector3 axis2 = Vector3(vertex1 - vertex2);

	Vector3 axis = Vector3(vertex1*coord2.y - vertex2*coord1.y);

	float invDet = 1.0f / (coord1.x * coord2.y - coord2.x * coord1.y);

	return axis * invDet;
}

void Mesh::DrawDebugNormals(float length)	{
	if(numVertices > 0) {
		GLuint array;
		GLuint buffer;
		GLuint cbuffer;

		glGenVertexArrays(1, &array);
		glBindVertexArray(array);

		Vector3 *tempV = new Vector3[numVertices*2];
		Vector3 *tempC = new Vector3[numVertices*2];

		for(GLuint i = 0; i < numVertices; ++i){
			tempC[(i*2)]  = Vector3(1,0,0);
			tempC[(i*2)+1] = Vector3(1,1,1);

			tempV[(i*2)]   = vertices[i];
			tempV[(i*2)+1] = vertices[i] + (normals[i] * length);
		}

		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, numVertices*sizeof(Vector3)*2, tempV, GL_STREAM_DRAW);
		glVertexAttribPointer(VERTEX_BUFFER, 3, GL_FLOAT, GL_FALSE, 0, 0); 
		glEnableVertexAttribArray(VERTEX_BUFFER);

		glGenBuffers(1, &cbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, cbuffer);
		glBufferData(GL_ARRAY_BUFFER, numVertices*sizeof(Vector3)*2, tempC, GL_STREAM_DRAW);
		glVertexAttribPointer(COLOUR_BUFFER, 3, GL_FLOAT, GL_FALSE, 0, 0); 
		glEnableVertexAttribArray(COLOUR_BUFFER);

		glPointSize(4.0f);
		glLineWidth(2.0f);
		glDrawArrays(GL_POINTS,  0, numVertices*2);	// draw ordered list of vertices
		glDrawArrays(GL_LINES,  0, numVertices*2);	// draw ordered list of vertices
		glLineWidth(1.0f);
		glPointSize(1.0f);

		glBindVertexArray(0);

		glDeleteVertexArrays(1, &array);
		glDeleteBuffers(1, &buffer);
		glDeleteBuffers(1, &cbuffer);

		delete[] tempV;
		delete[] tempC;
	}

	//for(unsigned int i = 0; i < children.size(); ++i) {
	//	children.at(i)->DrawDebugNormals();
	//}
}

void Mesh::DrawDebugTangents(float length)	{
	if(numVertices > 0) {
		GLuint array;
		GLuint buffer;
		GLuint cbuffer;

		glGenVertexArrays(1, &array);
		glBindVertexArray(array);

		Vector3 *tempV = new Vector3[numVertices*2];
		Vector3 *tempC = new Vector3[numVertices*2];

		for(GLuint i = 0; i < numVertices; ++i){
			tempC[(i*2)]  = Vector3(1,1,1);
			tempC[(i*2)+1] = Vector3(0,1,0);

			tempV[(i*2)]   = vertices[i];
			tempV[(i*2)+1] = vertices[i] + (tangents[i] * length);
		}

		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, numVertices*sizeof(Vector3)*2, tempV, GL_STREAM_DRAW);
		glVertexAttribPointer(VERTEX_BUFFER, 3, GL_FLOAT, GL_FALSE, 0, 0); 
		glEnableVertexAttribArray(VERTEX_BUFFER);

		glGenBuffers(1, &cbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, cbuffer);
		glBufferData(GL_ARRAY_BUFFER, numVertices*sizeof(Vector3)*2, tempC, GL_STREAM_DRAW);
		glVertexAttribPointer(COLOUR_BUFFER, 3, GL_FLOAT, GL_FALSE, 0, 0); 
		glEnableVertexAttribArray(COLOUR_BUFFER);

		glPointSize(4.0f);
		glLineWidth(2.0f);
		glDrawArrays(GL_POINTS,  0, numVertices*2);	// draw ordered list of vertices
		glDrawArrays(GL_LINES,  0, numVertices*2);	// draw ordered list of vertices
		glLineWidth(1.0f);
		glPointSize(1.0f);

		glBindVertexArray(0);

		glDeleteVertexArrays(1, &array);
		glDeleteBuffers(1, &buffer);
		glDeleteBuffers(1, &cbuffer);

		delete[] tempV;
		delete[] tempC;
	}

	//for(unsigned int i = 0; i < children.size(); ++i) {
	//	children.at(i)->DrawDebugTangents();
	//}
}