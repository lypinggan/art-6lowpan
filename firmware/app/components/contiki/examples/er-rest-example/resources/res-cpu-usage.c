/*
 * Copyright (c) 2013, Institute for Pervasive Computing, ETH Zurich
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 */

/**
 * \file
 *      Example of get the cpu usage resource
 */

#include "contiki.h"

#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include "rest-engine.h"
#include "rtthread.h"
#include <cpuusage.h>

static void res_get_handler(void *request, void *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset);
static uint8_t cpu_usage_major, cpu_usage_minor;

RESOURCE(res_cpu_usage,
         "title=\"CPU-usage\";rt=\"CPU-usage\";obs",
         res_get_handler,
         NULL,
         NULL,
         NULL);

static void res_get_handler(void *request, void *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset) {
	uint8_t length = 0;
	char usage[10] = { 0 }, usage_major[5] = { 0 }, usage_minor[5] = { 0 };
	cpu_usage_get(&cpu_usage_major, &cpu_usage_minor);
	snprintf(usage_major, sizeof(usage_major), "%d", cpu_usage_major);
	snprintf(usage_minor, sizeof(usage_minor), "%d", cpu_usage_minor);
	strncat(usage, usage_major, sizeof(usage_major));
	strncat(usage, ".", strlen("."));
	strncat(usage, usage_minor, sizeof(usage_minor));
	memcpy(buffer, usage, sizeof(usage));
	length = strlen((char *) buffer);
	REST.set_header_content_type(response, REST.type.TEXT_PLAIN);
	REST.set_header_etag(response, (uint8_t *) &length, 1);
	REST.set_response_payload(response, (uint8_t *) buffer, length);
}
