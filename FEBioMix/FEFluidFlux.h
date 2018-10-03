#pragma once
#include <FECore/FESurfaceLoad.h>
#include <FECore/FEModelParam.h>

//-----------------------------------------------------------------------------
//! This boundary condition sustains a fluid flux on a surface
//!
class FEFluidFlux : public FESurfaceLoad
{
public:
	//! constructor
	FEFluidFlux(FEModel* pfem);

	void SetLinear(bool blinear) { m_blinear = blinear; }

	void SetMixture(bool bmix) { m_bmixture = bmix; }

	//! Set the surface to apply the load to
	void SetSurface(FESurface* ps) override;

	//! calculate flux stiffness
	void StiffnessMatrix(const FETimeInfo& tp, FESolver* psolver) override;

	//! calculate residual
	void Residual(const FETimeInfo& tp, FEGlobalVector& R) override;

	//! unpack LM data
	void UnpackLM(FEElement& el, vector<int>& lm);

protected:
	//! calculate stiffness for an element
	void FluxStiffness(FESurfaceElement& el, matrix& ke, double dt, bool mixture);

	//! calculate stiffness for an element, for steady-state analysis
	void FluxStiffnessSS(FESurfaceElement& el, matrix& ke, double dt, bool mixture);

	//! Calculates volumetric flow rate due to flux
	bool FlowRate(FESurfaceElement& el, vector<double>& fe, double dt, bool mixture);

	//! Calculates volumetric flow rate due to flux, for steady-state analysis
	bool FlowRateSS(FESurfaceElement& el, vector<double>& fe, double dt, bool mixture);

	//! Calculates the linear volumetric flow rate due to flux (ie. non-follower)
	bool LinearFlowRate(FESurfaceElement& el, vector<double>& fe, double dt, bool mixture);

	//! Calculates the linear volumetric flow rate due to flux (ie. non-follower), for steady-state analysis
	bool LinearFlowRateSS(FESurfaceElement& el, vector<double>& fe, double dt, bool mixture);

protected:
	FEParamDouble	m_flux;			//!< fluid flux
	bool	m_bmixture;		//!< mixture velocity or relative fluid flux
    bool    m_bshellb;      //!< flag for prescribing flux on shell bottom
	bool	m_blinear;		//!< type (linear or nonlinear)

	// degrees of freedom
	// (TODO: find a better way of defining this. 
	//        I don't want to have to do this in each class)
	int	m_dofX;
	int	m_dofY;
	int	m_dofZ;
	int	m_dofP;
	int	m_dofVX;
	int	m_dofVY;
	int	m_dofVZ;
    int	m_dofSX;
    int	m_dofSY;
    int	m_dofSZ;
    int	m_dofQ;
    int	m_dofSVX;
    int	m_dofSVY;
    int	m_dofSVZ;

	DECLARE_FECORE_CLASS();
};
