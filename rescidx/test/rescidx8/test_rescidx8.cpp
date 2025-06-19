// /////////////////////////////////////////////////////////////////////////////
///	@file test_rescidx8.cpp
///	@brief Unit tests for rescidx8
///
///	@par  Plataform Target:	Tests
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
#include <CppUTest/TestHarness.h>
#include <CppUTest/UtestMacros.h>
#include <rescidx/rescidx8.h>

// Definitions
// /////////////////////////////////////////////////////////////////////////////

// Setup
// /////////////////////////////////////////////////////////////////////////////

// clang-format off
// NOLINTBEGIN
TEST_GROUP( RESCIDX8 ){
	void setup()
	{

	}

	void teardown()
	{

	}


};
// NOLINTEND
// clang-format on

// NOLINTBEGIN
TEST( RESCIDX8, double_buffer )
// NOLINTEND
{
	sRESCIDX8_ctx ctx;
	uint8_t idx_state_buffer[2];

	eRESCIDX_ret ret = RESCIDX_RET_SUCCESS;

	ret = rescidx8_init( &ctx, idx_state_buffer, sizeof( idx_state_buffer ) );

	CHECK_EQUAL( RESCIDX_RET_SUCCESS, ret );

	uint8_t headIdx_1 = 0;

	ret = rescidx8_advance_head( &ctx, &headIdx_1 );

	CHECK_EQUAL( RESCIDX_RET_SUCCESS, ret );

	uint8_t headIdx_2 = 0;

	ret = rescidx8_advance_head( &ctx, &headIdx_2 );

	CHECK_EQUAL( RESCIDX_RET_SUCCESS, ret );

	CHECK_COMPARE( headIdx_1, !=, headIdx_2 );
	CHECK_COMPARE( headIdx_1, <, sizeof( idx_state_buffer ) );
	CHECK_COMPARE( headIdx_2, <, sizeof( idx_state_buffer ) );

	// Expect to overwrite and return to first index
	uint8_t headIdx_3 = 0;

	ret = rescidx8_advance_head( &ctx, &headIdx_3 );

	CHECK_EQUAL( RESCIDX_RET_SUCCESS_WITH_OVERWRITE, ret );
	CHECK_EQUAL( headIdx_3, headIdx_1 );

	// Expect to overwrite and return to second index
	uint8_t headIdx_4 = 0;

	ret = rescidx8_advance_head( &ctx, &headIdx_4 );

	CHECK_EQUAL( RESCIDX_RET_SUCCESS_WITH_OVERWRITE, ret );
	CHECK_EQUAL( headIdx_4, headIdx_2 );
}

// NOLINTBEGIN
TEST( RESCIDX8, double_buffer_push_pop )
// NOLINTEND
{
	sRESCIDX8_ctx ctx;
	uint8_t idx_state_buffer[2];

	eRESCIDX_ret ret = RESCIDX_RET_SUCCESS;

	ret = rescidx8_init( &ctx, idx_state_buffer, sizeof( idx_state_buffer ) );

	CHECK_EQUAL( RESCIDX_RET_SUCCESS, ret );

	uint8_t headIdx_1 = 0;

	ret = rescidx8_advance_head( &ctx, &headIdx_1 );

	CHECK_EQUAL( RESCIDX_RET_SUCCESS, ret );

	uint8_t headIdx_2 = 0;

	ret = rescidx8_advance_head( &ctx, &headIdx_2 );

	CHECK_EQUAL( RESCIDX_RET_SUCCESS, ret );

	CHECK_COMPARE( headIdx_1, !=, headIdx_2 );
	CHECK_COMPARE( headIdx_1, <, sizeof( idx_state_buffer ) );
	CHECK_COMPARE( headIdx_2, <, sizeof( idx_state_buffer ) );

	uint8_t tailIdx_1 = 0;

	ret = rescidx8_advance_tail_reserve( &ctx, &tailIdx_1 );

	CHECK_EQUAL( RESCIDX_RET_SUCCESS, ret );

	CHECK_EQUAL( headIdx_1, tailIdx_1 );

	uint8_t tailIdx_2 = 0;

	ret = rescidx8_advance_tail_reserve( &ctx, &tailIdx_2 );

	CHECK_EQUAL( RESCIDX_RET_SUCCESS, ret );

	CHECK_EQUAL( headIdx_2, tailIdx_2 );

	uint8_t tailIdx_3 = 0;

	ret = rescidx8_advance_tail_reserve( &ctx, &tailIdx_3 );

	CHECK_EQUAL( RESCIDX_RET_ERR_NO_USED_SLOTS, ret );
}

// NOLINTBEGIN
TEST( RESCIDX8, triple_buffer )
// NOLINTEND
{
	sRESCIDX8_ctx ctx;
	uint8_t idx_state_buffer[3];

	eRESCIDX_ret ret = RESCIDX_RET_SUCCESS;

	ret = rescidx8_init( &ctx, idx_state_buffer, sizeof( idx_state_buffer ) );

	CHECK_EQUAL( RESCIDX_RET_SUCCESS, ret );

	uint8_t headIdx_1 = 0;

	ret = rescidx8_advance_head( &ctx, &headIdx_1 );

	CHECK_EQUAL( RESCIDX_RET_SUCCESS, ret );

	uint8_t headIdx_2 = 0;

	ret = rescidx8_advance_head( &ctx, &headIdx_2 );

	CHECK_EQUAL( RESCIDX_RET_SUCCESS, ret );

	uint8_t headIdx_3 = 0;

	ret = rescidx8_advance_head( &ctx, &headIdx_3 );

	CHECK_EQUAL( RESCIDX_RET_SUCCESS, ret );
	CHECK_COMPARE( headIdx_1, !=, headIdx_2 );
	CHECK_COMPARE( headIdx_1, !=, headIdx_3 );
	CHECK_COMPARE( headIdx_2, !=, headIdx_3 );
	CHECK_COMPARE( headIdx_1, <, sizeof( idx_state_buffer ) );
	CHECK_COMPARE( headIdx_2, <, sizeof( idx_state_buffer ) );
	CHECK_COMPARE( headIdx_3, <, sizeof( idx_state_buffer ) );

	// Expect to overwrite and return to first index
	uint8_t headIdx_4 = 0;

	ret = rescidx8_advance_head( &ctx, &headIdx_4 );

	CHECK_EQUAL( RESCIDX_RET_SUCCESS_WITH_OVERWRITE, ret );
	CHECK_EQUAL( headIdx_1, headIdx_4 );

	//
	uint8_t tailIdx_1 = 0;

	ret = rescidx8_advance_tail_reserve( &ctx, &tailIdx_1 );

	CHECK_EQUAL( RESCIDX_RET_SUCCESS, ret );
	CHECK_EQUAL( headIdx_2, tailIdx_1 );

	//
	uint8_t tailIdx_2 = 0;

	ret = rescidx8_advance_tail_reserve( &ctx, &tailIdx_2 );

	CHECK_EQUAL( RESCIDX_RET_SUCCESS, ret );
	CHECK_EQUAL( headIdx_3, tailIdx_2 );

	// Expect to overwrite and keep on first index
	uint8_t headIdx_5 = 0;

	ret = rescidx8_advance_head( &ctx, &headIdx_5 );

	CHECK_EQUAL( RESCIDX_RET_SUCCESS_WITH_OVERWRITE, ret );
	CHECK_EQUAL( headIdx_1, headIdx_5 );

	// Free the index got on tailIdx_2
	ret = rescidx8_clear_reserved( &ctx, tailIdx_2 );
	CHECK_EQUAL( RESCIDX_RET_SUCCESS, ret );

	// Expect to get the last free index
	uint8_t headIdx_6 = 0;

	ret = rescidx8_advance_head( &ctx, &headIdx_6 );

	CHECK_EQUAL( RESCIDX_RET_SUCCESS, ret );
	CHECK_EQUAL( tailIdx_2, headIdx_6 );

	// Try to free the index got on tailIdx_2, but it was already free
	ret = rescidx8_clear_reserved( &ctx, tailIdx_2 );
	CHECK_EQUAL( RESCIDX_RET_ERR_BAD_ARG, ret );

	// Free the index got on tailIdx_1
	ret = rescidx8_clear_reserved( &ctx, tailIdx_1 );
	CHECK_EQUAL( RESCIDX_RET_SUCCESS, ret );

	// Expect to get the last free index
	uint8_t headIdx_7 = 0;

	ret = rescidx8_advance_head( &ctx, &headIdx_7 );
	CHECK_EQUAL( RESCIDX_RET_SUCCESS, ret );
	CHECK_EQUAL( tailIdx_1, headIdx_7 );
}

// EOF
// /////////////////////////////////////////////////////////////////////////////
