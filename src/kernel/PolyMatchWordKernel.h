/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Written (W) 1999-2008 Soeren Sonnenburg
 * Copyright (C) 1999-2008 Fraunhofer Institute FIRST and Max-Planck-Society
 */

#ifndef _POLYMATCHWORDKERNEL_H___
#define _POLYMATCHWORDKERNEL_H___

#include "lib/common.h"
#include "kernel/SimpleKernel.h"
#include "features/WordFeatures.h"

/** The class PolyMatchWordKernel computes a variant of the polynomial kernel
 * on word-features (makes sense for strings of same length mapped to word
 * features). It is computed as
 * \f[
 * k({\bf x},{\bf x'})= \sum_{i=0}^L I(x_i=x'_i}+c)^d
 * \f]
 *
 * where I is the indicator function which evaluates to 1 if its argument is
 * true and to 0 otherwise.
 *
 * Note that additional normalisation is applied, i.e.
 * \f[
 *     k'({\bf x}, {\bf x'})=\frac{k({\bf x}, {\bf x'})}{\sqrt{k({\bf x}, {\bf x})k({\bf x'}, {\bf x'})}}
 * \f]
 */
class CPolyMatchWordKernel: public CSimpleKernel<WORD>
{
	public:
		/** constructor
		 *
		 * @param size cache size
		 * @param degree degree
		 * @param inhomogene is inhomogeneous
		 * @param use_normalization use normalization
		 */
		CPolyMatchWordKernel(INT size, INT degree, bool inhomogene,
			bool use_normalization=true);

		/** constructor
		 *
		 * @param l features of left-hand side
		 * @param r features of right-hand side
		 * @param degree degree
		 * @param inhomogene is inhomogeneous
		 * @param use_normalization use normalization
		 */
		CPolyMatchWordKernel(CWordFeatures* l, CWordFeatures* r,
			INT degree, bool inhomogene, bool use_normalization=true);

		virtual ~CPolyMatchWordKernel();

		/** initialize kernel
		 *
		 * @param l features of left-hand side
		 * @param r features of right-hand side
		 * @return if initializing was successful
		 */
		virtual bool init(CFeatures* l, CFeatures* r);

		/** clean up kernel */
		virtual void cleanup();

		/** load kernel init_data
		 *
		 * @param src file to load from
		 * @return if loading was successful
		 */
		virtual bool load_init(FILE* src);

		/** save kernel init_data
		 *
		 * @param dest file to save to
		 * @return if saving was successful
		 */
		virtual bool save_init(FILE* dest);

		/** return what type of kernel we are
		 *
		 * @return kernel type POLYMATCH
		 */
		virtual EKernelType get_kernel_type() { return K_POLYMATCH; }

		/** return the kernel's name
		 *
		 * @return name PolyMatchWord
		 */
		virtual const CHAR* get_name() { return "PolyMatchWord"; }

	protected:
		/** compute kernel function for features a and b
		 * idx_{a,b} denote the index of the feature vectors
		 * in the corresponding feature object
		 *
		 * @param idx_a index a
		 * @param idx_b index b
		 * @return computed kernel function at indices a,b
		 */
		virtual DREAL compute(INT idx_a, INT idx_b);

	protected:
		/** degree */
		INT degree;
		/** if kernel is inhomogeneous */
		bool inhomogene;
		/** if normalization is used */
		bool use_normalization;

		/** sqrt diagonal of left-hand side */
		DREAL *sqrtdiag_lhs;
		/** sqrt diagonal of right-hand side */
		DREAL *sqrtdiag_rhs;
		/** if kernel is initialized */
		bool initialized;
};

#endif /* _POLYMATCHWORDKERNEL_H__ */
