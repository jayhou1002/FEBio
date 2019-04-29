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



#include "stdafx.h"
#include "FEScalarValuator.h"
#include "FEMaterialPoint.h"
#include "FEModelParam.h"
#include "FEModel.h"
#include "DumpStream.h"

REGISTER_SUPER_CLASS(FEScalarValuator, FESCALARGENERATOR_ID);

//=============================================================================
BEGIN_FECORE_CLASS(FEConstValue, FEScalarValuator)
	ADD_PARAMETER(m_val, "const");
END_FECORE_CLASS();

//=============================================================================

BEGIN_FECORE_CLASS(FEMathValue, FEScalarValuator)
	ADD_PARAMETER(m_expr, "math");
END_FECORE_CLASS();

void FEMathValue::setMathString(const std::string& s)
{
	m_expr = s;
}

bool FEMathValue::Init()
{
	return create();
}

void FEMathValue::Serialize(DumpStream& ar)
{
	FEScalarValuator::Serialize(ar);
	if (ar.IsShallow()) return;
	if (ar.IsLoading()) create();
}

bool FEMathValue::create(FECoreBase* pc)
{
	m_math.AddVariable("X");
	m_math.AddVariable("Y");
	m_math.AddVariable("Z");
	bool b = m_math.Create(m_expr, true);

	// lookup all the other variables.
	if (m_math.Variables() > 3)
	{
		if (pc == nullptr)
		{
			// try to find the owner of this parameter
			// First, we need the model parameter
			FEModelParam* param = GetModelParam();
			if (param == nullptr) return false;

			// we'll need the model for this
			FEModel* fem = GetFEModel();
			if (fem == nullptr) return false;

			// Now try to find the owner of this parameter
			pc = fem->FindParameterOwner(param);
			if (pc == nullptr) return false;
		}

		for (int i = 3; i < m_math.Variables(); ++i)
		{
			MVariable* vari = m_math.Variable(i);

			ParamString ps(vari->Name().c_str());
			FEParam* p = pc->FindParameter(ps);
			assert(p);
			assert((p->type() == FE_PARAM_DOUBLE_MAPPED) || (p->type() == FE_PARAM_DOUBLE) || (p->type() == FE_PARAM_INT));

			m_vars.push_back(p);
		}
	}

	assert(b);
	return b;
}

FEMathValue::~FEMathValue()
{
}

FEScalarValuator* FEMathValue::copy()
{
	FEMathValue* newExpr = new FEMathValue(GetFEModel());
	newExpr->m_expr = m_expr;
	newExpr->m_math = m_math;
	newExpr->m_vars = m_vars;
	return newExpr;
}

double FEMathValue::operator()(const FEMaterialPoint& pt)
{
	std::vector<double> var(3 + m_vars.size());
	var[0] = pt.m_r0.x;
	var[1] = pt.m_r0.y;
	var[2] = pt.m_r0.z;
	if (m_vars.empty() == false)
	{
		for (int i = 0; i < (int)m_vars.size(); ++i)
		{
			FEParam* pi = m_vars[i];
			switch (pi->type())
			{
			case FE_PARAM_INT: var[3 + i] = (double)pi->value<int>(); break;
			case FE_PARAM_DOUBLE: var[3 + i] = pi->value<double>(); break;
			case FE_PARAM_DOUBLE_MAPPED: var[3 + i] = pi->value<FEParamDouble>()(pt); break;
			}
		}
	}
	return m_math.value_s(var);
}

//---------------------------------------------------------------------------------------

FEMappedValue::FEMappedValue(FEModel* fem) : FEScalarValuator(fem), m_val(nullptr), m_scale(0.0)
{
}

void FEMappedValue::setDataMap(FEDataMap* val, double scl)
{
	m_val = val;
	m_scale = scl;
}

double FEMappedValue::operator()(const FEMaterialPoint& pt)
{
	return m_scale*m_val->value(pt);
}

FEScalarValuator* FEMappedValue::copy()
{
	FEMappedValue* map = new FEMappedValue(GetFEModel());
	map->setDataMap(m_val, m_scale);
	return map;
}

//---------------------------------------------------------------------------------------

FENodeMappedValue::FENodeMappedValue(FEModel* fem) : FEScalarValuator(fem), m_val(nullptr), m_scale(0.0)
{

}

void FENodeMappedValue::setDataMap(FENodeDataMap* val, double scale)
{
	m_val = val;
	m_scale = scale;
}

double FENodeMappedValue::operator()(const FEMaterialPoint& pt)
{
	return m_scale*m_val->getValue(pt.m_index);
}

FEScalarValuator* FENodeMappedValue::copy()
{
	FENodeMappedValue* map = new FENodeMappedValue(GetFEModel());
	map->setDataMap(m_val, m_scale);
	return map;
}
