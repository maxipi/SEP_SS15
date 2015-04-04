#include "OutOfMemory.h"
#include <iostream>

OutOfMemory::OutOfMemory() : exception()
{
}

OutOfMemory::OutOfMemory(OutOfMemory const &src)
: exception(src)
{
}

OutOfMemory::~OutOfMemory() throw()
{
}

const char *OutOfMemory::what() const throw()
{
  return "Error: Out of Memory!";
  //std::cout << "Error: Out of Memory!" << std::endl;
}