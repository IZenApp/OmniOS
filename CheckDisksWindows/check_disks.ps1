# Получить информацию о всех дисках
$volumes = Get-Volume

# Вывести информацию о каждом диске
foreach ($volume in $volumes) {
    Write-Output "Drive Letter: $($volume.DriveLetter)"
    Write-Output "File System: $($volume.FileSystem)"
    Write-Output "Size: $([math]::round($volume.Size/1GB, 2)) GB"
    Write-Output "Free Space: $([math]::round($volume.SizeRemaining/1GB, 2)) GB"
    Write-Output "Health Status: $($volume.HealthStatus)"
    Write-Output "Operational Status: $($volume.OperationalStatus)"
    Write-Output "----------------------------------------"
}