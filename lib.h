#pragma once

#include "version.h"

int version();

#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <tuple>

using LineParts = std::vector<std::string>;

using IpPart    = unsigned char;
using IpAddress = std::vector<IpPart>;
using IpPool    = std::vector<IpAddress>;
using CharIdx   = std::string::size_type;

template <typename T>
auto split(const std::string& str, char d, size_t count, std::tuple<bool, T> (*fn)(const std::string& ))
{
    std::vector<T> res;
    bool isValid = true;

    T part;
    bool isPartValid;

    CharIdx start = 0;
    CharIdx stop = str.find_first_of(d);
    std::string subStr;

    while(stop != std::string::npos)
    {
        subStr = str.substr(start, stop - start);
        std::tie(isPartValid, part) = fn(subStr);
        isValid = isPartValid ? isValid : false;
        res.push_back(part);
        start = stop + 1;
        stop = str.find_first_of(d, start);
    }

    subStr = str.substr(start);
    std::tie(isPartValid, part) = fn(subStr);
    isValid = isPartValid ? isValid : false;
    res.push_back(part);

    return std::make_tuple(isValid && res.size() == count, res);
}

std::tuple<bool, IpAddress> makeAddress(const std::string& str);

IpPart operator"" _octet(unsigned long long a);

std::string address2String(const IpAddress& address, const std::string& delim);
void printPool(const IpPool& pool, bool(*filter)(const IpAddress& addr) = nullptr);

IpPool filterAny(const IpPool &pool, const IpPart &octet);

template<typename... Args>
IpPool filterBegin(const IpPool& pool, Args ... args)
{
    IpPool result;
    size_t size = sizeof...(args);
    if(size > 4)
        return result;

    IpAddress filterAddress = {args ...};

    std::copy_if(pool.begin(), pool.end(), std::back_inserter(result),
                  [&](IpAddress address) {
        return std::equal(address.begin(), address.begin() + size, filterAddress.begin());
    });

    return result;
}
