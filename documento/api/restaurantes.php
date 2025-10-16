<?php
header("Content-Type: application/json");
include("../db/conexion.php");

$method = $_SERVER["REQUEST_METHOD"];

switch ($method) {
  case 'GET':
    $result = $conn->query("SELECT * FROM restaurantes");
    $restaurantes = [];
    while ($row = $result->fetch_assoc()) {
      $restaurantes[] = $row;
    }
    echo json_encode($restaurantes);
    break;

  case 'POST':
    $data = json_decode(file_get_contents("php://input"), true);
    if (!isset($data["name"])) {
      http_response_code(400);
      echo json_encode(["error" => "Falta el campo 'name'"]);
      exit;
    }
    $name = $conn->real_escape_string($data["name"]);
    $address = $conn->real_escape_string($data["address"] ?? '');
    $food_type = $conn->real_escape_string($data["food_type"] ?? '');

    $sql = "INSERT INTO restaurantes (name, address, food_type)
            VALUES ('$name', '$address', '$food_type')";

    if ($conn->query($sql)) {
      $id = $conn->insert_id;
      $nuevo = $conn->query("SELECT * FROM restaurantes WHERE id=$id")->fetch_assoc();
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