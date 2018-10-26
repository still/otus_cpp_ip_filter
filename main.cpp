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
        std::sort(pool.begin(), pool.end(), std::greater<IpAddress>());
        printPool(pool);

        // filter 1
        printPool(filterBegin(pool, 1_octet));

        // filter 2
        printPool(filterBegin(pool, 46_octet, 70_octet));

        // filter 3
        printPool(filterAny(pool, 46_octet));
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
