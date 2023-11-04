#include "exceptions/DocException.hpp"

namespace YAML {
DocException::DocException()
{
  message = "[DocException]";
}

DocException::DocException(const std::string pMessage)
{
  message = "[DocException] " + pMessage;
}

const char* DocException::what() const noexcept
{
  return message.c_str();
}
}
