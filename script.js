// Conexão com o broker HiveMQ via WebSocket
const client = mqtt.connect("wss://broker.hivemq.com:8884/mqtt");

client.on("connect", () => {
  console.log("✅ Conectado ao broker HiveMQ!");
  client.subscribe("esp32/temperatura");
  client.subscribe("esp32/umidade");
  client.subscribe("esp32/luminosidade");
});

client.on("message", (topic, message) => {
  const valor = message.toString();

  if (topic === "esp32/temperatura")
    document.getElementById("temp").textContent = `${valor} °C`;
  if (topic === "esp32/umidade")
    document.getElementById("hum").textContent = `${valor} %`;
  if (topic === "esp32/luminosidade")
    document.getElementById("luz").textContent = valor;
});

client.on("error", (err) => {
  console.error("Erro de conexão MQTT:", err);
});
