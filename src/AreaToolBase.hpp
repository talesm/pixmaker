#pragma once

#include "Tool.hpp"

namespace pix {

/**
 * @brief A base class for area tools.
 *
 * Area tools are any tool where you can select an area and the shape will be
 * rendered inside it.
 *
 */
class AreaToolBase : public Tool
{
public:
  enum Mode : uint8_t
  {
    WIRED,
    FILLED
  };
  AreaToolBase(Mode mode, bool square)
    : mode(mode)
    , square(square)
  {}

  virtual bool clickDown(const ClickContext& ctx) final;
  virtual bool clickUp(const ClickContext& ctx) final;
  virtual bool move(const MoveContext& ctx) final;

protected:
  virtual void draw(cairo_t* cr,
                    float    x1,
                    float    y1,
                    float    x2,
                    float    y2,
                    bool     filled,
                    bool     square) = 0;

private:
  float oldX, oldY;
  Mode  mode;
  bool  square;
  bool  mouseDown = false;
};
}
