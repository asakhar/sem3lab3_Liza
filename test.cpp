#include "LogicGateOperators.hpp"
#include "catch2/catch.hpp"
#include <sstream>

TEST_CASE("correctly sets & gets values for terminals") {
  Gate a = Gate({{1, 0, 2}, {1, 0, 1}, {0, 0, 0}});
  a(0, 1);
  CHECK(a[0] == 1);
  a(1, 3);
  CHECK(a[1] == 1);
  a(2, 2);
  CHECK(a[2] == 2);
  CHECK_THROWS(a.set(4, 0));
  CHECK_THROWS(a.at(4));
  CHECK_THROWS(a.set(4, 0));
  CHECK_NOTHROW(a(18, 50));
  CHECK_NOTHROW(a[4]);
}

TEST_CASE("connections test") {
  Gate a = Gate({{1, 0, 2}, {1, 0, 1}, {0, 1, 0}});
  a.connect(0);
  CHECK(a.getConnections(0) == 1);
  a.disconnect(2);
  CHECK(a.getConnections(2) == 0);
}

TEST_CASE("adds terminal") {
  Gate a;
  Terminal term{1, 2, 2};
  a += std::move(term);
  REQUIRE(a.size() == 3);
  CHECK(a.getConnections(2) == 2);
  CHECK(a.getTerminalType(2) == true);
  CHECK(a[2] == 2);
}

TEST_CASE("inputs from stream") {
  Gate a{2, 1, 0};
  std::stringstream ss;
  ss << "10X";
  ss.flush();
  ss >> a;
  CHECK(a[0] == 1);
  CHECK(a[1] == 0);
  CHECK(a[2] == 2);
}

TEST_CASE("outputs to stream") {
  Gate a{{{1, 0, 1}, {1, 0, 0}, {1, 0, 2}}};
  std::stringstream ss;
  std::string ch;
  ss << a;
  ss >> ch;
  CHECK(ch == "High");
  ss >> ch;
  CHECK(ch == "Low");
  ss >> ch;
  CHECK(ch == "X");
}