#include "BrushTool.hpp"
#include <cairo.h>
#include "Button.hpp"
#include "ToolContext.hpp"

namespace pix {

bool
BrushTool::clickDown(const ClickContext& ctx)
{
  if (ctx.button != Button::LEFT) {
    if (mouseDown) {
      mouseDown = false;
      return true;
    }
    return false;
  }
  float x   = ctx.x;
  float y   = ctx.y;
  oldX      = x;
  oldY      = y;
  mouseDown = true;
  ctx.execute([x, y](cairo_t* cr) {
    cairo_rectangle(cr, x, y, 0.1, 0.1);
    cairo_stroke(cr);
  });
  return true;
}

bool
BrushTool::clickUp(const ClickContext& ctx)
{
  if (mouseDown) {
    mouseDown = false;
    return true;
  }
  return false;
}

bool
BrushTool::move(const MoveContext& ctx)
{
  float x = ctx.x;
  float y = ctx.y;
  if (mouseDown) {
    ctx.execute([ x, y, oldX = this->oldX, oldY = this->oldY ](cairo_t * cr) {
      cairo_move_to(cr, oldX, oldY);
      cairo_line_to(cr, x, y);
      cairo_stroke(cr);
    });
    oldX = x;
    oldY = y;
  } else {
    ctx.preview([x, y](cairo_t* cr) {
      cairo_rectangle(cr, x, y, .1, .1);
      cairo_stroke(cr);
    });
  }
  return true;
}
}
