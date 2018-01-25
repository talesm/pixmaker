/**
 * @brief Private Header
 *
 */
#pragma once

#include <functional>

typedef struct _cairo cairo_t; // Forward declaration. Ignore

namespace pix {

enum class Button : uint8_t; // Forward declaration. Ignore
struct Subject;              // Forward declaration. Ignore

/**
 * @brief Represents a Context for tool use
 *
 */
class ToolContext
{
public:
  ToolContext(Subject& subject)
    : subject(subject)
  {}

  using Action = std::function<void(cairo_t*)>;
  void preview(Action action) const;
  void execute(Action action) const;

private:
  Subject& subject;
};

/**
 * @brief Represents a Context at Move event
 *
 */
struct MoveContext : public ToolContext
{
public:
  MoveContext(Subject& subject, float x, float y)
    : ToolContext(subject)
    , x(x)
    , y(y)
  {}

  float x;
  float y;
};

/**
 * @brief Represents a Context at Click event
 *
 */
struct ClickContext : public MoveContext
{
public:
  ClickContext(Subject& subject, float x, float y, Button button)
    : MoveContext(subject, x, y)
    , button(button)
  {}

  Button button;
};
}