#include "assignment4.h"

int main(int, char**)
{
    int result = EXIT_SUCCESS;
    try
    {
        assignment4();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        result = EXIT_FAILURE;  // abnormal termination of program
    }

    return result;
}
