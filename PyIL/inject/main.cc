#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Expected 2 arguments: dll_path, process_id\n";
        return 1;
    }

    char* dll_path = argv[0];
    int process_id = std::stoi(argv[1]);

    std::cout << dll_path << '\n' << std::to_string(process_id) << '\n';
}