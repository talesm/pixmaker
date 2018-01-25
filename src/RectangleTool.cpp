#include "RectangleTool.hpp"
#include <algorithm>
#include <cmath>
#include <cairo.h>
#include "Button.hpp"
#include "ToolContext.hpp"

namespace pix {
using namespace std;

bool
RectangleTool::clickDown(const ClickContext& ctx)
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
  ctx.preview([x, y](cairo_t* cr) {
    cairo_rectangle(cr, x, y, 0.1, 0.1);
    cairo_stroke(cr);
  });
  return true;
}

static void
draw(cairo_t* cr,
     float    x1,
     float    y1,
     float    x2,
     float    y2,
     bool     filled,
     bool     square)
{
  auto  w = abs(x1 - x2);
  auto  h = abs(y1 - y2);
  float x, y;
  if (square) {
    if (x1 <= x2) {
      x = x1;
    } else {
      x = x2;
      if (w > h) {
        x += w - h;
      }
    }
    if (y1 <= y2) {
      y = y1;
    } else {
      y = y2;
      if (h > w) {
        y += h - w;
      }
    }
    w = h = min(w, h);
  } else {
    x = min(x1, x2);
    y = min(y1, y2);
  }
  cairo_rectangle(cr, x, y, w, h);
  if (filled) {
    cairo_fill(cr);
  } else {
    cairo_stroke(cr);
  }
}

bool
RectangleTool::clickUp(const ClickContext& ctx)
{
  if (!mouseDown) {
    return false;
  }
  float x = ctx.x;
  float y = ctx.y;
  ctx.execute([
    x,
    y,
    oldX   = this->oldX,
    oldY   = this->oldY,
    mode   = this->mode,
    square = this->square
  ](cairo_t * cr) { draw(cr, oldX, oldY, x, y, mode == FILLED, square); });
  mouseDown = false;
  return true;
}

bool
RectangleTool::move(const MoveContext& ctx)
{
  float x = ctx.x;
  float y = ctx.y;
  if (mouseDown) {
    ctx.preview([
      x,
      y,
      oldX   = this->oldX,
      oldY   = this->oldY,
      mode   = this->mode,
      square = this->square
    ](cairo_t * cr) { draw(cr, oldX, oldY, x, y, mode == FILLED, square); });
    return true;
  }
  return false;
}
}
