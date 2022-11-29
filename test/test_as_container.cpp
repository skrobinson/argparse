#include <argparse/argparse.hpp>
#include <doctest.hpp>

using doctest::test_suite;

TEST_CASE("Get argument with .at()" * test_suite("as_container")) {
  argparse::ArgumentParser PROGRAM("test");
  auto &dir_arg = PROGRAM.add_argument("--dir");
  PROGRAM.at("--dir").default_value(std::string("/home/user"));

  SUBCASE("and default value") {
    PROGRAM.parse_args({"test"});
    REQUIRE(&(PROGRAM.at("--dir")) == &dir_arg);
    REQUIRE(PROGRAM["--dir"] == std::string("/home/user"));
    REQUIRE(PROGRAM.at("--dir") == std::string("/home/user"));
  }

  SUBCASE("and user-supplied value") {
    PROGRAM.parse_args({"test", "--dir", "/usr/local/database"});
    REQUIRE(&(PROGRAM.at("--dir")) == &dir_arg);
    REQUIRE(PROGRAM["--dir"] == std::string("/usr/local/database"));
    REQUIRE(PROGRAM.at("--dir") == std::string("/usr/local/database"));
  }

  SUBCASE("with unknown argument") {
    PROGRAM.parse_args({"test"});
    REQUIRE_THROWS_WITH_AS(PROGRAM.at("--folder"),
                           "No such argument: --folder", std::logic_error);
  }
}

TEST_CASE("Get subparser with .at()" * test_suite("as_container")) {
  argparse::ArgumentParser PROGRAM("test");

  argparse::ArgumentParser walk_cmd("walk");
  auto &speed = walk_cmd.add_argument("speed");

  PROGRAM.add_subparser(walk_cmd);

  SUBCASE("and its argument") {
    PROGRAM.parse_args({"test", "walk", "4km/h"});
    REQUIRE(&(PROGRAM.at<argparse::ArgumentParser>("walk")) == &walk_cmd);
    REQUIRE(&(PROGRAM.at<argparse::ArgumentParser>("walk").at("speed")) == &speed);
    REQUIRE(PROGRAM.at<argparse::ArgumentParser>("walk").is_used("speed"));
  }

  SUBCASE("with unknown command") {
    PROGRAM.parse_args({"test"});
    REQUIRE_THROWS_WITH_AS(PROGRAM.at<argparse::ArgumentParser>("fly"),
                           "No such subparser: fly", std::logic_error);
  }
}
