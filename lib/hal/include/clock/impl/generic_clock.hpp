/**************************************************************
 *                                                            *
 *               Copyright (c) 2021 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#pragma once

#include "clock/clock_base.hpp"

namespace SAMD
{

class GenericClock : public ClockBase
{
public:
    /**
     * @brief      ClockSource itself is RAII. The Gclk however needs to be
     *             started explicitly to allow for additional runtime
     *             configuration.
     *             TODO: Or does it?
     * @param[in]  id  The gclk identifier
     */
    GenericClock(gclk_id_t id);

    virtual ~GenericClock();

    // GCLK Interface
    error_t Enable(uint32_t uDivisionFactor) override;

    error_t Disable() override;

    /**
     * @brief      Adds a peripheral to the clock output.
     * @param[in]  eOutput  The e output
     * @todo       In the future, all peripherals that need a clock source,
     *             need to inherit from an interface that forces them to take a
     *             clock source in their constructor. The peripheral constructor
     * will then automatically add the output to that clock.
     */
    void AddOutput(ClockOutput eOutput) override;

    /**
     * @brief      Removes an output.
     * @param[in]  eOutput  The e output
     */
    void RemoveOutput(ClockOutput eOutput) override;

    bool IsEnabled() const override;

    // Clock Source
    error_t WaitOnClockIsRunning() const override;

    /**
     * @brief      Gets the divided (actual) output frequency.
     *
     * @return     The output frequency.
     */
    frequency_t GetOutputFrequency() const override;

protected:
    error_t WaitOnClockReady();

    /**
     * @brief      Gets the frequency of the clock source.
     * @details    This does not equal the output frequency as this function
     *             returns the frequency before dividing.
     *
     * @return     The frequency.
     */
    virtual frequency_t GetFrequency() const = 0;

private:
    void SetOutput(ClockOutput eOutput, bool enable);
    unsigned GetDivisionFactor() const;

    // purely virtuals
    virtual bool PollIsRunning() const           = 0;
    virtual ClockType GetClockSourceType() const = 0;

private:
    gclk_id_t m_uGclkId;

public:
    GenericClock(const GenericClock&) = delete;
    GenericClock& operator=(const GenericClock&) = delete;
};

} // namespace SAMD
