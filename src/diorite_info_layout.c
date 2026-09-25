#include "diorite_info_layout.h"
#include "common.h"
#include "weather.h"

#if defined(PBL_PLATFORM_DIORITE)
DioriteInfoLayout diorite_info_layout_get(void) {
  DioriteInfoLayout layout = {0};
  bool visible[3] = {show_heart_rate, weather_is_enabled(), show_steps};
  int *centers[3] = {
    &layout.heart_rate_center,
    &layout.weather_center,
    &layout.steps_center,
  };

  for (int element = 0; element < 3; element++) {
    if (visible[element]) {
      layout.visible_count++;
    }
  }

  if (layout.visible_count == 1) {
    for (int element = 0; element < 3; element++) {
      if (visible[element]) {
        *centers[element] = 72;
      }
    }
  } else if (layout.visible_count == 2) {
    int next_center = 36;
    for (int element = 0; element < 3; element++) {
      if (visible[element]) {
        *centers[element] = next_center;
        next_center = 108;
      }
    }
  } else if (layout.visible_count == 3) {
    // Keep weather between the Bluetooth and battery indicators.  BPM and
    // steps then use the centred two-item layout on the lower row.
    layout.heart_rate_center = 36;
    layout.weather_center = 72;
    layout.steps_center = 108;
    layout.weather_top = true;
  }

  return layout;
}
#else
DioriteInfoLayout diorite_info_layout_get(void) {
  return (DioriteInfoLayout){0};
}
#endif
