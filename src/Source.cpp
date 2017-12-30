#include "Source.hpp"
#include "SourceDetail.hpp"

using namespace std;

namespace pix {

Source::Source(std::unique_ptr<SourceDetail> detail)
  : detail(std::move(detail))
{}
Source::~Source()        = default;
Source::Source(Source&&) = default;
Source&
Source::operator=(Source&&) = default;

struct ColorSource : public SourceDetail
{
  double r;
  double g;
  double b;
  ColorSource(double r, double g, double b)
    : r(r)
    , g(g)
    , b(b)
  {}

  void apply(cairo_t* cr) const override { cairo_set_source_rgb(cr, r, g, b); }
};
Source
Source::FromColorName(const std::string& colorName)
{
  // Sourced from https://developer.mozilla.org/en-US/docs/Web/CSS/color_value
  if (colorName == "black")
    return Source(
      make_unique<ColorSource>(0x00 / 255.0, 0x00 / 255.0, 0x00 / 255.0));
  if (colorName == "silver")
    return Source(
      make_unique<ColorSource>(0xc0 / 255.0, 0xc0 / 255.0, 0xc0 / 255.0));
  if (colorName == "gray")
    return Source(
      make_unique<ColorSource>(0x80 / 255.0, 0x80 / 255.0, 0x80 / 255.0));
  if (colorName == "white")
    return Source(
      make_unique<ColorSource>(0xff / 255.0, 0xff / 255.0, 0xff / 255.0));
  if (colorName == "maroon")
    return Source(
      make_unique<ColorSource>(0x80 / 255.0, 0x00 / 255.0, 0x00 / 255.0));
  if (colorName == "red")
    return Source(
      make_unique<ColorSource>(0xff / 255.0, 0x00 / 255.0, 0x00 / 255.0));
  if (colorName == "purple")
    return Source(
      make_unique<ColorSource>(0x80 / 255.0, 0x00 / 255.0, 0x80 / 255.0));
  if (colorName == "fuchsia")
    return Source(
      make_unique<ColorSource>(0xff / 255.0, 0x00 / 255.0, 0xff / 255.0));
  if (colorName == "green")
    return Source(
      make_unique<ColorSource>(0x00 / 255.0, 0x80 / 255.0, 0x00 / 255.0));
  if (colorName == "lime")
    return Source(
      make_unique<ColorSource>(0x00 / 255.0, 0xff / 255.0, 0x00 / 255.0));
  if (colorName == "olive")
    return Source(
      make_unique<ColorSource>(0x80 / 255.0, 0x80 / 255.0, 0x00 / 255.0));
  if (colorName == "yellow")
    return Source(
      make_unique<ColorSource>(0xff / 255.0, 0xff / 255.0, 0x00 / 255.0));
  if (colorName == "navy")
    return Source(
      make_unique<ColorSource>(0x00 / 255.0, 0x00 / 255.0, 0x80 / 255.0));
  if (colorName == "blue")
    return Source(
      make_unique<ColorSource>(0x00 / 255.0, 0x00 / 255.0, 0xff / 255.0));
  if (colorName == "teal")
    return Source(
      make_unique<ColorSource>(0x00 / 255.0, 0x80 / 255.0, 0x80 / 255.0));
  if (colorName == "aqua")
    return Source(
      make_unique<ColorSource>(0x00 / 255.0, 0xff / 255.0, 0xff / 255.0));
  if (colorName == "orange")
    return Source(
      make_unique<ColorSource>(0xff / 255.0, 0xa5 / 255.0, 0x00 / 255.0));
  throw runtime_error("Invalid color: " + colorName);
}
}
