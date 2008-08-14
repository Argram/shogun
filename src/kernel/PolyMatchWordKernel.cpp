/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Written (W) 1999-2008 Soeren Sonnenburg
 * Copyright (C) 1999-2008 Fraunhofer Institute FIRST and Max-Planck-Society
 */

#include "lib/common.h"
#include "lib/io.h"
#include "kernel/PolyMatchWordKernel.h"
#include "features/Features.h"
#include "features/WordFeatures.h"

CPolyMatchWordKernel::CPolyMatchWordKernel(INT size, INT d, bool i, bool un)
: CSimpleKernel<WORD>(size),degree(d),inhomogene(i),use_normalization(un),
	sqrtdiag_lhs(NULL), sqrtdiag_rhs(NULL), initialized(false)
{
}

CPolyMatchWordKernel::CPolyMatchWordKernel(
	CWordFeatures* l, CWordFeatures* r, INT d, bool i, bool un)
: CSimpleKernel<WORD>(10),degree(d),inhomogene(i), use_normalization(un),
	sqrtdiag_lhs(NULL), sqrtdiag_rhs(NULL), initialized(false)
{
	init(l, r);
}

CPolyMatchWordKernel::~CPolyMatchWordKernel()
{
	cleanup();
}

bool CPolyMatchWordKernel::init(CFeatures* l, CFeatures* r)
{
	bool result=CSimpleKernel<WORD>::init(l,r);

	initialized = false ;
	INT i;

	if (sqrtdiag_lhs != sqrtdiag_rhs)
		delete[] sqrtdiag_rhs;
	sqrtdiag_rhs=NULL ;
	delete[] sqrtdiag_lhs;
	sqrtdiag_lhs=NULL ;

	if (use_normalization)
	{
		sqrtdiag_lhs= new DREAL[lhs->get_num_vectors()];

		for (i=0; i<lhs->get_num_vectors(); i++)
			sqrtdiag_lhs[i]=1;

		if (l==r)
			sqrtdiag_rhs=sqrtdiag_lhs;
		else
		{
			sqrtdiag_rhs= new DREAL[rhs->get_num_vectors()];
			for (i=0; i<rhs->get_num_vectors(); i++)
				sqrtdiag_rhs[i]=1;
		}

		ASSERT(sqrtdiag_lhs);
		ASSERT(sqrtdiag_rhs);

		this->lhs=(CWordFeatures*) l;
		this->rhs=(CWordFeatures*) l;

		//compute normalize to 1 values
		for (i=0; i<lhs->get_num_vectors(); i++)
		{
			sqrtdiag_lhs[i]=sqrt(compute(i,i));

			//trap divide by zero exception
			if (sqrtdiag_lhs[i]==0)
				sqrtdiag_lhs[i]=1e-16;
		}

		// if lhs is different from rhs (train/test data)
		// compute also the normalization for rhs
		if (sqrtdiag_lhs!=sqrtdiag_rhs)
		{
			this->lhs=(CWordFeatures*) r;
			this->rhs=(CWordFeatures*) r;

			//compute normalize to 1 values
			for (i=0; i<rhs->get_num_vectors(); i++)
			{
				sqrtdiag_rhs[i]=sqrt(compute(i,i));

				//trap divide by zero exception
				if (sqrtdiag_rhs[i]==0)
					sqrtdiag_rhs[i]=1e-16;
			}
		}
	}

	this->lhs=(CWordFeatures*) l;
	this->rhs=(CWordFeatures*) r;

	initialized = true ;
	return result;
}

void CPolyMatchWordKernel::cleanup()
{
	if (sqrtdiag_lhs != sqrtdiag_rhs)
		delete[] sqrtdiag_rhs;
	sqrtdiag_rhs=NULL;

	delete[] sqrtdiag_lhs;
	sqrtdiag_lhs=NULL;

	initialized=false;

	CKernel::cleanup();
}

bool CPolyMatchWordKernel::load_init(FILE* src)
{
	return false;
}

bool CPolyMatchWordKernel::save_init(FILE* dest)
{
	return false;
}

DREAL CPolyMatchWordKernel::compute(INT idx_a, INT idx_b)
{
	INT alen, blen;
	bool afree, bfree;

	//fprintf(stderr, "LinKernel.compute(%ld,%ld)\n", idx_a, idx_b) ;
	WORD* avec=((CWordFeatures*) lhs)->get_feature_vector(idx_a, alen, afree);
	WORD* bvec=((CWordFeatures*) rhs)->get_feature_vector(idx_b, blen, bfree);

	ASSERT(alen==blen);

	DREAL sqrt_a= 1 ;
	DREAL sqrt_b= 1 ;

	if (initialized && use_normalization)
	{
		sqrt_a=sqrtdiag_lhs[idx_a] ;
		sqrt_b=sqrtdiag_rhs[idx_b] ;
	} ;

	DREAL sqrt_both=sqrt_a*sqrt_b;

	INT sum=0;
	{
		for (INT i=0; i<alen; i++)
			sum+= (avec[i]==bvec[i]) ? 1 : 0;
	}

	if (inhomogene)
		sum+=1;

	DREAL result=sum;

	for (INT j=1; j<degree; j++)
		result*=sum;

	result/=sqrt_both;

	((CWordFeatures*) lhs)->free_feature_vector(avec, idx_a, afree);
	((CWordFeatures*) rhs)->free_feature_vector(bvec, idx_b, bfree);

	return result;
}
