// /////////////////////////////////////////////////////////////////////////////
///	@file rescidx8.h
///	@brief Declaration for rescidx using of 8bit index size.
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
#ifndef _RESCIDX8_H_
#define _RESCIDX8_H_

// Includes
// /////////////////////////////////////////////////////////////////////////////
#include <rescidx/rescidx.h>
#include <stdint.h> // NOLINT

// clang-format off
#ifdef __cplusplus
extern "C" {
#endif
// clang-format on

// Definitions
// /////////////////////////////////////////////////////////////////////////////
typedef struct s_RESCIDX8_ctx
{
	uint8_t *idxState;		///< State of indexes, internally managed, externally provided, size of nIndexes.
	uint8_t idxStateSize; ///< Number of indexes to manage (size of idxState)
	uint8_t currentSize;	///< Current size of the snake
	uint8_t headIdx;			///< Current head of the snake
	uint8_t tailIdx;			///< Current head of the snake
	uint8_t currentAgeCounter;
} sRESCIDX8_ctx;

/**
 * @brief Initialize context and provided buffer
 *
 * @param aCtx Context to be initialized.
 * @param aIdxStateBuffer An external provided array with aIdxStateBufferLength.
 * @param aIdxStateBufferSize The length of aStateBuffer which correspond to the
 * number of indexes to manage. Must be >2 and < 254
 * @return eRESCIDX_ret
 */
eRESCIDX_ret rescidx8_init( sRESCIDX8_ctx *aCtx, uint8_t *aIdxStateBuffer, uint8_t aIdxStateBufferSize );

/**
 * @brief Advance head and get a new usable index
 *
 * @param aCtx Context.
 * @param aOutHeadIdx The index that is available and you can use.
 * @return eRESCIDX_ret
 */
eRESCIDX_ret rescidx8_advance_head( sRESCIDX8_ctx *aCtx, uint8_t *aOutHeadIdx );

/**
 * @brief Advance tail and reserve the given index
 *
 * @param aCtx Context.
 * @param aOutTailIdx The tail index that was reserved from use in the circular buffer.
 * @return eRESCIDX_ret
 */
eRESCIDX_ret rescidx8_advance_tail_reserve( sRESCIDX8_ctx *aCtx, uint8_t *aOutTailIdx );

/**
 * @brief Put back to use the index.
 *
 * @param aCtx Context.
 * @param aIdx Mark this index slot to be empty and free to be used again. The index must be on reserved state.
 * @return eRESCIDX_ret
 */
eRESCIDX_ret rescidx8_clear_reserved( sRESCIDX8_ctx *aCtx, uint8_t aIdx );

#ifdef __cplusplus
}
#endif

#endif

// EOF
// /////////////////////////////////////////////////////////////////////////////
