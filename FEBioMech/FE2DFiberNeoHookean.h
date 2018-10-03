#pragma once
#include "FEElasticMaterial.h"

//-----------------------------------------------------------------------------
class FE2DFiberNeoHookean :	public FEElasticMaterial
{
	enum { NSTEPS = 12 };	// nr of integration steps

public:
	FE2DFiberNeoHookean(FEModel* pfem);

public:
	FEParamDouble	m_E;	//!< Young's modulus
	FEParamDouble	m_v;	//!< Poisson's ratio

	//--- active contraction stuff ---
	double	m_a[2];
	double	m_ac;
	// -------------------------------

public:
	//! calculate stress at material point
	virtual mat3ds Stress(FEMaterialPoint& pt) override;

	//! calculate tangent stiffness at material point
	virtual tens4ds Tangent(FEMaterialPoint& pt) override;

	// declare the parameter list
	DECLARE_FECORE_CLASS();

protected:
	static double	m_cth[NSTEPS];
	static double	m_sth[NSTEPS];
};
