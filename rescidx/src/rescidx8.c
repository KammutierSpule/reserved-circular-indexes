// /////////////////////////////////////////////////////////////////////////////
///	@file rescidx8.c
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

// Includes
// /////////////////////////////////////////////////////////////////////////////
#include <rescidx/rescidx8.h>
#include <stdbool.h>
#include <string.h>
#include "rescidx_assert.h"

// Definitions
// /////////////////////////////////////////////////////////////////////////////
#define RESCIDX8_SLOT_EMPTY 0x00
#define RESCIDX8_MAX_OLD_AGE 0xFE
#define RESCIDX8_SLOT_RESERVED 0xFF

// Implementation
// /////////////////////////////////////////////////////////////////////////////
eRESCIDX_ret rescidx8_init( sRESCIDX8_ctx *aCtx, uint8_t *aIdxStateBuffer, uint8_t aIdxStateBufferSize )
{
	RESCIDX_ASSERT_OR_RETURN( aCtx, RESCIDX_RET_ERR_BAD_ARG );
	RESCIDX_ASSERT_OR_RETURN( aIdxStateBuffer, RESCIDX_RET_ERR_BAD_ARG );
	RESCIDX_ASSERT_OR_RETURN( aIdxStateBufferSize > 1, RESCIDX_RET_ERR_BAD_ARG );
	RESCIDX_ASSERT_OR_RETURN( aIdxStateBufferSize < RESCIDX8_SLOT_RESERVED, RESCIDX_RET_ERR_BAD_ARG );

	memset( aIdxStateBuffer, 0x00, aIdxStateBufferSize );

	aCtx->idxState					= aIdxStateBuffer;
	aCtx->idxStateSize			= aIdxStateBufferSize;
	aCtx->currentSize				= 0;
	aCtx->headIdx						= 0;
	aCtx->tailIdx						= 0;
	aCtx->currentAgeCounter = 1;

	return RESCIDX_RET_SUCCESS;
}

static void rescidx8_advance_head_idx( sRESCIDX8_ctx *aCtx )
{
	aCtx->headIdx = ( aCtx->headIdx + 1 ) % ( aCtx->idxStateSize );
}

static void rescidx8_advance_age_counter( sRESCIDX8_ctx *aCtx )
{
	aCtx->currentAgeCounter = ( aCtx->currentAgeCounter == RESCIDX8_MAX_OLD_AGE ) ? 1 : aCtx->currentAgeCounter + 1;
}

static void rescidx8_advance_tail_idx( sRESCIDX8_ctx *aCtx )
{
	uint8_t tailIdx = aCtx->tailIdx;

	const uint8_t currentTailAge = aCtx->idxState[tailIdx];

	RESCIDX_ASSERT( currentTailAge != RESCIDX8_SLOT_RESERVED );

	const uint8_t nextTailAgeCounter = ( currentTailAge == RESCIDX8_MAX_OLD_AGE ) ? 1 : currentTailAge + 1;

#if RESCIDX_IS_DEBUG_BUILD == 1
	uint8_t oldestIdxAge = RESCIDX8_SLOT_RESERVED;
	uint8_t oldestIdx		 = 0;
#endif

	// Search for the next oldest snake piece
	for( uint8_t i = 0; i < aCtx->idxStateSize; ++i )
	{
		tailIdx = ( tailIdx + 1 ) % ( aCtx->idxStateSize );

		const uint8_t idxState = aCtx->idxState[tailIdx];

		if( ( idxState != RESCIDX8_SLOT_RESERVED ) && ( idxState != RESCIDX8_SLOT_EMPTY ) )
		{
			if( idxState == nextTailAgeCounter )
			{
				aCtx->tailIdx = tailIdx;
				return;
			}

#if RESCIDX_IS_DEBUG_BUILD == 1
			if( idxState < oldestIdxAge )
			{
				oldestIdxAge = idxState;
				oldestIdx		 = tailIdx;
			}
#endif
		}
	}

#if RESCIDX_IS_DEBUG_BUILD == 1
	// Keep on same tail
	RESCIDX_ASSERT( aCtx->tailIdx == oldestIdx );

	// No usable slots
	aCtx->tailIdx = oldestIdx;
#endif
}

eRESCIDX_ret rescidx8_advance_head( sRESCIDX8_ctx *aCtx, uint8_t *aOutHeadIdx )
{
	RESCIDX_ASSERT_OR_RETURN( aCtx, RESCIDX_RET_ERR_BAD_ARG );
	RESCIDX_ASSERT_OR_RETURN( aCtx->idxStateSize > 0, RESCIDX_RET_ERR_BAD_ARG );
	RESCIDX_ASSERT_OR_RETURN( aCtx->idxStateSize < RESCIDX8_SLOT_RESERVED, RESCIDX_RET_ERR_BAD_ARG );
	RESCIDX_ASSERT_OR_RETURN( aOutHeadIdx != NULL, RESCIDX_RET_ERR_BAD_ARG );

	for( uint8_t i = 0; i < aCtx->idxStateSize; ++i )
	{
		const uint8_t idxHead	 = aCtx->headIdx;
		const uint8_t idxState = aCtx->idxState[idxHead];

		if( idxState == RESCIDX8_SLOT_EMPTY )
		{
			*aOutHeadIdx						= idxHead;
			aCtx->idxState[idxHead] = aCtx->currentAgeCounter;
			aCtx->currentSize++;

			rescidx8_advance_head_idx( aCtx );
			rescidx8_advance_age_counter( aCtx );

			return RESCIDX_RET_SUCCESS;
		}

		rescidx8_advance_head_idx( aCtx );
	}

	// No empty slot found; overwrite oldest (tail) if available
	const uint8_t idxTail = aCtx->tailIdx;

	if( aCtx->idxState[idxTail] != RESCIDX8_SLOT_RESERVED )
	{
		*aOutHeadIdx = idxTail;

		rescidx8_advance_tail_idx( aCtx );

		const uint8_t newIdxHead = idxTail;

		aCtx->headIdx = newIdxHead;

		aCtx->idxState[newIdxHead] = aCtx->currentAgeCounter;

		rescidx8_advance_age_counter( aCtx );

		return RESCIDX_RET_SUCCESS_WITH_OVERWRITE;
	}

	return RESCIDX_RET_ERR_ALL_SLOTS_RESERVED;
}

eRESCIDX_ret rescidx8_advance_tail_reserve( sRESCIDX8_ctx *aCtx, uint8_t *aOutTailIdx )
{
	RESCIDX_ASSERT_OR_RETURN( aCtx, RESCIDX_RET_ERR_BAD_ARG );
	RESCIDX_ASSERT_OR_RETURN( aCtx->idxStateSize > 0, RESCIDX_RET_ERR_BAD_ARG );
	RESCIDX_ASSERT_OR_RETURN( aCtx->idxStateSize < RESCIDX8_SLOT_RESERVED, RESCIDX_RET_ERR_BAD_ARG );

	if( aCtx->currentSize == 0 )
	{
		return RESCIDX_RET_ERR_NO_USED_SLOTS;
	}

	const uint8_t tailIdx		= aCtx->tailIdx;
	const uint8_t tailState = aCtx->idxState[tailIdx];

	if( tailState == RESCIDX8_SLOT_RESERVED )
	{
		return RESCIDX_RET_ERR_ALL_SLOTS_RESERVED;
	}

	if( tailState != RESCIDX8_SLOT_EMPTY )
	{
		rescidx8_advance_tail_idx( aCtx );

		*aOutTailIdx = tailIdx;

		aCtx->idxState[tailIdx] = RESCIDX8_SLOT_RESERVED;

		RESCIDX_ASSERT( aCtx->currentSize > 0 );

		aCtx->currentSize--;

		return RESCIDX_RET_SUCCESS;
	}

	RESCIDX_ASSERT( aCtx->tailIdx == aCtx->headIdx );

	return RESCIDX_RET_ERR_NO_USED_SLOTS;
}

eRESCIDX_ret rescidx8_clear_reserved( sRESCIDX8_ctx *aCtx, uint8_t aIdx )
{
	RESCIDX_ASSERT_OR_RETURN( aCtx, RESCIDX_RET_ERR_BAD_ARG );
	RESCIDX_ASSERT_OR_RETURN( aCtx->idxStateSize > 0, RESCIDX_RET_ERR_BAD_ARG );
	RESCIDX_ASSERT_OR_RETURN( aCtx->idxStateSize < RESCIDX8_SLOT_RESERVED, RESCIDX_RET_ERR_BAD_ARG );

	if( aIdx < aCtx->idxStateSize )
	{
		uint8_t *idxState = &aCtx->idxState[aIdx];

		if( *idxState == RESCIDX8_SLOT_RESERVED )
		{
			*idxState = RESCIDX8_SLOT_EMPTY;

			return RESCIDX_RET_SUCCESS;
		}
	}

	return RESCIDX_RET_ERR_BAD_ARG;
}

// EOF
// /////////////////////////////////////////////////////////////////////////////
