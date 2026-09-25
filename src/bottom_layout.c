#include "bottom_layout.h"

#if defined(PBL_PLATFORM_GABBRO)
#define BOTTOM_LAYOUT_WIDTH 184

static int centered_element_start(int available_width, int element_width) {
  return (available_width - element_width) / 2;
}

// Widths are expressed in the 144px reference canvas. They deliberately
// describe the complete reserved slot, not only the currently rendered value.
// Steps and BPM use the agreed maximum examples: 13947 and 180.
static const int s_max_width[BOTTOM_ELEMENT_COUNT] = {
  12, // Bluetooth: largest reference bitmap (10x17, scaled from 14x23)
  32, // Heart rate: heart plus a three-digit BPM value
  42, // Steps: walking icon plus the value 13947
  24, // Battery: the outline is identical for every percentage
  42, // Weather: icon plus the temperature 104
};
#endif

#if defined(PBL_PLATFORM_GABBRO)
static int count_visible(const bool visible[BOTTOM_ELEMENT_COUNT]) {
  int count = 0;
  for (int i = 0; i < BOTTOM_ELEMENT_COUNT; i++) {
    if (visible[i]) {
      count++;
    }
  }
  return count;
}
#endif

#if !defined(PBL_PLATFORM_GABBRO)
static void set_legacy_layout(const bool visible[BOTTOM_ELEMENT_COUNT],
                              BottomLayout *layout) {
  enum BottomElement visible_elements[BOTTOM_ELEMENT_COUNT];
  int slot_x[BOTTOM_ELEMENT_COUNT] = {78, 78, 78, 78, 78};
  int centers[BOTTOM_ELEMENT_COUNT] = {90, 90, 90, 90, 90};
  int slot_y[BOTTOM_ELEMENT_COUNT] = {135, 135, 135, 135, 135};

  for (int element = 0; element < BOTTOM_ELEMENT_COUNT; element++) {
    if (visible[element]) {
      visible_elements[layout->visible_count++] = (enum BottomElement)element;
    }
  }

  if (layout->visible_count == 2) {
    slot_x[0] = 70;
    slot_x[1] = 112;
    centers[0] = 70;
    centers[1] = 112;
  } else if (layout->visible_count == 3) {
    slot_x[0] = 42;
    centers[0] = 47;
    slot_x[1] = 77;
    centers[1] = 90;
    slot_x[2] = 128;
    centers[2] = 140;
  } else if (layout->visible_count == 4) {
    centers[0] = 30;
    centers[1] = 70;
    centers[2] = 110;
    centers[3] = 150;
  } else if (layout->visible_count == 5) {
    centers[0] = 30;
    centers[1] = 70;
    centers[2] = 110;
    centers[3] = 150;
    centers[4] = 90;
  }

  for (int index = 0; index < layout->visible_count; index++) {
    enum BottomElement element = visible_elements[index];
    layout->center[element] = centers[index];
    layout->x[element] = slot_x[index];
    layout->y[element] = slot_y[index];
  }
  if (layout->visible_count == 3) {
    layout->steps_text_y_offset = 5;
  }
  if (layout->visible_count == 4) {
    layout->x[BOTTOM_BLUETOOTH] = 30;
    layout->x[BOTTOM_HEART_RATE] = 62;
    layout->x[BOTTOM_STEPS] = 86;
    layout->x[BOTTOM_BATTERY] = 129;
    layout->steps_text_x_offset = -2;
    layout->steps_text_y_offset = 5;
    layout->y[BOTTOM_BATTERY] += 2;
  }
}
#endif

#if defined(PBL_PLATFORM_GABBRO)
static int popcount(int value) {
  int result = 0;
  while (value != 0) {
    result += value & 1;
    value >>= 1;
  }
  return result;
}

static int row_width(int mask) {
  int width = 0;
  for (int i = 0; i < BOTTOM_ELEMENT_COUNT; i++) {
    if (mask & (1 << i)) {
      width += s_max_width[i];
    }
  }
  return width;
}

static void choose_rows(const bool visible[BOTTOM_ELEMENT_COUNT],
                        int row_masks[2], int count) {
  int visible_mask = 0;
  for (int i = 0; i < BOTTOM_ELEMENT_COUNT; i++) {
    if (visible[i]) {
      visible_mask |= 1 << i;
    }
  }

  if (count <= 3) {
    row_masks[0] = visible_mask;
    row_masks[1] = 0;
    return;
  }

  int first_row_size = count == 4 ? 2 : 3;
  int best_mask = 0;
  int best_difference = 10000;
  for (int mask = 0; mask < (1 << BOTTOM_ELEMENT_COUNT); mask++) {
    if ((mask & visible_mask) != mask || popcount(mask) != first_row_size) {
      continue;
    }
    int other_mask = visible_mask ^ mask;
    int difference = row_width(mask) - row_width(other_mask);
    if (difference < 0) {
      difference = -difference;
    }
    if (difference < best_difference) {
      best_difference = difference;
      best_mask = mask;
    }
  }
  row_masks[0] = best_mask;
  row_masks[1] = visible_mask ^ best_mask;
}

static void place_row(const int mask, int row, int y, BottomLayout *layout) {
  const int margin = 8;
  const int available_width = BOTTOM_LAYOUT_WIDTH - (2 * margin);
  int count = popcount(mask);
  int total_width = row_width(mask);
  int gap = count > 1 ? (available_width - total_width) / (count - 1) : 0;
  // With one element the whole available area is the slot: do not anchor it
  // to the left margin just because there is no inter-element gap to compute.
  int x = count == 1 ? centered_element_start(BOTTOM_LAYOUT_WIDTH, total_width) : margin;

  for (int element = 0; element < BOTTOM_ELEMENT_COUNT; element++) {
    if (!(mask & (1 << element))) {
      continue;
    }
    int width = s_max_width[element];
    int center = x + width / 2;
    layout->center[element] = center;
    layout->slot_width[element] = width;
    layout->row[element] = row;
    layout->y[element] = y;
    // x is the left edge of the reserved slot. Each renderer centres its
    // actual artwork inside this slot using its own drawing width.
    layout->x[element] = x;
    x += width + gap;
  }
}
#endif

void bottom_layout_calculate(const bool visible[BOTTOM_ELEMENT_COUNT],
                             BottomLayout *layout) {
  *layout = (BottomLayout){0};

#if defined(PBL_PLATFORM_GABBRO)
  int count = count_visible(visible);
  layout->visible_count = count;
  if (count == 0) {
    return;
  }

  int row_masks[2] = {0, 0};
  choose_rows(visible, row_masks, count);
  if (row_masks[1] == 0) {
    place_row(row_masks[0], 0, 135, layout);
  } else {
    // Two rows share the lower band and use the same maximum-slot rules.
    place_row(row_masks[0], 0, 119, layout);
    place_row(row_masks[1], 1, 151, layout);
  }
#else
  set_legacy_layout(visible, layout);
#endif
}
