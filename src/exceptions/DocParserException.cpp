#include "exceptions/DocParserException.hpp"

namespace YAML {
DocParserException::DocParserException(const std::string pMessage)
{
  message = "[DocParserException] " + pMessage;
}
}
