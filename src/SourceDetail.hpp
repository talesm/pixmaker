/**
 * @brief Private header
 *
 */
#pragma once
#include <cairo.h>

namespace pix {
/**
 * @brief Implementation of source.
 *
 */
class SourceDetail
{
public:
  virtual ~SourceDetail()               = default;
  virtual void apply(cairo_t* cr) const = 0;

protected:
  SourceDetail() = default;
};
}