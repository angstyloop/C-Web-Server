# <0.o> # <0.-> # <x_x> # <#^#> # <(/)..(/)>
function fun_compile {
  $target="cl "
  param($file)
  Select-String $target $file|Select-Object -ExpandProperty Line|Set-Clipboard
}
