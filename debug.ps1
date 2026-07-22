trap [System.Management.Automation.PipelineStoppedException] {
    Stop-Process -Name tetris.exe
    Remove-Item .\tetris.exe
    Remove-Item .\tetris.pdb
}
param([switch]$EmbedAssets)
if ($EmbedAssets) {
    cmake -B build -DEMBED_ASSETS=ON
} else {
    cmake -B build
}
cmake --build build
Copy-Item .\build\Debug\tetris.exe .\ 
Copy-Item .\build\Debug\tetris.pdb .\ 
.\tetris.exe
Remove-Item .\tetris.exe
Remove-Item .\tetris.pdb