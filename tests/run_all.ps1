$tests = @(
    "test_1",
    "test_2",
    "test_3a"
)

foreach ($name in $tests) {
    & "$PSScriptRoot\run_test.ps1" -Name $name
}