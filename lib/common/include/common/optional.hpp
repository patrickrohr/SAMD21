/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#pragma once

namespace SAMD
{

template<typename T>
class Optional
{
public:
    Optional() : m_type(), m_bIsInitialized()
    {
    }

    explicit Optional(const T& type) :
        m_type(type),
        m_bIsInitialized(true)
    {}

    Optional& operator=(const T& type)
    {
        m_type = type;
        m_bIsInitialized = true;
    }

    bool Get(T& type)
    {
        if (m_bIsInitialized)
        {
            type = m_type;
        }

        return m_bIsInitialized;
    }

    // CAREFUL: This function does not check if Optional was initialized
    const T& Get() const
    {
        return m_type;
    }

    operator bool() const
    {
        return m_bIsInitialized;
    }

private:
    T m_type;
    bool m_bIsInitialized;
}:

} // namespace SAMD
