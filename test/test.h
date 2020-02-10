#include <iostream>
#include <string>

#define PRINT(container)                      \
    do {                                      \
        std::string con_name = #container;    \
        std::cout << " " << con_name << " :"; \
        for (auto it : container)             \
            std::cout << " " << it;           \
        std::cout << "\n";                    \
    } while (0)

#define FUN_AFTER(con, fun)                           \
    do {                                              \
        std::string fun_name = #fun;                  \
        std::cout << " After " << fun_name << " :\n"; \
        fun;                                          \
        PRINT(con);                                   \
    } while (0)

#define FUN_VALUE(fun)                                        \
    do {                                                      \
        std::string fun_name = #fun;                          \
        std::cout << " " << fun_name << " : " << fun << "\n"; \
    } while (0)
