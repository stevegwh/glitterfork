
#include <string>
#include "Application.hpp"

int main(int argc, char *argv[])
{
    std::string fullPath = std::string(argv[0]);
    std:: string BINARY_PATH = fullPath.substr(0, fullPath.find_last_of('/', fullPath.size())) + "/";
    sage::Application app;
    app.BINARY_PATH = BINARY_PATH;
    app.Update();
    return 0;
}