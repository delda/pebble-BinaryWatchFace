#include "render.h"
#include "render_layout.h"
#include "weather.h"
#if defined(PBL_PLATFORM_EMERY)
#include "render_weather_emery.h"
#endif
#if defined(PBL_PLATFORM_GABBRO)
#include "render_weather_gabbro.h"
#endif
#if defined(PBL_PLATFORM_DIORITE)
#include "render_weather_diorite.h"
#endif

static bool bottom_element_is_visible(const RenderState *state, enum BottomElement element) {
  switch (element) {
    case BOTTOM_BLUETOOTH:
      return state->bluetooth == BT_ALWAYS ||
             (state->bluetooth == BT_ON_DISCONNECT && state->bluetooth_status == 0);
    case BOTTOM_HEART_RATE:
      return state->show_heart_rate;
    case BOTTOM_STEPS:
      return state->show_steps;
    case BOTTOM_BATTERY:
      return (state->battery == BA_UNDER_20_PERC &&
              state->battery_level < BA_PERCENT_WARNING) ||
             state->battery == BA_ALWAYS;
    case BOTTOM_WEATHER:
      return state->weather_enabled;
    default:
      return false;
  }
}

static BottomLayout bottom_layout_create(const RenderState *state) {
  BottomLayout layout = {0};
  bool visible[BOTTOM_ELEMENT_COUNT] = {false};
  for (int element = 0; element < BOTTOM_ELEMENT_COUNT; element++) {
    visible[element] = bottom_element_is_visible(state, (enum BottomElement)element);
  }
  bottom_layout_calculate(visible, &layout);
  return layout;
}

void render_watchface(GContext *gContext, Color palettes[], const RenderState *state,
                      struct Flake *flakes,
                      Layer *flake_layers[NUM_FLAKES]) {
  int easter_egg = isEasterEggDay();
  RenderState render_state = *state;
  if (easter_egg != 0) {
    render_state.shape = (easter_egg == 2) ? 11 : render_state.shape;
    render_state.shape = (easter_egg == 3) ? 12 : render_state.shape;
    render_state.shape = (easter_egg == 4) ? 13 : render_state.shape;
#ifdef PBL_PLATFORM_APLITE
    render_state.color = 0;
#else
    render_state.color = (easter_egg == 1) ? 15 :
                         (easter_egg == 2) ? 16 :
                         (easter_egg == 3) ? 17 : 18;
#endif
  }

  Color palette = palettes[render_state.color];
  BottomLayout bottom_layout = bottom_layout_create(&render_state);
  draw_background(gContext, 0, GCornerNone, palette);
  if (render_state.number > 0 && easter_egg != 4) {
    draw_time_background(gContext, palette, render_state.hour, render_state.minute);
  }
  draw_clock(gContext, palette, (bool)render_state.help_num, render_state.shape,
             render_state.bullets_number, render_state.buffer_time);

  if (render_state.bluetooth > 0) {
    draw_bluetooth(gContext, render_state.bluetooth, render_state.bluetooth_status,
                   render_state.battery, render_state.battery_level, render_state.color,
                   &bottom_layout);
  }
  if (render_state.battery > 0) {
    draw_battery(gContext, render_state.battery, render_state.bluetooth,
                 render_state.bluetooth_status, render_state.battery_level,
                 render_state.battery_modality, easter_egg, &bottom_layout, palette);
  }
  // Easter keeps its greeting visible even if the regular date is disabled.
  if (render_state.date > 0 || easter_egg == 4) {
    draw_date(gContext, palette, render_state.date, easter_egg);
  }

  render_layout_draw_health_indicators(gContext, palette, render_state.show_heart_rate,
                                        render_state.show_steps, render_state.heart_rate_bpm,
                                        render_state.steps_today, &bottom_layout);

#if defined(PBL_PLATFORM_EMERY)
  if (render_state.weather_enabled) {
    render_weather_emery(gContext, palette, weather_get_data());
  }
#endif
#if defined(PBL_PLATFORM_GABBRO)
  if (render_state.weather_enabled) {
    render_weather_gabbro(gContext, palette, weather_get_data(), &bottom_layout);
  }
#endif
#if defined(PBL_PLATFORM_DIORITE)
  if (render_state.weather_enabled) {
    render_weather_diorite(gContext, palette, weather_get_data());
  }
#endif

  if (easter_egg == 1 || easter_egg == 2 || render_state.snow) {
    for (int i = 0; i < NUM_FLAKES; i++) {
      draw_flake(gContext, flake_layers[i], flakes[i]);
    }
  }
}
