#include "AreaToolBase.hpp"
#include <cairo.h>
#include "Button.hpp"
#include "ToolContext.hpp"

namespace pix {
using namespace std;

bool
AreaToolBase::clickDown(const ClickContext& ctx)
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

bool
AreaToolBase::clickUp(const ClickContext& ctx)
{
  if (!mouseDown) {
    return false;
  }
  float x = ctx.x;
  float y = ctx.y;
  ctx.execute([
    this,
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
AreaToolBase::move(const MoveContext& ctx)
{
  float x = ctx.x;
  float y = ctx.y;
  if (mouseDown) {
    ctx.preview([
      this,
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
