/*
 * Copyright (c) 2013-2019 Huawei Technologies Co., Ltd. All rights reserved.
 * Copyright (c) 2020-2021 Huawei Device Co., Ltd. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 *    conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 *    of conditions and the following disclaimer in the documentation and/or other materials
 *    provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 *    to endorse or promote products derived from this software without specific prior written
 *    permission.
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

#include "It_vfs_jffs.h"

static UINT32 Testcase(VOID)
{
    INT32 ret, dirFd, len;
    INT32 fd = -1;
    CHAR pathname1[JFFS_STANDARD_NAME_LENGTH] = { JFFS_PATH_NAME01 };
    CHAR pathname2[JFFS_STANDARD_NAME_LENGTH] = { JFFS_PATH_NAME01 };
    DIR *dir = NULL;
    struct dirent *ptr = NULL;

    ret = mkdir(pathname1, HIGHEST_AUTHORITY);
    ICUNIT_GOTO_EQUAL(ret, JFFS_NO_ERROR, ret, EXIT);

    dir = opendir(pathname1);
    ICUNIT_GOTO_NOT_EQUAL(dir, NULL, dir, EXIT1);

    dirFd = dirfd(dir);
    ICUNIT_GOTO_NOT_EQUAL(dirFd, JFFS_IS_ERROR, dirFd, EXIT1);
    printf("dirFd = %d\n", dirFd);

    (void)strcat_s(pathname2, JFFS_STANDARD_NAME_LENGTH, "/test.txt");
    fd = open(pathname2, O_CREAT | O_RDWR | O_TRUNC);
    ICUNIT_GOTO_NOT_EQUAL(fd, JFFS_IS_ERROR, fd, EXIT1);

    len = write(fd, "01234567890123456789012345", 16); // 16 means write len

    ret = close(fd);
    ICUNIT_GOTO_NOT_EQUAL(ret, JFFS_IS_ERROR, ret, EXIT1);

    ret = unlinkat(dirFd, "test.txt", 0);
    ICUNIT_GOTO_NOT_EQUAL(ret, JFFS_IS_ERROR, ret, EXIT1);

    ret = closedir(dir);
    ICUNIT_GOTO_NOT_EQUAL(ret, JFFS_IS_ERROR, ret, EXIT);

    ret = rmdir(JFFS_PATH_NAME01);
    ICUNIT_GOTO_NOT_EQUAL(ret, JFFS_IS_ERROR, ret, EXIT);

    return JFFS_NO_ERROR;

EXIT1:
    close(fd);
    unlink(pathname1);
    closedir(dir);
EXIT:
    rmdir(JFFS_PATH_NAME01);
    return JFFS_NO_ERROR;
}

VOID ItJffs023(VOID)
{
    TEST_ADD_CASE(__FUNCTION__, Testcase, TEST_VFS, TEST_JFFS, TEST_LEVEL2, TEST_FUNCTION);
}

