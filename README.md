# 🍷 Vinheria Agnello — IoT Monitoring System

Projeto acadêmico desenvolvido para a **FIAP** com o objetivo de criar uma solução IoT para monitoramento das condições de armazenamento de vinhos da **Vinheria Agnello**.

A solução realiza a leitura de **temperatura**, **umidade** e **luminosidade** utilizando um ESP32, envia os dados via **MQTT** para o broker público da HiveMQ e apresenta as informações em tempo real por meio de um dashboard desenvolvido no **Node-RED**.

---

## 📌 Visão geral

O armazenamento adequado é essencial para preservar a qualidade dos vinhos. Este projeto monitora variáveis ambientais importantes da adega:

* 🌡️ **Temperatura**
* 💧 **Umidade**
* ☀️ **Luminosidade**

As leituras são realizadas a cada **4 segundos**, exibidas no monitor serial em formato JSON e publicadas em tópicos MQTT.

### Arquitetura

```text
┌─────────────────────┐
│       Sensores      │
│  DHT22 + LDR Module │
└──────────┬──────────┘
           │
           ▼
┌─────────────────────┐
│        ESP32        │
│      Arduino/C++    │
└──────────┬──────────┘
           │ Wi-Fi
           │ MQTT / JSON
           ▼
┌─────────────────────┐
│   HiveMQ Broker     │
│ broker.hivemq.com   │
└──────────┬──────────┘
           │
           ▼
┌─────────────────────┐
│      Node-RED       │
│ Dashboard + Debug   │
└─────────────────────┘
```

---

## 🛠️ Tecnologias utilizadas

* **ESP32 DevKit C V4**
* **Arduino / C++**
* **DHT22**
* **Photoresistor Sensor Module (LDR)**
* **Wokwi**
* **MQTT**
* **HiveMQ Public Broker**
* **Node-RED**
* **Node-RED Dashboard**

---

## 🔌 Circuito

O circuito é composto por um ESP32, um sensor DHT22 e o módulo de fotoresistor disponível no Wokwi.

### DHT22

| DHT22 | ESP32   |
| ----- | ------- |
| VCC   | 3V3     |
| GND   | GND     |
| DATA  | GPIO 15 |

### Photoresistor Sensor

| Photoresistor | ESP32   |
| ------------- | ------- |
| VCC           | 3V3     |
| GND           | GND     |
| AO            | GPIO 34 |

---

## 📡 Comunicação MQTT

O projeto utiliza o broker público da HiveMQ:

```text
Broker: broker.hivemq.com
Porta: 1883
```

### Tópicos utilizados

```text
Fiap/iot/3ESOA-2026/xfive/temperatura
Fiap/iot/3ESOA-2026/xfive/umidade
Fiap/iot/3ESOA-2026/xfive/luminosidade
Fiap/iot/3ESOA-2026/xfive/todos
```

Os três primeiros tópicos publicam cada grandeza individualmente. O tópico `todos` publica uma mensagem consolidada com todos os valores.

---

## 📦 Formato das mensagens

A cada ciclo de leitura, o ESP32 gera um JSON contendo os dados dos sensores.

Exemplo:

```json
{
  "temperatura": 18.5,
  "umidade": 64.2,
  "luminosidade": 37
}
```

As mensagens individuais são publicadas no seguinte formato:

### Temperatura

```json
{
  "temperatura": 18.5
}
```

### Umidade

```json
{
  "umidade": 64.2
}
```

### Luminosidade

```json
{
  "luminosidade": 37
}
```

---

## 💻 Simulação no Wokwi

O projeto pode ser executado integralmente no Wokwi:

**Projeto Wokwi:**
https://wokwi.com/projects/474821750634228737

Ao iniciar a simulação, o ESP32:

1. conecta-se à rede `Wokwi-GUEST`;
2. conecta-se ao broker MQTT da HiveMQ;
3. realiza a leitura dos sensores;
4. gera os dados em JSON;
5. exibe o JSON no Serial Monitor;
6. publica os valores nos tópicos MQTT;
7. repete o processo a cada 4 segundos.

---

## 📊 Node-RED

O arquivo [`flows.json`](./flows.json) contém o fluxo utilizado no Node-RED.

O fluxo:

* conecta-se ao broker `broker.hivemq.com`;
* assina os tópicos MQTT do projeto;
* recebe mensagens JSON;
* separa temperatura, umidade e luminosidade;
* exibe os valores em nós de debug;
* alimenta os componentes do dashboard;
* permite o monitoramento dos sensores em tempo real.

### Importando o fluxo

1. Instale e execute o Node-RED.
2. Acesse:

```text
http://localhost:1880
```

3. Abra o menu ☰.
4. Selecione **Import**.
5. Escolha o arquivo `flows.json` deste repositório.
6. Importe o fluxo.
7. Clique em **Deploy**.

Caso os nós `ui-*` apareçam como desconhecidos, instale o pacote de dashboard utilizado pelo fluxo através de:

```text
Menu → Manage palette → Install
```

e procure por:

```text
@flowfuse/node-red-dashboard
```

Depois do deploy, o dashboard fica disponível no caminho configurado pelo fluxo:

```text
http://localhost:1880/dashboard
```

---

## 📈 Dashboard

O dashboard apresenta as informações coletadas pelos sensores em tempo real.

Entre os componentes utilizados estão:

* 🌡️ indicador de temperatura;
* 💧 indicador de umidade;
* ☀️ indicador de luminosidade;
* mensagens de debug para validação das informações;
* visualização consolidada das condições monitoradas.

O dashboard é atualizado conforme novas mensagens MQTT são recebidas.

---

## 📚 Bibliotecas Arduino

As bibliotecas utilizadas pelo projeto estão registradas em [`libraries.txt`](./libraries.txt):

```text
DHT sensor library
PubSubClient
WiFi
```

### Principais responsabilidades

| Biblioteca     | Função                        |
| -------------- | ----------------------------- |
| `WiFi`         | Conexão do ESP32 à rede Wi-Fi |
| `PubSubClient` | Comunicação MQTT              |
| `DHT`          | Leitura do sensor DHT22       |

---

## 📁 Estrutura do repositório

```text
VinheriaAgnello-IOT/
│
├── diagram.json
├── flows.json
├── libraries.txt
├── sketch.ino
├── wokwi-project.txt
└── README.md
```

### Arquivos

| Arquivo             | Descrição                                    |
| ------------------- | -------------------------------------------- |
| `sketch.ino`        | Código Arduino executado pelo ESP32          |
| `diagram.json`      | Configuração e conexões do circuito no Wokwi |
| `libraries.txt`     | Bibliotecas utilizadas na simulação          |
| `flows.json`        | Fluxo completo exportado do Node-RED         |
| `wokwi-project.txt` | Referência para o projeto no Wokwi           |
| `README.md`         | Documentação do projeto                      |

---

## 🚀 Como executar

### 1. Wokwi

Abra o projeto:

https://wokwi.com/projects/474821750634228737

Inicie a simulação e mantenha o projeto em execução.

### 2. Node-RED

Inicie o Node-RED:

```bash
node-red
```

Importe o arquivo:

```text
flows.json
```

e clique em **Deploy**.

### 3. Dashboard

Abra no navegador:

```text
http://localhost:1880/dashboard
```

### 4. Teste

No Wokwi, altere os valores dos sensores durante a simulação.

As alterações devem aparecer no:

* Serial Monitor;
* broker MQTT;
* debug do Node-RED;
* dashboard do Node-RED.

---

## 🔄 Fluxo dos dados

```text
DHT22 ───────────────┐
                     │
Photoresistor ───────┤
                     ▼
                   ESP32
                     │
                     │ JSON
                     ▼
                   MQTT
                     │
                     ▼
             HiveMQ Public Broker
                     │
                     ▼
                  Node-RED
                     │
              ┌──────┴──────┐
              ▼             ▼
            Debug        Dashboard
```

---

## 📖 Contexto acadêmico

Projeto desenvolvido como atividade da **FIAP**, aplicando conceitos de:

* Internet das Coisas (IoT);
* sistemas embarcados;
* sensores e microcontroladores;
* comunicação MQTT;
* integração de sistemas;
* processamento de dados em JSON;
* dashboards e monitoramento em tempo real.

---

## ⚠️ Observação

O projeto utiliza o **HiveMQ Public Broker**, destinado a testes e prototipação. Como se trata de um broker público, os tópicos e mensagens não devem conter informações sensíveis.

---

## 📄 Licença

Este projeto foi desenvolvido para fins acadêmicos e educacionais.
