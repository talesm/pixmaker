#pragma once
#include <functional>
#include <memory>

namespace pix {

/**
 * @brief Represents mouse buttons
 */
enum class Button : unsigned char
{
  LEFT,
  MIDDLE,
  RIGHT
};

struct SubjectDetail; // Forward decl. Ignore this

/**
 * @brief Represents a drawable target.
 */
class Subject
{
  std::unique_ptr<SubjectDetail> detail;
  Subject(std::unique_ptr<SubjectDetail> detail);

public:
  ~Subject();
  Subject(const Subject&) = delete;
  Subject& operator=(const Subject&) = delete;
  Subject(Subject&&);
  Subject& operator=(Subject&&);

  static Subject create(unsigned w, unsigned h);
  static Subject load(const char* file);

  bool clickDown(float x, float y, Button button);
  bool clickUp(float x, float y, Button button);
  bool move(float x, float y);

  using Renderer = std::function<void(unsigned             w,
                                      unsigned             h,
                                      unsigned char        bpp,
                                      unsigned             stride,
                                      const unsigned char* pixels)>;
  /**
   * Renders the resulting image
   */
  void render(const Renderer& renderer) const;

  /**
   * Save with the internal format.
   */
  void save(const char* path) const;
};
}