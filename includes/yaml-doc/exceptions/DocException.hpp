#pragma once

#include <exception>
#include <string>

namespace YAML {
class DocException : std::exception
{
public:
  DocException();
  DocException(const std::string pMessage);

  virtual const char* what() const noexcept override;

protected:
  std::string message;
};
}
