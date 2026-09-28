#include "render.h"
#include "render_layout.h"
#include "weather.h"
#if defined(PBL_PLATFORM_EMERY)
#include "render_weather_emery.h"
#endif
#if defined(PBL_PLATFORM_GABBRO)
#include "render_weather_gabbro.h"
#endif
#if defined(PBL_PLATFORM_CHALK)
#include "render_weather_chalk.h"
#endif
#if defined(PBL_PLATFORM_DIORITE)
#include "render_weather_diorite.h"
#endif

// Chalk has a smaller protected app stack than the newer color platforms.
// Keep the render state and layout out of that stack; the update callback is
// single-threaded, so static storage is safe here.
static RenderState s_render_state;
static BottomLayout s_bottom_layout;

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

#if defined(PBL_PLATFORM_GABBRO) || defined(PBL_PLATFORM_CHALK)
static int reference_width_from_pixels(int pixels) {
  return (pixels * 9 + 12) / 13;
}

static int rendered_text_width(const char *text, GFont font, int max_width) {
  GSize size = graphics_text_layout_get_content_size(
      text, font, GRect(0, 0, max_width, 40),
      GTextOverflowModeTrailingEllipsis, GTextAlignmentLeft);
  return size.w;
}
#endif

static void bottom_element_widths(const RenderState *state,
                                  int widths[BOTTOM_ELEMENT_COUNT]) {
  int defaults[BOTTOM_ELEMENT_COUNT] = {12, 52, 62, 24, 42};
  for (int i = 0; i < BOTTOM_ELEMENT_COUNT; i++) {
    widths[i] = defaults[i];
  }
#if defined(PBL_PLATFORM_GABBRO) || defined(PBL_PLATFORM_CHALK)
  char buffer[12];
  GFont font = fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD);

  if (state->heart_rate_bpm > 0) {
    snprintf(buffer, sizeof(buffer), "%u", (unsigned int)state->heart_rate_bpm);
  } else {
    snprintf(buffer, sizeof(buffer), "-");
  }
  widths[BOTTOM_HEART_RATE] = 18 +
      reference_width_from_pixels(rendered_text_width(buffer, font, 34));

  if (state->steps_today > 0) {
    snprintf(buffer, sizeof(buffer), "%d", state->steps_today);
  } else {
    snprintf(buffer, sizeof(buffer), "-");
  }
  widths[BOTTOM_STEPS] = 19 +
      reference_width_from_pixels(rendered_text_width(buffer, font, 62));

  widths[BOTTOM_BATTERY] = state->battery_modality == 0 ? 24 : 30;

  WeatherData weather = weather_get_data();
  if (weather.has_data) {
    snprintf(buffer, sizeof(buffer), "%d\xC2\xB0", weather.temperature);
  } else {
    snprintf(buffer, sizeof(buffer), "--\xC2\xB0");
  }
  widths[BOTTOM_WEATHER] = 22 +
      reference_width_from_pixels(rendered_text_width(buffer, font, 36));
#endif
}

static BottomLayout bottom_layout_create(const RenderState *state) {
  BottomLayout layout = {0};
  bool visible[BOTTOM_ELEMENT_COUNT] = {false};
  int widths[BOTTOM_ELEMENT_COUNT];
  for (int element = 0; element < BOTTOM_ELEMENT_COUNT; element++) {
    visible[element] = bottom_element_is_visible(state, (enum BottomElement)element);
  }
  bottom_element_widths(state, widths);
  bottom_layout_calculate(visible, widths, &layout);
  return layout;
}

void render_watchface(GContext *gContext, Color palettes[], const RenderState *state,
                      struct Flake *flakes,
                      Layer *flake_layers[NUM_FLAKES]) {
  int easter_egg = isEasterEggDay();
  s_render_state = *state;
  if (easter_egg != 0) {
    s_render_state.shape = (easter_egg == 2) ? 11 : s_render_state.shape;
    s_render_state.shape = (easter_egg == 3) ? 12 : s_render_state.shape;
    s_render_state.shape = (easter_egg == 4) ? 13 : s_render_state.shape;
#ifdef PBL_PLATFORM_APLITE
    s_render_state.color = 0;
#else
    s_render_state.color = (easter_egg == 1) ? 15 :
                         (easter_egg == 2) ? 16 :
                         (easter_egg == 3) ? 17 : 18;
#endif
  }

  Color palette = palettes[s_render_state.color];
  s_bottom_layout = bottom_layout_create(&s_render_state);
  draw_background(gContext, 0, GCornerNone, palette);
  if (s_render_state.number > 0 && easter_egg != 4) {
    draw_time_background(gContext, palette, s_render_state.hour, s_render_state.minute);
  }
  draw_clock(gContext, palette, (bool)s_render_state.help_num, s_render_state.shape,
             s_render_state.bullets_number, s_render_state.buffer_time);

  if (s_render_state.bluetooth > 0) {
    draw_bluetooth(gContext, s_render_state.bluetooth, s_render_state.bluetooth_status,
                   s_render_state.battery, s_render_state.battery_level, s_render_state.color,
                   &s_bottom_layout);
  }
  if (s_render_state.battery > 0) {
    draw_battery(gContext, s_render_state.battery, s_render_state.bluetooth,
                 s_render_state.bluetooth_status, s_render_state.battery_level,
                 s_render_state.battery_modality, easter_egg, &s_bottom_layout, palette);
  }
  // Easter keeps its greeting visible even if the regular date is disabled.
  if (s_render_state.date > 0 || easter_egg == 4) {
    draw_date(gContext, palette, s_render_state.date, easter_egg);
  }

  render_layout_draw_health_indicators(gContext, palette, s_render_state.show_heart_rate,
                                        s_render_state.show_steps, s_render_state.heart_rate_bpm,
                                        s_render_state.steps_today, &s_bottom_layout);

#if defined(PBL_PLATFORM_EMERY)
  if (s_render_state.weather_enabled) {
    render_weather_emery(gContext, palette, weather_get_data());
  }
#endif
#if defined(PBL_PLATFORM_GABBRO)
  if (s_render_state.weather_enabled) {
    render_weather_gabbro(gContext, palette, weather_get_data(), &s_bottom_layout);
  }
#endif
#if defined(PBL_PLATFORM_CHALK)
  if (s_render_state.weather_enabled) {
    render_weather_chalk(gContext, palette, weather_get_data(), &s_bottom_layout);
  }
#endif
#if defined(PBL_PLATFORM_DIORITE)
  if (s_render_state.weather_enabled) {
    render_weather_diorite(gContext, palette, weather_get_data());
  }
#endif

  if (easter_egg == 1 || easter_egg == 2 || s_render_state.snow) {
    for (int i = 0; i < NUM_FLAKES; i++) {
      draw_flake(gContext, flake_layers[i], flakes[i]);
    }
  }
}
