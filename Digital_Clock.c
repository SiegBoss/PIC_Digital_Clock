// Reloj Digital | Digital Clock

// Librerias | Libraries
#include <16F877a.h>
#fuses XT, NOWDT, NOLVP, NOPROTECT
#use delay(clock = 4000000)
#include <lcd.c>
#use fast_io(B)
#use fast_io(D)

// Declaracion de variables | Variables declaration
int y = 0, x = 0, hour = 0, minutes = 0, seconds = 0;

// Funcion de interrupcion Timer 1 | Timer 1 interrupt function
#int_TIMER1
void Reloj(void)
{
    set_timer1(3200);

    x++;

    if (x == 2)
    {
        output_toggle(PIN_B2);
        x = 0;
        seconds++;
    }
}

// Funcion de interrupcion externa | External interrupt function
#int_ext
void Ajustar_Reloj(void)
{
    disable_interrupts(INT_TIMER1);

    y++;

    if (y > 3)
    {
        y = 0;
        enable_interrupts(INT_TIMER1);
    }
}

// Programa principal | Main program
void main()
{
    // Configuracion de los pines de entrada y salida | Input and output pins configuration
    set_tris_B(0b00000011);
    set_tris_D(0b00001000);
    // inicializacion del LCD | LCD initialization
    lcd_init();
    // Configuracion de las interrupciones | Interrupts configuration
    setup_timer_1(T1_INTERNAL | T1_DIV_BY_8);
    set_timer1(3200);
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

        // Minutes | Minutos
        if (minutes > 59)
        {
            hour++;
            minutes = 0;
        }

        // Hour | Hora
        if (hour > 23)
        {
            hour = 0;
        }

        // Ajustar reloj | Adjust clock
        if (y == 1)
        {
            disable_interrupts(INT_TIMER1);
            lcd_gotoxy(1, 1);
            printf(lcd_putc, " .:Ajustar Hor:. ");

            if (input(PIN_B1) == 1)
            {
                delay_ms(250);

                hour++;

                if (hour > 23)
                {
                    hour = 0;
                }
            }
            lcd_gotoxy(1, 2);
            printf(lcd_putc, "    %2d:%2d:%2d", hour, minutes, seconds);
        }

        // Ajustar minutos | Adjust minutes
        else if (y == 2)
        {
            disable_interrupts(INT_TIMER1);
            lcd_gotoxy(1, 1);
            printf(lcd_putc, " .:Ajustar Min:. ");

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
        else if (y == 3)
        {
            disable_interrupts(INT_TIMER1);
            lcd_gotoxy(1, 1);
            printf(lcd_putc, " .:Ajustar Seg:. ");

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

        // Reloj digital | Digital clock
        else
        {
            lcd_gotoxy(1, 1);
            printf(lcd_putc, " :Reloj Digital: ");
            lcd_gotoxy(1, 2);
            printf(lcd_putc, "    %2d:%2d:%2d", hour, minutes, seconds);
        }
    }
}
