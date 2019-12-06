#include <string_view>
#include <variant>

#include <boost/test/unit_test.hpp>

#include "../../include/gwasm/detail/step_args.hpp"

#include "../args.hpp"

BOOST_AUTO_TEST_SUITE(detail_step_args)

BOOST_AUTO_TEST_SUITE(split)

BOOST_AUTO_TEST_CASE(ok)
{
    // given
    auto args = Args{{"exec", "split", "work_dir", "arg1", "arg2"}};

    // when
    const auto variant_result = gwasm::detail::parse_args(args.c(), args.v());

    // then
    const auto& result = std::get<gwasm::detail::SplitStepArgs>(variant_result);
    BOOST_REQUIRE_EQUAL(result.argc, 2);
    BOOST_CHECK_EQUAL(std::string_view{result.argv[0]}, "arg1");
    BOOST_CHECK_EQUAL(std::string_view{result.argv[1]}, "arg2");
    BOOST_CHECK_EQUAL(result.work_dir, "work_dir");
}

BOOST_AUTO_TEST_CASE(no_work_dir)
{
    // given
    auto args = Args{{"exec", "split"}};

    // when
    const auto variant_result = gwasm::detail::parse_args(args.c(), args.v());

    // then
    BOOST_CHECK(std::holds_alternative<gwasm::detail::Error>(variant_result));
}

BOOST_AUTO_TEST_SUITE_END() // split

BOOST_AUTO_TEST_SUITE(exec)

BOOST_AUTO_TEST_CASE(ok)
{
    // given
    auto args = Args{{"exec", "exec", "task_path", "task_out_path"}};

    // when
    const auto variant_result = gwasm::detail::parse_args(args.c(), args.v());

    // then
    const auto& result =
        std::get<gwasm::detail::ExecuteStepArgs>(variant_result);
    BOOST_CHECK_EQUAL(result.task_path, "task_path");
    BOOST_CHECK_EQUAL(result.task_out_path, "task_out_path");
}

BOOST_AUTO_TEST_CASE(not_enough_args)
{
    // given
    auto args = Args{{"exec", "exec", "task_path"}};

    // when
    const auto variant_result = gwasm::detail::parse_args(args.c(), args.v());

    // then
    BOOST_CHECK(std::holds_alternative<gwasm::detail::Error>(variant_result));
}

BOOST_AUTO_TEST_CASE(too_much_args)
{
    // given
    auto args =
        Args{{"exec", "exec", "task_path", "task_out_path", "something"}};

    // when
    const auto variant_result = gwasm::detail::parse_args(args.c(), args.v());

    // then
    BOOST_CHECK(std::holds_alternative<gwasm::detail::Error>(variant_result));
}

BOOST_AUTO_TEST_SUITE_END() // exec

BOOST_AUTO_TEST_SUITE(merge)

BOOST_AUTO_TEST_CASE(ok)
{
    // given
    auto args = Args{{"exec",
                      "merge",
                      "tasks_path",
                      "tasks_out_path",
                      "--",
                      "arg1",
                      "arg2"}};

    // when
    const auto variant_result = gwasm::detail::parse_args(args.c(), args.v());

    // then
    const auto& result = std::get<gwasm::detail::MergeStepArgs>(variant_result);
    BOOST_REQUIRE_EQUAL(result.argc, 2);
    BOOST_CHECK_EQUAL(std::string_view{result.argv[0]}, "arg1");
    BOOST_CHECK_EQUAL(std::string_view{result.argv[1]}, "arg2");
    BOOST_CHECK_EQUAL(result.tasks_path, "tasks_path");
    BOOST_CHECK_EQUAL(result.tasks_out_path, "tasks_out_path");
}

BOOST_AUTO_TEST_CASE(not_enough_args)
{
    // given
    auto args = Args{{"exec", "merge", "tasks_path", "tasks_out_path"}};

    // when
    const auto variant_result = gwasm::detail::parse_args(args.c(), args.v());

    // then
    BOOST_CHECK(std::holds_alternative<gwasm::detail::Error>(variant_result));
}

BOOST_AUTO_TEST_SUITE_END() // merge

BOOST_AUTO_TEST_SUITE_END() // detail_step_args
