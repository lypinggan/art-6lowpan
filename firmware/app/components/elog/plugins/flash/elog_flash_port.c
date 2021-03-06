/*
 * This file is part of the EasyLogger Library.
 *
 * Copyright (c) 2015, Armink, <armink.ztl@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * 'Software'), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Function: Portable interface for EasyLogger's flash log pulgin.
 * Created on: 2015-07-28
 */

#include "elog_flash.h"
#include <rthw.h>
#include <rtthread.h>

static struct rt_semaphore flash_log_lock;

/**
 * EasyLogger flash log pulgin port initialize
 *
 * @return result
 */
ElogErrCode elog_flash_port_init(void) {
    ElogErrCode result = ELOG_NO_ERR;

    rt_sem_init(&flash_log_lock, "elog flash lock", 1, RT_IPC_FLAG_PRIO);

    return result;
}

/**
 * output flash saved log port interface
 *
 * @param log flash saved log
 * @param size log size
 */
void elog_flash_port_output(const char *log, size_t size) {
    extern void output_log_to_console_or_flash(bool console, bool flash, const char *log, size_t size);
    /* output to terminal */
    output_log_to_console_or_flash(true, false, log, size);
}

/**
 * flash log lock
 */
void elog_flash_port_lock(void) {
    rt_sem_take(&flash_log_lock, RT_WAITING_FOREVER);
}

/**
 * flash log unlock
 */
void elog_flash_port_unlock(void) {
    rt_sem_release(&flash_log_lock);
}

#if defined(RT_USING_FINSH) && defined(FINSH_USING_MSH)
#include <finsh.h>
static void elog_flash(uint8_t argc, char **argv) {
    if (argc >= 2) {
        if (!strcmp(argv[1], "read")) {
            if (argc >= 3) {
                elog_flash_output_recent(atol(argv[2]));
            }else {
                elog_flash_output_all();
            }
        } else if (!strcmp(argv[1], "clean")) {
            elog_flash_clean();
        } else if (!strcmp(argv[1], "flush")) {

#ifdef ELOG_FLASH_USING_BUF_MODE
            elog_flash_flush();
#else
            rt_kprintf("EasyLogger flash log buffer mode is not open.\n");
#endif

        } else {
            rt_kprintf("Please input elog_flash {<read>, <clean>, <flush>}.\n");
        }
    } else {
        rt_kprintf("Please input elog_flash {<read>, <clean>, <flush>}.\n");
    }
}
MSH_CMD_EXPORT(elog_flash, EasyLogger <read> <clean> <flush> flash log);
#endif /* defined(RT_USING_FINSH) && defined(FINSH_USING_MSH) */
