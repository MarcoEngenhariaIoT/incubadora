# Incubadora com Controle PID

Projeto desenvolvido como parte do curso de **Engenharia de Controle e Automação** na **Faculdade Anhanguera de Porto Alegre**, no 8º semestre de 2019, sob orientação do **Prof. Eng. Msc. Walter Diniz**.

## Resumo do Projeto

Este projeto consiste em uma **incubadora automática** com controle de temperatura baseado em um **controlador PID**, utilizando dois microcontroladores **Arduino** (Master e Slave) para leitura de sensores, atuação sobre lâmpadas e interface com o usuário via display LCD e teclado matricial.

## Funcionalidades

- Controle de temperatura via **PID** (Proporcional, Integral e Derivativo)
- Leitura de três sensores de temperatura (**LM35**)
- Interface com display **LCD 16x2** e teclado matricial **4x4**
- Comunicação **I2C** entre Arduino Master e Slave
- Modos de operação:
  - **Malha Fechada (PID)**
  - **Malha Aberta**
  - **Desligado**
- Ajuste de **setpoint** via teclado
- Armazenamento do setpoint na **EEPROM**
- Saída PWM para controle de lâmpadas

## Tecnologias e Componentes

### Hardware
- 2x Arduino (Modelo Nano/UNO)
- Display LCD 16x2
- Teclado Matricial 4x4
- Sensores de Temperatura LM35
- Lâmpadas (12V)
- Amplificador Operacional LM358
- MOSFET IRF740
- Resistores, Potenciômetros, Capacitores
- Fonte 12V e 5V

### Software
- Arduino IDE
- Bibliotecas:
  - `Wire.h` (comunicação I2C)
  - `LiquidCrystal.h` (controle do LCD)
  - `EEPROM.h` (armazenamento não volátil)

## Estrutura do Projeto

```
Incubadora-PID/
├── Esquema.pdf                 # Diagrama elétrico completo
├── LCD.ino                     # Código do Arduino Master (LCD + Teclado)
├── atuadorPID.ino              # Código do Arduino Slave (Sensores + PID + Atuador)

```

## Esquemático

O esquema completo do circuito está disponível no arquivo **`esquema.pdf`**, incluindo:
- Conexões do Arduino Master e Slave
- Interface com LCD e teclado
- Circuito de condicionamento de sinal com LM358
- Driver das lâmpadas com MOSFET

## Funcionamento

### Arduino Master
- Controla o display LCD e o teclado
- Exibe temperaturas interna, externa, do setpoint e da lâmpada
- Envia comandos via I2C para o Slave

### Arduino Slave
- Lê os sensores de temperatura (LM35)
- Executa o algoritmo PID
- Controla a potência das lâmpadas via PWM
- Comunica-se com o Master via I2C

## Como Usar

1. Carregue os sketches `LCD.ino` e `atuadorPID.ino` nos respectivos Arduinos.
2. Monte o circuito conforme o esquemático.
3. Alimente o sistema com 12V e 5V.
4. Use o teclado para:
   - **A**: Aumentar setpoint
   - **B**: Diminuir setpoint
   - **C**: Modo Malha Aberta
   - **D**: Ligar/Desligar sistema
   - **\***: Reset (segure por 2s)

## Autores

- **Marco Aurélio Machado**
- **Luciano Grassi Kuyven**
- **André M. Machado Júnior**

## Vídeo no YouTube
https://youtu.be/7spTsU_MetM?si=_UMo8YmrhWojLtKS

## Licença

Este projeto é de uso educacional. Sinta-se à vontade para referenciar e adaptar.


