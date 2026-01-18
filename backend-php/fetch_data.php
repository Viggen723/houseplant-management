<?php
header("Content-Type: application/json");
header("Access-Control-Allow-Origin: *");

$hostname = "localhost";
$username = "root";
$password = "";
$database = "sensor_db";

$conn = mysqli_connect($hostname, $username, $password, $database);

if (!$conn) {
    die("Connection failed: " . mysqli_connect_error());
}

$query = "select Temperature, Humidity, Soil_Dryness, Timestamp from sensor_readings ORDER BY Timestamp DESC";
$result = mysqli_query($conn, $query);

$data = [];

while ($row = mysqli_fetch_assoc($result))
{
    $data[] = $row;
}

echo json_encode($data);

?>
