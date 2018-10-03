//
//  FEFiberIntegrationGeodesic.cpp
//  FEBioXCode4
//
//  Created by Gerard Ateshian on 11/19/13.
//  Copyright (c) 2013 Columbia University. All rights reserved.
//

#include "FEFiberIntegrationGeodesic.h"

#ifndef SQR
#define SQR(x) ((x)*(x))
#endif

class FEFiberIntegrationGeodesic::Iterator : public FEFiberIntegrationSchemeIterator
{
public:
	Iterator(int nint, const double* cth, const double* cph, const double* sth, const double* sph, const double* wn)
	{
		m_nint = nint;
		m_cth = cth;
		m_cph = cph;
		m_sth = sth;
		m_sph = sph;
		m_wn = wn;

		n = -1;
		Next();
	}

	bool IsValid()
	{
		return (n < m_nint);
	}

	// move to the next integration point
	bool Next()
	{
		n++;
		if (n < m_nint)
		{
			m_fiber.x = m_cth[n] * m_sph[n];
			m_fiber.y = m_sth[n] * m_sph[n];
			m_fiber.z = m_cph[n];

			m_weight = m_wn[n];

			return true;
		}
		else return false;
	}

public:
	int		n;
	int		m_nint;
	const double* m_cth;
	const double* m_cph;
	const double* m_sth;
	const double* m_sph;
	const double* m_wn;
};

//-----------------------------------------------------------------------------
// FEFiberIntegrationGeodesic
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// define the material parameters
BEGIN_FECORE_CLASS(FEFiberIntegrationGeodesic, FEFiberIntegrationScheme)
	ADD_PARAMETER(m_nres, "resolution");
END_FECORE_CLASS();

//-----------------------------------------------------------------------------
void FEFiberIntegrationGeodesic::Serialize(DumpStream& ar)
{
	FEFiberIntegrationScheme::Serialize(ar);
	if (ar.IsSaving() == false)
	{
		InitIntegrationRule();
	}
}

//-----------------------------------------------------------------------------
FEFiberIntegrationGeodesic::FEFiberIntegrationGeodesic(FEModel* pfem) : FEFiberIntegrationScheme(pfem)
{ 
	m_nres = 0; 
}

//-----------------------------------------------------------------------------
FEFiberIntegrationGeodesic::~FEFiberIntegrationGeodesic()
{
}

//-----------------------------------------------------------------------------
bool FEFiberIntegrationGeodesic::Init()
{
	if ((m_nres != 0) && (m_nres != 1)) return MaterialError("resolution must be 0 (low) or 1 (high).");
    
	// initialize integration rule data
	InitIntegrationRule();
		
    // also initialize the parent class
    return FEFiberIntegrationScheme::Init();
}

//-----------------------------------------------------------------------------
void FEFiberIntegrationGeodesic::InitIntegrationRule()
{
	// select the integration rule
	m_nint = (m_nres == 0? NSTL  : NSTH  );
	const double* phi = (m_nres == 0? PHIL  : PHIH  );
	const double* the = (m_nres == 0? THETAL: THETAH);
	const double* w   = (m_nres == 0? AREAL : AREAH );
		
	for (int n=0; n<m_nint; ++n)
	{
		m_cth[n] = cos(the[n]);
		m_sth[n] = sin(the[n]);
		m_cph[n] = cos(phi[n]);
		m_sph[n] = sin(phi[n]);
		m_w[n] = w[n];
	}
}

//-----------------------------------------------------------------------------
FEFiberIntegrationSchemeIterator* FEFiberIntegrationGeodesic::GetIterator(FEMaterialPoint* mp)
{
	return new Iterator(m_nint, &m_cth[0], &m_cph[0], &m_sth[0], &m_sph[0], &m_w[0]);
}

/*
//-----------------------------------------------------------------------------
mat3ds FEFiberIntegrationGeodesic::Stress(FEMaterialPoint& mp)
{
	FEElasticMaterialPoint& pt = *mp.ExtractData<FEElasticMaterialPoint>();
	
	// get the element's local coordinate system
	mat3d QT = (pt.m_Q).transpose();
    
	// loop over all integration points
	double R;
	vec3d n0e, n0a;
	mat3ds s;
	s.zero();
    
	for (int n=0; n<m_nint; ++n)
	{
		// set the global fiber direction in reference configuration
		n0e.x = m_cth[n]*m_sph[n];
		n0e.y = m_sth[n]*m_sph[n];
		n0e.z = m_cph[n];
        m_pFmat->SetFiberDirection(mp, n0e);
        
        // get the local material fiber direction in reference configuration
        n0a = QT*n0e;
        // evaluate the fiber density
        R = m_pFDD->FiberDensity(n0a);
        
        // evaluate this fiber's contribution to the stress
		s += m_pFmat->Stress(mp)*(R*m_w[n]);
	}
	return s;
}

//-----------------------------------------------------------------------------
tens4ds FEFiberIntegrationGeodesic::Tangent(FEMaterialPoint& mp)
{
	FEElasticMaterialPoint& pt = *mp.ExtractData<FEElasticMaterialPoint>();
	
	// get the element's local coordinate system
	mat3d QT = (pt.m_Q).transpose();
    
	// loop over all integration points
	double R;
	vec3d n0e, n0a;
	tens4ds c;
	c.zero();
	
	for (int n=0; n<m_nint; ++n)
	{
		// set the global fiber direction in reference configuration
		n0e.x = m_cth[n]*m_sph[n];
		n0e.y = m_sth[n]*m_sph[n];
		n0e.z = m_cph[n];
        m_pFmat->SetFiberDirection(mp, n0e);
        
        // get the local material fiber direction in reference configuration
        n0a = QT*n0e;
        // evaluate the fiber density
        R = m_pFDD->FiberDensity(n0a);
        
        // evaluate this fiber's contribution to the tangent
		c += m_pFmat->Tangent(mp)*(R*m_w[n]);
	}
	
	return c;
}

//-----------------------------------------------------------------------------
double FEFiberIntegrationGeodesic::StrainEnergyDensity(FEMaterialPoint& mp)
{
	FEElasticMaterialPoint& pt = *mp.ExtractData<FEElasticMaterialPoint>();
	
	// get the element's local coordinate system
	mat3d QT = (pt.m_Q).transpose();
    
	// loop over all integration points
	double R;
	vec3d n0e, n0a;
	double sed = 0.0;
    
	for (int n=0; n<m_nint; ++n)
	{
		// set the global fiber direction in reference configuration
		n0e.x = m_cth[n]*m_sph[n];
		n0e.y = m_sth[n]*m_sph[n];
		n0e.z = m_cph[n];
        m_pFmat->SetFiberDirection(mp, n0e);
        
        // get the local material fiber direction in reference configuration
        n0a = QT*n0e;
        // evaluate the fiber density
        R = m_pFDD->FiberDensity(n0a);
        
        // evaluate this fiber's contribution to the stress
		sed += m_pFmat->StrainEnergyDensity(mp)*(R*m_w[n]);
	}
	return sed;
}

//-----------------------------------------------------------------------------
double FEFiberIntegrationGeodesic::IntegratedFiberDensity()
{
    // initialize integrated fiber density distribution
    double IFD = 1;
    
	// loop over all integration points
	double R;
	vec3d n0a;
    double C = 0;
    
	for (int n=0; n<m_nint; ++n)
	{
		// set the global fiber direction in reference configuration
		n0a.x = m_cth[n]*m_sph[n];
		n0a.y = m_sth[n]*m_sph[n];
		n0a.z = m_cph[n];
        
        // evaluate the fiber density
        R = m_pFDD->FiberDensity(n0a);
        
        // integrate the fiber density
		C += R*m_w[n];
	}
	IFD = C;
    return IFD;
}
*/
