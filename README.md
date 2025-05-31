# 🏭 ArduinoFactoryControl - Sistema de Controle de Planta Industrial 🌳

## 📌 Visão Geral

Sistema embarcado para automação industrial de corte de madeira utilizando dois Arduino Nano, com comunicação UART e controle de sensores/atuadores em linguagem C a nível de registrador.

## 🎯 Objetivos

- Implementar controle industrial para corte de blocos de madeira (10cm x 25cm) 🪵✂️
- Demonstrar habilidades em:
  - Programação em C de baixo nível (registradores) ⚙️👨‍💻
  - Comunicação (UART) 📡
  - Integração de sensores e atuadores 🔄
  - Tratamento de interrupções e temporização ⏱️

## 🏗️ Arquitetura do Sistema

### 🔝 Arduino 1 (Supervisor)

**Funções principais:**

- 🎛️ Controle central da planta industrial
- 📊 Monitoramento de status
- 🚦 Controle de emergência

**Recursos implementados:**

- 2 potenciômetros 🎚️ para controle de velocidade dos motores
- Botão de parada de emergência 🛑 com interrupção externa
- Comunicação serial 📡 com Arduino 2
- Exibição de dados no Monitor Serial 🖥️

### 🏭 Arduino 2 (Chão de Fábrica)

**Funções principais:**

- ⚙️ Controle direto dos atuadores
- 🌡️ Monitoramento de sensores
- 🛡️ Segurança operacional

**Componentes implementados:**

- 2 servomotores ⚙️ para corte
- 1 servomotor para Inclinação
- Sistema de segurança com:
  - Sensor de temperatura DHT11 🌡️ (10-40°C)
  - Sensor de inclinação 📐 (botão simulado)
  - Sensor de presença 👀 (LDR+LM358)
  - Sensor de nível de óleo 🛢️ (infravermelho)
- Display de 7 segmentos 🔢 com 74HC595
- Indicadores visuais (LEDs 🟢🔴) e sonoros (buzzer 🚨)

## ⚙️ Implementação Técnica

### 🔧 Hardware

- **Microcontrolador:** ATmega328P 💾
- **Protocolo de comunicação:** UART 📡 (1M baud)
- **Técnicas utilizadas:**
  - PWM ⚡ para controle de motores
  - ADC 📊 para leitura de sensores
  - GPIO 📌 para I/O digital
  - Timers 0, 1 e 2 ⏱️ para temporização
  - Interrupções externas (INT0, INT1) e PCINT 📟

### 💾 Software

- **Ambiente de desenvolvimento:** Microchip Studio 7.0 🖥️
- **Simulador:** SimulIDE 🧪
- **Bibliotecas:** Nenhuma (implementação direta em registradores) 🚫📚
- **Estrutura do código:**
  - Inicialização de periféricos 🔄
  - Rotinas de interrupção ⚡
  - Máquina de estados principal 🤖
  - Comunicação serial 📡

## 📋 Especificações de Funcionamento

### 🔄 Fluxo Operacional

1. Inicialização e teste dos sistemas 🔌
2. Verificação contínua de segurança 🛡️
3. Controle de velocidade via potenciômetros 🎚️
4. Monitoramento serial a cada 3s ⏱️
5. Tratamento de emergências:
   - Parada manual 🛑
   - Temperatura crítica 🌡️🔥
   - Inclinação irregular 📐⚠️
   - Presença humana 👨‍🔧🚫

### 🚨 Condições de Alarme

| Condição               | Ações                              | Indicadores     |
|------------------------|------------------------------------|-----------------|
| Temperatura >40°C      | Parada, LED vermelho, buzzer       | 🔴🚨🌡️         |
| Inclinação irregular   | Ajuste servo, parada              | 📐🛑⚙️         |
| Presença humana        | Parada imediata                   | 👀⚠️🚫         |
| Nível de óleo crítico  | Alerta serial                     | 🛢️❌📡         |

## 📊 Resultados Obtidos

- ✅ Controle preciso de velocidade dos motores ⚙️
- ✅ Comunicação serial estável 📡
- ✅ Tratamento de interrupções eficiente ⚡
- ⚠️ Limitação: Display mostra apenas unidade (devido a restrições de GPIO) 🔢❌

## 🛠️ Instalação e Uso

1. **Hardware:**
   - Conectar Arduino 1 e 2 via TX/RX 🔌
   - Montar circuito conforme diagrama 📐
   - Alimentar com 5V estável ⚡

2. **Programação:**

   ```bash
   # Compilar com Microchip Studio 🖥️
   # Gravar .hex nos Arduinos 💾
