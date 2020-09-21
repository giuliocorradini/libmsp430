namespace sensor {

    const int INTERNAL_TEMPERATURE_SENSOR_PIN = 10;

    class AnalogDigitalConverter {
        private:
            int last_measure;
            float last_scaled;
            float offset;
            int adu_offset;
            float scale;

        public:
            AnalogDigitalConverter();
            AnalogDigitalConverter(int pin_number);
            AnalogDigitalConverter(int pin_number, float offset, int adu_offset, float scale_k);

            int perform_measure();
            int get_last_measure();

            void set_offset(float offset);
            void set_scaling(float m);

            float perform_scaled_measure();
            float get_last_scaled_measure();

            float scale_measure();

            void get_formatted_measure(char *dst_string);
    };

}