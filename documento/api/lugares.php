<?php
header("Content-Type: application/json");
include("../db/conexion.php");

$method = $_SERVER["REQUEST_METHOD"];

switch ($method) {
  case 'GET':
    $result = $conn->query("SELECT * FROM lugares");
    $lugares = [];
    while ($row = $result->fetch_assoc()) {
      $lugares[] = $row;
    }
    echo json_encode($lugares);
    break;

  case 'POST':
    $data = json_decode(file_get_contents("php://input"), true);
    if (!isset($data["name"])) {
      http_response_code(400);
      echo json_encode(["error" => "Falta el campo 'name'"]);
      exit;
    }
    $name = $conn->real_escape_string($data["name"]);
    $description = $conn->real_escape_string($data["description"] ?? '');
    $location = $conn->real_escape_string($data["location"] ?? '');

    $sql = "INSERT INTO lugares (name, description, location)
            VALUES ('$name', '$description', '$location')";

    if ($conn->query($sql)) {
      $id = $conn->insert_id;
      $nuevo = $conn->query("SELECT * FROM lugares WHERE id=$id")->fetch_assoc();
      http_response_code(201);
      echo json_encode($nuevo);
    } else {
      http_response_code(500);
      echo json_encode(["error" => "Error al insertar: " . $conn->error]);
    }
    break;

  default:
    http_response_code(405);
    echo json_encode(["error" => "Método no permitido"]);
}
?>