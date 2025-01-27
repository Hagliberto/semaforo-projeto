
# 🚦 Projeto Semáforo com Display OLED SSD1306

## 👨‍💻 Autor: Hagliberto Alves de Oliveira - 202421511720221

---

## 📝 Visão Geral

Este projeto é uma implementação de um semáforo utilizando o microcontrolador Raspberry Pi Pico, um display OLED SSD1306 e LEDs. O código demonstra como controlar um display OLED para exibir mensagens e ao mesmo tempo operar LEDs para representar as cores de um semáforo (vermelho, amarelo e verde). Além disso, o projeto permite a interação por meio de um botão que ajusta o tempo das cores do semáforo.

---

## 📋 Requisitos

### 🖥️ Hardware

1. **Raspberry Pi Pico**

   - Um microcontrolador baseado no chip RP2040.

2. **Display OLED SSD1306**

   - Resoluções suportadas: 128x32 ou 128x64 pixels.
   - Interface: I2C.

3. **LEDs**

   - LED Vermelho, Verde e Azul.
   - Resistores de 220 Ω para proteger os LEDs.

4. **Botão Push**

   - Um botão momentâneo conectado ao pino GPIO.

5. **Jumpers e Protoboard**

   - Para conexões elétricas.

6. **Fonte de Alimentação USB**

   - Cabo USB para conectar o Raspberry Pi Pico ao computador.

### 💻 Software

1. **Toolchain para RP2040**

   - GCC para ARM Cortex-M (`arm-none-eabi-gcc`).

2. **CMake**

   - Versão mínima: 3.13.

3. **SDK do Raspberry Pi Pico**

   - Disponível no repositório oficial [Pico SDK](https://github.com/raspberrypi/pico-sdk).

4. **Editor de Texto/IDE**

   - Recomendação: VS Code com a extensão Pico.

5. **Bibliotecas Externas**

   - `ssd1306_i2c.h` e `ssd1306_font.h` (disponíveis no repositório do projeto).

---

## ⚙️ Instalação e Configuração

### 1. 🔄 Clonar o Repositório

```bash
# Clone o repositório do projeto
$ git clone https://github.com/seu-usuario/semaforo-projeto.git
$ cd semaforo-projeto
```

### 2. 🛠️ Configurar o Ambiente do Pico SDK

- Certifique-se de que o Pico SDK está configurado corretamente no sistema:

```bash
export PICO_SDK_PATH=/caminho/para/pico-sdk
```

### 3. 📂 Criar o Diretório de Build

```bash
# Crie o diretório de build e navegue até ele
$ mkdir build && cd build

# Configure o projeto com CMake
$ cmake -G "Ninja" ..
```

### 4. 🧱 Compilar o Projeto

```bash
# Compile o projeto
$ ninja
```

### 5. 🚀 Enviar o Firmware para o Pico

- Conecte o Raspberry Pi Pico ao computador pressionando o botão `BOOTSEL`.
- Copie o arquivo `.uf2` gerado para a unidade do Pico:

```bash
$ cp semaforo-projeto.uf2 /media/<usuario>/RPI-RP2
```

---

## 🔌 Fios e Conexões

### Display OLED SSD1306

| Pino SSD1306 | Pino Raspberry Pi Pico |
| ------------ | ---------------------- |
| VCC          | 3V3                    |
| GND          | GND                    |
| SDA          | GP14                   |
| SCL          | GP15                   |

### LEDs

| Cor do LED  | Pino do Pico |
| ----------- | ------------ |
| 🔴 Vermelho | GP13         |
| 🟢 Verde    | GP11         |
| 🔵 Azul     | GP12         |

### Botão Push

| Botão | Pino do Pico |
| ----- | ------------ |
| 🔘 A  | GP5          |

---

## ▶️ Execução do Programa

1. **Ligar o Pico**: Conecte o Raspberry Pi Pico a uma fonte de alimentação USB.
2. **Introdução**: O display exibe uma mensagem de introdução com o texto:
   - "Me chamo Hagliberto, aluno do Embarcatech, irei mostrar o projeto."
3. **Semáforo**:
   - O semáforo exibe três estados (vermelho, amarelo e verde) sincronizados com os LEDs.
   - A duração de cada estado pode ser ajustada pressionando o botão.

---

## 🗂️ Estrutura do Código

### Principais Arquivos

1. `semaforo-projeto.c`:

   - Contém a lógica principal do programa, incluindo a inicialização e o loop infinito.

2. `ssd1306_i2c.c` e `ssd1306_i2c.h`:

   - Implementações para o controle do display SSD1306 (desenho de pixels, linhas e textos).

3. `ssd1306_font.h`:

   - Fonte utilizada para exibir caracteres no display OLED.

4. `CMakeLists.txt`:

   - Configura o ambiente de build para compilar o projeto.

---

## ✏️ Personalização

### Alterar Textos

Os textos exibidos no display podem ser alterados editando as variáveis no arquivo `semaforo-projeto.c`:

```c
char *GREEN_TRAFFIC_LIGHT_TEXT[] = {
    "               ",
    " SINAL ABERTO  ",
    " ATRAVESSE COM ",
    "    CUIDADO    "
};
```

### Ajustar Tempos

Os tempos para cada estado do semáforo podem ser alterados no loop principal:

```c
int yellow_traffic_light_time = 2000; // 2 segundos para o sinal amarelo
int green_traffic_light_time = 8000; // 8 segundos para o sinal verde
```

---

## 📜 Licença

Este projeto é distribuído sob a Licença MIT. Consulte o arquivo `LICENSE` para mais informações.
