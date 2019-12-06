#ifndef GWASM_TEST_ARGS_HPP
#define GWASM_TEST_ARGS_HPP

#include <string>
#include <vector>

class Args
{
private:
    std::vector<std::string> m_args;
    std::vector<char*> m_c_args;

public:
    Args(std::vector<std::string>&& args);

    int c() const;
    char** v();
};

#endif // GWASM_TEST_ARGS_HPP
