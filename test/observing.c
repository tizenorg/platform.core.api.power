/*
 * capi-system-power
 * Copyright (c) 2012 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <stdbool.h>
#include <power.h>

static GMainLoop *mainloop;

void power_cb(power_state_e state, void *ud)
{
    char* txt = (char*)ud;
    printf("power state [%d] -- %s\n", state, txt); 
}

static void sig_quit(int signo)
{
	if(mainloop)
	{
		g_main_loop_quit(mainloop);
	}
}

static void errp(power_error_e e){
    switch(e){
        case POWER_ERROR_INVALID_PARAMETER:
            printf("invalid parameter!\n");
            break;
        case POWER_ERROR_IO_ERROR:
            printf("operation failed!\n");
            break;
        case POWER_ERROR_NONE:
            printf("success!\n");
            break;
        default:
            printf("unknown!\n");
    }
}

int main(int argc, char *argv[])
{
    int err;

	signal(SIGINT, sig_quit);
	signal(SIGTERM, sig_quit);
	signal(SIGQUIT, sig_quit);

	mainloop = g_main_loop_new(NULL, FALSE);

    power_state_e state;
    if( (state = power_get_state()) < 0){
        printf("power state return ");
        errp(err);
    }
    printf("current state -> %d\n", state);

    if( (err=power_set_changed_cb(power_cb, "PIUS!!")) < 0){
        printf("set cb return ");
        errp(err);
    }

	g_main_loop_run(mainloop);
	g_main_loop_unref(mainloop);

    if( (err=power_unset_changed_cb()) < 0){
        printf("unset cb return ");
        errp(err);
    }

	return 0;
}
