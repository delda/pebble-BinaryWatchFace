#pragma once

#include <pebble.h>

typedef void (*HealthUpdateCallback)(void);

void health_init(HealthUpdateCallback update_callback);
void health_deinit(void);
void health_set_enabled(bool heart_rate_enabled, bool steps_enabled);
int health_get_heart_rate(void);
int health_get_steps(void);
