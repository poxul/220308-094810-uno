#ifndef SOIL_MOISTURE_CLASS
#define SOIL_MOISTURE_CLASS

class SoilResult
{
public:
    // color may be red (value 0), yellow (value 1), green (value 20), or blue (value 21)
    enum SoilCondition
    {
        dry,
        humid,
        wet,
        water,
    };

    SoilResult();
    void setValue(int v);
    void setCondition(SoilCondition c);
    void setPercent(float p);

    int getValue();
    SoilCondition getSoilCondition();
    void setValid(bool ok);
    bool isValid();
    float getPercent();

private:
    bool valid;
    int value;
    SoilCondition soil;
    float percent;
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
    float percent;
};

#endif