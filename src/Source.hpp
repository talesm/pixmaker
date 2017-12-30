#pragma once
#include <memory>

namespace pix {

struct SourceDetail;

class Source
{
public:
  Source(std::unique_ptr<SourceDetail> detail);
  ~Source();
  Source(const Source&) = delete;
  Source& operator=(const Source&) = delete;
  Source(Source&&);
  Source& operator=(Source&&);

  /**
   * @brief Generate
   *
   * @param colorName one of CSS2 Color names
   * @return Source
   */
  static Source FromColorName(const std::string& colorName);

private:
  std::unique_ptr<SourceDetail> detail;

  friend class Subject;
};
}