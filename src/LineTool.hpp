#pragma once

#include "Tool.hpp"

namespace pix {

/**
 * @brief A Pencil/Brush like tool
 *
 */
class LineTool : public Tool
{
public:
  LineTool(bool multiline = true)
    : multiline(multiline)
  {}

  virtual bool clickDown(const ClickContext& ctx) override;
  virtual bool clickUp(const ClickContext& ctx) override;
  virtual bool move(const MoveContext& ctx) override;

private:
  float oldX, oldY;
  bool  multiline;
  bool  mouseDown = false;
};
}
