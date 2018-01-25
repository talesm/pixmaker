#pragma once

#include "Tool.hpp"

namespace pix {

/**
 * @brief A Pencil/Brush like tool
 *
 */
class BrushTool : public Tool
{
public:
  virtual bool clickDown(const ClickContext& ctx) override;
  virtual bool clickUp(const ClickContext& ctx) override;
  virtual bool move(const MoveContext& ctx) override;

private:
  float oldX, oldY;
  bool  mouseDown = false;
};
}