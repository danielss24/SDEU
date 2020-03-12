#include "mbed.h"
#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "motordriver.h"

DigitalOut myled1(LED1);
DigitalOut myled2(LED2);
DigitalOut myled3(LED3);
DigitalOut myled4(LED4);


struct Component {
    Motor *motor;
    DigitalOut *dira;
    DigitalOut *dirh;
    AnalogIn *a;
    float initial;
    float current;
    float last;
};

void secuencia_1_leds();

void mover_horario(Component *component);
void mover_anti_horario(Component *component);
void parar_motor(Component *component);
void modificar_velocidad_a(Component *component, float speed);
void modificar_velocidad_h(Component *component, float speed);
void mantener_distancia(int sensor, Component *holobot);
void rotar_horario(Component *holobot);
void rotar_anti_horario(Component *holobot);
void parar_motores (Component *holobot);
void ajustar_rotacion(Component *holobot);
void update_measures(Component *holobot);
int equal(Component *holobot);
int jail_rotating(Component *holobot);
int get_danger_component(Component *holobot);
int calibrator(Component *holobot);
int in(float last, float current);

float speedTest = 0.3f;
float AJUSTE = 0.03f;
float AJUSTE_GIRO = 0.08f;
float MOVEMENT_TIME = 0.005f;

int main() {


    Serial pc(USBTX, USBRX);


    /*
        Control de sensores de distancia
    */

    AnalogIn a1(p17);
    AnalogIn a2(p19);
    AnalogIn a3(p20);

    /*
        Control de motores
    */


    DigitalOut motorEN(p24);

    DigitalOut dir1a(p30);
    DigitalOut dir2a(p29);
    DigitalOut dir3a(p26);

    DigitalOut dir1h(p14);
    DigitalOut dir2h(p15);
    DigitalOut dir3h(p16);

    Motor m1(p23, p30, p14, 0);
    Motor m2(p22, p29, p15, 0);
    Motor m3(p21, p26, p16, 0);

    m1.speed(0.0f);
    m2.speed(0.0f);
    m3.speed(0.0f);

    Component holobot[3];

    secuencia_1_leds();

    float actualA1i = 0.0f;
    float actualA2i = 0.0f;
    float actualA3i = 0.0f;

    holobot[0].motor = &m1;
    holobot[0].dira = &dir1a;
    holobot[0].dirh = &dir1h;
    holobot[0].a = &a1;
    holobot[0].initial = a1.read();
    holobot[0].current = holobot[0].initial;

    holobot[1].motor = &m2;
    holobot[1].dira = &dir2a;
    holobot[1].dirh = &dir2h;
    holobot[1].a = &a2;
    holobot[1].initial = a2.read();
    holobot[1].current = holobot[1].initial;

    holobot[2].motor = &m3;
    holobot[2].dira = &dir3a;
    holobot[2].dirh = &dir3h;
    holobot[2].a = &a3;
    holobot[2].initial = a3.read();
    holobot[2].current = holobot[2].initial;

    if ((in(holobot[0].current, holobot[1].current)) && (in(holobot[0].current, holobot[2].current))){
        myled4 = 1;
    }

    while(1) {

        update_measures(holobot);

        if (equal(holobot)) continue;

        myled1 = 1;

        if (jail_rotating(holobot)){
            //logica rotar
            AJUSTE_GIRO = 0.02f;
            float current, attempt;
            float initial = holobot[0].initial + holobot[1].initial + holobot[2].initial;
            int mode = 1;
            myled2 = 1;

            while (jail_rotating(holobot)) {
                current = holobot[0].current + holobot[1].current + holobot[2].current;

                if (mode) rotar_horario(holobot);
                else rotar_anti_horario(holobot);
                wait(MOVEMENT_TIME);

                update_measures(holobot);
                attempt = holobot[0].current + holobot[1].current + holobot[2].current;

                if (attempt < current) {
                    mode = !mode;
                    parar_motores(holobot);
                    myled3 = 1;
                }
                update_measures(holobot);
            }
            parar_motores(holobot);
            AJUSTE_GIRO = 0.08f;
        }

        int danger_component = get_danger_component(holobot);
        if (danger_component) {
            mantener_distancia(danger_component, holobot);
            wait(MOVEMENT_TIME);
            myled4 = 1;
        }
        parar_motores(holobot);
    }
}

void secuencia_1_leds(){

    myled1 = 1;
    wait(0.2f);
    myled2 = 1;
    wait(0.2f);
    myled3 = 1;
    wait(0.2f);
    myled4 = 1;
    wait(0.2f);

    myled4 = 0;
    wait(0.2f);
    myled3 = 0;
    wait(0.2f);
    myled2 = 0;
    wait(0.2f);
    myled1 = 0;
    wait(0.2f);

}

int equal(Component *holobot){

    float margenSup = holobot[0].initial + holobot[0].initial * AJUSTE;
    float margenInf = holobot[0].initial - holobot[0].initial * AJUSTE;

    if ((holobot[0].current < margenInf) || (margenSup < holobot[0].current)) return 0;

    margenSup = holobot[1].initial + holobot[1].initial * AJUSTE;
    margenInf = holobot[1].initial - holobot[1].initial * AJUSTE;

    if ((holobot[1].current < margenInf) || (margenSup < holobot[1].current)) return 0;

    margenSup = holobot[2].initial + holobot[2].initial * AJUSTE;
    margenInf = holobot[2].initial - holobot[2].initial * AJUSTE;

    if ((holobot[2].current < margenInf) || (margenSup < holobot[2].current)) return 0;

    return 1;

}

int get_danger_component(Component *holobot){

    float margen0 = holobot[0].initial + holobot[0].initial * AJUSTE;
    float diff0 = margen0 - holobot[0].current;

    float margen1 = holobot[1].initial + holobot[1].initial * AJUSTE;
    float diff1 = margen1 - holobot[1].current;

    float margen2 = holobot[2].initial + holobot[2].initial * AJUSTE;
    float diff2 = margen2 - holobot[2].current;

    if ((diff0 < diff1) && (diff0 < diff2) && (diff0 < 0)) return 1;
    if ((diff1 < diff0) && (diff1 < diff2) && (diff1 < 0)) return 2;
    if ((diff2 < diff0) && (diff2 < diff1) && (diff2 < 0)) return 3;
    return 0;

}

int in(float last, float current){

    float margenSup = last + last * AJUSTE;
    float margenInf = last - last * AJUSTE;

    if ((current < margenInf) || (margenSup < current)) return 0;

    return 1;
}

int jail_rotating(Component *holobot){

    float margen = holobot[0].initial - holobot[0].initial * AJUSTE_GIRO;

    if (holobot[0].current > margen) return 0;

    margen = holobot[1].initial - holobot[1].initial * AJUSTE_GIRO;

    if (holobot[1].current > margen) return 0;

    margen = holobot[2].initial - holobot[2].initial * AJUSTE_GIRO;

    if (holobot[2].current > margen) return 0;

    return 1;

}

void update_measures(Component *holobot){

    holobot[0].last = holobot[0].current;
    holobot[1].last = holobot[1].current;
    holobot[2].last = holobot[2].current;

    holobot[0].current = holobot[0].a->read();
    holobot[1].current = holobot[1].a->read();
    holobot[2].current = holobot[2].a->read();

}

void mover_horario(Component *component) {
    component->motor->speed(speedTest);
    DigitalOut *da = component->dira;
    *da = 0;
    DigitalOut *dh = component->dirh;
    *dh = 1;
}

void mover_anti_horario(Component *component){
    component->motor->speed(2.5f * speedTest);
    DigitalOut *da = component->dira;
    *da = 1;
    DigitalOut *dh = component->dirh;
    *dh = 0;
}

void parar_motor(Component *component){
    component->motor->speed(0.0f);
    DigitalOut *da = component->dira;
    *da = 0;
    DigitalOut *dh = component->dirh;
    *dh = 0;
}

void modificar_velocidad_a(Component *component, float speed){
    component->motor->speed(speed);
    DigitalOut *da = component->dira;
    *da = 1;
    DigitalOut *dh = component->dirh;
    *dh = 0;
}

void modificar_velocidad_h(Component *component, float speed){
    component->motor->speed(speed);
    DigitalOut *da = component->dira;
    *da = 0;
    DigitalOut *dh = component->dirh;
    *dh = 1;
}

void mantener_distancia(int sensor, Component *holobot){

    if (sensor == 1) {

        mover_horario(&holobot[1]);
        mover_anti_horario(&holobot[2]);

    } else if (sensor == 2) {

        mover_horario(&holobot[2]);
        mover_anti_horario(&holobot[0]);

    } else if (sensor == 3) {

        mover_horario(&holobot[0]);
        mover_anti_horario(&holobot[1]);

    }
}

void rotar_horario(Component *holobot){

    mover_anti_horario(&holobot[0]);
    mover_anti_horario(&holobot[1]);
    mover_anti_horario(&holobot[2]);
}

void rotar_anti_horario(Component *holobot){

    mover_horario(&holobot[0]);
    mover_horario(&holobot[1]);
    mover_horario(&holobot[2]);
}

void parar_motores (Component *holobot){

    parar_motor(&holobot[0]);
    parar_motor(&holobot[1]);
    parar_motor(&holobot[2]);
}

void ajustar_rotacion(Component *holobot){

}

