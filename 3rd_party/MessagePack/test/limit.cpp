#include <msgpack.hpp>
#include <gtest/gtest.h>
#include <sstream>

TEST(limit, unpack_array_no_over)
{
    std::stringstream ss;
    std::vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    msgpack::pack(ss, v);
    try {
        msgpack::unpacked unp;
        msgpack::unpack(unp, ss.str().c_str(), ss.str().size(), nullptr, nullptr,
                        msgpack::unpack_limit(3, 0, 0, 0, 0));
        EXPECT_TRUE(true);
    }
    catch(msgpack::array_size_overflow const&) {
        EXPECT_TRUE(false);
    }
    catch(...) {
        EXPECT_TRUE(false);
    }
}

TEST(limit, unpack_array_over)
{
    std::stringstream ss;
    std::vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    msgpack::pack(ss, v);
    try {
        msgpack::unpacked unp;
        msgpack::unpack(unp, ss.str().c_str(), ss.str().size(), nullptr, nullptr,
                        msgpack::unpack_limit(2, 0, 0, 0, 0));
        EXPECT_TRUE(false);
    }
    catch(msgpack::array_size_overflow const&) {
        EXPECT_TRUE(true);
    }
    catch(...) {
        EXPECT_TRUE(false);
    }
}

TEST(limit, unpack_map_no_over)
{
    std::stringstream ss;
    std::map<int, int> m;
    m[1] = 1;
    m[2] = 2;
    m[3] = 3;
    msgpack::pack(ss, m);
    try {
        msgpack::unpacked unp;
        msgpack::unpack(unp, ss.str().c_str(), ss.str().size(), nullptr, nullptr,
                        msgpack::unpack_limit(0, 3, 0, 0, 0));
        EXPECT_TRUE(true);
    }
    catch(msgpack::map_size_overflow const&) {
        EXPECT_TRUE(false);
    }
    catch(...) {
        EXPECT_TRUE(false);
    }
}

TEST(limit, unpack_map_over)
{
    std::stringstream ss;
    std::map<int, int> m;
    m[1] = 1;
    m[2] = 2;
    m[3] = 3;
    msgpack::pack(ss, m);
    try {
        msgpack::unpacked unp;
        msgpack::unpack(unp, ss.str().c_str(), ss.str().size(), nullptr, nullptr,
                        msgpack::unpack_limit(0, 2, 0, 0, 0));
        EXPECT_TRUE(false);
    }
    catch(msgpack::map_size_overflow const&) {
        EXPECT_TRUE(true);
    }
    catch(...) {
        EXPECT_TRUE(false);
    }
}

TEST(limit, unpack_str_no_over)
{
    std::stringstream ss;
    std::string s("123");
    msgpack::pack(ss, s);
    try {
        msgpack::unpacked unp;
        msgpack::unpack(unp, ss.str().c_str(), ss.str().size(), nullptr, nullptr,
                        msgpack::unpack_limit(0, 0, 3, 0, 0));
        EXPECT_TRUE(true);
    }
    catch(msgpack::str_size_overflow const&) {
        EXPECT_TRUE(false);
    }
    catch(...) {
        EXPECT_TRUE(false);
    }
}

TEST(limit, unpack_str_over)
{
    std::stringstream ss;
    std::string s("123");
    msgpack::pack(ss, s);
    try {
        msgpack::unpacked unp;
        msgpack::unpack(unp, ss.str().c_str(), ss.str().size(), nullptr, nullptr,
                        msgpack::unpack_limit(0, 0, 2, 0, 0));
        EXPECT_TRUE(false);
    }
    catch(msgpack::str_size_overflow const&) {
        EXPECT_TRUE(true);
    }
    catch(...) {
        EXPECT_TRUE(false);
    }
}

TEST(limit, unpack_bin_no_over)
{
    std::stringstream ss;
    std::vector<char> v;
    v.push_back('1');
    v.push_back('2');
    v.push_back('3');
    msgpack::pack(ss, v);
    try {
        msgpack::unpacked unp;
        msgpack::unpack(unp, ss.str().c_str(), ss.str().size(), nullptr, nullptr,
                        msgpack::unpack_limit(0, 0, 0, 3, 0));
        EXPECT_TRUE(true);
    }
    catch(msgpack::bin_size_overflow const&) {
        EXPECT_TRUE(false);
    }
    catch(...) {
        EXPECT_TRUE(false);
    }
}

TEST(limit, unpack_bin_over)
{
    std::stringstream ss;
    std::vector<char> v;
    v.push_back('1');
    v.push_back('2');
    v.push_back('3');
    msgpack::pack(ss, v);
    try {
        msgpack::unpacked unp;
        msgpack::unpack(unp, ss.str().c_str(), ss.str().size(), nullptr, nullptr,
                        msgpack::unpack_limit(0, 0, 0, 2, 0));
        EXPECT_TRUE(false);
    }
    catch(msgpack::bin_size_overflow const&) {
        EXPECT_TRUE(true);
    }
    catch(...) {
        EXPECT_TRUE(false);
    }
}

TEST(limit, unpack_ext_no_over)
{
    std::stringstream ss;
    msgpack::packer<std::stringstream> packer(ss);
    char const buf [] = { 1, 2, 3 };
    packer.pack_ext(sizeof(buf), 1);
    packer.pack_ext_body(buf, sizeof(buf));
    try {
        msgpack::unpacked unp;
        msgpack::unpack(unp, ss.str().c_str(), ss.str().size(), nullptr, nullptr,
                        msgpack::unpack_limit(0, 0, 0, 0, 3+1));
        EXPECT_TRUE(true);
    }
    catch(msgpack::ext_size_overflow const&) {
        EXPECT_TRUE(false);
    }
    catch(...) {
        EXPECT_TRUE(false);
    }
}

TEST(limit, unpack_ext_over)
{
    std::stringstream ss;
    msgpack::packer<std::stringstream> packer(ss);
    char const buf [] = { 1, 2, 3 };
    packer.pack_ext(sizeof(buf), 1);
    packer.pack_ext_body(buf, sizeof(buf));
    try {
        msgpack::unpacked unp;
        msgpack::unpack(unp, ss.str().c_str(), ss.str().size(), nullptr, nullptr,
                        msgpack::unpack_limit(0, 0, 0, 0, 2+1));
        EXPECT_TRUE(false);
    }
    catch(msgpack::ext_size_overflow const&) {
        EXPECT_TRUE(true);
    }
    catch(...) {
        EXPECT_TRUE(false);
    }
}

TEST(limit, unpack_ext_over_32_bit)
{
    if (sizeof(std::size_t) == 4) {
        char const buf [] = {
            static_cast<char>(0xc9u),
            static_cast<char>(0xffu),
            static_cast<char>(0xffu),
            static_cast<char>(0xffu),
            static_cast<char>(0xffu),
            static_cast<char>(0x01u),
        };
        try {
            msgpack::unpacked unp;
            msgpack::unpack(unp, buf, sizeof(buf), nullptr, nullptr,
                            msgpack::unpack_limit(0, 0, 0, 0, 0xffffffff));
            EXPECT_TRUE(false);
        }
        catch(msgpack::ext_size_overflow const&) {
            EXPECT_TRUE(true);
        }
        catch(...) {
            EXPECT_TRUE(false);
        }
    }
}

TEST(limit, unpack_ext_no_over_64_bit)
{
    if (sizeof(std::size_t) == 8) {
        char const buf [] = {
            static_cast<char>(0xc9u),
            static_cast<char>(0xffu),
            static_cast<char>(0xffu),
            static_cast<char>(0xffu),
            static_cast<char>(0xffu),
            static_cast<char>(0x01u),
        };
        try {
            msgpack::unpacked unp;
            msgpack::unpack(unp, buf, sizeof(buf), nullptr, nullptr,
                            msgpack::unpack_limit(0, 0, 0, 0, 0xffffffff));
            EXPECT_TRUE(false);
        }
        catch(msgpack::ext_size_overflow const&) {
            EXPECT_TRUE(false);
        }
        catch(msgpack::insufficient_bytes const&) {
            EXPECT_TRUE(true);
        }
        catch(...) {
            EXPECT_TRUE(false);
        }
    }
}

TEST(limit, unpack_depth_no_over)
{
    std::stringstream ss;
    std::vector<int> inner;
    inner.push_back(1);
    std::vector<std::vector<int> > outer;
    outer.push_back(inner);
    msgpack::pack(ss, outer);
    try {
        msgpack::unpacked unp;
        msgpack::unpack(unp, ss.str().c_str(), ss.str().size(), nullptr, nullptr,
                        msgpack::unpack_limit(1, 0, 0, 0, 0, 2));
        EXPECT_TRUE(true);
    }
    catch(msgpack::depth_size_overflow const&) {
        EXPECT_TRUE(false);
    }
    catch(...) {
        EXPECT_TRUE(false);
    }
}

TEST(limit, unpack_depth_over)
{
    std::stringstream ss;
    std::vector<int> inner;
    inner.push_back(1);
    std::vector<std::vector<int> > outer;
    outer.push_back(inner);
    msgpack::pack(ss, outer);
    try {
        msgpack::unpacked unp;
        msgpack::unpack(unp, ss.str().c_str(), ss.str().size(), nullptr, nullptr,
                        msgpack::unpack_limit(1, 0, 0, 0, 0, 1));
        EXPECT_TRUE(false);
    }
    catch(msgpack::depth_size_overflow const&) {
        EXPECT_TRUE(true);
    }
    catch(...) {
        EXPECT_TRUE(false);
    }
}



#if !defined(MSGPACK_USE_CPP03)

TEST(limit, unpack_array_over_cpp11_no_off_no_ref)
{
    std::stringstream ss;
    std::vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    msgpack::pack(ss, v);
    try {
        msgpack::unpacked unp =
            msgpack::unpack(ss.str().c_str(), ss.str().size(), nullptr, nullptr,
                            msgpack::unpack_limit(2, 0, 0, 0, 0));
        EXPECT_TRUE(false);
    }
    catch(msgpack::array_size_overflow const&) {
        EXPECT_TRUE(true);
    }
    catch(...) {
        EXPECT_TRUE(false);
    }
}

TEST(limit, unpack_array_over_cpp11_no_off_ref)
{
    std::stringstream ss;
    std::vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    msgpack::pack(ss, v);
    try {
        bool ref;
        msgpack::unpacked unp =
            msgpack::unpack(ss.str().c_str(), ss.str().size(), ref, nullptr, nullptr,
                            msgpack::unpack_limit(2, 0, 0, 0, 0));
        EXPECT_TRUE(false);
    }
    catch(msgpack::array_size_overflow const&) {
        EXPECT_TRUE(true);
    }
    catch(...) {
        EXPECT_TRUE(false);
    }
}

TEST(limit, unpack_array_over_cpp11_off_no_ref)
{
    std::stringstream ss;
    std::vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    msgpack::pack(ss, v);
    try {
        std::size_t off = 0;
        msgpack::unpacked unp =
            msgpack::unpack(ss.str().c_str(), ss.str().size(), off, nullptr, nullptr,
                            msgpack::unpack_limit(2, 0, 0, 0, 0));
        EXPECT_TRUE(false);
    }
    catch(msgpack::array_size_overflow const&) {
        EXPECT_TRUE(true);
    }
    catch(...) {
        EXPECT_TRUE(false);
    }
}

TEST(limit, unpack_array_over_cpp11_off_ref)
{
    std::stringstream ss;
    std::vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    msgpack::pack(ss, v);
    try {
        bool ref;
        std::size_t off = 0;
        msgpack::unpacked unp =
            msgpack::unpack(ss.str().c_str(), ss.str().size(), off, ref, nullptr, nullptr,
                            msgpack::unpack_limit(2, 0, 0, 0, 0));
        EXPECT_TRUE(false);
    }
    catch(msgpack::array_size_overflow const&) {
        EXPECT_TRUE(true);
    }
    catch(...) {
        EXPECT_TRUE(false);
    }
}

#endif // !defined(MSGPACK_USE_CPP03)

TEST(limit, unpack_array_over_no_off_no_ref)
{
    std::stringstream ss;
    std::vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    msgpack::pack(ss, v);
    try {
        msgpack::unpacked unp;
        msgpack::unpack(unp, ss.str().c_str(), ss.str().size(), nullptr, nullptr,
                        msgpack::unpack_limit(2, 0, 0, 0, 0));
        EXPECT_TRUE(false);
    }
    catch(msgpack::array_size_overflow const&) {
        EXPECT_TRUE(true);
    }
    catch(...) {
        EXPECT_TRUE(false);
    }
}

TEST(limit, unpack_array_over_no_off_ref)
{
    std::stringstream ss;
    std::vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    msgpack::pack(ss, v);
    try {
        bool ref;
        msgpack::unpacked unp;
        msgpack::unpack(unp, ss.str().c_str(), ss.str().size(), ref, nullptr, nullptr,
                        msgpack::unpack_limit(2, 0, 0, 0, 0));
        EXPECT_TRUE(false);
    }
    catch(msgpack::array_size_overflow const&) {
        EXPECT_TRUE(true);
    }
    catch(...) {
        EXPECT_TRUE(false);
    }
}

TEST(limit, unpack_array_over_off_no_ref)
{
    std::stringstream ss;
    std::vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    msgpack::pack(ss, v);
    try {
        std::size_t off = 0;
        msgpack::unpacked unp;
        msgpack::unpack(unp, ss.str().c_str(), ss.str().size(), off, nullptr, nullptr,
                        msgpack::unpack_limit(2, 0, 0, 0, 0));
        EXPECT_TRUE(false);
    }
    catch(msgpack::array_size_overflow const&) {
        EXPECT_TRUE(true);
    }
    catch(...) {
        EXPECT_TRUE(false);
    }
}

TEST(limit, unpack_array_over_off_ref)
{
    std::stringstream ss;
    std::vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    msgpack::pack(ss, v);
    try {
        bool ref;
        std::size_t off = 0;
        msgpack::unpacked unp;
        msgpack::unpack(unp, ss.str().c_str(), ss.str().size(), off, ref, nullptr, nullptr,
                        msgpack::unpack_limit(2, 0, 0, 0, 0));
        EXPECT_TRUE(false);
    }
    catch(msgpack::array_size_overflow const&) {
        EXPECT_TRUE(true);
    }
    catch(...) {
        EXPECT_TRUE(false);
    }
}

// obsolete
TEST(limit, unpack_array_over_off_ref_pointer)
{
    std::stringstream ss;
    std::vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    msgpack::pack(ss, v);
    try {
        bool ref;
        std::size_t off = 0;
        msgpack::unpacked unp;
        msgpack::unpack(&unp, ss.str().c_str(), ss.str().size(), &off, &ref, nullptr, nullptr,
                        msgpack::unpack_limit(2, 0, 0, 0, 0));
        EXPECT_TRUE(false);
    }
    catch(msgpack::array_size_overflow const&) {
        EXPECT_TRUE(true);
    }
    catch(...) {
        EXPECT_TRUE(false);
    }
}

TEST(limit, unpacker_array_over)
{
    std::stringstream ss;
    std::vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    msgpack::pack(ss, v);
    try {
        msgpack::unpacker u(nullptr, nullptr, MSGPACK_UNPACKER_INIT_BUFFER_SIZE,
                            msgpack::unpack_limit(2, 0, 0, 0, 0));
        u.reserve_buffer(ss.str().size());
        memcpy(u.buffer(), ss.str().c_str(), ss.str().size());
        u.buffer_consumed(ss.str().size());
        msgpack::unpacked unp;
        u.next(unp);
        EXPECT_TRUE(false);
    }
    catch(msgpack::array_size_overflow const&) {
        EXPECT_TRUE(true);
    }
    catch(...) {
        EXPECT_TRUE(false);
    }
}
