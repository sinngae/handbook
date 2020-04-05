# valgrind

```sh
valgrind --tool=memcheck --leak-check=full --main-stacksize=10000000 ./video_thumb -d /media/sda1/videos -o /media/sda3/bin/thumb
```