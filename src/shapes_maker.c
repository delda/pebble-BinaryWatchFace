#include <pebble.h>
#include <math.h>
#include "shapes_maker.h"
#include "common.h"

#include "settings.c"

static struct Flake tmp;

GPathInfo *draw_star(int number_of_sides, int w, int h, int radius){
  if(DEBUG) APP_LOG(APP_LOG_LEVEL_INFO, "[%s] %s()", logTime(), __func__);

  number_of_sides = 5;
  GPathInfo *shape;
  shape = (GPathInfo *) malloc (sizeof(GPathInfo));
  GPoint *inPoints, *outPoints, *points;
  inPoints = (GPoint *) calloc (number_of_sides, sizeof(GPoint));
  outPoints = (GPoint *) calloc (number_of_sides, sizeof(GPoint));
  points = (GPoint *) calloc (number_of_sides*2, sizeof(GPoint));
  int32_t outAngle = 0;
  int32_t inAngle = TRIG_MAX_ANGLE / (number_of_sides * 2);
  int outRadius = radius;
  int inRadius = radius / 2;
  for(int i=0; i<number_of_sides; i++){
    outPoints[i].x = (int16_t)(sin_lookup(outAngle) * (int32_t)outRadius / TRIG_MAX_RATIO) + w;
    outPoints[i].y = (int16_t)(-cos_lookup(outAngle) * (int32_t)outRadius / TRIG_MAX_RATIO) + h;
//    points[(number_of_sides-i)%number_of_sides].x = (int16_t)(sin_lookup(-outAngle) * (int32_t)outRadius / TRIG_MAX_RATIO) + w;
//    points[(number_of_sides-i)%number_of_sides].y = (int16_t)(-cos_lookup(-outAngle) * (int32_t)outRadius / TRIG_MAX_RATIO) + h;
    outAngle += (int32_t)(TRIG_MAX_ANGLE / number_of_sides);
  }
  for(int i=0; i<number_of_sides; i++){
    inPoints[i].x = (int16_t)(sin_lookup(inAngle) * (int32_t)inRadius / TRIG_MAX_RATIO) + w;
    inPoints[i].y = (int16_t)(-cos_lookup(inAngle) * (int32_t)inRadius / TRIG_MAX_RATIO) + h;
//    points[(number_of_sides-i)%number_of_sides].x = (int16_t)(sin_lookup(-inAngle) * (int32_t)inRadius / TRIG_MAX_RATIO) + w;
//    points[(number_of_sides-i)%number_of_sides].y = (int16_t)(-cos_lookup(-inAngle) * (int32_t)inRadius / TRIG_MAX_RATIO) + h;
    inAngle += (int32_t)(TRIG_MAX_ANGLE / number_of_sides);
//    printf("%lu, %lu", outAngle, inAngle);
  }
  for(int i=0; i<number_of_sides; i++){
    points[i*2] = outPoints[i];
    points[i*2+1] = inPoints[i];
  }
  shape->num_points = number_of_sides * 2;
  shape->points = points;
  free(inPoints);
  free(outPoints);
  return shape;
}

GPathInfo *draw_regular_shape(int number_of_sides, int w, int h, int radius){
  if(DEBUG) APP_LOG(APP_LOG_LEVEL_INFO, "[%s] %s()", logTime(), __func__);

  GPathInfo *shape;
  shape = (GPathInfo *) malloc (sizeof(GPathInfo));
  GPoint *points;
  points = (GPoint *) calloc (number_of_sides, sizeof(GPoint));
  int32_t secondAngle = 0;
  for(int i=0; i<=ceil((number_of_sides+1)/2); i++){
    points[i].x = (int16_t)(sin_lookup(secondAngle) * (int32_t)radius / TRIG_MAX_RATIO) + w;
    points[i].y = (int16_t)(-cos_lookup(secondAngle) * (int32_t)radius / TRIG_MAX_RATIO) + h;
    points[(number_of_sides-i)%number_of_sides].x = (int16_t)(sin_lookup(-secondAngle) * (int32_t)radius / TRIG_MAX_RATIO) + w;
    points[(number_of_sides-i)%number_of_sides].y = (int16_t)(-cos_lookup(-secondAngle) * (int32_t)radius / TRIG_MAX_RATIO) + h;
    secondAngle += (int32_t)(TRIG_MAX_ANGLE / number_of_sides);
  }
  shape->num_points = number_of_sides;
  shape->points = points;
  return shape;
}

static void draw_and_destroy_shape(GContext *gContext, GPathInfo *shape) {
  if (shape == NULL) {
    return;
  }

  GPath *path = gpath_create(shape);
  if (path != NULL) {
    gpath_draw_filled(gContext, path);
    gpath_destroy(path);
  }

  free((void *)shape->points);
  free(shape);
}

static int layout_value(int value) {
#ifdef PBL_PLATFORM_GABBRO
  return (value * 13 + 4) / 9;
#elif defined(PBL_PLATFORM_EMERY)
  // Emery is 228px tall, versus Basalt's 168px design canvas.  Use one
  // uniform ratio so the clock's geometry is enlarged without distortion.
  return (value * 19 + 7) / 14;
#else
  return value;
#endif
}

static int layout_x(int value) {
#ifdef PBL_PLATFORM_EMERY
  // Centring the 195px-wide scaled canvas leaves a balanced 2px margin.
  return layout_value(value) + 2;
#else
  return layout_value(value);
#endif
}

static GPoint layout_point(GPoint point) {
  return GPoint(layout_x(point.x), layout_value(point.y));
}

void draw_shape(int shape, bool is_active, int currentWidth, int currentHeight, GContext *gContext, GColor strokeColor, GColor fillColor){
  if(DEBUG) APP_LOG(APP_LOG_LEVEL_INFO, "[%s] %s()", logTime(), __func__);

  currentWidth = layout_x(currentWidth);
  currentHeight = layout_value(currentHeight);

  int numberOfSides = 0;
  int border = 0;
  switch(shape){
    case 13: {  // Easter egg
      int w = currentWidth;
      int h = currentHeight;
      if (is_active) {
        int fracture_y_offset = layout_value(2);
        // The opened egg is the lower half of the normal egg.  Its broken
        // edge follows a zig-zag so the original outline remains visible
        // instead of ending in a flat, straight cut.
        GPathInfo outer_bottom = (GPathInfo){.num_points = 17, .points = (GPoint []){
          {w-layout_value(7), h-layout_value(5)-fracture_y_offset},
          {w-layout_value(9), h+layout_value(3)},
          {w-layout_value(7), h+layout_value(8)},
          {w-layout_value(3), h+layout_value(11)},
          {w+layout_value(3), h+layout_value(11)},
          {w+layout_value(7), h+layout_value(8)},
          {w+layout_value(9), h+layout_value(3)},
          {w+layout_value(5), h-layout_value(5)-fracture_y_offset},
          {w+layout_value(5), h-layout_value(2)-fracture_y_offset},
          {w+layout_value(4), h-layout_value(5)-fracture_y_offset},
          {w+layout_value(3), h-layout_value(2)-fracture_y_offset},
          {w+layout_value(2), h-layout_value(4)-fracture_y_offset},
          {w+layout_value(1), h-layout_value(1)-fracture_y_offset},
          {w-layout_value(1), h-layout_value(4)-fracture_y_offset},
          {w-layout_value(3), h-fracture_y_offset},
          {w-layout_value(4), h-layout_value(2)-fracture_y_offset},
          {w-layout_value(6), h+layout_value(2)-fracture_y_offset}
        }};
        GPathInfo inner_bottom = (GPathInfo){.num_points = 14, .points = (GPoint []){
          {w-layout_value(5), h-layout_value(2)-fracture_y_offset},
          {w-layout_value(7), h+layout_value(3)},
          {w-layout_value(5), h+layout_value(6)},
          {w-layout_value(2), h+layout_value(8)},
          {w+layout_value(2), h+layout_value(8)},
          {w+layout_value(5), h+layout_value(6)},
          {w+layout_value(7), h+layout_value(3)},
          {w+layout_value(3), h-layout_value(2)-fracture_y_offset},
          {w+layout_value(4), h-fracture_y_offset},
          {w+layout_value(2), h-layout_value(2)-fracture_y_offset},
          {w, h+layout_value(1)-fracture_y_offset},
          {w-layout_value(1), h-layout_value(1)-fracture_y_offset},
          {w-layout_value(3), h+layout_value(2)-fracture_y_offset},
          {w-layout_value(4), h-fracture_y_offset}
        }};
        GPoint cut_points[] = {
          {w-layout_value(7), h-layout_value(2)-fracture_y_offset},
          {w-layout_value(6), h+layout_value(2)-fracture_y_offset},
          {w-layout_value(4), h-layout_value(2)-fracture_y_offset},
          {w-layout_value(3), h-fracture_y_offset},
          {w-layout_value(1), h-layout_value(2)-fracture_y_offset},
          {w+layout_value(1), h-layout_value(1)-fracture_y_offset},
          {w+layout_value(2), h-layout_value(4)-fracture_y_offset},
          {w+layout_value(3), h-layout_value(2)-fracture_y_offset},
          {w+layout_value(4), h-layout_value(5)-fracture_y_offset},
          {w+layout_value(5), h-layout_value(2)-fracture_y_offset},
          {w+layout_value(5), h-layout_value(2)-fracture_y_offset}
        };
        GPath *path;
        graphics_context_set_fill_color(gContext, strokeColor);
        path = gpath_create(&outer_bottom);
        gpath_draw_filled(gContext, path);
        gpath_destroy(path);
        graphics_context_set_fill_color(gContext, fillColor);
        path = gpath_create(&inner_bottom);
        gpath_draw_filled(gContext, path);
        gpath_destroy(path);
        // Draw the whole fracture last so no section is obscured by the fill.
        graphics_context_set_stroke_color(gContext, strokeColor);
        graphics_context_set_stroke_width(gContext, layout_value(1));
        for (unsigned int i = 0; i < ARRAY_LENGTH(cut_points) - 1; i++) {
          graphics_draw_line(gContext, cut_points[i], cut_points[i + 1]);
        }
        graphics_context_set_stroke_width(gContext, 1);
        break;
      }
      GPathInfo outline = (GPathInfo){.num_points = 11, .points = (GPoint []){
        {w, h-layout_value(10)},
        {w-layout_value(5), h-layout_value(8)},
        {w-layout_value(8), h-layout_value(3)},
        {w-layout_value(9), h+layout_value(3)},
        {w-layout_value(7), h+layout_value(8)},
        {w-layout_value(3), h+layout_value(11)},
        {w+layout_value(3), h+layout_value(11)},
        {w+layout_value(7), h+layout_value(8)},
        {w+layout_value(9), h+layout_value(3)},
        {w+layout_value(8), h-layout_value(3)},
        {w+layout_value(5), h-layout_value(8)}
      }};
      GPathInfo inner = (GPathInfo){.num_points = 11, .points = (GPoint []){
        {w, h-layout_value(7)},
        {w-layout_value(3), h-layout_value(6)},
        {w-layout_value(6), h-layout_value(2)},
        {w-layout_value(7), h+layout_value(3)},
        {w-layout_value(5), h+layout_value(6)},
        {w-layout_value(2), h+layout_value(8)},
        {w+layout_value(2), h+layout_value(8)},
        {w+layout_value(5), h+layout_value(6)},
        {w+layout_value(7), h+layout_value(3)},
        {w+layout_value(6), h-layout_value(2)},
        {w+layout_value(3), h-layout_value(6)}
      }};
      GPath *path;
      graphics_context_set_fill_color(gContext, strokeColor);
      path = gpath_create(&outline);
      gpath_draw_filled(gContext, path);
      gpath_destroy(path);
      graphics_context_set_fill_color(gContext, fillColor);
      path = gpath_create(&inner);
      gpath_draw_filled(gContext, path);
      gpath_destroy(path);
      break;
    }
    case 12: {  // heart
      int w = currentWidth;
      int h = currentHeight;
      GPathInfo outline = (GPathInfo){.num_points = 12, .points = (GPoint []){
        {w, h+layout_value(11)},
        {w-layout_value(10), h+layout_value(1)},
        {w-layout_value(10), h-layout_value(4)},
        {w-layout_value(8), h-layout_value(8)},
        {w-layout_value(4), h-layout_value(9)},
        {w, h-layout_value(5)},
        {w+layout_value(4), h-layout_value(9)},
        {w+layout_value(8), h-layout_value(8)},
        {w+layout_value(10), h-layout_value(4)},
        {w+layout_value(10), h+layout_value(1)},
        {w, h+layout_value(11)},
        {w, h+layout_value(11)}
      }};
      GPathInfo inner = (GPathInfo){.num_points = 12, .points = (GPoint []){
        {w, h+layout_value(8)},
        {w-layout_value(8), h},
        {w-layout_value(8), h-layout_value(3)},
        {w-layout_value(6), h-layout_value(6)},
        {w-layout_value(3), h-layout_value(7)},
        {w, h-layout_value(3)},
        {w+layout_value(3), h-layout_value(7)},
        {w+layout_value(6), h-layout_value(6)},
        {w+layout_value(8), h-layout_value(3)},
        {w+layout_value(8), h},
        {w, h+layout_value(8)},
        {w, h+layout_value(8)}
      }};
      GPath *path;
      graphics_context_set_fill_color(gContext, strokeColor);
      path = gpath_create(&outline);
      gpath_draw_filled(gContext, path);
      gpath_destroy(path);
      graphics_context_set_fill_color(gContext, fillColor);
      path = gpath_create(&inner);
      gpath_draw_filled(gContext, path);
      gpath_destroy(path);
      break;
    }
    case 11:   // star
      numberOfSides = 5;
      graphics_context_set_fill_color(gContext, strokeColor);
      draw_and_destroy_shape(gContext, draw_star(numberOfSides, currentWidth, currentHeight, layout_value(12)));
      graphics_context_set_fill_color(gContext, fillColor);
      draw_and_destroy_shape(gContext, draw_star(numberOfSides, currentWidth, currentHeight, layout_value(8)));
      break;
    case 3:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
      switch(shape){
        case 3: numberOfSides = 4; border = 3; break;
        case 6: numberOfSides = 3; border = 4; break;
        case 7: numberOfSides = 5; border = 2; break;
        case 8: numberOfSides = 6; border = 2; break;
        case 9: numberOfSides = 7; border = 2; break;
        case 10: numberOfSides = 8; border = 2; break;
      }
      #if !defined(PBL_PLATFORM_CHALK) && !defined(PBL_PLATFORM_GABBRO)
        currentHeight += 2;
      #endif
      graphics_context_set_fill_color(gContext, strokeColor);
      draw_and_destroy_shape(gContext, draw_regular_shape(numberOfSides, currentWidth, currentHeight, layout_value(10)));
      graphics_context_set_fill_color(gContext, fillColor);
      draw_and_destroy_shape(gContext, draw_regular_shape(numberOfSides, currentWidth, currentHeight, layout_value(10-border)));
      break;
    case 5:     // cross
      graphics_context_set_fill_color(gContext, strokeColor);
      int w = currentWidth;
      int h = currentHeight;
      gpath_draw_filled(gContext, gpath_create(&(GPathInfo){.num_points=13, .points=(GPoint []){
        {w-layout_value(10), h-layout_value(2)},
        {w-layout_value(5) , h-layout_value(3)},
        {w-layout_value(5) , h-layout_value(8)},
        {w+layout_value(5) , h-layout_value(8)},
        {w+layout_value(5) , h-layout_value(3)},
        {w+layout_value(10), h-layout_value(3)},
        {w+layout_value(10), h+layout_value(6)},
        {w+layout_value(5) , h+layout_value(6)},
        {w+layout_value(5) , h+layout_value(11)},
        {w-layout_value(5) , h+layout_value(11)},
        {w-layout_value(5) , h+layout_value(7)},
        {w-layout_value(10), h+layout_value(7)},
        {w-layout_value(10), h-layout_value(2)}
      }}));
      graphics_context_set_fill_color(gContext, fillColor);
      gpath_draw_filled(gContext, gpath_create(&(GPathInfo){.num_points=13, .points=(GPoint []){
        {w-layout_value(8), h},
        {w-layout_value(3), h-layout_value(1)},
        {w-layout_value(3), h-layout_value(6)},
        {w+layout_value(3), h-layout_value(6)},
        {w+layout_value(3), h-layout_value(1)},
        {w+layout_value(8), h-layout_value(1)},
        {w+layout_value(8), h+layout_value(4)},
        {w+layout_value(3), h+layout_value(4)},
        {w+layout_value(3), h+layout_value(9)},
        {w-layout_value(3), h+layout_value(9)},
        {w-layout_value(3), h+layout_value(5)},
        {w-layout_value(8), h+layout_value(5)},
        {w-layout_value(8), h-layout_value(1)}
      }}));
      break;
    case 4:     // triangle
      graphics_context_set_fill_color(gContext, strokeColor);
      gpath_draw_filled(gContext, gpath_create(&(GPathInfo){.num_points=3, .points=(GPoint []){{currentWidth, currentHeight-layout_value(11)}, {currentWidth+layout_value(9), currentHeight+layout_value(8)}, {currentWidth-layout_value(8), currentHeight+layout_value(8)}}}));
      graphics_context_set_fill_color(gContext, fillColor);
      gpath_draw_filled(gContext, gpath_create(&(GPathInfo){.num_points=3, .points=(GPoint []){{currentWidth, currentHeight-layout_value(7)}, {currentWidth+layout_value(6), currentHeight+layout_value(6)}, {currentWidth-layout_value(5), currentHeight+layout_value(6)}}}));
      break;
    case 2:     // rectangle
      graphics_context_set_fill_color(gContext, fillColor);
      graphics_context_set_stroke_color(gContext, strokeColor);
      graphics_fill_rect(gContext, (GRect){.origin={currentWidth-layout_value(6), currentHeight-layout_value(7)}, .size={layout_value(12),layout_value(19)}}, 0, GCornerNone);
      graphics_draw_rect(gContext, (GRect){.origin={currentWidth-layout_value(6), currentHeight-layout_value(7)}, .size={layout_value(12),layout_value(19)}});
      graphics_draw_rect(gContext, (GRect){.origin={currentWidth-layout_value(5), currentHeight-layout_value(6)}, .size={layout_value(10),layout_value(17)}});
      break;
    case 1:     // square
      graphics_context_set_fill_color(gContext, strokeColor);
      graphics_fill_rect(gContext, (GRect){.origin={currentWidth-layout_value(8), currentHeight-layout_value(6)}, .size={layout_value(16),layout_value(16)}}, 0, GCornerNone);
      graphics_context_set_fill_color(gContext, fillColor);
      graphics_fill_rect(gContext, (GRect){.origin={currentWidth-layout_value(6), currentHeight-layout_value(4)}, .size={layout_value(12),layout_value(12)}}, 0, GCornerNone);
      break;
    case 0:
    default:    // disk
      graphics_context_set_fill_color(gContext, strokeColor);
      graphics_fill_circle(gContext, GPoint(currentWidth, currentHeight), layout_value(8));
      graphics_context_set_fill_color(gContext, fillColor);
      graphics_fill_circle(gContext, GPoint(currentWidth, currentHeight), layout_value(6));
      break;
  }
}

// Gabbro and Emery scale their respective reference layouts at the drawing
// boundary, keeping every element in the same visual relationship.
static GRect layout_rect(GRect rect) {
#if defined(PBL_PLATFORM_GABBRO) || defined(PBL_PLATFORM_EMERY)
  return GRect(layout_x(rect.origin.x),
               layout_value(rect.origin.y),
               layout_value(rect.size.w),
               layout_value(rect.size.h));
#else
  return rect;
#endif
}

static void fill_number_rect(GContext *gContext, GRect rect,
                             uint16_t corner_radius, GCornerMask corner_mask) {
  graphics_fill_rect(gContext, layout_rect(rect), corner_radius, corner_mask);
}

#define graphics_fill_rect(context, rect, radius, mask) \
  fill_number_rect(context, rect, radius, mask)
void fill_number(int number, GPoint position, GContext *gContext){
  if(DEBUG) APP_LOG(APP_LOG_LEVEL_INFO, "[%s] %s()", logTime(), __func__);

  switch(number){
    case 0:
      graphics_fill_rect(gContext, GRect(position.x +  0, position.y +  0, 60, 14), 0, GCornerNone);
      graphics_fill_rect(gContext, GRect(position.x + 46, position.y +  0, 14, 90), 0, GCornerNone);
      graphics_fill_rect(gContext, GRect(position.x +  0, position.y + 76, 60, 14), 0, GCornerNone);
      graphics_fill_rect(gContext, GRect(position.x +  0, position.y +  0, 14, 90), 0, GCornerNone);
      break;
    case 1:
      graphics_fill_rect(gContext, GRect(position.x +  4, position.y +  0, 32, 14), 0, GCornerNone);
      graphics_fill_rect(gContext, GRect(position.x + 24, position.y +  0, 14, 90), 0, GCornerNone);
      graphics_fill_rect(gContext, GRect(position.x +  4, position.y + 76, 52, 14), 0, GCornerNone);
      break;
    case 2:
      graphics_fill_rect(gContext, GRect(position.x +  0, position.y +  0, 60, 14), 0, GCornerNone);
      graphics_fill_rect(gContext, GRect(position.x +  0, position.y + 36, 60, 14), 0, GCornerNone);
      graphics_fill_rect(gContext, GRect(position.x +  0, position.y + 76, 60, 14), 0, GCornerNone);
      graphics_fill_rect(gContext, GRect(position.x + 46, position.y +  0, 14, 50), 0, GCornerNone);
      graphics_fill_rect(gContext, GRect(position.x +  0, position.y + 36, 14, 50), 0, GCornerNone);
      graphics_fill_rect(gContext, GRect(position.x +  0, position.y +  0, 14, 24), 0, GCornerNone);
      break;
    case 3:
      graphics_fill_rect(gContext, GRect(position.x +  0, position.y +  0, 60, 14), 0, GCornerNone);
      graphics_fill_rect(gContext, GRect(position.x +  0, position.y + 76, 60, 14), 0, GCornerNone);
      graphics_fill_rect(gContext, GRect(position.x +  8, position.y + 36, 52, 14), 0, GCornerNone);
      graphics_fill_rect(gContext, GRect(position.x + 46, position.y +  0, 14, 90), 0, GCornerNone);
      break;
    case 4:
      graphics_fill_rect(gContext, GRect(position.x +  0, position.y +  0, 14, 52), 0, GCornerNone);
      graphics_fill_rect(gContext, GRect(position.x + 46, position.y +  0, 14, 90), 0, GCornerNone);
      graphics_fill_rect(gContext, GRect(position.x +  0, position.y + 38, 60, 14), 0, GCornerNone);
      break;
    case 5:
      graphics_fill_rect(gContext, GRect(position.x +  0, position.y +  0, 60, 14), 0, GCornerNone);
      graphics_fill_rect(gContext, GRect(position.x +  0, position.y + 36, 60, 14), 0, GCornerNone);
      graphics_fill_rect(gContext, GRect(position.x +  0, position.y + 76, 60, 14), 0, GCornerNone);
      graphics_fill_rect(gContext, GRect(position.x +  0, position.y +  0, 14, 50), 0, GCornerNone);
      graphics_fill_rect(gContext, GRect(position.x + 46, position.y + 36, 14, 50), 0, GCornerNone);
      graphics_fill_rect(gContext, GRect(position.x +  0, position.y + 66, 14, 24), 0, GCornerNone);
      break;
    case 6:
      graphics_fill_rect(gContext, GRect(position.x +  0, position.y +  0, 60, 14), 0, GCornerNone);
      graphics_fill_rect(gContext, GRect(position.x +  0, position.y + 36, 60, 14), 0, GCornerNone);
      graphics_fill_rect(gContext, GRect(position.x +  0, position.y + 76, 60, 14), 0, GCornerNone);
      graphics_fill_rect(gContext, GRect(position.x +  0, position.y +  0, 14, 90), 0, GCornerNone);
      graphics_fill_rect(gContext, GRect(position.x + 46, position.y + 36, 14, 50), 0, GCornerNone);
      break;
    case 7:
      graphics_fill_rect(gContext, GRect(position.x +  0, position.y +  0, 60, 14), 0, GCornerNone);
      graphics_fill_rect(gContext, GRect(position.x + 46, position.y +  0, 14, 90), 0, GCornerNone);
      graphics_fill_rect(gContext, GRect(position.x +  0, position.y +  0, 14, 24), 0, GCornerNone);
      break;
    case 8:
      graphics_fill_rect(gContext, GRect(position.x +  0, position.y +  0, 60, 14), 0, GCornerNone);
      graphics_fill_rect(gContext, GRect(position.x +  0, position.y + 36, 60, 14), 0, GCornerNone);
      graphics_fill_rect(gContext, GRect(position.x +  0, position.y + 76, 60, 14), 0, GCornerNone);
      graphics_fill_rect(gContext, GRect(position.x +  0, position.y +  0, 14, 90), 0, GCornerNone);
      graphics_fill_rect(gContext, GRect(position.x + 46, position.y +  0, 14, 90), 0, GCornerNone);
      break;
    case 9:
      graphics_fill_rect(gContext, GRect(position.x +  0, position.y +  0, 60, 14), 0, GCornerNone);
      graphics_fill_rect(gContext, GRect(position.x +  0, position.y + 36, 60, 14), 0, GCornerNone);
      graphics_fill_rect(gContext, GRect(position.x +  0, position.y + 76, 60, 14), 0, GCornerNone);
      graphics_fill_rect(gContext, GRect(position.x + 46, position.y +  0, 14, 90), 0, GCornerNone);
      graphics_fill_rect(gContext, GRect(position.x +  0, position.y +  0, 14, 50), 0, GCornerNone);
      break;
  }
}
#undef graphics_fill_rect

void draw_number(int number, GPoint position, GContext *gContext){
  if(DEBUG) APP_LOG(APP_LOG_LEVEL_INFO, "[%s] %s()", logTime(), __func__);

  switch(number){
    case 0:
      graphics_draw_line(gContext, GPoint(position.x +  0, position.y +  0), GPoint(position.x + 60, position.y +  0));
      graphics_draw_line(gContext, GPoint(position.x + 60, position.y +  0), GPoint(position.x + 60, position.y + 90));
      graphics_draw_line(gContext, GPoint(position.x + 60, position.y + 90), GPoint(position.x +  0, position.y + 90));
      graphics_draw_line(gContext, GPoint(position.x +  0, position.y + 90), GPoint(position.x +  0, position.y +  0));
      graphics_draw_line(gContext, GPoint(position.x + 15, position.y + 15), GPoint(position.x + 45, position.y + 15));
      graphics_draw_line(gContext, GPoint(position.x + 45, position.y + 15), GPoint(position.x + 45, position.y + 75));
      graphics_draw_line(gContext, GPoint(position.x + 45, position.y + 75), GPoint(position.x + 15, position.y + 75));
      graphics_draw_line(gContext, GPoint(position.x + 15, position.y + 75), GPoint(position.x + 15, position.y + 15));
      break;
    case 1:
      graphics_draw_line(gContext, GPoint(position.x + 10, position.y +  0), GPoint(position.x + 34, position.y +  0));
      graphics_draw_line(gContext, GPoint(position.x + 34, position.y +  0), GPoint(position.x + 34, position.y + 75));
      graphics_draw_line(gContext, GPoint(position.x + 34, position.y + 75), GPoint(position.x + 52, position.y + 75));
      graphics_draw_line(gContext, GPoint(position.x + 52, position.y + 75), GPoint(position.x + 52, position.y + 90));
      graphics_draw_line(gContext, GPoint(position.x + 52, position.y + 90), GPoint(position.x +  4, position.y + 90));
      graphics_draw_line(gContext, GPoint(position.x +  4, position.y + 90), GPoint(position.x +  4, position.y + 75));
      graphics_draw_line(gContext, GPoint(position.x +  4, position.y + 75), GPoint(position.x + 22, position.y + 75));
      graphics_draw_line(gContext, GPoint(position.x + 22, position.y + 75), GPoint(position.x + 22, position.y + 15));
      graphics_draw_line(gContext, GPoint(position.x + 22, position.y + 15), GPoint(position.x + 10, position.y + 15));
      graphics_draw_line(gContext, GPoint(position.x + 10, position.y + 15), GPoint(position.x + 10, position.y +  0));
      break;
    case 2:
      graphics_draw_line(gContext, GPoint(position.x +  0, position.y +  0), GPoint(position.x + 60, position.y +  0));
      graphics_draw_line(gContext, GPoint(position.x + 60, position.y +  0), GPoint(position.x + 60, position.y + 52));
      graphics_draw_line(gContext, GPoint(position.x + 60, position.y + 52), GPoint(position.x + 14, position.y + 52));
      graphics_draw_line(gContext, GPoint(position.x + 14, position.y + 52), GPoint(position.x + 14, position.y + 72));
      graphics_draw_line(gContext, GPoint(position.x + 14, position.y + 72), GPoint(position.x + 60, position.y + 72));
      graphics_draw_line(gContext, GPoint(position.x + 60, position.y + 72), GPoint(position.x + 60, position.y + 90));
      graphics_draw_line(gContext, GPoint(position.x + 60, position.y + 90), GPoint(position.x +  0, position.y + 90));
      graphics_draw_line(gContext, GPoint(position.x +  0, position.y + 90), GPoint(position.x +  0, position.y + 36));
      graphics_draw_line(gContext, GPoint(position.x +  0, position.y + 36), GPoint(position.x + 46, position.y + 36));
      graphics_draw_line(gContext, GPoint(position.x + 46, position.y + 36), GPoint(position.x + 46, position.y + 14));
      graphics_draw_line(gContext, GPoint(position.x + 46, position.y + 14), GPoint(position.x +  0, position.y + 14));
      graphics_draw_line(gContext, GPoint(position.x +  0, position.y + 14), GPoint(position.x +  0, position.y +  0));
      break;
    case 3:
      graphics_draw_line(gContext, GPoint(position.x +  0, position.y +  0), GPoint(position.x + 60, position.y +  0));
      graphics_draw_line(gContext, GPoint(position.x + 60, position.y +  0), GPoint(position.x + 60, position.y + 90));
      graphics_draw_line(gContext, GPoint(position.x + 60, position.y + 90), GPoint(position.x +  0, position.y + 90));
      graphics_draw_line(gContext, GPoint(position.x +  0, position.y + 90), GPoint(position.x +  0, position.y + 75));
      graphics_draw_line(gContext, GPoint(position.x +  0, position.y + 75), GPoint(position.x + 46, position.y + 75));
      graphics_draw_line(gContext, GPoint(position.x + 46, position.y + 75), GPoint(position.x + 46, position.y + 54));
      graphics_draw_line(gContext, GPoint(position.x + 46, position.y + 54), GPoint(position.x +  8, position.y + 54));
      graphics_draw_line(gContext, GPoint(position.x +  8, position.y + 54), GPoint(position.x +  8, position.y + 36));
      graphics_draw_line(gContext, GPoint(position.x +  8, position.y + 36), GPoint(position.x + 46, position.y + 36));
      graphics_draw_line(gContext, GPoint(position.x + 46, position.y + 36), GPoint(position.x + 46, position.y + 14));
      graphics_draw_line(gContext, GPoint(position.x + 46, position.y + 14), GPoint(position.x +  0, position.y + 14));
      graphics_draw_line(gContext, GPoint(position.x +  0, position.y + 14), GPoint(position.x +  0, position.y +  0));
      break;
    case 4:
      graphics_draw_line(gContext, GPoint(position.x +  0, position.y +  0), GPoint(position.x + 14, position.y +  0));
      graphics_draw_line(gContext, GPoint(position.x + 14, position.y +  0), GPoint(position.x + 14, position.y + 36));
      graphics_draw_line(gContext, GPoint(position.x + 14, position.y + 36), GPoint(position.x + 46, position.y + 36));
      graphics_draw_line(gContext, GPoint(position.x + 46, position.y + 36), GPoint(position.x + 46, position.y +  0));
      graphics_draw_line(gContext, GPoint(position.x + 46, position.y +  0), GPoint(position.x + 60, position.y +  0));
      graphics_draw_line(gContext, GPoint(position.x + 60, position.y +  0), GPoint(position.x + 60, position.y + 90));
      graphics_draw_line(gContext, GPoint(position.x + 60, position.y + 90), GPoint(position.x + 46, position.y + 90));
      graphics_draw_line(gContext, GPoint(position.x + 46, position.y + 90), GPoint(position.x + 46, position.y + 54));
      graphics_draw_line(gContext, GPoint(position.x + 46, position.y + 54), GPoint(position.x +  0, position.y + 54));
      graphics_draw_line(gContext, GPoint(position.x +  0, position.y + 54), GPoint(position.x +  0, position.y +  0));
      break;
    case 5:
      graphics_draw_line(gContext, GPoint(position.x +  0, position.y +  0), GPoint(position.x + 60, position.y +  0));
      graphics_draw_line(gContext, GPoint(position.x + 60, position.y +  0), GPoint(position.x + 60, position.y + 14));
      graphics_draw_line(gContext, GPoint(position.x + 60, position.y + 14), GPoint(position.x + 14, position.y + 14));
      graphics_draw_line(gContext, GPoint(position.x + 14, position.y + 14), GPoint(position.x + 14, position.y + 36));
      graphics_draw_line(gContext, GPoint(position.x + 14, position.y + 36), GPoint(position.x + 60, position.y + 36));
      graphics_draw_line(gContext, GPoint(position.x + 60, position.y + 36), GPoint(position.x + 60, position.y + 90));
      graphics_draw_line(gContext, GPoint(position.x + 60, position.y + 90), GPoint(position.x +  0, position.y + 90));
      graphics_draw_line(gContext, GPoint(position.x +  0, position.y + 90), GPoint(position.x +  0, position.y + 66));
      graphics_draw_line(gContext, GPoint(position.x +  0, position.y + 66), GPoint(position.x + 14, position.y + 66));
      graphics_draw_line(gContext, GPoint(position.x + 14, position.y + 66), GPoint(position.x + 14, position.y + 76));
      graphics_draw_line(gContext, GPoint(position.x + 14, position.y + 76), GPoint(position.x + 46, position.y + 76));
      graphics_draw_line(gContext, GPoint(position.x + 46, position.y + 76), GPoint(position.x + 46, position.y + 52));
      graphics_draw_line(gContext, GPoint(position.x + 46, position.y + 52), GPoint(position.x +  0, position.y + 52));
      graphics_draw_line(gContext, GPoint(position.x +  0, position.y + 52), GPoint(position.x +  0, position.y +  0));
      break;
    case 6:
      graphics_draw_line(gContext, GPoint(position.x +  0, position.y +  0), GPoint(position.x + 60, position.y +  0));
      graphics_draw_line(gContext, GPoint(position.x + 60, position.y +  0), GPoint(position.x + 60, position.y + 14));
      graphics_draw_line(gContext, GPoint(position.x + 60, position.y + 14), GPoint(position.x + 14, position.y + 14));
      graphics_draw_line(gContext, GPoint(position.x + 14, position.y + 14), GPoint(position.x + 14, position.y + 36));
      graphics_draw_line(gContext, GPoint(position.x + 14, position.y + 36), GPoint(position.x + 60, position.y + 36));
      graphics_draw_line(gContext, GPoint(position.x + 60, position.y + 36), GPoint(position.x + 60, position.y + 90));
      graphics_draw_line(gContext, GPoint(position.x + 60, position.y + 90), GPoint(position.x +  0, position.y + 90));
      graphics_draw_line(gContext, GPoint(position.x +  0, position.y + 90), GPoint(position.x +  0, position.y +  0));
      graphics_draw_line(gContext, GPoint(position.x + 14, position.y + 52), GPoint(position.x + 46, position.y + 52));
      graphics_draw_line(gContext, GPoint(position.x + 46, position.y + 52), GPoint(position.x + 46, position.y + 74));
      graphics_draw_line(gContext, GPoint(position.x + 46, position.y + 74), GPoint(position.x + 14, position.y + 74));
      graphics_draw_line(gContext, GPoint(position.x + 14, position.y + 74), GPoint(position.x + 14, position.y + 52));
      break;
    case 7:
      graphics_draw_line(gContext, GPoint(position.x +  0, position.y +  0), GPoint(position.x + 60, position.y +  0));
      graphics_draw_line(gContext, GPoint(position.x + 60, position.y +  0), GPoint(position.x + 60, position.y + 90));
      graphics_draw_line(gContext, GPoint(position.x + 60, position.y + 90), GPoint(position.x + 46, position.y + 90));
      graphics_draw_line(gContext, GPoint(position.x + 46, position.y + 90), GPoint(position.x + 46, position.y + 14));
      graphics_draw_line(gContext, GPoint(position.x + 46, position.y + 14), GPoint(position.x + 14, position.y + 14));
      graphics_draw_line(gContext, GPoint(position.x + 14, position.y + 14), GPoint(position.x + 14, position.y + 24));
      graphics_draw_line(gContext, GPoint(position.x + 14, position.y + 24), GPoint(position.x +  0, position.y + 24));
      graphics_draw_line(gContext, GPoint(position.x +  0, position.y + 24), GPoint(position.x +  0, position.y +  0));
      break;
    case 8:
      graphics_draw_line(gContext, GPoint(position.x +  0, position.y +  0), GPoint(position.x + 60, position.y +  0));
      graphics_draw_line(gContext, GPoint(position.x + 60, position.y +  0), GPoint(position.x + 60, position.y + 90));
      graphics_draw_line(gContext, GPoint(position.x + 60, position.y + 90), GPoint(position.x +  0, position.y + 90));
      graphics_draw_line(gContext, GPoint(position.x +  0, position.y + 90), GPoint(position.x +  0, position.y +  0));
      graphics_draw_line(gContext, GPoint(position.x + 14, position.y + 14), GPoint(position.x + 46, position.y + 14));
      graphics_draw_line(gContext, GPoint(position.x + 46, position.y + 14), GPoint(position.x + 46, position.y + 36));
      graphics_draw_line(gContext, GPoint(position.x + 46, position.y + 36), GPoint(position.x + 14, position.y + 36));
      graphics_draw_line(gContext, GPoint(position.x + 14, position.y + 36), GPoint(position.x + 14, position.y + 14));
      graphics_draw_line(gContext, GPoint(position.x + 14, position.y + 54), GPoint(position.x + 46, position.y + 54));
      graphics_draw_line(gContext, GPoint(position.x + 46, position.y + 54), GPoint(position.x + 46, position.y + 74));
      graphics_draw_line(gContext, GPoint(position.x + 46, position.y + 74), GPoint(position.x + 14, position.y + 74));
      graphics_draw_line(gContext, GPoint(position.x + 14, position.y + 74), GPoint(position.x + 14, position.y + 54));
      break;
    case 9:
      graphics_draw_line(gContext, layout_point(GPoint(position.x +  0, position.y +  0)), layout_point(GPoint(position.x + 60, position.y +  0)));
      graphics_draw_line(gContext, layout_point(GPoint(position.x + 60, position.y +  0)), layout_point(GPoint(position.x + 60, position.y + 90)));
      graphics_draw_line(gContext, layout_point(GPoint(position.x + 60, position.y + 90)), layout_point(GPoint(position.x +  0, position.y + 90)));
      graphics_draw_line(gContext, layout_point(GPoint(position.x +  0, position.y + 90)), layout_point(GPoint(position.x +  0, position.y + 76)));
      graphics_draw_line(gContext, layout_point(GPoint(position.x +  0, position.y + 76)), layout_point(GPoint(position.x + 46, position.y + 76)));
      graphics_draw_line(gContext, layout_point(GPoint(position.x + 46, position.y + 76)), layout_point(GPoint(position.x + 46, position.y + 54)));
      graphics_draw_line(gContext, layout_point(GPoint(position.x + 46, position.y + 54)), layout_point(GPoint(position.x +  0, position.y + 54)));
      graphics_draw_line(gContext, layout_point(GPoint(position.x +  0, position.y + 54)), layout_point(GPoint(position.x +  0, position.y +  0)));
      graphics_draw_line(gContext, layout_point(GPoint(position.x + 14, position.y + 14)), layout_point(GPoint(position.x + 46, position.y + 14)));
      graphics_draw_line(gContext, layout_point(GPoint(position.x + 46, position.y + 14)), layout_point(GPoint(position.x + 46, position.y + 36)));
      graphics_draw_line(gContext, layout_point(GPoint(position.x + 46, position.y + 36)), layout_point(GPoint(position.x + 14, position.y + 36)));
      graphics_draw_line(gContext, layout_point(GPoint(position.x + 14, position.y + 36)), layout_point(GPoint(position.x + 14, position.y + 14)));
      break;
  }
}

static GRect screen_bounds(void) {
  Window *window = window_stack_get_top_window();
  return layer_get_bounds(window_get_root_layer(window));
}

void draw_background(GContext *gContext, uint16_t corner_radius, GCornerMask corner_mask, Color palette){
  if(DEBUG) APP_LOG(APP_LOG_LEVEL_INFO, "[%s] %s()", logTime(), __func__);

  GRect rect = screen_bounds();
  graphics_context_set_fill_color(gContext, palette.background);
  graphics_fill_rect(gContext, rect, corner_radius, corner_mask);
  #if defined(PBL_PLATFORM_CHALK) || defined(PBL_PLATFORM_GABBRO)
    GRect base = layout_rect(GRect(0, 165, 180, 20));
    graphics_context_set_fill_color(gContext, palette.fillDot);
    graphics_fill_rect(gContext, base, 0, GCornerNone);
  #endif

}

void draw_time_background(GContext *gContext, Color palette){
  if(DEBUG) APP_LOG(APP_LOG_LEVEL_INFO, "[%s] %s()", logTime(), __func__);

  #if defined(PBL_PLATFORM_APLITE) || defined(PBL_PLATFORM_FLINT)
    graphics_context_set_stroke_color(gContext, palette.time);
    draw_number((hour-(hour%10))/10, (GPoint){10, -6}, gContext);
    draw_number(hour%10, (GPoint){72, -6}, gContext);
    draw_number((minute-(minute%10))/10, (GPoint){10, 84}, gContext);
    draw_number(minute%10, (GPoint){72, 84}, gContext);
  #elif defined(PBL_PLATFORM_CHALK) || defined(PBL_PLATFORM_GABBRO)
    graphics_context_set_fill_color(gContext, palette.time);
    fill_number((hour-(hour%10))/10, (GPoint){30, -6}, gContext);
    fill_number(hour%10, (GPoint){92, -6}, gContext);
    fill_number((minute-(minute%10))/10, (GPoint){30, 84}, gContext);
    fill_number(minute%10, (GPoint){92, 84}, gContext);
  #else
    graphics_context_set_fill_color(gContext, palette.time);
    fill_number((hour-(hour%10))/10, (GPoint){10, -6}, gContext);
    fill_number(hour%10, (GPoint){72, -6}, gContext);
    fill_number((minute-(minute%10))/10, (GPoint){10, 84}, gContext);
    fill_number(minute%10, (GPoint){72, 84}, gContext);
  #endif
  #if defined(PBL_PLATFORM_CHALK) || defined(PBL_PLATFORM_GABBRO)
    GRect base = layout_rect(GRect(0, 165, 180, 20));
    graphics_context_set_fill_color(gContext, palette.fillDot);
    graphics_fill_rect(gContext, base, 0, GCornerNone);
  #endif
}

void draw_clock(GContext *gContext, Color palette, bool drawNumbers){
  if(DEBUG) APP_LOG(APP_LOG_LEVEL_INFO, "[%s] %s()", logTime(), __func__);

  int currentWidth, currentHeight;
  int widthSingleLayer;

  #if defined(PBL_PLATFORM_CHALK) || defined(PBL_PLATFORM_GABBRO)
    s_layerRect[0] = (GRect){.origin={20, 45}, .size={104, 24}};
    s_layerRect[1] = (GRect){.origin={20, 70}, .size={104, 24}};
  #else
    s_layerRect[0] = (GRect){.origin={20, 30}, .size={104, 24}};
    if(drawNumbers == true){
      s_layerRect[1] = (GRect){.origin={20, 70}, .size={104, 24}};
    }else{
      s_layerRect[1] = (GRect){.origin={20, 60}, .size={104, 24}};
    }
  #endif

  graphics_context_set_stroke_color(gContext, palette.strokeDot);
  for(int j=0; j<2; j++){
    // trick to simulate the round function
    widthSingleLayer = (int)(s_layerRect[j].size.w/(s_bulletsNumber[j]-1));
    int wRest = s_layerRect[j].size.w%(s_bulletsNumber[j]-1);
    if(wRest > (s_bulletsNumber[j]-1)/2){
      widthSingleLayer++;
    }

    for(int i=0; i<s_bulletsNumber[j]; i++){
      // Defines the bullets position width
      if(i == (s_bulletsNumber[j] - 1)){
        currentWidth = s_layerRect[j].origin.x;
      }else if(i == 0){
        currentWidth = s_layerRect[j].origin.x + s_layerRect[j].size.w;
      }else{
        currentWidth = s_layerRect[j].origin.x + widthSingleLayer * (s_bulletsNumber[j] - i - 1);
      }
      currentWidth -= 1;
      // Very simple define of height
      currentHeight = s_layerRect[j].origin.y + 24;
      // Chalk platform correction
      #if defined(PBL_PLATFORM_CHALK) || defined(PBL_PLATFORM_GABBRO)
        currentWidth += 20;
        currentHeight += 10;
      #endif

      // Draws the dots
      GColor strokeColor, fillColor;
      strokeColor = palette.strokeDot;
      if(s_bufferTime[j][i] == 1){
        fillColor = palette.fillDot;
      }else if(shape == 13){
        // Keep both open and closed Easter eggs white.
        fillColor = GColorWhite;
      }else{
        fillColor = palette.background;
      }
      draw_shape(shape, s_bufferTime[j][i] == 1, currentWidth, currentHeight,
                 gContext, strokeColor, fillColor);

      // Prints texts
      currentHeight = s_layerRect[j].origin.y;
      #if defined(PBL_PLATFORM_CHALK) || defined(PBL_PLATFORM_GABBRO)
        currentHeight += 7;
        if(j == 1){
          currentHeight += 36;
        }
      #endif

      graphics_context_set_text_color(gContext, palette.text);
      if(drawNumbers == true){
        GFont help_number_font = fonts_get_system_font(FONT_KEY_GOTHIC_14);
        #ifdef PBL_PLATFORM_GABBRO
          help_number_font = fonts_get_system_font(FONT_KEY_GOTHIC_18);
        #endif
        #ifdef PBL_PLATFORM_EMERY
          help_number_font = fonts_get_system_font(FONT_KEY_GOTHIC_18);
        #endif
        graphics_draw_text(gContext,
                           s_textBase[i],
                           help_number_font,
                           layout_rect((GRect){.origin={currentWidth-8, currentHeight}, .size={16, 18}}),
                           GTextOverflowModeWordWrap,
                           GTextAlignmentCenter,
                           NULL
                          );
      }
    }
  }
}

void draw_bluetooth(GContext *gContext){
  if(DEBUG) APP_LOG(APP_LOG_LEVEL_INFO, "[%s] %s()", logTime(), __func__);

  #if defined(PBL_PLATFORM_APLITE)
    if(bt_bitmap_off == NULL)
      bt_bitmap_off = gbitmap_create_with_resource(RESOURCE_ID_BLUETOOTH_BW_OFF_IMG);
    if(bt_bitmap_on == NULL)
      bt_bitmap_on = gbitmap_create_with_resource(RESOURCE_ID_BLUETOOTH_BW_ON_IMG);
  #elif defined(PBL_PLATFORM_GABBRO) || defined(PBL_PLATFORM_EMERY)
    if(bt_bitmap_off == NULL)
      bt_bitmap_off = gbitmap_create_with_resource(RESOURCE_ID_BLUETOOTH_GABBRO_OFF_IMG);
    if(bt_bitmap_on == NULL)
      bt_bitmap_on = gbitmap_create_with_resource(RESOURCE_ID_BLUETOOTH_GABBRO_ON_IMG);
  #else
    if(bt_bitmap_off == NULL)
      bt_bitmap_off = gbitmap_create_with_resource(RESOURCE_ID_BLUETOOTH_OFF_IMG);
    if(bt_bitmap_on == NULL)
      bt_bitmap_on = gbitmap_create_with_resource(RESOURCE_ID_BLUETOOTH_ON_IMG);
  #endif

  if(bluetooth == BT_ALWAYS || (bluetooth == BT_ON_DISCONNECT && bluetooth_status == 0)){
    int x = 7;
    int y = 7;
    int w = 10;
    int h = 16;
    #if defined(PBL_PLATFORM_APLITE)
      if(bluetooth_status == 1){
        w = 6;
        h = 14;
      }else{
        w = 12;
        h = 13;
      }
    #elif defined(PBL_PLATFORM_BASALT) || defined(PBL_PLATFORM_FLINT)
      y = 6;
    #elif defined(PBL_PLATFORM_EMERY)
      // Emery's scaled destination is 14x23px.  Use the matching bitmap:
      // drawing the 10x15px asset in this larger rect makes Pebble tile it.
      h = 17;
    #elif defined(PBL_PLATFORM_CHALK) || defined(PBL_PLATFORM_GABBRO)
      x = 85;
      y = 140;
      if((battery == BA_UNDER_20_PERC && battery_level < BA_PERCENT_WARNING) || battery == BA_ALWAYS){
        x -= 20;
      }
    #endif
    GRect rect = layout_rect(GRect(x, y, w, h));
    #if defined(PBL_PLATFORM_APLITE)
      // The Bluetooth artwork is white with transparency.  Clear its opaque
      // pixels on the light theme so it remains visible against white.
      graphics_context_set_compositing_mode(gContext,
                                            color == 0 ? GCompOpClear : GCompOpOr);
    #else
      graphics_context_set_compositing_mode(gContext, GCompOpSet);
    #endif
    if(bluetooth_status == 0){
      graphics_draw_bitmap_in_rect(gContext, bt_bitmap_off, rect);
    }else{
      graphics_draw_bitmap_in_rect(gContext, bt_bitmap_on, rect);
    }
  }
}

void draw_battery(GContext *gContext, int battery, Color palette){
  if(DEBUG) APP_LOG(APP_LOG_LEVEL_INFO, "[%s] %s()", logTime(), __func__);

  if(battery != BA_NEVER){
    GColor batteryColor;
      #if defined(PBL_PLATFORM_APLITE) || defined(PBL_PLATFORM_FLINT)
      batteryColor = palette.text;
    #else
      if(battery_level < BA_PERCENT_WARNING){
        if(isEasterEggDay()){
          batteryColor = palette.text;
        }else{
          batteryColor = GColorRed;
        }
      }else{
        batteryColor = palette.text;
      }
    #endif
    graphics_context_set_fill_color(gContext, batteryColor);
    graphics_context_set_stroke_color(gContext, batteryColor);
    graphics_context_set_text_color(gContext, batteryColor);
  }

  if((battery == BA_UNDER_20_PERC && battery_level < BA_PERCENT_WARNING) || battery == BA_ALWAYS){
    int x, y;
    #if defined(PBL_PLATFORM_CHALK) || defined(PBL_PLATFORM_GABBRO)
      x = 80;
      y = 142;
    #else
      x = 115;
      y = 7;
    #endif
    // if i display bluetooth image too, battery sign must shift right
    #if defined(PBL_PLATFORM_CHALK) || defined(PBL_PLATFORM_GABBRO)
      if(bluetooth == BT_ALWAYS || (bluetooth == BT_ON_DISCONNECT && bluetooth_status == 0)){
        x += 20;
      }
    #endif
    #if defined(PBL_PLATFORM_GABBRO) || defined(PBL_PLATFORM_EMERY)
      x = layout_x(x);
      y = layout_value(y);
    #endif
    if(battery_modality == 0){
      int battery_width = layout_value(23);
      #ifdef PBL_PLATFORM_GABBRO
        battery_width -= 1;
      #elif defined(PBL_PLATFORM_EMERY)
        battery_width -= 1;
      #endif
      graphics_draw_rect(gContext,
                         GRect(x, y, battery_width, layout_value(13)));
      graphics_draw_line(gContext, GPoint(x + battery_width, y + layout_value(4)), GPoint(x + battery_width, y + layout_value(9)));

      int battery_blocks = battery_level / 10;
      #if defined(PBL_PLATFORM_GABBRO) || defined(PBL_PLATFORM_EMERY)
        battery_blocks = battery_blocks > 9 ? 9 : battery_blocks;
      #endif
      for(int z=1; z<=battery_blocks; z++){
        #if defined(PBL_PLATFORM_GABBRO) || defined(PBL_PLATFORM_EMERY)
          int battery_bar_height = layout_value(10) - layout_value(2) + 1;
          int battery_bar_x = x + 2 + (z - 1) * 3;
          #ifdef PBL_PLATFORM_GABBRO
            battery_bar_height += 1;
            battery_bar_x += 1;
          #endif
          graphics_fill_rect(gContext,
                             GRect(battery_bar_x, y + layout_value(2),
                                   2, battery_bar_height),
                             0, GCornerNone);
        #else
        graphics_draw_line(gContext, GPoint(x + layout_value(2*z), y + layout_value(2)), GPoint(x + layout_value(2*z), y + layout_value(10)));
        #endif
      }
      if(battery_level < BA_PERCENT_WARNING){
        graphics_fill_rect(gContext, GRect(x + layout_value(10), y + layout_value(2), layout_value(4), layout_value(4)), 0, GCornerNone);
        graphics_fill_rect(gContext, GRect(x + layout_value(11), y + layout_value(6), layout_value(2), layout_value(2)), 0, GCornerNone);
        graphics_fill_rect(gContext, GRect(x + layout_value(11), y + layout_value(9), layout_value(2), layout_value(2)), 0, GCornerNone);
      }
    }else{
      #ifndef PBL_PLATFORM_CHALK
        GFont font = fonts_get_system_font(FONT_KEY_GOTHIC_14);
        #if defined(PBL_PLATFORM_GABBRO) || defined(PBL_PLATFORM_EMERY)
          font = fonts_get_system_font(FONT_KEY_GOTHIC_18);
        #endif
        graphics_draw_rect(gContext,
                           GRect(x, y, layout_value(24), layout_value(13)));
        graphics_draw_line(gContext,
                           GPoint(x + layout_value(24), y + layout_value(4)),
                           GPoint(x + layout_value(24), y + layout_value(9)));
        x -= layout_value(2);
        y -= layout_value(3);
      #else
        GFont font = fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD);
      #endif
      snprintf(battery_buffer, sizeof(battery_buffer), "%d%%", battery_level);
      graphics_draw_text(gContext,
                         battery_buffer,
                         font,
                         GRect(x, y, layout_value(30), layout_value(10)),
                         GTextOverflowModeFill,
                         GTextAlignmentCenter,
                         NULL
                        );
    }
  }
}

void draw_heart_rate(GContext *gContext, Color palette, uint8_t heart_rate_bpm) {
  // This function is called only for the HR-capable target displays. Keep the
  // indicator between the binary clock and date, or in the round display's
  // bottom strip.
  // The heart (14px) plus the BPM field (34px) occupy 52px in the
  // reference layout; centre that group horizontally on each display.
  // The heart indicator occupies 52px. Centre it alone, or align its icon to
  // the "16" column of the second binary row on Emery.
  int x = show_steps ? 0 : 46;
  int y = 102;
  #if defined(PBL_PLATFORM_CHALK) || defined(PBL_PLATFORM_GABBRO)
    y = 166;
  #elif defined(PBL_PLATFORM_EMERY)
    y = 118;
    if (show_steps) {
      // The heart centre is x + 7. The "16" column is centred at x = 40.
      x = 33;
    }
  #endif

  graphics_context_set_fill_color(gContext, palette.text);
  const int radius = layout_value(4);
  graphics_fill_circle(gContext, layout_point(GPoint(x + 4, y + 5)), radius);
  graphics_fill_circle(gContext, layout_point(GPoint(x + 10, y + 5)), radius);

  GPoint points[] = {
    layout_point(GPoint(x, y + 6)),
    layout_point(GPoint(x + 14, y + 6)),
    layout_point(GPoint(x + 7, y + 15)),
  };
  GPathInfo heart_info = {
    .num_points = ARRAY_LENGTH(points),
    .points = points,
  };
  GPath *heart = gpath_create(&heart_info);
  if (heart != NULL) {
    gpath_draw_filled(gContext, heart);
    gpath_destroy(heart);
  }

  char heart_rate_buffer[4];
  if (heart_rate_bpm > 0) {
    snprintf(heart_rate_buffer, sizeof(heart_rate_buffer), "%u",
             (unsigned int)heart_rate_bpm);
  } else {
    snprintf(heart_rate_buffer, sizeof(heart_rate_buffer), "-");
  }
  int text_y = y;
  GFont font = fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD);
  #ifdef PBL_PLATFORM_GABBRO
    font = fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD);
  #endif
  #ifdef PBL_PLATFORM_EMERY
    font = fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD);
    if (!show_steps) {
      text_y -= 3;
    }
  #endif
  graphics_context_set_text_color(gContext, palette.text);
  #ifdef PBL_PLATFORM_EMERY
  if (show_steps) {
    // Keep the BPM value after the heart while leaving space for the walking
    // icon aligned with the second-row "4" column.
    graphics_draw_text(gContext,
                       heart_rate_buffer,
                       fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD),
                       layout_rect(GRect(x + 16, text_y - 5, 25, 22)),
                       GTextOverflowModeFill,
                       GTextAlignmentLeft,
                       NULL);
    return;
  }
  #endif
  graphics_draw_text(gContext,
                     heart_rate_buffer,
                     font,
                     layout_rect(GRect(x + 18, text_y, 34, 24)),
                     GTextOverflowModeFill,
                     GTextAlignmentLeft,
                     NULL);
}

void draw_steps(GContext *gContext, Color palette, int steps) {
  bool show_heart_indicator = false;
  #if defined(PBL_PLATFORM_EMERY) || defined(PBL_PLATFORM_GABBRO)
    show_heart_indicator = show_heart_rate;
  #endif
  // The walking-person indicator is centred when alone; beside the heart its
  // value extends to the right edge of the 144px design canvas.
  int x = show_heart_indicator ? 76 : 30;
  int y = 102;
  #if defined(PBL_PLATFORM_CHALK) || defined(PBL_PLATFORM_GABBRO)
    y = 166;
  #elif defined(PBL_PLATFORM_EMERY)
    y = 118;
    if (show_heart_indicator) {
      // The walking-person centre is x + 8. The "4" column is x = 82.
      x = 74;
    }
  #endif
  #ifdef PBL_PLATFORM_GABBRO
    // Gabbro's 180px-wide screen uses a 13/9 horizontal scale. With both
    // indicators visible, start the 68px step group at 56 so its value ends
    // exactly at the right edge instead of being clipped beyond it.
    if (show_heart_indicator) {
      x = 56;
    }
  #endif

  // A walking-person pictogram is more immediately associated with a step
  // counter than a generic shoe or footprint.
  graphics_context_set_fill_color(gContext, palette.text);
  graphics_context_set_stroke_color(gContext, palette.text);
  graphics_fill_circle(gContext, layout_point(GPoint(x + 8, y + 3)), layout_value(2));
  graphics_context_set_stroke_width(gContext, layout_value(2));
  graphics_draw_line(gContext, layout_point(GPoint(x + 8, y + 6)),
                     layout_point(GPoint(x + 8, y + 11)));
  graphics_draw_line(gContext, layout_point(GPoint(x + 8, y + 7)),
                     layout_point(GPoint(x + 3, y + 9)));
  graphics_draw_line(gContext, layout_point(GPoint(x + 8, y + 7)),
                     layout_point(GPoint(x + 13, y + 8)));
  graphics_draw_line(gContext, layout_point(GPoint(x + 8, y + 11)),
                     layout_point(GPoint(x + 4, y + 15)));
  graphics_draw_line(gContext, layout_point(GPoint(x + 8, y + 11)),
                     layout_point(GPoint(x + 13, y + 14)));

  char steps_buffer[12];
  snprintf(steps_buffer, sizeof(steps_buffer), "%d", steps);
  GFont font = fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD);
  #ifdef PBL_PLATFORM_GABBRO
    font = fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD);
  #endif
  #ifdef PBL_PLATFORM_EMERY
    font = fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD);
  #endif
  graphics_context_set_text_color(gContext, palette.text);
  graphics_draw_text(gContext,
                     steps_buffer,
                     font,
                     layout_rect(GRect(x + 19, y + (show_heart_indicator ? -5 : -1),
                                       show_heart_indicator ? 49 : 62,
                                       show_heart_indicator ? 22 : 26)),
                     GTextOverflowModeTrailingEllipsis,
                     GTextAlignmentLeft,
                     NULL);
}

void draw_date(GContext *gContext, Color palette){
  if(DEBUG) APP_LOG(APP_LOG_LEVEL_INFO, "[%s] %s()", logTime(), __func__);

  get_date_format(date);
  int x;
  int y;
  int w;
  int h = (date > 28 && !isEasterEggDay()) ? 120 : 136;
  #if defined(PBL_PLATFORM_CHALK) || defined(PBL_PLATFORM_GABBRO)
    x = 24;
    y = (date > 28) ? 5 : 20;
    w = 132;
  #else
    x = 0;
    y = (date > 28 && !isEasterEggDay()) ? 120 : 136;
    w = 144;
  #endif
  y += 3;
  GRect rect = layout_rect(GRect(x, y, w, h));
  int esternEgg = isEasterEggDay();
  if(esternEgg != 0){
    if(esternEgg == 1){
      snprintf(date_buffer, sizeof(date_buffer), "Merry Christmas!");
    }else if(esternEgg == 2){
      snprintf(date_buffer, sizeof(date_buffer), "Happy new year!");
    }else if(esternEgg == 3){
      snprintf(date_buffer, sizeof(date_buffer), "Be my Valentine!");
    }else if(esternEgg == 4){
      snprintf(date_buffer, sizeof(date_buffer), "Happy Easter!");
    }
  }
  //////////////////////////////////////////////////////////////////
  // graphics_context_set_text_color(gContext, GColorWhite);
  // snprintf(date_buffer, sizeof(date_buffer), "Happy new year!");
  //////////////////////////////////////////////////////////////////
  GFont date_font = fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD);
  #ifdef PBL_PLATFORM_GABBRO
    date_font = fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD);
  #endif
  #ifdef PBL_PLATFORM_EMERY
    date_font = fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD);
  #endif
  graphics_context_set_text_color(gContext, palette.text);
  graphics_draw_text(gContext,
                     date_buffer,
                     date_font,
                     rect,
                     GTextOverflowModeWordWrap,
                     GTextAlignmentCenter,
                     NULL
                    );
}

void get_date_format(int dateKey){
  if(DEBUG) APP_LOG(APP_LOG_LEVEL_INFO, "[%s] %s()", logTime(), __func__);

  time_t rawtime;
  struct tm *info;

  time(&rawtime);
  info = localtime(&rawtime);
  switch(dateKey){
    case 0:
      strftime(date_buffer, sizeof(date_buffer), " ", info);
      break;
    case 1:
      strftime(date_buffer, sizeof(date_buffer), "%d-%m-%y", info);
      break;
    case 2:
      strftime(date_buffer, sizeof(date_buffer), "%d.%m.%y", info);
      break;
    case 3:
      strftime(date_buffer, sizeof(date_buffer), "%d/%m/%y", info);
      break;
    case 4:
      strftime(date_buffer, sizeof(date_buffer), "%d %m %y", info);
      break;
    case 5:
      strftime(date_buffer, sizeof(date_buffer), "%d-%m-%Y", info);
      break;
    case 6:
      strftime(date_buffer, sizeof(date_buffer), "%d.%m.%Y", info);
      break;
    case 7:
      strftime(date_buffer, sizeof(date_buffer), "%d/%m/%Y", info);
      break;
    case 8:
      strftime(date_buffer, sizeof(date_buffer), "%d %m %Y", info);
      break;
    case 9:
      strftime(date_buffer, sizeof(date_buffer), "%m-%d-%Y", info);
      break;
    case 10:
      strftime(date_buffer, sizeof(date_buffer), "%m.%d.%Y", info);
      break;
    case 11:
      strftime(date_buffer, sizeof(date_buffer), "%m/%d/%Y", info);
      break;
    case 12:
      strftime(date_buffer, sizeof(date_buffer), "%m %d %Y", info);
      break;
    case 13:
      strftime(date_buffer, sizeof(date_buffer), "%Y-%m-%d", info);
      break;
    case 14:
      strftime(date_buffer, sizeof(date_buffer), "%Y.%m.%d", info);
      break;
    case 15:
      strftime(date_buffer, sizeof(date_buffer), "%Y/%m/%d", info);
      break;
    case 16:
      strftime(date_buffer, sizeof(date_buffer), "%Y %m %d", info);
      break;
    case 17:
      strftime(date_buffer, sizeof(date_buffer), "%b/%d/%Y", info);
      break;
    case 18:
      strftime(date_buffer, sizeof(date_buffer), "%b %e, %Y", info);
      break;
    case 19:
      strftime(date_buffer, sizeof(date_buffer), "%d %b %Y", info);
      break;
    case 20:
      strftime(date_buffer, sizeof(date_buffer), "%d %b, %Y", info);
      break;
    case 21:
      strftime(date_buffer, sizeof(date_buffer), "%a, %b/%d/%Y", info);
      break;
    case 22:
      strftime(date_buffer, sizeof(date_buffer), "%a, %b %e, %Y", info);
      break;
    case 23:
      strftime(date_buffer, sizeof(date_buffer), "%a, %d %b %Y", info);
      break;
    case 24:
      strftime(date_buffer, sizeof(date_buffer), "%a, %d %b, %Y", info);
      break;
    case 25:
      strftime(date_buffer, sizeof(date_buffer), "%e %B %Y", info);
      break;
    case 26:
      strftime(date_buffer, sizeof(date_buffer), "%d %B %Y", info);
      break;
    case 27:
      strftime(date_buffer, sizeof(date_buffer), "%B %d, %Y", info);
      break;
    case 28:
      strftime(date_buffer, sizeof(date_buffer), "%B-%d-%Y", info);
      break;
    case 29:
      strftime(date_buffer, sizeof(date_buffer), "%a,\n%e %B %Y", info);
      break;
    case 30:
      strftime(date_buffer, sizeof(date_buffer), "%a,\n%d %B %Y", info);
      break;
    case 31:
      strftime(date_buffer, sizeof(date_buffer), "%a,\n%B %d, %Y", info);
      break;
    case 32:
      strftime(date_buffer, sizeof(date_buffer), "%a,\n%B-%d-%Y", info);
      break;
    case 33:
      strftime(date_buffer, sizeof(date_buffer), "%A,\n%e %B %Y", info);
      break;
    case 34:
      strftime(date_buffer, sizeof(date_buffer), "%A,\n%d %B %Y", info);
      break;
    case 35:
      strftime(date_buffer, sizeof(date_buffer), "%A,\n%B %d, %Y", info);
      break;
    case 36:
      strftime(date_buffer, sizeof(date_buffer), "%A,\n%B-%d-%Y", info);
      break;
  }
}

void draw_flake(GContext *gContext, Layer *flake_layer, struct Flake flake){
  graphics_context_set_stroke_color(gContext, GColorWhite);
  graphics_context_set_stroke_color(gContext, GColorChromeYellow);
  switch(flake.size){
    case 0:
      graphics_draw_pixel(gContext, flake.pos);
      break;
    case 1:
      graphics_draw_pixel(gContext, (GPoint){flake.pos.x,   flake.pos.y  });
      graphics_draw_pixel(gContext, (GPoint){flake.pos.x+1, flake.pos.y  });
      graphics_draw_pixel(gContext, (GPoint){flake.pos.x+1, flake.pos.y+1});
      graphics_draw_pixel(gContext, (GPoint){flake.pos.x,   flake.pos.y+1});
      break;
    case 2:
      graphics_draw_pixel(gContext, (GPoint){flake.pos.x,   flake.pos.y  });
      graphics_draw_pixel(gContext, (GPoint){flake.pos.x,   flake.pos.y+1});
      graphics_draw_pixel(gContext, (GPoint){flake.pos.x,   flake.pos.y+2});
      graphics_draw_pixel(gContext, (GPoint){flake.pos.x+1, flake.pos.y+1});
      graphics_draw_pixel(gContext, (GPoint){flake.pos.x-1, flake.pos.y+1});
      break;
    case 3:
      graphics_draw_pixel(gContext, (GPoint){flake.pos.x-1, flake.pos.y-1});
      graphics_draw_pixel(gContext, (GPoint){flake.pos.x+1, flake.pos.y-1});
      graphics_draw_pixel(gContext, (GPoint){flake.pos.x,   flake.pos.y  });
      graphics_draw_pixel(gContext, (GPoint){flake.pos.x-1, flake.pos.y+1});
      graphics_draw_pixel(gContext, (GPoint){flake.pos.x+1, flake.pos.y+1});
      break;
    case 4:
      graphics_draw_pixel(gContext, (GPoint){flake.pos.x,   flake.pos.y  });
      graphics_draw_pixel(gContext, (GPoint){flake.pos.x,   flake.pos.y-1});
      graphics_draw_pixel(gContext, (GPoint){flake.pos.x,   flake.pos.y+1});
      graphics_draw_pixel(gContext, (GPoint){flake.pos.x+1, flake.pos.y  });
      graphics_draw_pixel(gContext, (GPoint){flake.pos.x-1, flake.pos.y  });
      graphics_draw_pixel(gContext, (GPoint){flake.pos.x-2, flake.pos.y-1});
      graphics_draw_pixel(gContext, (GPoint){flake.pos.x-2, flake.pos.y+1});
      graphics_draw_pixel(gContext, (GPoint){flake.pos.x-1, flake.pos.y-2});
      graphics_draw_pixel(gContext, (GPoint){flake.pos.x-1, flake.pos.y+2});
      graphics_draw_pixel(gContext, (GPoint){flake.pos.x+1, flake.pos.y-2});
      graphics_draw_pixel(gContext, (GPoint){flake.pos.x+1, flake.pos.y+2});
      graphics_draw_pixel(gContext, (GPoint){flake.pos.x+2, flake.pos.y-1});
      graphics_draw_pixel(gContext, (GPoint){flake.pos.x+2, flake.pos.y+1});
      break;
    case 5:
      tmp.pos.x = flake.pos.x;
      tmp.pos.y = flake.pos.y;
      tmp.size = 3;
      tmp.pos.y = flake.pos.y-2;
      draw_flake(gContext, flake_layer, tmp);
      tmp.pos.y = flake.pos.y+2;
      draw_flake(gContext, flake_layer, tmp);
      tmp.pos.y = flake.pos.y;
      tmp.pos.x = flake.pos.x-2;
      draw_flake(gContext, flake_layer, tmp);
      tmp.pos.x = flake.pos.x+2;
      draw_flake(gContext, flake_layer, tmp);
      break;
  }
}

void shake_flakes(struct Flake *flakes){
  if(DEBUG) APP_LOG(APP_LOG_LEVEL_INFO, "[%s] %s()", logTime(), __func__);

  int x, y, size;
  GRect bounds = screen_bounds();
  int windowWidth = bounds.size.w;
  int windowHeight = bounds.size.h;
  // Intializes random number generator
  srand(time(NULL));
  for(int i=0; i<NUM_FLAKES; i++){
    x = rand() % windowWidth;
    y = rand() % windowHeight;
    size = rand() % 6;
    flakes[i].pos = (GPoint){x, y};
    flakes[i].size = size;
  }
}

void draw_snow(GContext *gContext, struct Flake *flakes, Layer *flake_layers[NUM_FLAKES]){
  if(DEBUG) APP_LOG(APP_LOG_LEVEL_INFO, "[%s] %s()", logTime(), __func__);

  shake_flakes(flakes);
  for(int i=0; i<NUM_FLAKES; i++){
    draw_flake(gContext, flake_layers[i], flakes[i]);
  }
}

void anim_stopped_handler(Animation *animation, bool finished, void *context) {}

static bool is_easter_day(int year, int month, int day) {
  // Meeus/Jones/Butcher Gregorian computus. Month is 1-based.
  int a = year % 19;
  int b = year / 100;
  int c = year % 100;
  int d = b / 4;
  int e = b % 4;
  int f = (b + 8) / 25;
  int g = (b - f + 1) / 3;
  int h = (19 * a + b - d - g + 15) % 30;
  int i = c / 4;
  int k = c % 4;
  int l = (32 + 2 * e + 2 * i - h - k) % 7;
  int m = (a + 11 * h + 22 * l) / 451;
  int easter_month = (h + l - 7 * m + 114) / 31;
  int easter_day = (h + l - 7 * m + 114) % 31 + 1;

  return month == easter_month && day == easter_day;
}

int isEasterEggDay(){
  time_t now = time(NULL);
  struct tm *timeinfo = localtime(&now);
  // is easter egg
  if(timeinfo->tm_mon==11 && timeinfo->tm_mday==25){
    return 1;
  }else if(timeinfo->tm_mon==0 && timeinfo->tm_mday==1){
    return 2;
  }else if(timeinfo->tm_mon==1 && timeinfo->tm_mday==14){
    return 3;
  }else if(is_easter_day(timeinfo->tm_year + 1900,
                          timeinfo->tm_mon + 1, timeinfo->tm_mday)){
    return 4;
  }
  return 0;
}
