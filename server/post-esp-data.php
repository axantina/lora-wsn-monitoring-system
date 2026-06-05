<?php
/*
  Simplified local testing script for ESP32 LoRa monitoring system.
  Copy config.example.php to config.php and update the database credentials.
*/

if (file_exists(__DIR__ . "/config.php")) {
    include __DIR__ . "/config.php";
} else {
    $servername = "localhost";
    $dbname = "YOUR_DATABASE_NAME";
    $username = "YOUR_DATABASE_USERNAME";
    $password = "YOUR_DATABASE_PASSWORD";

    $api_key_value = "YOUR_API_KEY";
}

$api_key = $tipe = $counter = $humidity = $temperature = $heatindex = $soilmoisture = $rssi = "";

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $api_key = test_input($_POST["api_key"]);

    if ($api_key == $api_key_value) {
        $tipe = test_input($_POST["tipe"]);
        $counter = test_input($_POST["counter"]);
        $humidity = test_input($_POST["humidity"]);
        $temperature = test_input($_POST["temperature"]);
        $heatindex = test_input($_POST["heatindex"]);
        $soilmoisture = test_input($_POST["soilmoisture"]);
        $rssi = test_input($_POST["rssi"]);

        $conn = new mysqli($servername, $username, $password, $dbname);

        if ($conn->connect_error) {
            die("Connection failed: " . $conn->connect_error);
        }

        $sql = "INSERT INTO SensorData (tipe, counter, humidity, temperature, heatindex, soilmoisture, rssi)
        VALUES ('" . $tipe . "', '" . $counter . "', '" . $humidity . "', '" . $temperature . "', '" . $heatindex . "', '" . $soilmoisture . "', '" . $rssi . "')";

        if ($conn->query($sql) === TRUE) {
            echo "New record created successfully";
        } else {
            echo "Error: " . $sql . "<br>" . $conn->error;
        }

        $conn->close();
    } else {
        echo "Wrong API Key provided.";
    }
} else {
    echo "No data posted with HTTP POST.";
}

function test_input($data) {
    $data = trim($data);
    $data = stripslashes($data);
    $data = htmlspecialchars($data);
    return $data;
}
?>
