CREATE DATABASE IF NOT EXISTS lora_monitoring;

USE lora_monitoring;

CREATE TABLE IF NOT EXISTS SensorData (
    id INT AUTO_INCREMENT PRIMARY KEY,
    tipe VARCHAR(50),
    counter VARCHAR(50),
    humidity VARCHAR(50),
    temperature VARCHAR(50),
    heatindex VARCHAR(50),
    soilmoisture VARCHAR(50),
    rssi VARCHAR(50),
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);
