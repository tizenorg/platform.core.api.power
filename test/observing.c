/*
 * 
 * Copyright (c) 2011 Samsung Electronics Co., Ltd All Rights Reserved
 * PROPRIETARY/CONFIDENTIAL
 * 
 * This software is the confidential and proprietary information of SAMSUNG 
 * ELECTRONICS ("Confidential Information"). You agree and acknowledge that 
 * this software is owned by Samsung and you shall not disclose such 
 * Confidential Information and shall use it only in accordance with the terms 
 * of the license agreement you entered into with SAMSUNG ELECTRONICS. SAMSUNG 
 * make no representations or warranties about the suitability of the software, 
 * either express or implied, including but not limited to the implied 
 * warranties of merchantability, fitness for a particular purpose, or 
 * non-infringement. SAMSUNG shall not be liable for any damages suffered by 
 * licensee arising out of or related to this software.
 * 
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
