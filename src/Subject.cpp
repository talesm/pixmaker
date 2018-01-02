#include "Subject.hpp"
#include <cairo.h>
#include "SourceDetail.hpp"

using namespace std;

namespace pix {
struct SubjectDetail
{
  cairo_surface_t* targetSurface  = nullptr;
  cairo_t*         targetCr       = nullptr;
  cairo_surface_t* previewSurface = nullptr;
  cairo_t*         previewCr      = nullptr;
  Source           source;

  SubjectDetail()
    : SubjectDetail(nullptr)
  {}

  SubjectDetail(cairo_surface_t* targetSurface)
    : targetSurface(targetSurface)
    , source(Source::FromColorName("black"))
  {
    if (targetSurface) {
      targetCr       = cairo_create(targetSurface);
      auto w         = cairo_image_surface_get_width(targetSurface);
      auto h         = cairo_image_surface_get_height(targetSurface);
      previewSurface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, w, h);
      previewCr      = cairo_create(previewSurface);
    }
  }

  cairo_surface_t* render() const
  {
    cairo_set_source_surface(previewCr, targetSurface, 0, 0);
    cairo_paint(previewCr);
    return previewSurface;
  }

  ~SubjectDetail()
  {
    cairo_destroy(previewCr);
    cairo_surface_destroy(previewSurface);
    cairo_destroy(targetCr);
    cairo_surface_destroy(targetSurface);
  }
};

Subject::Subject(std::unique_ptr<SubjectDetail> detail)
  : detail(std::move(detail))
{}

Subject::~Subject() = default;

Subject::Subject(Subject&&) = default;
Subject&
Subject::operator=(Subject&&) = default;

Subject
Subject::create(unsigned w, unsigned h)
{
  auto detail = make_unique<SubjectDetail>(
    cairo_image_surface_create(CAIRO_FORMAT_ARGB32, int(w), int(h)));
  cairo_set_source_rgb(detail->targetCr, 1, 1, 1);
  cairo_paint(detail->targetCr);
  cairo_set_source_rgb(detail->targetCr, 0, 0, 0);
  return Subject(std::move(detail));
}

Subject
Subject::load(const char* file)
{
  return Subject(
    make_unique<SubjectDetail>(cairo_image_surface_create_from_png(file)));
}

static bool  mouseDown = false;
static float old_x, old_y;

bool
Subject::clickDown(float x, float y, Button button)
{
  cairo_move_to(detail->targetCr, x, y);
  old_x = x;
  old_y = y;
  cairo_stroke(detail->targetCr);
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
    cairo_move_to(detail->targetCr, old_x, old_y);
    cairo_line_to(detail->targetCr, x, y);
    old_x = x;
    old_y = y;

    cairo_stroke(detail->targetCr);
    return true;
  }
  return false;
}

void
Subject::render(const Renderer& renderer) const
{
  detail->render();
  cairo_surface_flush(detail->previewSurface);
  auto data   = cairo_image_surface_get_data(detail->previewSurface);
  auto w      = cairo_image_surface_get_width(detail->previewSurface);
  auto h      = cairo_image_surface_get_height(detail->previewSurface);
  auto stride = cairo_image_surface_get_stride(detail->previewSurface);
  renderer(w, h, 32, stride, data);
}

void
Subject::save(const char* path) const
{
  auto status = cairo_surface_write_to_png(detail->targetSurface, path);
  if (status != CAIRO_STATUS_SUCCESS) {
    exit(status); // TODO: Error handling.
  }
}

void
Subject::source(Source&& value)
{
  detail->source = std::move(value);
  detail->source.detail->apply(detail->targetCr);
}

const Source&
Subject::source() const
{
  return detail->source;
}
}