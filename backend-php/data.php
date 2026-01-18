<?php
$hostname = "localhost";
$username = "root";
$password = "";
$database = "sensor_db";

$conn = mysqli_connect($hostname, $username, $password, $database);
if (!$conn) {
    die("Connection failed: " . mysqli_connect_error());
}

echo "Database connection was successful";

if (isset($_POST["soil"]) && isset($_POST["temp"]) && isset($_POST["hum"])) {

    $soil = mysqli_real_escape_string($conn, $_POST["soil"]);
    $temp = mysqli_real_escape_string($conn, $_POST["temp"]);
    $hum  = mysqli_real_escape_string($conn, $_POST["hum"]);

    $sql = "INSERT INTO sensor_readings (Soil_Dryness, Temperature, Humidity) 
            VALUES ('$soil', '$temp', '$hum')";

    if (mysqli_query($conn, $sql)) {
        echo "New record created successfully";
    } else {
        echo "Error: " . mysqli_error($conn);
    }

} else {
    echo "POST data missing: soil, temp, or hum";
}

mysqli_close($conn);
?>

