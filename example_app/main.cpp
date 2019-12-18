#include <gwasm.hpp>

#include "../test/my_functions.hpp"

int
main(int argc, char* argv[])
{
    return gwasm::run(argc, argv, my_splitter, my_executor, my_merger);
}
