/*
 * Copyright (c) 2015-2016, Luca Fulchir<luca@fulchir.it>, All rights reserved.
 *
 * This file is part of "libRaptorQ".
 *
 * libRaptorQ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * libRaptorQ is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * and a copy of the GNU Lesser General Public License
 * along with libRaptorQ.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "RaptorQ/v1/block_sizes.hpp"
#include "RaptorQ/v1/common.hpp"
#include "RaptorQ/v1/wrapper/C_common.h"
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

    struct RAPTORQ_LOCAL RaptorQ_ptr;
    struct RAPTORQ_LOCAL RaptorQ_future;
    struct RAPTORQ_LOCAL RaptorQ_future_enc;
    struct RAPTORQ_LOCAL RaptorQ_future_dec;

    struct RAPTORQ_API RaptorQ_dec_result {
        const RaptorQ_Error err;
        const uint16_t esi;
    };

    RAPTORQ_API struct RaptorQ_base_api* RaptorQ_api (uint32_t version);
    RAPTORQ_API void RaptorQ_free_api (struct RaptorQ_base_api **api);

    struct RAPTORQ_API RaptorQ_base_api
    {
        size_t version;
        #ifdef __cplusplus
        RaptorQ_base_api (size_t ver) : version (ver) {}
        #endif
    };

    struct RAPTORQ_API RaptorQ_v1
    {
        struct RaptorQ_base_api base;
        #ifdef __cplusplus
        RaptorQ_v1();
        #endif

        // precomputation caching
        RaptorQ_Compress (*supported_compressions)();
        RaptorQ_Compress (*get_compression)();
        bool (*set_compression) (const RaptorQ_Compress);
        size_t (*shared_cache_size) (const size_t);
        size_t  (*local_cache_size) (const size_t);
        size_t (*get_shared_cache_size)();
        size_t  (*get_local_cache_size)();

        // constructos
        struct RaptorQ_ptr* (*Encoder) (RaptorQ_type type,
                                            const RaptorQ_Block_Size symbols,
                                            const size_t symbol_size);
        struct RaptorQ_ptr* (*Decoder) (RaptorQ_type type,
                                            const RaptorQ_Block_Size symbols,
                                            const size_t symbol_size,
                                            const RaptorQ_Compute report);
        bool (*initialized) (const RaptorQ_ptr *ptr);

        // common functions
        uint16_t (*symbols)     (const RaptorQ_ptr *ptr);
        size_t   (*symbol_size) (const RaptorQ_ptr *ptr);
        void (*stop) (const RaptorQ_ptr *ptr);
        RaptorQ_Error (*future_state) (struct RaptorQ_future *const f);
        RaptorQ_Error (*future_wait_for) (struct RaptorQ_future *const f,
                                                const uint64_t time,
                                                const RaptorQ_Unit_Time unit);
        void (*future_wait) (struct RaptorQ_future *const f);
        void (*future_free) (struct RaptorQ_future **f);
        void (*free) (struct RaptorQ_ptr **ptr);

        // encoder-specific
        uint32_t (*max_repair)  (const RaptorQ_ptr *enc);
        size_t (*set_data) (const RaptorQ_ptr *enc, void **from,
                                                            const size_t size);
        bool (*has_data) (const RaptorQ_ptr *enc);
        void (*clear_data) (const RaptorQ_ptr *enc);
        bool (*precompute_sync) (const RaptorQ_ptr *enc);
        bool (*compute_sync) (const RaptorQ_ptr *enc);
        RaptorQ_future_enc* (*precompute) (const RaptorQ_ptr *enc);
        RaptorQ_future_enc* (*compute) (const RaptorQ_ptr *enc);
        RaptorQ_Error (*enc_future_get) (struct RaptorQ_future_enc *f);
        size_t (*encode) (const RaptorQ_ptr *enc, void **from,
                                            const size_t size, const uint32_t);

        // decoder-specific
        RaptorQ_Error (*add_symbol) (const RaptorQ_ptr *dec, void **from,
                                                            const size_t size,
                                                            const uint32_t esi);

        bool (*can_decode) (const RaptorQ_ptr *dec);
        uint16_t (*needed_symbols) (const RaptorQ_ptr *dec);

        RaptorQ_dec_result (*poll) (const RaptorQ_ptr *dec);
        RaptorQ_dec_result (*wait_sync) (const RaptorQ_ptr *dec);
        RaptorQ_future_dec* (*wait) (const RaptorQ_ptr *dec);
        RaptorQ_dec_result (*dec_future_get) (struct RaptorQ_future_dec *f);
    };


#ifdef __cplusplus
}   // extern "C"
#endif
