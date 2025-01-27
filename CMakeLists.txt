# Define a versão mínima do CMake necessária.
cmake_minimum_required(VERSION 3.13)

# Define o padrão de linguagem C e C++ que será utilizado.
set(CMAKE_C_STANDARD 11)   # Padrão C11.
set(CMAKE_CXX_STANDARD 17) # Padrão C++17.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # Exporta os comandos de compilação (útil para ferramentas como análise estática).

# == Configurações necessárias para a extensão VS Code do Raspberry Pi Pico funcionar corretamente ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE}) # Define o caminho do diretório do usuário no Windows.
else()
    set(USERHOME $ENV{HOME})        # Define o caminho do diretório do usuário no Linux/macOS.
endif()

# Define as versões do SDK, toolchain e picotool a serem usadas.
set(sdkVersion 2.1.0)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.1.0)

# Define o caminho para o script de configuração da extensão VS Code.
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)

# Inclui o script de configuração do VS Code, se ele existir.
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================

# Define o tipo de placa utilizada no projeto (Pico W no caso).
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Inclui o arquivo para importar o SDK do Pico.
include(pico_sdk_import.cmake)

# Cria o projeto com o nome "semaforo-projeto" e especifica que ele usará C, C++ e ASM.
project(semaforo-projeto C CXX ASM)

# Inicializa o SDK do Pico.
pico_sdk_init()

# Adiciona o executável do projeto com os arquivos fonte utilizados.
add_executable(semaforo-projeto 
    semaforo-projeto.c       # Arquivo principal do projeto.
    inc/ssd1306_i2c.c        # Implementação para controlar o display SSD1306 via I2C.
)

# Define o nome e a versão do programa.
pico_set_program_name(semaforo-projeto "semaforo-projeto")
pico_set_program_version(semaforo-projeto "0.1")

# Habilita/desabilita a entrada/saída padrão (stdio) via UART e USB.
pico_enable_stdio_uart(semaforo-projeto 0) # Desabilita UART.
pico_enable_stdio_usb(semaforo-projeto 1) # Habilita USB.

# Adiciona as bibliotecas necessárias para o projeto.
target_link_libraries(semaforo-projeto 
    pico_stdlib         # Biblioteca padrão do Pico.
    hardware_i2c        # Biblioteca para comunicação via I2C.
)

# Adiciona os diretórios de cabeçalhos para inclusão (includes).
target_include_directories(semaforo-projeto PRIVATE ${CMAKE_CURRENT_LIST_DIR})

# Gera os arquivos de saída adicionais (ex: UF2, bin, hex).
pico_add_extra_outputs(semaforo-projeto)
