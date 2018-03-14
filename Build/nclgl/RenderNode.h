/******************************************************************************
Class:RenderNode
Implements:
Author:Rich Davison	<richard.davison4@newcastle.ac.uk>
Description:A basic, but functional RenderNode class. SceneNodes have a
transformation matrix, and any number of children. They also have a parent, 
forming a tree structure, with their parent above them, and children below.

They also have a seperate scale for their Mesh, for no other reason than
it made the CubeRobot class easier ;) You might find it useful, though...

-_-_-_-_-_-_-_,------,   
_-_-_-_-_-_-_-|   /\_/\   NYANYANYAN
-_-_-_-_-_-_-~|__( ^ .^) /
_-_-_-_-_-_-_-""  ""   

*//////////////////////////////////////////////////////////////////////////////


#pragma once
#include "Matrix4.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Mesh.h"
#include <vector>

class Material;

class RenderNode	{
public:
	 RenderNode(Mesh*m = NULL, string name = "RenderNode", Vector4 colour = Vector4(1,1,1,1));
	virtual ~RenderNode(void);

	virtual bool IsRenderable()
	{
		return (this->mesh != NULL);
	}

	virtual void DrawOpenGL(bool isShadowPass, Material* tempMat = nullptr);


	void			SetTransform(const Matrix4 &matrix) { transform = matrix;}
	const Matrix4&	GetTransform() const				{ return transform;}
	const Matrix4&	GetWorldTransform() const			{ return worldTransform;}

	virtual void	Update(float msec);

	const Vector4&	GetColor()		const				{return color;}
	void			SetColor(const Vector4 &c)			{color = c;}

	const Vector4&	GetBaseColor()const { return baseColor; }
	void			SetBaseColor(const Vector4 &c) { baseColor = c; }

	RenderNode *	GetChild() {
		if (!children.empty()) { return children[0]; }
		else { return nullptr; }
	}

	const Vector4&  GetChildColor()	const { return children[0]->color; }
	void			SetChildColor(const Vector4 &c) { children[0]->color = c; }

	const Vector4&	GetchildBaseColor() const { return children[0]->baseColor; }
	void			SetChildBaseColor(const Vector4 &c) { children[0]->baseColor = c; }

	const Vector3&	GetModelScale()		const			{return modelScale;}
	void			SetModelScale(const Vector3 &s)		{modelScale = s;}

	void			AddChild(RenderNode* s);
	bool			RemoveChild(RenderNode* s,bool recursive = true);

	float			GetBoundingRadius() const	{return boundingRadius;}
	void			SetBoundingRadius(float f)	{boundingRadius = f;}
	void			AutoSetBoundingRadius();

	float			GetCameraDistance() const	{return distanceFromCamera;}
	void			SetCameraDistance(float f)	{distanceFromCamera = f;}

	Material*		GetMaterial()const			{ return material; }
	void			SetMaterial(Material* mat, bool isSetChild = false);

	void			SetMesh(Mesh*m)				{mesh = m;}
	Mesh*			GetMesh()					{return mesh;}

	void			SetCulling(bool b)			{ cullFaces = b; }
	bool			IsCulling()					{ return cullFaces; }

	void			SetHasShadow(bool b)		{ hasShadow = b; }
	bool			HasShadow()					{ return hasShadow; }

	RenderNode*		GetChildWithName(string s);

	RenderNode* GetParent() { return parent; }

	void			SetName(string s) { name = s; }
	string			GetName() { return name; }

	bool	IsAwake()	{return awake;}
	void	Wake()		{awake = true;}
	void	Sleep()		{awake = false;} 

	std::vector<RenderNode*>::const_iterator GetChildIteratorStart() { return children.begin(); }
	std::vector<RenderNode*>::const_iterator GetChildIteratorEnd()	{ return children.end(); }

	static bool		CompareByCameraDistance(RenderNode*a,RenderNode*b) ;
	static bool		CompareByZ(RenderNode*a,RenderNode*b) ;

	void	 SetColliding(bool b) {
		isCollided = b;
		for (vector<RenderNode*>::iterator i = children.begin(); i != children.end(); ++i) {
			(*i)->SetColliding(b);
		}
	}
	bool 	 IsCollide() { return isCollided; }

	float GetSmoothness() { return smoothness; }
	void SetSmoothness(float smoothness) { this->smoothness = smoothness; }

	GLuint GetTexture() { return texture; }
	void SetTexture(int tex) { texture = tex; }

protected:

	void RecursiveSetMaterial(Material* mat, RenderNode* renderNode);

	Matrix4		worldTransform;
	Matrix4		transform;
	RenderNode*	parent;
	float		distanceFromCamera;
	float		boundingRadius;
	Vector4		color;
	Vector4		baseColor;
	Vector4		collideColor;
	Vector3		modelScale;
	Material*	material;
	Mesh*		mesh;
	bool		awake;
	bool		isCollided = false;
	bool		cullFaces = true;
	bool		hasShadow = true;
	string		name;
	GLuint		texture;
	std::vector<RenderNode*>		children;

protected :
	float smoothness;
};

