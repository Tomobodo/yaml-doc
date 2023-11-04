#pragma once

#include "exceptions/DocException.hpp"

namespace YAML {
class DocParserException : public DocException
{
public:
  DocParserException(const std::string pMessage);
};
}
