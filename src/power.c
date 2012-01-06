/*
 * Copyright (c) 2011 Samsung Electronics Co., Ltd All Rights Reserved
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


#include <string.h>
#include <stdbool.h>
#include <dlog.h>
#include <pmapi.h>

#include <power.h>
#include <vconf.h>

#undef LOG_TAG
#define LOG_TAG "TIZEN_SYSTEM_POWER"

#define _MSG_POWER_ERROR_IO_ERROR "I/O error"
#define _MSG_POWER_ERROR_INVALID_PARAMETER "Invalid parameter"

#define RETURN_ERR(err_code) \
    do { \
        LOGE("[%s] "_MSG_##err_code"(0x%08x)", __FUNCTION__, err_code); \
        return err_code; \
    }while(0)

/*
 * Enum translations to the old API
 */
int POWER_OLD_API_STATE[] =
{
    LCD_NORMAL,  /* POWER_STATE_NORMAL     */
    LCD_DIM,     /* POWER_STATE_SCREEN_DIM */
    LCD_OFF,     /* POWER_STATE_SCREEN_OFF */
};

int power_lock_state(power_state_e power_state, int timeout)
{
    if ( power_state > POWER_STATE_SCREEN_OFF || power_state < 0 || timeout < 0) {
        RETURN_ERR(POWER_ERROR_INVALID_PARAMETER);
    }
    if(pm_lock_state(POWER_OLD_API_STATE[power_state], STAY_CUR_STATE, timeout) == 0)
        return POWER_ERROR_NONE;
    else
        RETURN_ERR(POWER_ERROR_IO_ERROR);
}

int power_unlock_state(power_state_e power_state)
{
    if ( power_state > POWER_STATE_SCREEN_OFF || power_state < 0 ) {
        RETURN_ERR(POWER_ERROR_INVALID_PARAMETER);
    }
    if (pm_unlock_state(POWER_OLD_API_STATE[power_state], PM_SLEEP_MARGIN) == 0)
        return POWER_ERROR_NONE;
    else
        RETURN_ERR(POWER_ERROR_IO_ERROR);
}

int power_wakeup()
{
    if(pm_change_state(LCD_NORMAL) < 0)
        RETURN_ERR(POWER_ERROR_IO_ERROR);
    else
        return POWER_ERROR_NONE;
}

power_state_e power_get_state(void)
{
    // VCONFKEY_PM_STATE
    int value, err;
    err = vconf_get_int(VCONFKEY_PM_STATE, &value);
    if(err < 0){
        RETURN_ERR(POWER_ERROR_IO_ERROR);
    }
    switch(value){
        case 1:
            return POWER_STATE_NORMAL;
        case 2:
            return POWER_STATE_SCREEN_DIM;
        case 3:
            return POWER_STATE_SCREEN_OFF;

        default:
            return POWER_ERROR_IO_ERROR;
    }
}

static power_changed_cb changed_callback = NULL;
static void* changed_callback_user_data = NULL;

static void power_changed_inside_cb(keynode_t* key, void* user_data)
{
    char* keyname = vconf_keynode_get_name(key);

    if(keyname != NULL && changed_callback != NULL && strcmp(keyname, VCONFKEY_PM_STATE) == 0){
        power_state_e state = power_get_state();
        changed_callback(state, changed_callback_user_data);
    }
}

int power_set_changed_cb(power_changed_cb callback, void* user_data)
{
    int err;

    if(callback == NULL)
        RETURN_ERR(POWER_ERROR_INVALID_PARAMETER);

    changed_callback = callback;
    changed_callback_user_data = user_data;

    err = vconf_notify_key_changed(VCONFKEY_PM_STATE, power_changed_inside_cb, NULL);
    if(err < 0){
        RETURN_ERR(POWER_ERROR_INVALID_PARAMETER);
    }

    return POWER_ERROR_NONE;
}

int power_unset_changed_cb(void)
{
    int err = vconf_ignore_key_changed(VCONFKEY_PM_STATE, power_changed_inside_cb);
    if(err < 0){
        RETURN_ERR(POWER_ERROR_IO_ERROR);
    }
    changed_callback = NULL;
    changed_callback_user_data = NULL;

    return POWER_ERROR_NONE;
}
