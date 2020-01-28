#include <iostream>
#include <stdexcept>
#include <cstdlib>

#include "templateApplication.h"

int main (int argc, char* argv[]) {

  TemplateApplication app;

  try {
      app.run();
  } catch (const std::exception& e) {
      std::cerr << e.what() << std::endl;
      return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
