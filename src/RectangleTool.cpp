#include "RectangleTool.hpp"
#include <cmath>
#include <cairo.h>

namespace pix {
using namespace std;

void
RectangleTool::draw(cairo_t* cr,
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
}
