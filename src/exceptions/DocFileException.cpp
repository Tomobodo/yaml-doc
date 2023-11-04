#include "exceptions/DocFileException.hpp"

namespace YAML {
DocFileException::DocFileException(const std::string pMessage,
                                   const std::string pFileName)
{
  message = "[DocFileException] " + pFileName + ", " + pMessage;
}
}
