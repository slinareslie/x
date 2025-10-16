<?php
header("Content-Type: application/json");

$url = "https://wttr.in/Tacna?format=j1";
$response = @file_get_contents($url);

if ($response === FALSE) {
  http_response_code(500);
  echo json_encode(["error" => "No se pudo obtener el clima"]);
  exit;
}

$data = json_decode($response, true);

if (!isset($data["current_condition"][0])) {
  echo json_encode(["error" => "Formato de respuesta inesperado"]);
  exit;
}

$current = $data["current_condition"][0];
$temp = $current["temp_C"];
$desc = $current["weatherDesc"][0]["value"];

echo json_encode([
  "temperature_c" => $temp,
  "description" => $desc
]);
?>