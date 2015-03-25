#ifndef ADJACENT_ELEMENT_ITERATOR_HPP_
#define ADJACENT_ELEMENT_ITERATOR_HPP_

#include <cstdint>
#include <iterator>
#include <boost/iterator/iterator_adaptor.hpp>
#include <tuple>

template <std::size_t NumberOfElements, class WrappedIterator>
struct AdjacentElementRandomAccessIterator;

/**
 * @brief Helps dereferencing into a tuple of the given size
 */
template <std::size_t NumberOfElements, class WrappedIterator,
          std::size_t CurrentRepeatNumber = 0>
struct DereferenceHelper {
  template <class... Arguments>
  static auto dereference(WrappedIterator const &current,
                          Arguments... arguments)
      -> decltype(DereferenceHelper<NumberOfElements, WrappedIterator,
                                    (sizeof...(Arguments) +
                                     1)>::dereference(current,
                                                      std::forward<Arguments>(
                                                          arguments)...,
                                                      current)) {
    auto next = std::next(current);
    return DereferenceHelper<NumberOfElements, WrappedIterator,
                             (sizeof...(Arguments) +
                              1)>::dereference(next,
                                               std::forward<Arguments>(
                                                   arguments)...,
                                               next);
  }
};

/**
 * @brief Ends the recursion and actually creates the tuple
 */
template <std::size_t NumberOfElements, class WrappedIterator>
struct DereferenceHelper<NumberOfElements, WrappedIterator, NumberOfElements> {
  template <class... Arguments>
  static auto dereference(WrappedIterator const & /*current*/,
                          Arguments... arguments)
      -> decltype(std::tie(*arguments...)) {
    return std::tie(*arguments...);
  }
};

/**
 * @brief Shorthand for a NumberOfElements tuple of dereference WrappedIterators
 */
template <std::size_t NumberOfElements, class WrappedIterator>
using AdjacentElementValue =
    decltype(DereferenceHelper<NumberOfElements, WrappedIterator>::dereference(
        std::declval<WrappedIterator>(), std::declval<WrappedIterator>()));

/**
 * @brief An iterator adaptor that returns NumberOfElements neighboring elements
 * 		  at once
 */
template <std::size_t NumberOfElements, class WrappedIterator>
struct AdjacentElementRandomAccessIterator
    : boost::iterator_adaptor<
          AdjacentElementRandomAccessIterator<NumberOfElements,
                                              WrappedIterator>,
          WrappedIterator,
          AdjacentElementValue<NumberOfElements, WrappedIterator>,
          boost::use_default,
          AdjacentElementValue<NumberOfElements, WrappedIterator>> {
  static_assert(NumberOfElements >= 2, "Need at least two adjacent elements");

  using BaseClass = typename boost::iterator_adaptor<
      AdjacentElementRandomAccessIterator<NumberOfElements, WrappedIterator>,
      WrappedIterator, AdjacentElementValue<NumberOfElements, WrappedIterator>,
      boost::use_default,
      AdjacentElementValue<NumberOfElements, WrappedIterator>>;

  AdjacentElementRandomAccessIterator() = default;

  AdjacentElementRandomAccessIterator(WrappedIterator const &wrappedIterator)
      : BaseClass{wrappedIterator} {}

private:
  friend class boost::iterator_core_access;

  typename BaseClass::reference dereference() const {
    return DereferenceHelper<NumberOfElements, WrappedIterator>::dereference(
        this->base(), this->base());
  }
};

/**
 * @brief Wrap an iterator into an AdjacentElementIterator
 *
 * Use make_AdjacentElementRange instead of this function to generate the
 * iterators of a range.
 *
 * Do not use this on end iterators! Use make_AdjacentElementEndIterator
 * instead!                                                  ~~~
 */
template <std::size_t NumberOfElements, class WrappedIterator>
AdjacentElementRandomAccessIterator<NumberOfElements, WrappedIterator>
make_AdjacentElementIterator(WrappedIterator const &wrappedIterator) {
  return {wrappedIterator};
}

/**
 * @brief Create an end AdjacentElementIterator
 *
 * Use make_AdjacentElementRange instead of this function to generate the
 * iterators of a range. This function will not work properly if the range is
 * smaller than NumberOfElements.
 */
template <std::size_t NumberOfElements, class WrappedIterator>
AdjacentElementRandomAccessIterator<NumberOfElements, WrappedIterator>
make_AdjacentElementEndIterator(WrappedIterator const &wrappedIterator) {
  return {std::prev(wrappedIterator, NumberOfElements - 1)};
}

#endif /* ADJACENT_ELEMENT_ITERATOR_HPP_ */
