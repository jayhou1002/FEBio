// FEIncompNeoHookean.h: interface for the FEIncompNeoHookean class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FEINCOMPNEOHOOKEAN_H__8ECAD0BE_54FA_4924_9952_68EA377A8D8E__INCLUDED_)
#define AFX_FEINCOMPNEOHOOKEAN_H__8ECAD0BE_54FA_4924_9952_68EA377A8D8E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FEUncoupledMaterial.h"

//-----------------------------------------------------------------------------
//! Incompressible Neo-Hookean material

class FEIncompNeoHookean : public FEUncoupledMaterial
{
public:
	FEIncompNeoHookean(FEModel* pfem) : FEUncoupledMaterial(pfem) {}

public:
	double	m_G;	//!< Shear modulus

public:
	//! calculate deviatoric stress at material point
	mat3ds DevStress(FEMaterialPoint& pt) override;

	//! calculate deviatoric tangent stiffness at material point
	tens4ds DevTangent(FEMaterialPoint& pt) override;

	// declare the parameter list
	DECLARE_FECORE_CLASS();
};

#endif // !defined(AFX_FEINCOMPNEOHOOKEAN_H__8ECAD0BE_54FA_4924_9952_68EA377A8D8E__INCLUDED_)
