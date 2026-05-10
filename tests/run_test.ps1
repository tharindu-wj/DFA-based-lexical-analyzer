param(
    [Parameter(Mandatory=$true)][string]$Name
)

$lexer = Join-Path $PSScriptRoot "..\cmake-build-debug\lexer.exe"
$input = Join-Path $PSScriptRoot "inputs\$Name.c"
$expected = Join-Path $PSScriptRoot "expected\$Name.txt"

$actual_text = (& $lexer $input | Out-String).Trim()
$expected_text = (Get-Content $expected -Raw).Trim()

if ($actual_text -eq $expected_text) {
    Write-Host "PASS  $Name" -ForegroundColor Green
} else {
    Write-Host "FAIL  $Name" -ForegroundColor Red
    Write-Host ""
    Write-Host "expected" -ForegroundColor Cyan
    Write-Host $expected_text
    Write-Host "actual" -ForegroundColor Cyan
    Write-Host $actual_text
}