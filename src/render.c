#include "render.h"
#include "render_layout.h"

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
    default:
      return false;
  }
}

static int bottom_element_x_from_center(enum BottomElement element, int center_x) {
  static const int left_offset[BOTTOM_ELEMENT_COUNT] = {5, 26, 13, 12};
  return center_x - left_offset[element];
}

static BottomLayout bottom_layout_create(const RenderState *state) {
  BottomLayout layout = {0};
  enum BottomElement visible[BOTTOM_ELEMENT_COUNT];
  for (int element = 0; element < BOTTOM_ELEMENT_COUNT; element++) {
    if (bottom_element_is_visible(state, (enum BottomElement)element)) {
      visible[layout.visible_count++] = (enum BottomElement)element;
    }
  }

  // Defaults for a single item: centre its visual group on the lower row.
  int centers[BOTTOM_ELEMENT_COUNT] = {90, 90, 90, 90};
  int row_y = 135;
  if (layout.visible_count == 2) {
    centers[0] = 70;
    centers[1] = 112;
  } else if (layout.visible_count == 3) {
    centers[0] = 47;
    centers[1] = 90;
    centers[2] = 140;
  } else if (layout.visible_count == 4) {
    // Four equally spaced centres in the lower band.
    centers[0] = 30;
    centers[1] = 70;
    centers[2] = 110;
    centers[3] = 150;
  }

  for (int index = 0; index < layout.visible_count; index++) {
    enum BottomElement element = visible[index];
    layout.x[element] = bottom_element_x_from_center(element, centers[index]);
    layout.y[element] = row_y;
  }

  // Preserve the established two-item Bluetooth/battery coordinates exactly.
  if (layout.visible_count == 2 &&
      bottom_element_is_visible(state, BOTTOM_BLUETOOTH) &&
      bottom_element_is_visible(state, BOTTOM_BATTERY)) {
    layout.x[BOTTOM_BLUETOOTH] = 65;
    layout.y[BOTTOM_BLUETOOTH] = 140;
    layout.x[BOTTOM_BATTERY] = 100;
    layout.y[BOTTOM_BATTERY] = 142;
  }

  // Preserve the approved three-item Bluetooth/steps/battery layout.
  if (layout.visible_count == 3 &&
      bottom_element_is_visible(state, BOTTOM_BLUETOOTH) &&
      bottom_element_is_visible(state, BOTTOM_STEPS) &&
      bottom_element_is_visible(state, BOTTOM_BATTERY)) {
    layout.x[BOTTOM_BLUETOOTH] = 42;
    layout.y[BOTTOM_BLUETOOTH] = 135;
    layout.x[BOTTOM_STEPS] = 77;
    layout.y[BOTTOM_STEPS] = 135;
    layout.steps_text_y_offset = 5;
    layout.x[BOTTOM_BATTERY] = 128;
    layout.y[BOTTOM_BATTERY] = 137;
  }
  if (layout.visible_count == 4) {
    // These are origins, rather than centres: compensate for each element's
    // different visual width so their visible icons/groups are evenly spaced.
    layout.x[BOTTOM_BLUETOOTH] = 30;
    layout.x[BOTTOM_HEART_RATE] = 62;
    layout.x[BOTTOM_STEPS] = 86;
    layout.x[BOTTOM_BATTERY] = 129;
    layout.steps_text_x_offset = -2;
    layout.steps_text_y_offset = 5;
    layout.y[BOTTOM_BATTERY] += 2;
  }
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

  if (easter_egg == 1 || easter_egg == 2 || render_state.snow) {
    for (int i = 0; i < NUM_FLAKES; i++) {
      draw_flake(gContext, flake_layers[i], flakes[i]);
    }
  }
}
