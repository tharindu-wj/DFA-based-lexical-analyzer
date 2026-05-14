param(
    [Parameter(Mandatory=$true)][string]$Name
)

$lexer = Join-Path $PSScriptRoot "..\cmake-build-release\lexer.exe"
$input = Join-Path $PSScriptRoot "inputs\$Name.c"
$expected = Join-Path $PSScriptRoot "expected\$Name.txt"

# Normalise: strip all \r, then trim outer whitespace
$actual_text = ([string]((& $lexer $input | Out-String)) -replace "`r", "").Trim()
$expected_text = ([string](Get-Content $expected -Raw) -replace "`r", "").Trim()

if ($actual_text -eq $expected_text) {
    Write-Host "PASS  $Name" -ForegroundColor Green
} else {
    Write-Host "FAIL  $Name" -ForegroundColor Red
    Write-Host ""
    # Show a real diff instead not just two blobs
    $actualLines = $actual_text   -split "`n"
    $expectedLines = $expected_text -split "`n"
    $max = [Math]::Max($actualLines.Count, $expectedLines.Count)
    for ($i = 0; $i -lt $max; $i++) {
        $a = if ($i -lt $actualLines.Count) { $actualLines[$i] } else { "<missing>" }
        $e = if ($i -lt $expectedLines.Count) { $expectedLines[$i] } else { "<missing>" }
        if ($a -ne $e) {
            Write-Host ("Line {0}:" -f ($i+1)) -ForegroundColor Yellow
            Write-Host ("  expected: {0}" -f $e) -ForegroundColor Cyan
            Write-Host ("  actual:   {0}" -f $a) -ForegroundColor Cyan
        }
    }
}