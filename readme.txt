Projeto de prova de conceito (PoC) que monitora temperatura, umidade e luminosidade de um ambiente usando um ESP32 (simulado no Wokwi).]

Objetivos:

-Demonstrar comunicação ponta-a-ponta (device → broker → dashboard).
-Mostrar dados em tempo real no app móvel (MyMQTT) e no dashboard web.
-Entregar documentação detalhada para a Sprint 4.

Lista de Componentes (simulados no Wokwi)

-ESP32 DevKit (simulado)
-Sensor DHT11 ou DHT22 (temperatura e umidade)
-LDR (sensor de luminosidade)
-Resistor 10kΩ (divisor de tensão para LDR) '
-Cabos e GND / VCC do ESP32
-No Wokwi você monta o circuito virtualmente; se for para hardware real, use 3.3V para sensores e cuide de resistores em série com LEDs, se usar.

Esboço de montagem

Ligações principais (tanto para Wokwi quanto físico):

DHT11/DHT22
VCC → 3.3V (ESP32)
GND → GND (ESP32)
DATA → GPIO 15 (ou pino definido no código)
LDR (divisor de tensão)
LDR lado A → 3.3V
LDR lado B → A0 (GPIO 34)
Do mesmo nó (LDR lado B) → resistor 10kΩ → GND

ESP32
GPIO 15 → DHT DATA
GPIO 34 → leitura analógica do LDR
Inclua no relatório print do esquema do Wokwi (captura) mostrando essas ligações.

Como executar (passo a passo)
1. Rodar o ESP32 no Wokwi
Abra Wokwi
.
Crie novo projeto com ESP32.

Monte os componentes (DHT, LDR com resistor 10k no divisor).

Cole esp32_code/main.ino no editor do Wokwi.

Ajuste pinos se necessário.

Inicie simulação — monitor serial mostrará leituras e mensagens de conexão MQTT.

No Wokwi, publicação MQTT pode ser simulada — verifique console para prints de envio.

2. Visualizar no MyMQTT (app)

No app MyMQTT (celular), adicione conexão:

Host: 54.221.163.3

Port: 1883

Username: nicolas

Password: banana

Conecte e faça subscribe nos tópicos:

esp32/temperatura

esp32/umidade

esp32/luminosidade

Você verá mensagens a cada 5s.

3. Abrir dashboard localmente

Abra dashboard/index.html no navegador (duplo clique; ou sirva via um servidor local).

Abra console (F12 → Console) para ver logs.

O dashboard tentará conectar ao HiveMQ. Se conectado, verá mensagem no console: ✅ Conectado ao broker HiveMQ!

Valores nos cards serão atualizados em tempo real quando o ESP32 publicar nos tópicos esp32_dash/....