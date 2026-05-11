$tests = @(
    "test_1",
    "test_2",
    "test_3a",
    "test_3b",
    "test_3c",
    "test_3d",
    "test_5a"
    "test_5b",
    "test_5c"
)

foreach ($name in $tests) {
    & "$PSScriptRoot\run_test.ps1" -Name $name
}