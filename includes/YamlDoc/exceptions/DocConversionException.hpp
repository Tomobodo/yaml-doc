#pragma once

#include "DocException.hpp"

namespace YAML {
class DocConversionException : public DocException
{
public:
  DocConversionException(const std::string pMessage);
};
}
