$sourcePath = "$PSScriptRoot"
$destPath = "$PSScriptRoot/../_extractedCMakeLists"

mkdir "../_extractedCMakeLists"

Get-ChildItem $sourcePath -Recurse -Include 'CMakeLists.txt' | Foreach-Object `
    {
        $destDir = Split-Path ($_.FullName -Replace [regex]::Escape($sourcePath), $destPath)
        if (!(Test-Path $destDir))
        {
            New-Item -ItemType directory $destDir | Out-Null
        }
        Copy-Item $_ -Destination $destDir
    }