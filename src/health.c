#include "health.h"

static HealthUpdateCallback s_update_callback;
static int s_heart_rate_bpm;
static int s_steps_today;

#if defined(PBL_HEALTH)
static bool s_subscription_active;
static bool s_heart_rate_enabled;
static bool s_steps_enabled;

static void notify_update(void) {
  if (s_update_callback != NULL) {
    s_update_callback();
  }
}

static void refresh_heart_rate(void) {
  time_t now = time(NULL);
  HealthServiceAccessibilityMask accessibility =
      health_service_metric_accessible(HealthMetricHeartRateBPM, now, now);

  if (accessibility & HealthServiceAccessibilityMaskAvailable) {
    HealthValue value = health_service_peek_current_value(HealthMetricHeartRateBPM);
    s_heart_rate_bpm = value > 0 ? (int)value : 0;
  } else {
    s_heart_rate_bpm = 0;
  }
}

static void refresh_steps(void) {
  time_t now = time(NULL);
  HealthServiceAccessibilityMask accessibility =
      health_service_metric_accessible(HealthMetricStepCount, now, now);

  if (accessibility & HealthServiceAccessibilityMaskAvailable) {
    HealthValue value = health_service_sum_today(HealthMetricStepCount);
    s_steps_today = value > 0 ? (int)value : 0;
  } else {
    s_steps_today = 0;
  }
}

static void health_event_handler(HealthEventType event, void *context) {
  if (event == HealthEventHeartRateUpdate || event == HealthEventMovementUpdate ||
      event == HealthEventSignificantUpdate) {
    if (s_heart_rate_enabled) {
      refresh_heart_rate();
    }
    if (s_steps_enabled) {
      refresh_steps();
    }
    notify_update();
  }
}
#endif

void health_init(HealthUpdateCallback update_callback) {
  s_update_callback = update_callback;
}

void health_deinit(void) {
  health_set_enabled(false, false);
  s_update_callback = NULL;
}

void health_set_enabled(bool heart_rate_enabled, bool steps_enabled) {
#if defined(PBL_HEALTH)
  s_heart_rate_enabled = heart_rate_enabled;
  s_steps_enabled = steps_enabled;

  if (!s_heart_rate_enabled && !s_steps_enabled) {
    s_heart_rate_bpm = 0;
    s_steps_today = 0;
    if (s_subscription_active) {
      health_service_set_heart_rate_sample_period(0);
      health_service_events_unsubscribe();
      s_subscription_active = false;
    }
    return;
  }

  if (!s_subscription_active) {
    s_subscription_active = health_service_events_subscribe(health_event_handler, NULL);
  }
  if (s_subscription_active) {
    health_service_set_heart_rate_sample_period(s_heart_rate_enabled ? 60 : 0);
    if (s_heart_rate_enabled) {
      refresh_heart_rate();
    } else {
      s_heart_rate_bpm = 0;
    }
    if (s_steps_enabled) {
      refresh_steps();
    } else {
      s_steps_today = 0;
    }
  }
#else
  (void)heart_rate_enabled;
  (void)steps_enabled;
#endif
}

int health_get_heart_rate(void) {
  return s_heart_rate_bpm;
}

int health_get_steps(void) {
  return s_steps_today;
}
