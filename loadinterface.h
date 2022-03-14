#pragma once

class LoadInterface
{
public:
    virtual void enable() = 0;
    virtual void disable() = 0;
    virtual double getPowerThreshold() const = 0;
};

class DummyLoad : public LoadInterface
{

    void enable() {};

    void disable() { };

    double getPowerThreshold() const {
        return 0.0;
    };

};
