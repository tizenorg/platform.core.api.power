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





#ifndef __TIZEN_SYSTEM_POWER_H__
#define __TIZEN_SYSTEM_POWER_H__


#include <tizen.h>


#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @addtogroup CAPI_SYSTEM_POWER_MODULE
 * @{
 * @brief
 * This is a Power API of the System Service.
 * @}
 */

/**
 * @addtogroup CAPI_SYSTEM_POWER_MODULE
 * @{
 */


/**
 * @brief	Enumerations of available power states.
 * @details	An application cannot put the device into the power off state or the suspend state.
 */
typedef enum
{
    POWER_STATE_NORMAL,      /**< Normal state */
    POWER_STATE_SCREEN_DIM,  /**< Screen dim state */
    POWER_STATE_SCREEN_OFF,  /**< Screen off state */
} power_state_e;


/**
 * @brief	Enumerations of error code for Power.
 */
typedef enum
{
    POWER_ERROR_NONE                 = TIZEN_ERROR_NONE,				/**< Successful */
    POWER_ERROR_IO_ERROR             = TIZEN_ERROR_IO_ERROR,			/**< I/O error */
    POWER_ERROR_INVALID_PARAMETER    = TIZEN_ERROR_INVALID_PARAMETER,	/**< Invalid parameter */
} power_error_e;


/**
 * @}
*/


/**
 * @addtogroup CAPI_SYSTEM_POWER_MODULE
 * @{
 */

/**
 * @brief Called when an power state changed
 *
 * @param[in] state     The state of power
 * @param[in] user_data     The user data passed from the callback registration function
 *
 */
typedef void (*power_changed_cb)(power_state_e state, void *user_data); 

/**
 * @brief Locks the given power state for a specified time.
 *
 * @details After the given @a timeout_ms (in milliseconds), unlock the given power state automatically.
 * #SYSTEM_INFO_KEY_POWER_STATE key and associated system information function can give inform about the current state.
 *
 * @remarks If the process dies, every lock will be removed.
 *
 * @param[in] power_state	The power state to lock
 * @param[in] timeout_ms	The positive number in milliseconds or 0 for permanent lock \n
 *							So you must release the permanent lock of power state with #power_unlock_state() if @a timeout_ms is zero. \n
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #POWER_ERROR_NONE               Successful
 * @retval #POWER_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #POWER_ERROR_IO_ERROR           I/O error
 *
 * @see power_unlock_state()
 * @see	system_info_set_changed_cb()
 * @see system_info_get_value_int(SYSTEM_INFO_KEY_POWER_STATE, ...)
 */
int power_lock_state(power_state_e power_state, int timeout_ms);


/**
 * @brief Releases the given power state locked before.
 * @param[in] power_state	The power state to release
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #POWER_ERROR_NONE				Successful
 * @retval #POWER_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval #POWER_ERROR_IO_ERROR			I/O error
 *
 * @see power_lock_state()
 */
int power_unlock_state(power_state_e power_state);


/**
 * @brief Changes the current power state to the normal/dim state.
 *
 * @param[in] dim       Determine to go to dim state. 
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #POWER_ERROR_NONE		Successful
 * @retval #POWER_ERROR_IO_ERROR	I/O error
 *
 * @post The device will be in #POWER_STATE_NORMAL state.
 *
 */
int power_wakeup(bool dim);

/**
 * @brief Get the current power state.
 *
 * @return power state enum, otherwise a negative error value.
 * @retval #POWER_ERROR_NONE		Successful
 * @retval #POWER_ERROR_IO_ERROR	I/O error
 */
power_state_e power_get_state(void);

/**
 * @brief Set the callback for getting power changed event.
 *
 * @param[in]   power_changed_cb	The power state changed callback function pointer
 * @param[in]   user_data   The user data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #POWER_ERROR_NONE				Successful
 * @retval #POWER_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval #POWER_ERROR_IO_ERROR			I/O error
 */
int power_set_changed_cb(power_changed_cb callback, void* user_data);

/**
 * @brief Unset the callback for getting power changed event.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #POWER_ERROR_NONE				Successful
 * @retval #POWER_ERROR_IO_ERROR			I/O error
 */
int power_unset_changed_cb(void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_SYSTEM_POWER_H__*/
