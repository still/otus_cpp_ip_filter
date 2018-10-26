#include "lib.h"

int version()
{
    return PROJECT_VERSION_PATCH;
}

auto filterPool(const IpPool &pool, bool (*filter)(const IpAddress &))
{
    IpPool res;
    for(auto address: pool)
        if(!filter || filter(address))
            res.push_back(address);
    return res;
}

std::tuple<bool, IpAddress> makeAddress(const std::string &str)
{
    return split<IpPart>(str, '.', 4, [](const std::string& str) {
        if(str.empty())
            return std::make_tuple(false, IpPart(0));
        int val = atoi(str.data());
        return std::make_tuple(val >= 0 && val <= 255, IpPart(val));
    });
}

IpPart operator"" _octet(unsigned long long a)
{
    // can't use static_assert((
    assert(a < 256);
    return IpPart(a);
}

std::string address2String(const IpAddress &address, const std::string &delim)
{
    std::string res;
    for(size_t i = 0, i_end = address.size(); i < i_end; ++i)
    {
        res += i ? delim: "";
        res += std::to_string(address.at(i));
    }
    return res;
}

void printPool(const IpPool &pool, bool (*filter)(const IpAddress &))
{
    for(auto address: pool)
        if(!filter || filter(address))
            std::cout << address2String(address, ".") << std::endl;
}

IpPool filterAny(const IpPool& pool, const IpPart &octet)
{
    IpPool result;
    std::copy_if(pool.begin(), pool.end(), std::back_inserter(result),
                [&octet](IpAddress address) {
        return std::any_of(address.begin(), address.end(), [octet](IpPart part) { return part == octet; });
    });
    return result;
}
