#include "../adjacent_element_iterator.hpp"
#include "../adjacent_element_range.hpp"
#include <boost/test/unit_test.hpp>
#include <vector>

namespace {

using Vector = std::vector<int>;

/**
 * @brief Return the demangled type name as string
 */
template <class T> std::string type_name() {
  return boost::core::demangle(typeid(T).name());
}
}

namespace Fixtures {

struct VectorOfSize10 {
  const Vector vectorOfSize10 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
};
}

BOOST_FIXTURE_TEST_CASE(
    Test_make_AdjacentElementIterator_returns_AdjacentElementRandomAccessIterator_for_vector,
    Fixtures::VectorOfSize10) {
  auto adjacentElementIterator =
      make_AdjacentElementIterator<2>(vectorOfSize10.begin());
  BOOST_REQUIRE_EQUAL(
      (type_name<
          AdjacentElementRandomAccessIterator<2, Vector::const_iterator>>()),
      (type_name<typeof(adjacentElementIterator)>()));
}

BOOST_FIXTURE_TEST_CASE(
    Test_make_AdjacentElementEndIterator_returns_AdjacentElementRandomAccessIterator_for_vector,
    Fixtures::VectorOfSize10) {
  auto adjacentElementIterator =
      make_AdjacentElementEndIterator<2>(vectorOfSize10.end());
  BOOST_REQUIRE_EQUAL(
      (type_name<
          AdjacentElementRandomAccessIterator<2, Vector::const_iterator>>()),
      (type_name<typeof(adjacentElementIterator)>()));
}

BOOST_AUTO_TEST_CASE(
    Test_make_AdjacentElementRange_with_too_small_ranges_are_empty) {
  {
    Vector emptyVector;
    BOOST_REQUIRE(make_AdjacentElementRange<2>(emptyVector).empty());
  }
  {
    Vector vectorOfSize1 = {1};
    BOOST_REQUIRE(make_AdjacentElementRange<2>(vectorOfSize1).empty());
  }
}

BOOST_FIXTURE_TEST_CASE(Test_range_length_reduced_by_NumberOfElements_minus_1,
                        Fixtures::VectorOfSize10) {
  {
    BOOST_REQUIRE_EQUAL(
        boost::size(make_AdjacentElementRange<2>(vectorOfSize10)), 9);
  }
  {
    BOOST_REQUIRE_EQUAL(
        boost::size(make_AdjacentElementRange<5>(vectorOfSize10)), 6);
  }
}

BOOST_AUTO_TEST_CASE(Test_AdjacentElementValue_has_correct_type) {
  BOOST_REQUIRE_EQUAL(
      (type_name<AdjacentElementValue<2, Vector::iterator>>()),
      (type_name<std::tuple<Vector::reference, Vector::reference>>()));
}

namespace boost {

namespace {
template <std::size_t...> struct seq {};

template <std::size_t N, std::size_t... Is>
struct gen_seq : gen_seq<N - 1, N - 1, Is...> {};

template <std::size_t... Is> struct gen_seq<0, Is...> : seq<Is...> {};

template <class Tuple, std::size_t... Is>
void print_tuple(wrap_stringstream::wrapped_stream &os, Tuple const &t,
                 seq<Is...>) {
  using swallow = int[];
  (void)swallow{0,
                (void(os << (Is == 0 ? "" : ", ") << std::get<Is>(t)), 0)...};
}
}

template <class... TupleTypes>
inline wrap_stringstream::wrapped_stream &
operator<<(wrap_stringstream::wrapped_stream &wrapped,
           std::tuple<TupleTypes...> const &tuple) {
  wrapped << '<';
  print_tuple(wrapped, tuple, gen_seq<sizeof...(TupleTypes)>());
  wrapped << '>';
  return wrapped;
}
}

BOOST_AUTO_TEST_CASE(Test_make_AdjacentElementRange_returns_correct_elements) {
  Vector vectorOfSize4 = {0, 1, 2, 3};
  using std::make_tuple;
  {
    std::vector<std::tuple<int, int>> expected = {
        make_tuple(0, 1), make_tuple(1, 2), make_tuple(2, 3)};
    auto actual = make_AdjacentElementRange<2>(vectorOfSize4);
    BOOST_REQUIRE_EQUAL_COLLECTIONS(actual.begin(), actual.end(),
                                    expected.begin(), expected.end());
  }
  {
    std::vector<std::tuple<int, int, int>> expected = {make_tuple(0, 1, 2),
                                                       make_tuple(1, 2, 3)};
    auto actual = make_AdjacentElementRange<3>(vectorOfSize4);
    BOOST_REQUIRE_EQUAL_COLLECTIONS(actual.begin(), actual.end(),
                                    expected.begin(), expected.end());
  }
  {
    std::vector<std::tuple<int, int, int, int>> expected = {
        make_tuple(0, 1, 2, 3)};
    auto actual = make_AdjacentElementRange<4>(vectorOfSize4);
    BOOST_REQUIRE_EQUAL_COLLECTIONS(actual.begin(), actual.end(),
                                    expected.begin(), expected.end());
  }
}
