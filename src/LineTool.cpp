#include "LineTool.hpp"
#include <cairo.h>
#include "Button.hpp"
#include "ToolContext.hpp"

namespace pix {

bool
LineTool::clickDown(const ClickContext& ctx)
{
  if (ctx.button != Button::LEFT || mouseDown) {
    if (mouseDown && !multiline) {
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
  ctx.preview([x, y](cairo_t* cr) {
    cairo_rectangle(cr, x, y, 0.1, 0.1);
    cairo_stroke(cr);
  });
  return true;
}

bool
LineTool::clickUp(const ClickContext& ctx)
{
  if (!mouseDown) {
    return false;
  }
  float x = ctx.x;
  float y = ctx.y;
  ctx.execute([ x, y, oldX = this->oldX, oldY = this->oldY ](cairo_t * cr) {
    cairo_move_to(cr, oldX, oldY);
    cairo_line_to(cr, x, y);
    cairo_stroke(cr);
  });
  mouseDown = multiline && (ctx.button == Button::LEFT);
  if (mouseDown) {
    oldX = x;
    oldY = y;
  }
  return true;
}

bool
LineTool::move(const MoveContext& ctx)
{
  float x = ctx.x;
  float y = ctx.y;
  if (mouseDown) {
    ctx.preview([ x, y, oldX = this->oldX, oldY = this->oldY ](cairo_t * cr) {
      cairo_move_to(cr, oldX, oldY);
      cairo_line_to(cr, x, y);
      cairo_stroke(cr);
    });
  } else {
    ctx.preview([x, y](cairo_t* cr) {
      cairo_rectangle(cr, x, y, .1, .1);
      cairo_stroke(cr);
    });
  }
  return true;
}
}
