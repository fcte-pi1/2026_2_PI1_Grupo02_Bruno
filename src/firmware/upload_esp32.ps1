param(
    [string]$Port = "",
    [switch]$CompileOnly
)

$ErrorActionPreference = "Stop"

# 1. Mapeia unidade virtual R: sem acento (contorna bug do compilador GCC do ESP32 com "Vinícius")
if (-not (Test-Path "R:\")) {
    cmd /c subst R: "$env:USERPROFILE" | Out-Null
}

# 2. Sincroniza os arquivos do firmware para R:\Documents\Arduino\ratocego
$sketchDir = "R:\Documents\Arduino\ratocego"
$buildDir  = "R:\AppData\Local\Temp\ratocego_build"
New-Item -ItemType Directory -Force $sketchDir, $buildDir | Out-Null
Copy-Item "$PSScriptRoot\*.ino", "$PSScriptRoot\*.h", "$PSScriptRoot\*.cpp" $sketchDir -Force

# 3. Configura arduino-cli embutido da Arduino IDE apontando apenas para caminhos em R:
$cli = "R:\AppData\Local\Programs\Arduino IDE\resources\app\lib\backend\resources\arduino-cli.exe"
if (-not (Test-Path $cli)) {
    throw "arduino-cli.exe nao encontrado em $cli"
}

$env:ARDUINO_DIRECTORIES_DATA      = "R:\AppData\Local\Arduino15"
$env:ARDUINO_DIRECTORIES_USER      = "R:\Documents\Arduino"
$env:ARDUINO_DIRECTORIES_DOWNLOADS = "R:\AppData\Local\Arduino15\staging"

$gccRoot = Get-ChildItem "R:\AppData\Local\Arduino15\packages\esp32\tools\esp-x32" -Directory |
    Sort-Object Name -Descending | Select-Object -First 1 -ExpandProperty FullName
$gccVer = Get-ChildItem "$gccRoot\lib\gcc\xtensa-esp-elf" -Directory |
    Sort-Object Name -Descending | Select-Object -First 1 -ExpandProperty Name
$gccBase = $gccRoot -replace '\\', '/'
$extraL = "-L$gccBase/lib/gcc/xtensa-esp-elf/$gccVer/esp32/no-rtti " +
          "-L$gccBase/xtensa-esp-elf/lib/esp32/no-rtti " +
          "-L$gccBase/lib/gcc/xtensa-esp-elf/$gccVer/esp32 " +
          "-L$gccBase/xtensa-esp-elf/lib/esp32"

Write-Host ">>> Compilando firmware Rato Cego (ESP32 Dev Module)..." -ForegroundColor Cyan
& $cli compile `
    --fqbn esp32:esp32:esp32 `
    --build-path $buildDir `
    --build-property "compiler.c.elf.extra_flags=$extraL" `
    $sketchDir

if ($LASTEXITCODE -ne 0) {
    throw "Falha na compilacao (codigo $LASTEXITCODE)."
}

if ($CompileOnly) {
    Write-Host ">>> Compilacao concluida com sucesso!" -ForegroundColor Green
    exit 0
}

# 4. Detecta automaticamente a porta COM do ESP32 (CH340 / CP210x) se -Port nao foi informado
if (-not $Port) {
    $dev = Get-PnpDevice -Class Ports -PresentOnly |
        Where-Object { $_.FriendlyName -match 'CH340|CH341|CP210|Silicon Labs|USB-SERIAL|UART' } |
        Select-Object -First 1
    if ($dev -and $dev.FriendlyName -match '\((COM\d+)\)') {
        $Port = $Matches[1]
        Write-Host ">>> ESP32 detectado automaticamente na porta $Port ($($dev.FriendlyName))" -ForegroundColor Green
    } else {
        throw "Nenhum ESP32 encontrado via USB. Conecte o cabo USB (ou informe -Port COM9) e tente novamente."
    }
}

Write-Host ">>> Subindo firmware para o ESP32 na porta $Port..." -ForegroundColor Cyan
Write-Host "    (Se travar em 'Connecting...', segure o botao BOOT ate iniciar 'Writing...')" -ForegroundColor Yellow

& $cli upload `
    --fqbn esp32:esp32:esp32 `
    --input-dir $buildDir `
    -p $Port

if ($LASTEXITCODE -ne 0) {
    throw "Falha no upload para $Port (codigo $LASTEXITCODE)."
}

Write-Host ">>> Upload concluido com sucesso! Abra o Monitor Serial em $Port (115200 baud)." -ForegroundColor Green
