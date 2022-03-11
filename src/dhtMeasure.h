#ifndef DHT_MEASURE_CLASS
#define DHT_MEASURE_CLASS

#include <DHT.h>
#include <DHT_U.h>

class DhtResult {
    public:
        /**
         * @brief Construct a new Dht Result object
         * 
         * @param t temperature 
         * @param h humidity
         */
        DhtResult();

        void setTemperature(float t );
        void setHumidity(float h);

        /**
         * @brief Get the measured temperature
         * 
         * @return float 
         */
        float getTemperature();

        /**
         * @brief Get the measured humidity
         * 
         * @return float 
         */
        float getHumidity();

    private:
        float temperature;
        float humidity;

};

class DhtMeasure {
    public:
        DhtMeasure(DHT_Unified* sensor);
        void read(DhtResult* result);
        unsigned int setup();

    private:
        DHT_Unified* dhtSensor;
};

#endif