#pragma once

#include "AreaToolBase.hpp"

namespace pix {

/**
 * @brief An Ellipsis tool
 *
 */
class EllipseTool : public AreaToolBase
{
public:
  EllipseTool(Mode mode, bool square = false)
    : AreaToolBase(mode, square)
  {}

  virtual void draw(cairo_t* cr,
                    float    x1,
                    float    y1,
                    float    x2,
                    float    y2,
                    bool     filled,
                    bool     square) final;

private:
  float oldX, oldY;
  Mode  mode;
  bool  square;
  bool  mouseDown = false;
};
}
