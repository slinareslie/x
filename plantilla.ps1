# Abrir Excel
$excel = New-Object -ComObject Excel.Application
$excel.Visible = $false  # Establecer a $true si quieres ver Excel

# Abrir el archivo base.xls
$baseFilePath = "C:\ruta\a\base.xls"
$baseWorkbook = $excel.Workbooks.Open($baseFilePath)

# Obtener el dato de la celda A2
$cellValue = $baseWorkbook.Sheets.Item(1).Range("A2").Value()

# Abrir el archivo plantilla.xls
$templateFilePath = "C:\ruta\a\plantilla.xls"
$templateWorkbook = $excel.Workbooks.Open($templateFilePath)

# Insertar el dato en la siguiente fila vacía (por ejemplo en la columna A)
$lastRow = $templateWorkbook.Sheets.Item(1).Cells($templateWorkbook.Sheets.Item(1).Rows.Count, 1).End(-4162).Row + 1
$templateWorkbook.Sheets.Item(1).Cells.Item($lastRow, 1).Value = $cellValue

# Guardar el archivo plantilla.xls
$templateWorkbook.Save()

# Cerrar los archivos y Excel
$baseWorkbook.Close()
$templateWorkbook.Close()
$excel.Quit()

# Liberar el objeto COM
[System.Runtime.Interopservices.Marshal]::ReleaseComObject($excel) | Out-Null
