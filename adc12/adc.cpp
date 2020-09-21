#include <adc.h>
using namespace sensor;

#include <msp430.h>
#include <stdio.h>

AnalogDigitalConverter::AnalogDigitalConverter(int pin_number, float offset, int aduoffset, float scale_k) {

    //Configure PIN multiplexing for input
    if(pin_number < 8) {
        P6SEL |= 0x01 << pin_number;
    } else if (pin_number != 10) {
        P7SEL |= 0x01 << (pin_number - 8);
    }

    REFCTL0 &= ~REFMSTR;    //Internal reference generator is now controller by ADC12_A config bits

    //Power up ADC12, enable internal voltage reference, set sample&hold time
    ADC12CTL0 = ADC12ON | ADC12REFON | ADC12SHT0_8;

    //Disable conversion and configure
    ADC12CTL0 &= ~ADC12ENC;

        // *** Core Configuration ***

        /* Sample and hold */
        ADC12CTL1 |= ADC12SHP;  //Enable ADC12_A internal sampling timer to generate the S&H signal

        // *** Channel configuration ***

        /* Conversion target memory */
        ADC12MCTL0 = ADC12INCH_10 | ADC12SREF_1;  //Vref-=0V Vref+=1.5

        __delay_cycles(100);    //Allow REF to settle

    ADC12CTL0 |= ADC12ENC;

    this->offset = offset;
    this->adu_offset = aduoffset;
    this->scale = scale_k;
}

AnalogDigitalConverter::AnalogDigitalConverter(int pin_number) {
    AnalogDigitalConverter(pin_number, 0, 0, 1.5);
}

AnalogDigitalConverter::AnalogDigitalConverter() {
    AnalogDigitalConverter(INTERNAL_TEMPERATURE_SENSOR_PIN);
}

int AnalogDigitalConverter::perform_measure() {
    ADC12CTL0 |= ADC12SC;

    while(ADC12CTL1 & ADC12BUSY);

    last_measure = ADC12MEM0;
    return last_measure;
}

int AnalogDigitalConverter::get_last_measure() {
    return last_measure;
}

void AnalogDigitalConverter::set_offset(float offset) {
    this->offset = offset;
}

void AnalogDigitalConverter::set_scaling(float m) {
    this->scale = m;
}

float AnalogDigitalConverter::scale_measure() {
    // Linear transfer function expected

    last_scaled = (float)(last_measure + adu_offset) * scale + offset;

}

float AnalogDigitalConverter::perform_scaled_measure() {
    perform_measure();
    last_scaled = scale_measure();
    return last_scaled;
}

float AnalogDigitalConverter::get_last_scaled_measure() {
    return last_scaled;
}

void AnalogDigitalConverter::get_formatted_measure(char *dst_string) {
    scale_measure();

    int whole = (int)last_scaled;
    int decimal = (int)(last_scaled * 10) % 10;

    sprintf(dst_string, "%d.%0.1d", whole, decimal);
}