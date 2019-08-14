/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#pragma once

namespace SAMD
{

template<typename CLASS, typename TYPE = unsigned>
class id_traits
{
public:
    using type = TYPE;

public:
    constexpr id_traits() : m_id()
    {
    }
    constexpr explicit id_traits(type id) : m_id(id)
    {
    }

    constexpr bool operator==(id_traits rhs) const
    {
        return m_id == rhs.m_id;
    }

    constexpr bool operator!=(id_traits rhs) const
    {
        return !operator==(rhs);
    }

    constexpr bool operator<(id_traits rhs) const
    {
        return m_id < rhs.m_id;
    }

    constexpr bool operator>(id_traits rhs) const
    {
        return m_id > rhs.m_id;
    }

    constexpr type Get() const
    {
        return m_id;
    }

    constexpr explicit operator type() const
    {
        return m_id;
    }

private:
    type m_id;
};

} // namespace SAMD
