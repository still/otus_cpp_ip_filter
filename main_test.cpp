#include "lib.h"

#define BOOST_TEST_MODULE ip_filter_test

#include <boost/test/unit_test.hpp>

auto genAddress(const std::string& str)
{
    bool isOk;
    IpAddress address;
    std::tie(isOk, address) = makeAddress(str);
    return address;
}

BOOST_AUTO_TEST_SUITE(test_suite_main)

BOOST_AUTO_TEST_CASE(test_sort)
{
    IpPool pool;
    pool.push_back(genAddress("2.2.2.2"));
    pool.push_back(genAddress("3.3.3.3"));
    pool.push_back(genAddress("1.1.1.1"));

    std::sort(pool.begin(), pool.end(), std::greater<IpAddress>());
    BOOST_CHECK(pool.at(0).at(0) == 3_octet);
    BOOST_CHECK(pool.at(1).at(0) == 2_octet);
    BOOST_CHECK(pool.at(2).at(0) == 1_octet);
}

BOOST_AUTO_TEST_CASE(test_filter)
{
    IpPool pool;
    pool.push_back(genAddress("2.2.46.2"));
    pool.push_back(genAddress("46.70.3.3"));
    pool.push_back(genAddress("1.1.1.1"));


    IpPool resPool = filterBegin(pool, 1_octet);
    BOOST_CHECK(resPool.size() == 1);
    for(IpAddress address: resPool)
        BOOST_CHECK(address.at(0) == 1_octet);

    resPool = filterBegin(pool, 46_octet, 70_octet);
    BOOST_CHECK(resPool.size() == 1);
    for(IpAddress address: resPool)
    {
        BOOST_CHECK(address.at(0) == 46_octet);
        BOOST_CHECK(address.at(1) == 70_octet);
    }

    resPool = filterAny(pool, 46_octet);
    BOOST_CHECK(resPool.size() == 2);
    for(IpAddress address: resPool)
    {
        BOOST_CHECK(address.at(0) == 46_octet ||
                    address.at(1) == 46_octet ||
                    address.at(2) == 46_octet ||
                    address.at(3) == 46_octet);
    }

    resPool = filterBegin(pool);
    BOOST_CHECK(resPool.size() == 3);
}

BOOST_AUTO_TEST_SUITE_END()
