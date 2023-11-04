#pragma once

#include "exceptions/DocException.hpp"
namespace YAML {
class DocNodeException : public DocException
{
public:
  DocNodeException(std::string pMessage);
};
}
