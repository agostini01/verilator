// -*- mode: C++; c-file-style: "cc-mode" -*-
//*************************************************************************
//
// Copyright 2009-2017 by Wilson Snyder. This program is free software; you can
// redistribute it and/or modify it under the terms of either the GNU
// Lesser General Public License Version 3 or the Perl Artistic License
// Version 2.0.
// SPDX-License-Identifier: LGPL-3.0-only OR Artistic-2.0
//
//*************************************************************************

#include <cstdio>
#include <cstring>
#include <iostream>
#include "svdpi.h"

//======================================================================

// clang-format off
#if defined(VERILATOR)
# include "Vt_dpi_lib__Dpi.h"
#elif defined(VCS)
# include "../vc_hdrs.h"
#elif defined(NC)
# define NEED_EXTERNS
#else
# error "Unknown simulator for DPI test"
#endif
// clang-format on

#ifdef NEED_EXTERNS
extern "C" {
// If get ncsim: *F,NOFDPI: Function {foo} not found in default libdpi.
// Then probably forgot to list a function here.

extern int dpii_failure();
extern void dpii_check();
}
#endif

//======================================================================

int failure = 0;
int dpii_failure() { return failure; }

#define CHECK_RESULT_HEX(got, exp) \
    do { \
        if ((got) != (exp)) { \
            std::cout << std::dec << "%Error: " << __FILE__ << ":" << __LINE__ << std::hex \
                      << ": GOT=" << (got) << "   EXP=" << (exp) << std::endl; \
            failure = __LINE__; \
        } \
    } while (0)

//======================================================================

void dpii_lib_bit_check() {
    svBitVecVal bv[3];
    bv[0] = 0xa3a2a1a0;  // 31..0
    bv[1] = 0xa7a6a5a4;  // 63..32
    bv[2] = 0xabaaa9a8;  // 95..64
    CHECK_RESULT_HEX((int)svGetBitselBit(bv, 32), 0);
    CHECK_RESULT_HEX((int)svGetBitselBit(bv, 33), 0);
    CHECK_RESULT_HEX((int)svGetBitselBit(bv, 34), 1);
    CHECK_RESULT_HEX((int)svGetBitselBit(bv, 35), 0);
    CHECK_RESULT_HEX((int)svGetBitselBit(bv, 36), 0);
    CHECK_RESULT_HEX((int)svGetBitselBit(bv, 37), 1);
    CHECK_RESULT_HEX((int)svGetBitselBit(bv, 38), 0);
    CHECK_RESULT_HEX((int)svGetBitselBit(bv, 39), 1);

    svPutBitselBit(bv, 32, 1);
    svPutBitselBit(bv, 33, 0);
    svPutBitselBit(bv, 34, 1);
    svPutBitselBit(bv, 35, 1);
    CHECK_RESULT_HEX(bv[0], 0xa3a2a1a0);
    CHECK_RESULT_HEX(bv[1], 0xa7a6a5ad);
    CHECK_RESULT_HEX(bv[2], 0xabaaa9a8);

    svBitVecVal btmp[2];
    svGetPartselBit(btmp, bv, 40, 8);
    CHECK_RESULT_HEX(btmp[0], 0xa5);
    svGetPartselBit(btmp, bv, 32, 32);
    CHECK_RESULT_HEX(btmp[0], 0xa7a6a5ad);
    svGetPartselBit(btmp, bv, 48, 40);
    CHECK_RESULT_HEX(btmp[0], 0xa9a8a7a6);
    CHECK_RESULT_HEX(btmp[1], 0xaa);

    btmp[0] = 0xa5;
    svPutPartselBit(bv, btmp[0], 48, 8);
    CHECK_RESULT_HEX(bv[0], 0xa3a2a1a0);
    CHECK_RESULT_HEX(bv[1], 0xa7a5a5ad);
    CHECK_RESULT_HEX(bv[2], 0xabaaa9a8);

    btmp[0] = 0x11223344;
    svPutPartselBit(bv, btmp[0], 32, 32);
    CHECK_RESULT_HEX(bv[0], 0xa3a2a1a0);
    CHECK_RESULT_HEX(bv[1], 0x11223344);
    CHECK_RESULT_HEX(bv[2], 0xabaaa9a8);

    btmp[0] = 0x99887766;
    svPutPartselBit(bv, btmp[0], 24, 24);
    CHECK_RESULT_HEX(bv[0], 0x66a2a1a0);
    CHECK_RESULT_HEX(bv[1], 0x11228877);
    CHECK_RESULT_HEX(bv[2], 0xabaaa9a8);
}

void dpii_lib_logic_check() {
    svLogicVecVal lv[3];
    lv[0].aval = 0xb3b2b1b0;  // 31..0
    lv[1].aval = 0xb7b6b5b4;  // 63..32
    lv[2].aval = 0xbbbab9b8;  // 95..64
    lv[0].bval = 0xc3c2c1c0;  // 31..0
    lv[1].bval = 0xc7c6c5c4;  // 63..32
    lv[2].bval = 0xcbcac9c8;  // 95..64
    CHECK_RESULT_HEX((int)svGetBitselLogic(lv, 32), 0);
    CHECK_RESULT_HEX((int)svGetBitselLogic(lv, 33), 0);
    CHECK_RESULT_HEX((int)svGetBitselLogic(lv, 34), 3);
    CHECK_RESULT_HEX((int)svGetBitselLogic(lv, 35), 0);
    CHECK_RESULT_HEX((int)svGetBitselLogic(lv, 36), 1);
    CHECK_RESULT_HEX((int)svGetBitselLogic(lv, 37), 1);
    CHECK_RESULT_HEX((int)svGetBitselLogic(lv, 38), 2);
    CHECK_RESULT_HEX((int)svGetBitselLogic(lv, 39), 3);

    svPutBitselLogic(lv, 32, 1);
    svPutBitselLogic(lv, 33, 0);
    svPutBitselLogic(lv, 34, 1);
    svPutBitselLogic(lv, 35, 3);
    CHECK_RESULT_HEX(lv[0].aval, 0xb3b2b1b0);
    CHECK_RESULT_HEX(lv[1].aval, 0xb7b6b5bd);
    CHECK_RESULT_HEX(lv[2].aval, 0xbbbab9b8);
    CHECK_RESULT_HEX(lv[0].bval, 0xc3c2c1c0);
    CHECK_RESULT_HEX(lv[1].bval, 0xc7c6c5c8);
    CHECK_RESULT_HEX(lv[2].bval, 0xcbcac9c8);

    svLogicVecVal ltmp[2];
    svGetPartselLogic(ltmp, lv, 40, 8);
    CHECK_RESULT_HEX(ltmp[0].aval, 0xb5);
    CHECK_RESULT_HEX(ltmp[0].bval, 0xc5);
    svGetPartselLogic(ltmp, lv, 32, 32);
    CHECK_RESULT_HEX(ltmp[0].aval, 0xb7b6b5bd);
    CHECK_RESULT_HEX(ltmp[0].bval, 0xc7c6c5c8);
    svGetPartselLogic(ltmp, lv, 48, 40);
    CHECK_RESULT_HEX(ltmp[0].aval, 0xb9b8b7b6);
    CHECK_RESULT_HEX(ltmp[0].bval, 0xc9c8c7c6);
    CHECK_RESULT_HEX(ltmp[1].aval, 0xba);
    CHECK_RESULT_HEX(ltmp[1].bval, 0xca);

    ltmp[0].aval = 0xb5;
    ltmp[0].bval = 0xc5;
    svPutPartselLogic(lv, ltmp[0], 48, 8);
    CHECK_RESULT_HEX(lv[0].aval, 0xb3b2b1b0);
    CHECK_RESULT_HEX(lv[1].aval, 0xb7b5b5bd);
    CHECK_RESULT_HEX(lv[2].aval, 0xbbbab9b8);
    CHECK_RESULT_HEX(lv[0].bval, 0xc3c2c1c0);
    CHECK_RESULT_HEX(lv[1].bval, 0xc7c5c5c8);
    CHECK_RESULT_HEX(lv[2].bval, 0xcbcac9c8);

    ltmp[0].aval = 0x11223344;
    ltmp[0].bval = 0x81828384;
    svPutPartselLogic(lv, ltmp[0], 32, 32);
    CHECK_RESULT_HEX(lv[0].aval, 0xb3b2b1b0);
    CHECK_RESULT_HEX(lv[1].aval, 0x11223344);
    CHECK_RESULT_HEX(lv[2].aval, 0xbbbab9b8);
    CHECK_RESULT_HEX(lv[0].bval, 0xc3c2c1c0);
    CHECK_RESULT_HEX(lv[1].bval, 0x81828384);
    CHECK_RESULT_HEX(lv[2].bval, 0xcbcac9c8);

    ltmp[0].aval = 0x99887766;
    ltmp[0].bval = 0x89888786;
    svPutPartselLogic(lv, ltmp[0], 24, 24);
    CHECK_RESULT_HEX(lv[0].aval, 0x66b2b1b0);
    CHECK_RESULT_HEX(lv[1].aval, 0x11228877);
    CHECK_RESULT_HEX(lv[2].aval, 0xbbbab9b8);
    CHECK_RESULT_HEX(lv[0].bval, 0x86c2c1c0);
    CHECK_RESULT_HEX(lv[1].bval, 0x81828887);
    CHECK_RESULT_HEX(lv[2].bval, 0xcbcac9c8);
}

//======================================================================

void dpii_check() {
    dpii_lib_bit_check();
    dpii_lib_logic_check();
}
