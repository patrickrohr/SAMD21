// idea is for this to be some sort of platform driver registry that handles
// driver initialization and peripheral initialization

class Platform
{
public:
    void Init();

private:
    Peripheral* m_pPeripherals;
};

class SamdPlatform : public Platform
{
public:
    SamdPlatform() : Platform(&peripherals)
    {
    }
    static Peripheral peripherals[] = { A, B, C };
};
