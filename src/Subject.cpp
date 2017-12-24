#include "Subject.hpp"
#include <cairo.h>

using namespace std;

namespace pix {
struct SubjectDetail
{
  cairo_surface_t* surface;
  cairo_t*         cr;
};

Subject::Subject(std::unique_ptr<SubjectDetail> detail)
  : detail(std::move(detail))
{}

Subject::~Subject()
{
  if (detail) {
    cairo_destroy(detail->cr);
    cairo_surface_destroy(detail->surface);
  }
}

Subject
Subject::create(unsigned w, unsigned h)
{
  auto detail = make_unique<SubjectDetail>();
  detail->surface =
    cairo_image_surface_create(CAIRO_FORMAT_ARGB32, int(w), int(h));
  detail->cr = cairo_create(detail->surface);
  cairo_set_source_rgb(detail->cr, 1, 1, 1);
  cairo_paint(detail->cr);
  cairo_set_source_rgb(detail->cr, 0, 0, 0);
  return Subject(std::move(detail));
}

static bool  mouseDown = false;
static float old_x, old_y;

bool
Subject::clickDown(float x, float y, Button button)
{
  cairo_move_to(detail->cr, x, y);
  old_x = x;
  old_y = y;
  cairo_stroke(detail->cr);
  mouseDown = true;
  return true;
}

bool
Subject::clickUp(float x, float y, Button button)
{
  mouseDown = false;
  return true;
}

bool
Subject::move(float x, float y)
{
  if (mouseDown) {
    cairo_move_to(detail->cr, old_x, old_y);
    cairo_line_to(detail->cr, x, y);
    old_x = x;
    old_y = y;

    cairo_stroke(detail->cr);
    return true;
  }
  return false;
}

void
Subject::render(const Renderer& renderer) const
{
  cairo_surface_flush(detail->surface);
  auto data   = cairo_image_surface_get_data(detail->surface);
  auto w      = cairo_image_surface_get_width(detail->surface);
  auto h      = cairo_image_surface_get_height(detail->surface);
  auto stride = cairo_image_surface_get_stride(detail->surface);
  renderer(w, h, 32, stride, data);
}

void
Subject::save(const char* path) const
{
  auto status = cairo_surface_write_to_png(detail->surface, path);
  if (status != CAIRO_STATUS_SUCCESS) {
    exit(status); // TODO: Error handling.
  }
}
}