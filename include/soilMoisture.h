#ifndef SOIL_MOISTURE_CLASS
#define SOIL_MOISTURE_CLASS

class SoilResult
{
public:
    // color may be red (value 0), yellow (value 1), green (value 20), or blue (value 21)
    enum SoilCondition
    {
        water,
        wet,
        humid,
        dry
    };

    SoilResult();
    void setValue(int v);
    void setCondition(SoilCondition c);

    int getValue();
    SoilCondition getSoilCondition();
    bool setValid(bool ok);
    bool isValid();

private:
    bool valid;
    int value;
    SoilCondition soil;
};

class SoilMoisture
{
public:
    SoilMoisture(int ax);
    void setup();
    void read(SoilResult *result);
    void setEnabled(bool on);
    bool isEnabled();

private:
    bool enabled;
    int soilMoistureValue = 0; // set the initial value of variable soilMoistureValue to 0
    int analogInput;
    int step;
};

#endif