#include "exceptions/DocNodeException.hpp"

namespace YAML {
DocNodeException::DocNodeException(std::string pMessage)
{
  message = "[DocNodeException] " + pMessage;
}
}
