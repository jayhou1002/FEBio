/*This file is part of the FEBio source code and is licensed under the MIT license
listed below.

See Copyright-FEBio.txt for details.

Copyright (c) 2020 University of Utah, The Trustees of Columbia University in 
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
#include "FERefineMesh.h"
#include "DumpMemStream.h"

class TetMesh;
class tetgenio;

class FECORE_API FETetgenRefine : public FERefineMesh
{
public:
	FETetgenRefine(FEModel* fem);

	bool Apply(int iteration) override;

protected:
	bool DoTetRefinement(FEModel& fem);
	bool DoTetCoarsening(FEModel& fem);
	bool ReconstructMesh(FEModel& fem);

private:
	double	m_scale;			// element scale factor
	double	m_q;				// quality criterion
	double	m_tol;				// tolerance
	bool	m_splitFaces;		// split surface facets
	int		m_maxiter;			// max iterations
	int		m_maxelem;			// max elements
	bool	m_resetMesh;		// reset mesh after each refinement
	bool	m_bcoarsen;			// do coarsening before refinement
	double	m_coarsenLength;	// edge length for coarsening
	double	m_min_h;			// minimum element size
	int		m_nfeather;			// feather

	FEMeshAdaptorCriterion*	m_criterion;
	vector<pair<int, double> >	m_elemList;

private:
	tetgenio*		m_oldMesh;	// old mesh

	DECLARE_FECORE_CLASS();
};
