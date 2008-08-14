/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Written (W) 1999-2008 Gunnar Raetsch
 * Written (W) 1999-2008 Soeren Sonnenburg
 * Copyright (C) 1999-2008 Fraunhofer Institute FIRST and Max-Planck-Society
 */

#ifndef _LOCALITYIMPROVEDSTRINGKERNEL_H___
#define _LOCALITYIMPROVEDSTRINGKERNEL_H___

#include "lib/common.h"
#include "kernel/StringKernel.h"

/** The LocalityImprovedString kernel is inspired by the polynomial kernel.
 * Comparing neighboring characters it puts emphasize on local features.
 *
 * It can be defined as
 * \f[
 * K({\bf x},{\bf x'})=\left(\sum_{i=0}^{T-1}\left(\sum_{j=-l}^{+l}w_jI_{i+j}({\bf x},{\bf x'})\right)^{d_1}\right)^{d_2},
 * \f]
 * where
 * \f$ I_i({\bf x},{\bf x'})=1\f$ if \f$x_i=x'_i\f$ and 0 otherwise.
 */
class CLocalityImprovedStringKernel: public CStringKernel<CHAR>
{
	public:
		/** constructor
		 *
		 * @param size cache size
		 * @param length length
		 * @param inner_degree inner degree
		 * @param outer_degree outer degree
		 */
		CLocalityImprovedStringKernel(INT size, INT length,
			INT inner_degree, INT outer_degree);

		/** constructor
		 *
		 * @param l features of left-hand side
		 * @param r features of right-hand side
		 * @param length length
		 * @param inner_degree inner degree
		 * @param outer_degree outer degree
		 */
		CLocalityImprovedStringKernel(
			CStringFeatures<CHAR>* l, CStringFeatures<CHAR>* r,
			INT length, INT inner_degree, INT outer_degree);

		virtual ~CLocalityImprovedStringKernel();

		/** initialize kernel
		 *
		 * @param l features of left-hand side
		 * @param r features of right-hand side
		 * @return if initializing was successful
		 */
		virtual bool init(CFeatures* l, CFeatures* r);

		/** load kernel init_data
		 *
		 * @param src file to load from
		 * @return if loading was successful
		 */
		bool load_init(FILE* src);

		/** save kernel init_data
		 *
		 * @param dest file to save to
		 * @return if saving was successful
		 */
		bool save_init(FILE* dest);

		/** return what type of kernel we are
		 *
		 * @return kernel type LOCALITYIMPROVED
		 */
		virtual EKernelType get_kernel_type() { return K_LOCALITYIMPROVED; }

		/** return the kernel's name
		 *
		 * @return name LocalityImproved
		 */
		virtual const CHAR* get_name() { return "LocalityImproved"; }

	protected:
		/** compute kernel function for features a and b
		 * idx_{a,b} denote the index of the feature vectors
		 * in the corresponding feature object
		 *
		 * @param idx_a index a
		 * @param idx_b index b
		 * @return computed kernel function at indices a,b
		 */
		DREAL compute(INT idx_a, INT idx_b);

	protected:
		/** length */
		INT length;
		/** inner degree */
		INT inner_degree;
		/** outer degree */
		INT outer_degree;
};

#endif /* _LOCALITYIMPROVEDSTRINGKERNEL_H__ */

