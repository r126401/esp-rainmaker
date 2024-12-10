// Copyright 2020 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "esp_schedule.h"

/** Enable Schedules
 *
 * This API enables the scheduling service for the node. For more information,
 * check [here](https://rainmaker.espressif.com/docs/scheduling.html)
 *
 * It is recommended to set the timezone while using schedules. Check [here](https://rainmaker.espressif.com/docs/time-service.html#time-zone) for more information on timezones
 *
 * @note This API should be called after esp_rmaker_node_init() but before esp_rmaker_start().
 *
 * @return ESP_OK on success.
 * @return error in case of failure.
 */
#define MAX_ID_LEN 8
#define MAX_NAME_LEN 32

typedef enum trigger_type {
    TRIGGER_TYPE_INVALID = 0,
    TRIGGER_TYPE_DAYS_OF_WEEK,
    TRIGGER_TYPE_DATE,
    TRIGGER_TYPE_RELATIVE,
} trigger_type_t;

typedef struct esp_rmaker_schedule_trigger {
    trigger_type_t type;
    /* Relative Seconds */
    int relative_seconds;
    /* Minutes from 12am */
    uint16_t minutes;
    struct {
        /* 'OR' list of days or the week. Eg. Monday = 0b1, Tuesday = 0b10 */
        uint8_t repeat_days;
    } day;
    struct {
        /* Day of the month */
        uint8_t day;
        /* 'OR' list of months of the year. Eg. January = 0b1, February = 0b10.
        0 for next date (either this month or next). */
        uint16_t repeat_months;
        uint16_t year;
        bool repeat_every_year;
    } date;
    /* Used for non repeating schedules */
    int64_t next_timestamp;
} esp_rmaker_schedule_trigger_t;

typedef struct esp_rmaker_schedule_action {
    void *data;
    size_t data_len;
} esp_rmaker_schedule_action_t;

typedef struct esp_rmaker_schedule {
    char name[MAX_NAME_LEN + 1];        /* +1 for NULL termination */
    char id[MAX_ID_LEN + 1];            /* +1 for NULL termination */
    /* Info is used to store additional information, it is limited to 128 bytes. */
    char *info;
    /* Index is used in the callback to get back the schedule. */
    long index;
    /* Flags are used to identify the schedule. Eg. timing, countdown */
    uint32_t flags;
    bool enabled;
    esp_schedule_handle_t handle;
    esp_rmaker_schedule_action_t action;
    esp_rmaker_schedule_trigger_t trigger;
    esp_schedule_validity_t validity;
    struct esp_rmaker_schedule *next;
} esp_rmaker_schedule_t;
esp_err_t esp_rmaker_schedule_enable(void);
esp_rmaker_schedule_t* esp_rmaker_get_schedule_list(void);


#ifdef __cplusplus
}
#endif
