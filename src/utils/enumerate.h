/**
 * @file enumerate.h
 * @author daaguirre
 * @brief implementation of python's enumerate
 * implementation details: 
 * https://stackoverflow.com/questions/11328264/python-like-loop-enumeration-in-c
 * @date 2022-08-20
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef __UTILS_ENUMERATE_H_
#define __UTILS_ENUMERATE_H_

template <typename Iterable>
class EnumerateObject
{
private:
    Iterable m_iterable;
    size_t m_index;
    decltype(std::begin(m_iterable)) m_begin;
    const decltype(std::end(m_iterable)) m_end;

public:
    EnumerateObject(Iterable iter)
        : m_iterable(iter), m_index(0), m_begin(std::begin(iter)), m_end(std::end(iter))
    {
    }

    const EnumerateObject& begin() const { return *this; }
    const EnumerateObject& end() const { return *this; }

    bool operator!=(const EnumerateObject&) const { return m_begin != m_end; }

    void operator++()
    {
        ++m_begin;
        ++m_index;
    }

    auto operator*() const -> std::pair<std::size_t, decltype(*m_begin)>
    {
        return {m_index, *m_begin};
    }
};

template <typename Iterable>
auto enumerate(Iterable&& iter) -> EnumerateObject<Iterable>
{
    return {std::forward<Iterable>(iter)};
}

#endif  // __UTILS_ENUMERATE_H_
