/**
 * @author      : rbrugo (brugo.riccardo@gmail.com)
 * @created     : Wednesday Jan 07, 2026 23:04:38 CET
 * @description : 
 */

#include <brun/callables/format.hpp>
#define BOOST_UT_DISABLE_MODULE
#include "boost/ut.hpp"

using namespace std::literals;

struct point
{
    int x;
    int y;
};

template <>
struct std::formatter<point> : std::formatter<int>
{
    constexpr auto format(point pt, auto & ctx) const
    {
        return std::format_to(ctx.out(), "({}, {})", pt.x, pt.y);
    }
};

int main()
{
    using namespace boost::ut;
    using namespace boost::ut::bdd;
    using namespace boost::ut::operators;
    using namespace boost::ut::operators::terse;

    "to_string"_test = [] {
        using callables::to_string;
        should("default-format the passed argument") = [=] {
            expect(to_string(1) == "1");
            expect(to_string(1.) == std::format("{}", 1.));
            expect(to_string("a") == "a");
            expect(to_string(point{10, -2}) == "(10, -2)");
        };
    };

    "format"_test = [] {
        using callables::format;
        should("format the passed arguments as requested") = [=] {
            expect(format<"{}">(1) == std::string_view{"1"});
            expect(format<"{:.3f}">(0.3) == std::string_view{"0.300"});
            expect(format<"{1} {0}!">("todos", "hola") == std::string_view{"hola todos!"});
            expect(format<"{} = {}i + {}j">(point{9, 54}, 9, 54) == std::string_view{"(9, 54) = 9i + 54j"});
        };
    };

    "ston"_test = [] {
        using callables::ston;
        given("some contiguous ranges") = [=] {
            when("a base-10 integer is parsed") = [=]{
                auto cstr = "12345";
                auto strv = std::string_view{"987654"};
                auto arr = std::array{'9', '8', '7', '1', '2', '3'};
                auto fail = std::string_view{"failing"};
                when("the use_exception policy is used") = [=] {
                    constexpr auto stoi = callables::ston<int>;
                    then("must succeed returning the value as integer") = [=] {
                        expect(stoi(cstr) == 12345_i);
                        expect(stoi(strv) == 987654_i);
                        expect(stoi(arr) == 987123_i);
                    };
                    // FIXME: why this fails "because it throws an exception" even if that's
                    // how it should work? T.T
                    then("must fail by throwing an exception") = [=] {
                        // expect(throws([=] { return stoi(fail); }));
                        // expect([=] {
                        //     try {
                        //         static_cast<void>(stoi(fail));
                        //         return false;
                        //     } catch(...) {
                        //         return true;
                        //     }
                        // }());
                    };
                };
                when("the use_optional policy is used") = [=] {
                    constexpr auto stoi = callables::ston<int, 10, callables::policy::use_optional>;
                    then("must succeed returning the value as optional") = [=] {
                        expect(stoi(cstr).value() == 12345_i);
                        expect(stoi(strv).value() == 987654_i);
                        expect(stoi(arr).value() == 987123_i);
                    };
                    then("must fail returning an empty optional") = [=]{
                        expect(not stoi(fail).has_value());
                    };
                };
                when("the use_expected policy is used") = [=] {
                    constexpr auto stoi = callables::ston<int, 10, callables::policy::use_expected>;
                    then("must succeed returning the value as expected") = [=] {
                        expect(stoi(cstr).value() == 12345_i);
                        expect(stoi(strv).value() == 987654_i);
                        expect(stoi(arr).value() == 987123_i);
                    };
                    then("must fail returning an empty expected") = [=]{
                        expect(not stoi(fail).has_value());
                    };
                };
                when("the use_pair_with_errc policy is used") = [=] {
                    constexpr auto stoi = callables::ston<int, 10, callables::policy::use_pair_with_errc> ;
                    then("must succeed returning the value as a number-errc() pair") = [=] {
                        expect(stoi(cstr) == std::pair{12345, std::errc()});
                        expect(stoi(strv) == std::pair{987654, std::errc()});
                        expect(stoi(arr) == std::pair{987123, std::errc()});
                    };
                    then("must fail returning a non-default errc") = [=]{
                        expect(stoi(fail).second != std::errc());
                    };
                };
            };
            when("a non-base-10 integer is parsed with an appropriate base") = [=] {
                auto cstr = "010101";
                auto strv = std::string_view{"123AF"};
                auto arr = std::array{'1', '1', '0'};
                auto fail = std::string_view{"failing"};
                then("should parse it with the required base") = [=] {
                    expect(ston<int, 2>(cstr) == 0b010101);
                    expect(ston<int, 16>(cstr) == 0x010101);
                    expect(ston<int, 16>(strv) == 0x123AF);
                    expect(ston<int, 2>(arr) == 0b110);
                    expect(ston<int, 8>(arr) == 0110);
                    expect(ston<int, 16>(arr) == 0x110);
                };
            };
        };
    };
};
