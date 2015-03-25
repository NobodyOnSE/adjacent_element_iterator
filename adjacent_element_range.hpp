#ifndef ADJACENT_ELEMENT_RANGE_HPP_
#define ADJACENT_ELEMENT_RANGE_HPP_

#include "adjacent_element_iterator.hpp"
#include <boost/range/iterator_range.hpp>

/**
 * @brief Create a range of AdjacentElementiterators from begin/end iterators
 * @tparam NumberOfElements that should be returned by the AdjacentElementIteror
 */
template <std::size_t NumberOfElements, class WrappedIterator>
boost::iterator_range<
    AdjacentElementRandomAccessIterator<NumberOfElements, WrappedIterator>>
make_AdjacentElementRange(WrappedIterator const &begin,
                          WrappedIterator const &end) {
  if (std::distance(begin, end) >= NumberOfElements) {
    return {make_AdjacentElementIterator<NumberOfElements>(begin),
            make_AdjacentElementEndIterator<NumberOfElements>(end)};
  } else {
    /* The range is shorter than the resulting tuple size.
     * the EndIterator function would move the iterator in front of the begin
     * iterator. Logically, this is an empty range so we create one.
     */
    return boost::make_iterator_range_n(
        make_AdjacentElementIterator<NumberOfElements>(begin), 0);
  }
}

/**
 * @brief Create a range of AdjacentElementIterators from the given range
 * @tparam NumberOfElements that should be returned by the AdjacentElementIteror
 */
template <std::size_t NumberOfElements, class Range>
auto make_AdjacentElementRange(Range &range)
    -> boost::iterator_range<AdjacentElementRandomAccessIterator<
        NumberOfElements, decltype(std::begin(range))>> {
  return make_AdjacentElementRange<NumberOfElements>(std::begin(range),
                                                     std::end(range));
}

#endif /* ADJACENT_ELEMENT_RANGE_HPP_ */
