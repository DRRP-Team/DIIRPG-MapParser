# Doom II RPG Map Parser

> Copyright (c) DRRP-Team 2019 (UsernameAK & PROPHESSOR)

## mapparser.py

> Copyright (c) PROPHESSOR 2019

```sh
python mapparser.py path/to/map.bin
```

## mapparser.cpp

> Copyright (c) UsernameAK 2019

```sh
make
./mapparser path/to/map.bin
```

## Summary

mapparser.py является доработанной версией mapparser.cpp, поэтому рекомендуется использовать именно её.
На текущий момент скрипт умеет генерировать лишь .obj модели карт, которые, к тому же, содержат отсутствующие полигоны (в тех местах, где в игре видно небо, или же в местах, на которые никогда не смотрит камера).
В будущем планируется сделать генерацию карты в UDMF-формате, так что, если у вас есть идеи, как это можно сделать - ждём вашего Issue или Pull Request :)
