<?php
class Database {
    private static $dbName = 'tesis'; 
    private static $dbHost = 'localhost';
    private static $dbPort = '3306'; 
    private static $dbUsername = 'root';
    private static $dbUserPassword = '';
    private static $cont = null;
     
    public function __construct() {
        die('Init function is not allowed');
    }
     
    public static function connect() {
        if (null == self::$cont) {     
            try {
                self::$cont = new PDO("mysql:host=".self::$dbHost.";port=".self::$dbPort.";"."dbname=".self::$dbName, self::$dbUsername, self::$dbUserPassword); 
                self::$cont->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
            } catch(PDOException $e) {
                die($e->getMessage()); 
            }
        }
        return self::$cont;
    }
     
    public static function disconnect() {
        self::$cont = null;
    }
}

header('Content-Type: application/json');

if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    if (!empty($_POST['ir']) && !empty($_POST['red'])) {
        $ir = $_POST['ir'];
        $red = $_POST['red'];
        
        // Validación adicional
        if (!filter_var($ir, FILTER_VALIDATE_INT) || !filter_var($red, FILTER_VALIDATE_INT)) {
            echo json_encode(['status' => 'error', 'message' => 'Los datos "ir" y "red" deben ser números enteros válidos.']);
            exit;
        }

        $pdo = Database::connect();
        try {
            $sql = "INSERT INTO datos_oxi (ir, red) VALUES (?, ?)";
            $q = $pdo->prepare($sql);
            $q->execute([$ir, $red]);

            $lastId = $pdo->lastInsertId();

            Database::disconnect();
            echo json_encode(['status' => 'success', 'message' => 'Datos insertados correctamente.', 'id' => $lastId]);
        } catch (PDOException $e) {
            echo json_encode(['status' => 'error', 'message' => 'Error al insertar los datos: ' . $e->getMessage()]);
        }
    } else {
        echo json_encode(['status' => 'error', 'message' => 'Los datos POST están incompletos o vacíos.']);
    }
} else {
    echo json_encode(['status' => 'error', 'message' => 'Método no permitido.']);
}
?>
