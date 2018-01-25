#pragma once

#include "Tool.hpp"

namespace pix {

/**
 * @brief A Pencil/Brush like tool
 *
 */
class RectangleTool : public Tool
{
public:
  enum Mode : uint8_t
  {
    WIRED,
    FILLED
  };
  RectangleTool(Mode mode, bool square = false)
    : mode(mode)
    , square(square)
  {}

  virtual bool clickDown(const ClickContext& ctx) override;
  virtual bool clickUp(const ClickContext& ctx) override;
  virtual bool move(const MoveContext& ctx) override;

private:
  float oldX, oldY;
  Mode  mode;
  bool  square;
  bool  mouseDown = false;
};
}
