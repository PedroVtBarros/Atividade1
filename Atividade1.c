#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"

// Definição dos pinos dos LEDs
#define LED_RED_PIN 11
#define LED_YELLOW_PIN 12
#define LED_GREEN_PIN 13

// Variável para controlar o estado do semáforo
typedef enum {
    RED,
    YELLOW,
    GREEN
} TrafficLightState;

TrafficLightState currentState = RED;

// Função de callback para o temporizador periódico
bool repeating_timer_callback(struct repeating_timer *t) {
    // Desliga todos os LEDs antes de mudar o estado
    gpio_put(LED_RED_PIN, 0);
    gpio_put(LED_YELLOW_PIN, 0);
    gpio_put(LED_GREEN_PIN, 0);

    // Muda o estado do semáforo
    switch (currentState) {
        case RED:
            gpio_put(LED_YELLOW_PIN, 1);  // Liga o LED amarelo
            currentState = YELLOW;
            break;
        case YELLOW:
            gpio_put(LED_GREEN_PIN, 1);   // Liga o LED verde
            currentState = GREEN;
            break;
        case GREEN:
            gpio_put(LED_RED_PIN, 1);     // Liga o LED vermelho
            currentState = RED;
            break;
    }

    return true;  // Continua o temporizador
}

int main() {
    // Inicializa a stdio para usar a porta serial
    stdio_init_all();

    // Configura os pinos dos LEDs como saída
    gpio_init(LED_RED_PIN);
    gpio_init(LED_YELLOW_PIN);
    gpio_init(LED_GREEN_PIN);
    gpio_set_dir(LED_RED_PIN, GPIO_OUT);
    gpio_set_dir(LED_YELLOW_PIN, GPIO_OUT);
    gpio_set_dir(LED_GREEN_PIN, GPIO_OUT);

    // Inicializa o semáforo com o LED vermelho ligado
    gpio_put(LED_RED_PIN, 1);

    // Configura o temporizador periódico de 3 segundos (3000 ms)
    struct repeating_timer timer;
    add_repeating_timer_ms(3000, repeating_timer_callback, NULL, &timer);

    // Loop principal
    while (true) {
        // Imprime uma mensagem a cada segundo (1000 ms)
        printf("Semáforo em funcionamento...\n");
        sleep_ms(1000);
    }

    return 0;
}