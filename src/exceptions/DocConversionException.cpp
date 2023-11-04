#include "exceptions/DocConversionException.hpp"

namespace YAML {
DocConversionException::DocConversionException(const std::string pMessage)
{
  message = "[DocConversionException] " + pMessage;
}
}
