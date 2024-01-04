// Reloj Digital | Digital clock

// Librerias | Libraries
#include <16F877a.h>
#fuses XT, NOWDT, NOLVP, NOPROTECT
#use delay(clock = 4000000)
#include <lcd.c>
#use fast_io(B)
#use fast_io(D)

// Declaracion de variables | Variables declaration
int i = 0, i2 = 0, i3 = 0 , setting = 24 ,hour = 1, minutes = 0, seconds = 0;

// Funcion de interrupcion Timer 1 | Timer 1 interrupt function
#int_TIMER1
void clock()
{
    set_timer1(3200);

    i2++;

    if (i2 == 2)
    {
        output_toggle(PIN_B2);
        i2 = 0;
        seconds++;
    }
}

// Funcion de interrupcion externa | External interrupt function
#int_ext
void settingClock()
{
    // Deshabilitar interrupciones del timer 1 | Disable timer 1 interrupts
    disable_interrupts(INT_TIMER1);

    i++;

    if (i > 4)
    {
        i = 0;
        // Habilitar interrupciones del timer 1 | Enable timer 1 interrupts
        enable_interrupts(INT_TIMER1);
    }
}

// Programa principal | Main program
void main()
{
    // Configuracion de los pines de entrada y salida | Input and output pins configuration
    set_tris_B(0b00000011);
    set_tris_D(0b00001000);
    // Inicializacion del LCD | LCD initialization
    lcd_init();
    // Configuration del timer 1 | Timer 1 configuration
    setup_timer_1(T1_INTERNAL | T1_DIV_BY_8);
    set_timer1(3200);
    // Habilitar interrupciones | Enable interrupts
    enable_interrupts(INT_TIMER1);
    enable_interrupts(int_ext);
    ext_int_edge(L_TO_H);
    enable_interrupts(global);

    // Bucle infinito | Infinite loop
    while (true)
    {
        // Segundos | Seconds
        if (seconds > 59)
        {
            minutes++;
            seconds = 0;
        }

        // Minutos | Minutes
        if (minutes > 59)
        {
            hour++;
            minutes = 0;
        }

        // Hora | Hour
        if (hour > setting)
        {
            hour = 1;
        }

        // Ajustar hora | Adjust hour 
        if (i == 1)
        {
            disable_interrupts(INT_TIMER1);
            lcd_gotoxy(1, 1);
            printf(lcd_putc, "Ajustar Hora    ");

            if (input(PIN_B1) == 1)
            {
                delay_ms(250);

                hour++;

                if (hour > setting)
                {
                    hour = 1;
                }
            }
            lcd_gotoxy(1, 2);
            printf(lcd_putc, "    %2d:%2d:%2d", hour, minutes, seconds);
        }

        // Ajustar minutos | Adjust minutes
        else if (i == 2)
        {
            disable_interrupts(INT_TIMER1);
            lcd_gotoxy(1, 1);
            printf(lcd_putc, "Ajustar Minutos ");

            if (input(PIN_B1) == 1)
            {
                delay_ms(250);

                minutes++;

                if (minutes > 59)
                {
                    minutes = 0;
                }
            }
            lcd_gotoxy(1, 2);
            printf(lcd_putc, "    %2d:%2d:%2d", hour, minutes, seconds);
        }

        // Ajustar segundos | Adjust seconds
        else if (i == 3)
        {
            disable_interrupts(INT_TIMER1);
            lcd_gotoxy(1, 1);
            printf(lcd_putc, "Ajustar Segeundos ");

            if (input(PIN_B1) == 1)
            {
                delay_ms(250);

                seconds++;

                if (seconds > 59)
                {
                    seconds = 0;
                }
            }
            lcd_gotoxy(1, 2);
            printf(lcd_putc, "    %2d:%2d:%2d", hour, minutes, seconds);
        }

        // Configuracion del reloj de 12 o 24 horas | 12 or 24 hour clock configuration
        else if (i == 4)
        {
            disable_interrupts(INT_TIMER1);
            lcd_gotoxy(1, 1);
            printf(lcd_putc, "Ajustar Formato  ");

            if (input(PIN_B1) == 1)
            {
                delay_ms(250);
                i3++;

                if (i3 == 1)
                {
                    setting = 24;
                }
                else if (i3 == 2)
                {
                    setting = 12;

                    i3 = 0;
                }
            }

            lcd_gotoxy(1, 2);
            printf(lcd_putc, "Formato:%2d horas", setting);
        }

        // Reloj digital | Digital clock
        else
        {
            lcd_gotoxy(1, 1);
            printf(lcd_putc, "  Reloj Digital  ");
            lcd_gotoxy(1, 2);
            printf(lcd_putc, "    %2d:%2d:%2d    ", hour, minutes, seconds);
        }
    }
}
