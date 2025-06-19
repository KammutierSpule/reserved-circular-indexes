// /////////////////////////////////////////////////////////////////////////////
///	@file rescidx.h
///	@brief Declaration for rescidx
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
#ifndef _RESCIDX_H_
#define _RESCIDX_H_

// Includes
// /////////////////////////////////////////////////////////////////////////////

// clang-format off
#ifdef __cplusplus
extern "C" {
#endif
// clang-format on

// Definitions
// /////////////////////////////////////////////////////////////////////////////

typedef enum e_RESCIDX_ret
{
	RESCIDX_RET_SUCCESS = 0,
	RESCIDX_RET_SUCCESS_WITH_OVERWRITE,
	RESCIDX_RET_ERR_BAD_ARG,
	RESCIDX_RET_ERR_NO_USED_SLOTS,
	RESCIDX_RET_ERR_ALL_SLOTS_RESERVED,
} eRESCIDX_ret;

#ifdef __cplusplus
}
#endif

#endif

// EOF
// /////////////////////////////////////////////////////////////////////////////
