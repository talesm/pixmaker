#pragma once

#include <functional>

typedef struct _cairo cairo_t; // Forward declaration. Ignore

namespace pix {

struct ClickContext; // Forward declaractions.
struct MoveContext;  // Forward declaractions.

/**
 * @brief An abstract tool.
 *
 */
class Tool
{
public:
  virtual ~Tool()                                 = default;
  virtual bool clickDown(const ClickContext& ctx) = 0;
  virtual bool clickUp(const ClickContext& ctx)   = 0;
  virtual bool move(const MoveContext& ctx)       = 0;
};
}