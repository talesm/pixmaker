#include "ToolContext.hpp"
#include "Subject.hpp"

namespace pix {

void
ToolContext::preview(Action action) const
{
  subject.preview(action);
}
void
ToolContext::execute(Action action) const
{
  subject.execute(action);
}
}