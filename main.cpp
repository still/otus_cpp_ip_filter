#include "lib.h"

int main()
{
    try {
        IpPool pool;
        bool isOk;
        LineParts lineParts;
        IpAddress address;

        //parsing
        for(std::string line; getline(std::cin, line);)
        {
            std::tie(isOk, lineParts) = split<std::string>(line, '\t', 3, [](const std::string& str) {
                return std::make_tuple(!str.empty(), str);
            });
            if(!isOk) continue;

            std::tie(isOk, address) = makeAddress(lineParts.at(0));
            if(!isOk) continue;

            pool.push_back(address);
        }

        // sort Desc
        sortDesc(pool);
        printPool(pool);

        // sort Asc
//        sortAsc(pool);
//        printPool(pool);

        // filter 1
        printPool(filter1(pool));

        // filter 2
        printPool(filter2(pool));

        // filter 3
        printPool(filter3(pool));
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
