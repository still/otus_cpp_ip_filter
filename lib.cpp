#include "lib.h"

int version()
{
    return PROJECT_VERSION_PATCH;
}

auto generateSortLambda(bool (*compare)(const IpPart &, const IpPart &))
{
    return [compare](const IpAddress& a, const IpAddress& b) {
        for(size_t i = 0; i < a.size(); i++)
        {
            if(a.at(i) == b.at(i))
                continue;
            return compare(a.at(i), b.at(i));
        }
        return false;
    };
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

void sortAsc(IpPool &pool)
{
    std::sort(pool.begin(), pool.end(), generateSortLambda([](const IpPart& a, const IpPart& b) {
        return a < b;
    }));
}

void sortDesc(IpPool &pool)
{
    std::sort(pool.begin(), pool.end(), generateSortLambda([](const IpPart& a, const IpPart& b) {
        return a > b;
    }));
}

IpPool filter1(const IpPool &pool)
{
    return filterPool(pool, [](const IpAddress& v) {
        return
                v.at(0) == 1_octet;
    });
}

IpPool filter2(const IpPool &pool)
{
    return filterPool(pool, [](const IpAddress& v) {
        return
                v.at(0) == 46_octet &&
                v.at(1) == 70_octet;
    });
}

IpPool filter3(const IpPool &pool)
{
    return filterPool(pool, [](const IpAddress& v) {
        return std::find(v.begin(), v.end(), 46_octet) != v.end();
    });
}
