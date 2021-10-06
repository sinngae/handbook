```sh
# go标准库中查找代码
grep -nr "const debug = false" "$(dirname $(which go))/../src"
```
