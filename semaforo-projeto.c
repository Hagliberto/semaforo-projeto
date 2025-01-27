/*
Autor: Hagliberto Alves de Oliveira - 202421511720221
Descrição: Projeto de um semáforo utilizando o Raspberry Pi Pico, LEDs e um display OLED.
*/

#include <stdio.h>  // Biblioteca padrão para entrada e saída (ex: printf).
#include <string.h> // Biblioteca para manipulação de strings (ex: memset).

#include "hardware/i2c.h"  // Biblioteca específica do Raspberry Pi Pico para controle de I2C.
#include "inc/ssd1306.h"   // Biblioteca para controle do display OLED SSD1306.
#include "pico/stdlib.h"   // Biblioteca padrão do Raspberry Pi Pico para funcionalidades gerais.

#define I2C_PORT i2c1       // Define a porta I2C a ser utilizada.
#define I2C_SDA 14          // Define o pino para SDA (dados do I2C).
#define I2C_SCL 15          // Define o pino para SCL (relógio do I2C).

#define BUTTON_A_PIN 5      // Define o pino onde o botão "A" está conectado.

#define RED_LED_PIN 13      // Define o pino para o LED vermelho.
#define GREEN_LED_PIN 11    // Define o pino para o LED verde.
#define BLUE_LED_PIN 12     // Define o pino para o LED azul.

bool button_A_is_pressed = false; // Variável booleana para monitorar se o botão "A" foi pressionado.

enum Color {  // Enumeração para representar as cores do semáforo.
    RED,      // Vermelho.
    GREEN,    // Verde.
    YELLOW    // Amarelo.
};

// Textos exibidos no display para o sinal verde.
char *GREEN_TRAFFIC_LIGHT_TEXT[] = {
    "               ",
    "               ",
    " SINAL ABERTO  ",
    "               ",
    " ATRAVESSE COM ",
    "    CUIDADO    ",
    "               ",
    "               "};

// Textos exibidos no display para o sinal amarelo.
char *YELLOW_TRAFFIC_LIGHT_TEXT[] = {
    "               ",
    "               ",
    "   SINAL DE    ",
    "    ATENCAO    ",
    "               ",
    "  PREPARE SE   ",
    "               ",
    "               "};

// Textos exibidos no display para o sinal vermelho.
char *RED_TRAFFIC_LIGHT_TEXT[] = {
    "               ",
    "               ",
    " SINAL FECHADO ",
    "               ",
    "    AGUARDE    ",
    "               ",
    "               ",
    "               "};

// Função para exibir texto no display.
void display_text(uint8_t *ssd, struct render_area *frame_area, char *text[], size_t line_count) {
    memset(ssd, 0, ssd1306_buffer_length);  // Limpa o buffer do display.
    int y = 0;  // Coordenada inicial para desenhar o texto.
    for (size_t i = 0; i < line_count; i++) {  // Itera pelas linhas de texto.
        ssd1306_draw_string(ssd, 5, y, text[i]);  // Desenha cada linha no display.
        y += 8;  // Incrementa a posição vertical para a próxima linha.
    }
    render_on_display(ssd, frame_area);  // Renderiza o texto no display.
}

// Função para exibir a introdução na matriz de LED.
void display_introduction(uint8_t *ssd, struct render_area *frame_area) {
    char *INTRO_TEXT[] = {
        "               ",
        "   Me chamo    ",
        "  Hagliberto,  ",
        "   aluno do    ",
        "  Embarcatech  ",
        "               ",
        " Irei mostrar  ",
        "   o projeto   ",
    };

    memset(ssd, 0, ssd1306_buffer_length);  // Limpa o buffer do display.
    int y = 0;  // Coordenada inicial para desenhar o texto.
    for (size_t i = 0; i < 8; i++) {  // Itera pelas linhas de texto.
        ssd1306_draw_string(ssd, 5, y, INTRO_TEXT[i]);  // Exibe cada linha.
        y += 8;  // Incrementa a posição vertical.
    }
    render_on_display(ssd, frame_area);  // Atualiza o display.
    sleep_ms(5000);  // Exibe a introdução por 5 segundos.
}

// Aguarda um tempo ou até que o botão "A" seja pressionado.
bool wait_for_timeout_or_button_press(int time_ms) {
    for (int i = 0; i < time_ms; i++) {
        button_A_is_pressed = !gpio_get(BUTTON_A_PIN);  // Verifica se o botão foi pressionado.
        if (button_A_is_pressed) {
            return true;  // Retorna true se o botão for pressionado.
        }
        sleep_ms(1);  // Aguarda 1 ms.
    }
    return false;  // Retorna false se o tempo terminar sem pressionamento.
}

// Configura os LEDs de acordo com os valores booleanos fornecidos.
void set_leds(bool red, bool green, bool blue) {
    gpio_put(RED_LED_PIN, red);
    gpio_put(GREEN_LED_PIN, green);
    gpio_put(BLUE_LED_PIN, blue);
}

// Ativa apenas o LED vermelho.
void turn_on_red_traffic_light() {
    set_leds(true, false, false);
}

// Ativa apenas o LED verde.
void turn_on_green_traffic_light() {
    set_leds(false, true, false);
}

// Ativa o LED vermelho e o LED verde (cor amarela).
void turn_on_yellow_traffic_light() {
    set_leds(true, true, false);
}

// Define a cor do semáforo e exibe o texto correspondente no display.
void set_traffic_light(enum Color color, uint8_t *ssd, struct render_area *frame_area) {
    if (color == RED) {  // Se a cor for vermelha.
        turn_on_red_traffic_light();
        display_text(ssd, frame_area, RED_TRAFFIC_LIGHT_TEXT, sizeof(RED_TRAFFIC_LIGHT_TEXT) / sizeof(RED_TRAFFIC_LIGHT_TEXT[0]));
    } else if (color == GREEN) {  // Se a cor for verde.
        turn_on_green_traffic_light();
        display_text(ssd, frame_area, GREEN_TRAFFIC_LIGHT_TEXT, sizeof(GREEN_TRAFFIC_LIGHT_TEXT) / sizeof(GREEN_TRAFFIC_LIGHT_TEXT[0]));
    } else {  // Se a cor for amarela.
        turn_on_yellow_traffic_light();
        display_text(ssd, frame_area, YELLOW_TRAFFIC_LIGHT_TEXT, sizeof(YELLOW_TRAFFIC_LIGHT_TEXT) / sizeof(YELLOW_TRAFFIC_LIGHT_TEXT[0]));
    }
}

// Configuração inicial dos periféricos.
void setup() {
    stdio_init_all();  // Inicializa as funções de entrada/saída.

    gpio_init(BUTTON_A_PIN);  // Configura o pino do botão "A".
    gpio_set_dir(BUTTON_A_PIN, GPIO_IN);  // Define como entrada.
    gpio_pull_up(BUTTON_A_PIN);  // Ativa o resistor pull-up.

    gpio_init(RED_LED_PIN);  // Configura o pino do LED vermelho.
    gpio_init(GREEN_LED_PIN);  // Configura o pino do LED verde.
    gpio_init(BLUE_LED_PIN);  // Configura o pino do LED azul.
    gpio_set_dir(RED_LED_PIN, GPIO_OUT);  // Define como saída.
    gpio_set_dir(GREEN_LED_PIN, GPIO_OUT);  // Define como saída.
    gpio_set_dir(BLUE_LED_PIN, GPIO_OUT);  // Define como saída.

    i2c_init(I2C_PORT, ssd1306_i2c_clock * 400);  // Inicializa o barramento I2C.

    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);  // Configura o pino SDA para função I2C.
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);  // Configura o pino SCL para função I2C.
    gpio_pull_up(I2C_SDA);  // Ativa o resistor pull-up para SDA.
    gpio_pull_up(I2C_SCL);  // Ativa o resistor pull-up para SCL.

    ssd1306_init();  // Inicializa o display OLED SSD1306.
}

// Função principal do programa.
int main() {
    setup();  // Chama a configuração inicial.

    set_leds(false, false, false);  // Desliga todos os LEDs inicialmente.

    struct render_area frame_area = {  // Define a área de renderização do display.
        start_column : 0,
        end_column : ssd1306_width - 1,
        start_page : 0,
        end_page : ssd1306_n_pages - 1
    };

    calculate_render_area_buffer_length(&frame_area);  // Calcula o tamanho do buffer necessário.

    uint8_t ssd[ssd1306_buffer_length];  // Buffer para o display.

    // Exibe a introdução.
    display_introduction(ssd, &frame_area);

    enum Color R = RED, G = GREEN, Y = YELLOW;  // Atalhos para as cores do semáforo.

    int yellow_traffic_light_time, green_traffic_light_time;  // Tempos de duração para as luzes.

    while (true) {  // Loop infinito.
        yellow_traffic_light_time = 2 * 1000;  // Define o tempo inicial do sinal amarelo.
        green_traffic_light_time = 8 * 1000;  // Define o tempo inicial do sinal verde.

        set_traffic_light(R, ssd, &frame_area);  // Liga o semáforo vermelho.
        button_A_is_pressed = wait_for_timeout_or_button_press(8 * 1000);  // Aguarda 8 segundos ou o botão.

        if (button_A_is_pressed) {  // Se o botão foi pressionado.
            yellow_traffic_light_time = 5 * 1000;  // Altera o tempo do amarelo.
            green_traffic_light_time = 10 * 1000;  // Altera o tempo do verde.
        }

        set_traffic_light(Y, ssd, &frame_area);  // Liga o semáforo amarelo.
        sleep_ms(yellow_traffic_light_time);  // Aguarda o tempo do amarelo.
        set_traffic_light(G, ssd, &frame_area);  // Liga o semáforo verde.
        sleep_ms(green_traffic_light_time);  // Aguarda o tempo do verde.
    }

    return 0;  // Finaliza o programa (nunca será alcançado neste caso).
}
