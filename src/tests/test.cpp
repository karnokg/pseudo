#include <iostream>

#include <vector>
#include <array>
#include <memory>

#include "utest.h"

std::vector<std::string> split_string(const std::string& str,
                                      const std::string& delimiter = "\n")
{
    std::vector<std::string> strings;

    std::string::size_type pos = 0;
    std::string::size_type prev = 0;
    while ((pos = str.find(delimiter, prev)) != std::string::npos)
    {
        strings.push_back(str.substr(prev, pos - prev));
        prev = pos + delimiter.size();
    }

    // To get the last substring (or only, if delimiter is not found)
    strings.push_back(str.substr(prev));

    return strings;
}

std::vector<bool> get_exp_got_pair_result(std::string& output)
{
    std::vector<bool> result;
    auto split = split_string(output);

    for (auto it = split.begin(); it < split.end(); it = it + 2) 
    {
        auto exp = *it;
        auto got = *std::next(it);

        if (exp.empty())
        {
            continue;
        }

        exp = exp.substr(4, exp.size()); // remove "EXP " from str
        got = got.substr(4, got.size()); // remove "GOT " from str

        result.push_back(exp == got);
    }

    return result;
}

std::vector<bool> get_ok_result(std::string& output)
{
    std::vector<bool> result;
    auto split = split_string(output);

    for (auto i = 0; i < split.size(); ++i)
    {
        auto curr = split[i];

        if (curr != "OK")
        {
            std::cout << "GOT " << curr << " INSTEAD OF OK AT " << i << std::endl; 
        }

        result.push_back(curr == "OK");
    }
    for (auto it = split.begin(); it < split.end(); ++it) 
    {
    }

    return result;
}


std::string exec(const char* cmd)
{
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe)
    {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
    {
        result += buffer.data();
    }
    return result;
}

#define ASSERT_BOOL_VEC(vec) \
    for (auto i : vec) \
        ASSERT_TRUE(i);

UTEST(Assign, integer)
{
    std::string output = exec("./pseudo ../../testcodes/assign_integer.pseudo -r -s");
    auto res = get_exp_got_pair_result(output);
    ASSERT_BOOL_VEC(res);
}

UTEST(Assign, rational)
{
    std::string output = exec("./pseudo ../../testcodes/assign_rational.pseudo -r -s");
    auto res = get_exp_got_pair_result(output);
    ASSERT_BOOL_VEC(res);
}

UTEST(Vector, tests)
{
    std::string output = exec("./pseudo ../../testcodes/vector.pseudo -r -s");
    auto res = get_exp_got_pair_result(output);
    ASSERT_BOOL_VEC(res);
}

UTEST(Assign, boolean)
{
    std::string output = exec("./pseudo ../../testcodes/assign_boolean.pseudo -r -s");
    auto res = get_exp_got_pair_result(output);
    ASSERT_BOOL_VEC(res);
}

UTEST(Assign, string)
{
    std::string output = exec("./pseudo ../../testcodes/assign_string.pseudo -r -s");
    auto res = get_exp_got_pair_result(output);
    ASSERT_BOOL_VEC(res);
}

UTEST(Conditionals, compareoperator)
{
    std::string output = exec("./pseudo ../../testcodes/conditionals_compareoperator.pseudo -r -s");
    auto res = get_ok_result(output);
    ASSERT_BOOL_VEC(res);
}

UTEST(BinaryOperators, integer)
{
    std::string output = exec("./pseudo ../../testcodes/binaryoperators_integer.pseudo -r -s");
    auto res = get_exp_got_pair_result(output);
    ASSERT_BOOL_VEC(res);
}

UTEST(BinaryOperators, rational)
{
    std::string output = exec("./pseudo ../../testcodes/binaryoperators_rational.pseudo -r -s");
    auto res = get_exp_got_pair_result(output);
    ASSERT_BOOL_VEC(res);
}

UTEST(BinaryOperators, boolean)
{
    std::string output = exec("./pseudo ../../testcodes/binaryoperators_boolean.pseudo -r -s");
    auto res = get_exp_got_pair_result(output);
    ASSERT_BOOL_VEC(res);
}

UTEST(Loops, forloop)
{
    std::string output = exec("./pseudo ../../testcodes/loops_forloop.pseudo -r -s");
    auto res = get_exp_got_pair_result(output);
    ASSERT_BOOL_VEC(res);
}

UTEST(Loops, whileloop)
{
    std::string output = exec("./pseudo ../../testcodes/loops_while.pseudo -r -s");
    auto res = get_exp_got_pair_result(output);
    ASSERT_BOOL_VEC(res);
}

UTEST(Loops, dowhileloop)
{
    std::string output = exec("./pseudo ../../testcodes/loops_dowhile.pseudo -r -s");
    ASSERT_STREQ("OK", output.c_str());
}

UTEST(Functioncall, tests)
{
    std::string output = exec("./pseudo ../../testcodes/functioncall.pseudo -r -s");
    auto res = get_exp_got_pair_result(output);
    ASSERT_BOOL_VEC(res);
}

UTEST(foo, bar) {
  ASSERT_TRUE(1);
}

UTEST_MAIN();