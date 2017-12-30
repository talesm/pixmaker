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

static unsigned
hex2unsigned(const string& s)
{
  unsigned r = 0;
  for (auto ch : s) {
    if (ch >= '0' && ch <= '9') {
      r = (r << 4) | (ch & 0x0f);
    } else if (ch >= 'A' && ch <= 'F') {
      r = (r << 4) | ((ch & 0x0f) + 9);
    } else if (ch >= 'a' && ch <= 'f') {
      r = (r << 4) | ((ch & 0x0f) + 9);
    }
  }
  return r;
}

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
  if (colorName.size() == 4 && colorName[0] == '#') {
    double r = hex2unsigned(colorName.substr(1, 1)) / 15.;
    double g = hex2unsigned(colorName.substr(2, 1)) / 15.;
    double b = hex2unsigned(colorName.substr(3, 1)) / 15.;
    return Source(make_unique<ColorSource>(r, g, b));
  }
  if (colorName.size() == 7 && colorName[0] == '#') {
    double r = hex2unsigned(colorName.substr(1, 2)) / 255.;
    double g = hex2unsigned(colorName.substr(3, 2)) / 255.;
    double b = hex2unsigned(colorName.substr(5, 2)) / 255.;
    return Source(make_unique<ColorSource>(r, g, b));
  }
  throw runtime_error("Invalid color: " + colorName);
}
}
