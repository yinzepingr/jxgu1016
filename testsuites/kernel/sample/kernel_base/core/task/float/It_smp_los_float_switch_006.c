/*
 * Copyright (c) 2013-2019 Huawei Technologies Co., Ltd. All rights reserved.
 * Copyright (c) 2020-2021 Huawei Device Co., Ltd. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "It_los_float.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#ifndef LOSCFG_ARCH_FPU_DISABLE

static void HwiF01(UINT32 arg)
{
    UINT32 temp2 = 0xf;
    FLOAT a = 123.321;
    FLOAT b = 1234.4321;
    FLOAT c = 999.002345;
    FLOAT d, f, e;
    LOS_AtomicInc(&g_testCount);

    while (temp2--) {
        d = a * b + c + temp2;
        f = a * c + b + temp2;
        e = a + b * c + temp2;

        // 153230.406250, numbers involved in floating-point operations, without special functions.
        if (d != 153230.406250 + temp2) {
            dprintf("Error:d = %f----temp = 0x%x----\n", d, temp2);
            LOS_AtomicInc(&g_testCount);
            continue;
        // 124432.390625, numbers involved in floating-point operations, without special functions.
        } else if (f != 124432.390625 + temp2) {
            dprintf("Error:f = %f----temp = 0x%x----\n", f, temp2);
            LOS_AtomicInc(&g_testCount);
            continue;
        // 1233323.875000, numbers involved in floating-point operations, without special functions.
        } else if (e != (1233323.875000 + temp2)) {
            dprintf("Error:e = %f----temp = 0x%x----\n", e, temp2);
            LOS_AtomicInc(&g_testCount);
            continue;
        }
        // 153230.406250, numbers involved in floating-point operations, without special functions.
        ICUNIT_GOTO_EQUAL(d, 153230.406250 + temp2, temp2, EXIT);
        // 124432.390625, numbers involved in floating-point operations, without special functions.
        ICUNIT_GOTO_EQUAL(f, 124432.390625 + temp2, temp2, EXIT);
        // 1233323.875000, numbers involved in floating-point operations, without special functions.
        ICUNIT_GOTO_EQUAL(e, 1233323.875000 + temp2, temp2, EXIT);
    }
EXIT:
    return;
}

static UINT32 Testcase(void)
{
    TSK_INIT_PARAM_S testTask;
    UINT32 ret, currCpuid;
    UINT32 i;

    g_testCount = 0;

    currCpuid = (ArchCurrCpuid() + 1) % (LOSCFG_KERNEL_CORE_NUM);

    ret = LOS_HwiCreate(HWI_NUM_TEST, 1, 0, (HWI_PROC_FUNC)HwiF01, 0);
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);

    HalIrqSetAffinity(HWI_NUM_TEST, CPUID_TO_AFFI_MASK(ArchCurrCpuid())); // curret cpu

    ret = LOS_HwiCreate(HWI_NUM_TEST1, 1, 0, (HWI_PROC_FUNC)HwiF01, 0);
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);

    HalIrqSetAffinity(HWI_NUM_TEST1, CPUID_TO_AFFI_MASK(((ArchCurrCpuid() + 1) % LOSCFG_KERNEL_CORE_NUM))); // other cpu

    // 100, Number of cycles
    for (i = 0; i < 100; i++) {
        TestHwiTrigger(HWI_NUM_TEST1);
        TestHwiTrigger(HWI_NUM_TEST);
        LOS_TaskDelay(10); // 10, set delay time.
    }

    ICUNIT_GOTO_EQUAL(g_testCount, 200, g_testCount, EXIT); // 200, assert that g_testCount is equal to this.

EXIT:

    ret = LOS_HwiDelete(HWI_NUM_TEST, NULL);
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT);
    ret = LOS_HwiDelete(HWI_NUM_TEST1, NULL);
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT);
    return LOS_OK;
}
#endif

void ItSmpLosFloatSwitch006(void)
{
#ifndef LOSCFG_ARCH_FPU_DISABLE
    TEST_ADD_CASE("IT_SMP_LOS_FLOAT_SWITCH_006", Testcase, TEST_LOS, TEST_TASK, TEST_LEVEL0, TEST_FUNCTION);
#endif
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */
