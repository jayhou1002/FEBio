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
#include "FEBiphasicSolute.h"

//-----------------------------------------------------------------------------
// This class implements a material that has a constant solute supply

class FEBIOMIX_API FESupplyConst :	public FESoluteSupply
{
public:
	//! constructor
	FESupplyConst(FEModel* pfem);
	
	//! Solute supply
	double Supply(FEMaterialPoint& pt) override;
	
	//! Tangent of supply with respect to strain
	double Tangent_Supply_Strain(FEMaterialPoint& mp) override;
	
	//! Tangent of supply with respect to concentration
	double Tangent_Supply_Concentration(FEMaterialPoint& mp) override;
	
	//! receptor-ligand complex supply
	double ReceptorLigandSupply(FEMaterialPoint& mp) override;
	
	//! Solute supply at steady-state
	double SupplySS(FEMaterialPoint& pt) override;
	
	//! receptor-ligand complex concentration at steady-state
	double ReceptorLigandConcentrationSS(FEMaterialPoint& mp) override;

	//! referential solid supply
	double SolidSupply(FEMaterialPoint& pt) override;
	
	//! referential solid volume fraction under steady-state conditions
	double SolidConcentrationSS(FEMaterialPoint& pt) override;

public:
	double	m_supp;			//!< solute supply
	
	// declare parameter list
	DECLARE_FECORE_CLASS();
};
