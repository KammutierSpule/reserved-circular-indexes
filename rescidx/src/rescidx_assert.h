// /////////////////////////////////////////////////////////////////////////////
///	@file rescidx_assert.h
///	@brief Assert declaration
///
///	@par  Plataform Target:	Any
/// @par  Tab Size: 2
///
/// @copyright (C) 2025 Mario Luzeiro All rights reserved.
/// @author Mario Luzeiro <mluzeiro@ua.pt>
///
/// @par  License: Distributed under the 3-Clause BSD License. See accompanying
/// file LICENSE or a copy at https://opensource.org/licenses/BSD-3-Clause
/// SPDX-License-Identifier: BSD-3-Clause
///
// /////////////////////////////////////////////////////////////////////////////
#ifndef _RESCIDX_ASSERT_H_
#define _RESCIDX_ASSERT_H_

// Includes
// /////////////////////////////////////////////////////////////////////////////

#ifndef RESCIDX_IS_DEBUG_BUILD
#ifdef ASAP_IS_DEBUG_BUILD
#define RESCIDX_IS_DEBUG_BUILD 1
#endif
#endif

#ifndef RESCIDX_ASSERT

#if RESCIDX_IS_DEBUG_BUILD == 1
#include <assert.h>
#define RESCIDX_ASSERT( a ) assert( a )
#define RESCIDX_ASSERT_OR_RETURN( a, b ) assert( a )
#else
#define RESCIDX_ASSERT( a )
#define RESCIDX_ASSERT_OR_RETURN( a, b )                                                                               \
	if( !a )                                                                                                             \
	{                                                                                                                    \
		return b;                                                                                                          \
	}
#endif

#endif

#ifndef RESCIDX_RUN_ONDEBUG

#if RESCIDX_IS_DEBUG_BUILD == 1
#define RESCIDX_RUN_ONDEBUG( a ) ( a )
#else
#define RESCIDX_RUN_ONDEBUG( a )
#endif

#endif

#endif

// EOF
// /////////////////////////////////////////////////////////////////////////////
