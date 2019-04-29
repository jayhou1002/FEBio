/*This file is part of the FEBio source code and is licensed under the MIT license
listed below.

See Copyright-FEBio.txt for details.

Copyright (c) 2019 University of Utah, The Trustees of Columbia University in 
the City of New York, and others.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.*/



#pragma once
#include "FEScalarValuator.h"
#include "FEVec3dValuator.h"
#include "FEMat3dValuator.h"
#include "FEItemList.h"

//---------------------------------------------------------------------------------------
// Base for model parameters.
class FECORE_API FEModelParam
{
public:
	FEModelParam();
	virtual ~FEModelParam();

	// set the domain
	void SetItemList(FEItemList* itemList) { m_dom = itemList; }

	// get the domain list
	FEItemList* GetItemList() { return m_dom;  }

	// set the scale factor
	void SetScaleFactor(double s) { m_scl = s; }

	// serialization
	virtual void Serialize(DumpStream& ar);

protected:
	double			m_scl;	//!< scale factor. Used to store load curve value
	FEItemList*		m_dom;
};

//---------------------------------------------------------------------------------------
class FECORE_API FEParamDouble : public FEModelParam
{
public:
	FEParamDouble();

	FEParamDouble(const FEParamDouble& p);

	// set the value
	void operator = (double v);

	// set the valuator
	void setValuator(FEScalarValuator* val);

	// evaluate the parameter at a material point
	double operator () (const FEMaterialPoint& pt) { return m_scl*(*m_val)(pt); }

	// is this a const value
	bool isConst() const;

	// get the const value (returns 0 if param is not const)
	double& constValue();
	double constValue() const;

	void Serialize(DumpStream& ar) override;

private:
	FEScalarValuator*	m_val;
};

//=======================================================================================

//---------------------------------------------------------------------------------------
class FECORE_API FEParamVec3 : public FEModelParam
{
public:
	FEParamVec3();

	FEParamVec3(const FEParamVec3& p);

	// set the value
	void operator = (const vec3d& v);

	// set the valuator
	void setValuator(FEVec3dValuator* val);

	// evaluate the parameter at a material point
	vec3d operator () (const FEMaterialPoint& pt) { return (*m_val)(pt)*m_scl; }

	// return a unit vector
	vec3d unitVector(const FEMaterialPoint& pt) { return (*this)(pt).normalized(); }

	// is this a const
	bool isConst() const { return m_val->isConst(); }

	vec3d& constValue() { return *m_val->constValue(); };

	void Serialize(DumpStream& ar) override;

private:
	FEVec3dValuator*	m_val;
};

//=======================================================================================

//---------------------------------------------------------------------------------------
class FECORE_API FEParamMat3d : public FEModelParam
{
public:
	FEParamMat3d();

	FEParamMat3d(const FEParamMat3d& p);

	// set the value
	void operator = (const mat3d& v);

	// set the valuator
	void setValuator(FEMat3dValuator* val);

	// evaluate the parameter at a material point
	mat3d operator () (const FEMaterialPoint& pt) { return (*m_val)(pt)*m_scl; }

	// is this a const
	bool isConst() const { return m_val->isConst(); }

	mat3d& constValue() { return *m_val->constValue(); };

	void Serialize(DumpStream& ar) override;

private:
	FEMat3dValuator*	m_val;
};
