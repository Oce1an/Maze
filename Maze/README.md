# Maze Pathfinder (Qt / C++)

Приложение для генерации лабиринтов, визуализации поиска пути и сравнения алгоритмов.

## Возможности

- **Генерация:** DFS (Recursive Backtracker), Prim, Kruskal
- **Поиск:** BFS, DFS, Dijkstra, A* (общая реализация в `SolverUtils`)
- **Анимация** с вкл/выкл, слайдером скорости и кнопкой «Остановить»
- **Панель отладки** (время, узлы, трассировка)
- Стены — **отдельные клетки** сетки; проходы — клетки пола
- Подсветка поиска: оттенок синего зависит от **расстояния от старта**
- Размер лабиринта **5–200**; трассировка анимации до 50000 шагов

## Сборка

```powershell
$env:PATH = "C:\Qt\Tools\mingw1310_64\bin;C:\Qt\Tools\Ninja;C:\Qt\6.11.0\mingw_64\bin;" + $env:PATH
cmake -B build -DCMAKE_PREFIX_PATH="C:\Qt\6.11.0\mingw_64" -G Ninja -DCMAKE_BUILD_TYPE=Release `
  -DCMAKE_CXX_COMPILER="C:/Qt/Tools/mingw1310_64/bin/g++.exe"
cmake --build build
.\build\MazePathfinder.exe
```

## Структура

```
src/
  solvers/SolverUtils.*   — общий поиск (BFS/DFS/взвешенный)
  generators/             — генераторы лабиринта
  MazeWidget.*            — отрисовка и анимация
  MainWindow.*            — UI и отладка
```
