#include "Window.hpp"
#include <fstream>
#include <iostream>
#include <vector>

int main(int argc, char **argv)
{
    {
        gpgpu::Window win;
        win.Run();
    }

    return EXIT_SUCCESS;
}