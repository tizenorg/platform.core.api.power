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
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <glib.h>
#include <power.h>

char* sname[] = {
	"Normal",
	"Screen Dim",
	"Screen Off"
};

static void lock(int state)
{
	int err;
	err = power_lock_state(state, 0);
	if(err == POWER_ERROR_NONE){
		if(state >= POWER_STATE_NORMAL && state <= POWER_STATE_SCREEN_OFF)
			printf("lock state [%s] locked\n", sname[state]);
		else
			printf("unknown state\n");
	}else{
		printf("power_lock_state fail\n");
	}
}

static void unlock(int state)
{
	if(power_unlock_state(state) == POWER_ERROR_NONE){
		if(state >= POWER_STATE_NORMAL && state <= POWER_STATE_SCREEN_OFF)
			printf("lock state [%s] unlocked\n", sname[state]);
		else
			printf("unknown state\n");
	}else{
		printf("power_unlock_state fail\n");
	}
}

static void to_use(int state, int sleeptime)
{
    lock(state);
    sleep(sleeptime);
    unlock(state);
}

static void pulse(int sleeptime)
{

    power_lock_state(POWER_STATE_SCREEN_DIM, 1000);
    power_lock_state(POWER_STATE_SCREEN_OFF, 10000);
    power_lock_state(POWER_STATE_SCREEN_DIM, 1000);
    power_lock_state(POWER_STATE_NORMAL,     1000);

    printf("pulse~~~\n");
    sleep(sleeptime);
}

int main(int argc, char *argv[])
{
    int m;
	int state;
	int sleeptime;

	if(argc < 3){
		printf("1 : Normal\n");
		printf("2 : Screen Dim\n");
		printf("3 : Screen Off\n");
        printf("4 : pulse test\n");
		return 0;
	}else{
		switch(m = atoi(argv[1]))
		{
			case 1:
				state = POWER_STATE_NORMAL;
                printf("normal!!!\n");
				break;
			case 2:
				state = POWER_STATE_SCREEN_DIM;
                printf("dim!!!\n");
				break;
			case 3:
				state = POWER_STATE_SCREEN_OFF;
                printf("off!!!\n");
				break;
            case 4:
                break;
			default:
				return 0;
		}
        sleeptime = atoi(argv[2]);
		if (m < 4){
            printf("to_use\n");
            to_use(state, sleeptime);
        }else{
            printf("pulse\n");
            pulse(sleeptime);
        }
	}
	
	return 0;
}
