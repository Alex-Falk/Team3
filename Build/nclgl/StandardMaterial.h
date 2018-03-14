
/*****************************************************************************
:;-"""-::::::::::::::::::::::::::::::::::::::::::::::::.::..:... ..  .
(      .)::::::::::::::::::::::::::::::::::::::::::::.::..:... ..  .
        )-:::::::::::::::::::::::::::::::::::::::::::::::.::..:... .. .
      -'   )-"-:::::::::::::::::::::::::::::::::::::::.::..:... ..  .
 ___________)______________          _____
|                         \          \ U \__      _______
|        Yesheng Su        \__________\   \/_______\___\_____________
|        10/02/2018        /          < /_/   ..................... ^`-._
|_________________________/            `-----------,----,--------------'
                      )                          _/____/_
-.                .    ):::::::::::::::::::::::::::::.::..:... ..  .
  )--.__..--"-.__,'---':::::::::::::::::::::::::::::::::.::..:... ..  .
-':::::::::::::::::::::::::::::::::::::::::::::.::..:... ..  .
:::::::::::::::::::::::::::::::::::::::::::::::::.::..:... ..  .
															
*****************************************************************************/
#pragma once
#include "Material.h"

class StandardMaterial : public Material
{
public:
	virtual	bool Apply();
};

class DrawPathMaterial : public Material
{
public:
	void SetProjViewMtx(const Matrix4& _projViewMtx) { projViewMtx = _projViewMtx; }
	virtual	bool Apply();

protected :
	Matrix4 projViewMtx;
};

class GroundMaterial : public Material
{
public:
	virtual	bool Apply();
};

class BasicMaterial : public Material
{
public:
	virtual bool Apply();
};

class ChangeColorMaterial : public Material
{
public :
	virtual bool Apply();
};