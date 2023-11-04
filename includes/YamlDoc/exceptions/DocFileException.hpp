#pragma once

#include "DocException.hpp"

namespace YAML {
class DocFileException : public DocException
{
public:
  DocFileException(const std::string pMessage, const std::string pFileName);
};
}
