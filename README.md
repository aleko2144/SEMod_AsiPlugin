# ASI плагин модификации SEMod для игры "Дальнобойщики-2"
## Однако, работает.

### Отмазка:
#### Плагин представлен КАК ЕСТЬ.
#### При использовании кода из плагина указывать ссылку на первоисточник.

### Файлы:
#### .\AsiPlugin\dllmain.cpp - основа:
#### .\AsiPlugin\dllmain.cpp -> ReadBooleanFromIni - чтение bool'ов из ini;
#### .\AsiPlugin\dllmain.cpp -> GetPrivateProfileFloat - чтение float из ini;
#### .\AsiPlugin\dllmain.cpp -> GetPrivateProfileStr - чтение строки из ini;
#### .\AsiPlugin\dllmain.cpp -> PrintUserLog - запись лога плагина;
#### .\AsiPlugin\dllmain.cpp -> PrintDebugLog - запись отладочного лога плагина (если отладка включена);
#### .\AsiPlugin\dllmain.cpp -> PrintWarnLog - запись лога с ошибками (если запись сообщений об ошибках включена);
#### .\AsiPlugin\dllmain.cpp -> ReadParamsFromIni - чтение параметров плагина из SEMod.ini;
#### .\AsiPlugin\dllmain.cpp -> IsKeyPressed - проверка нажатия клавиш;
#### .\AsiPlugin\dllmain.cpp -> GetInput - обработка нажатий клавиш;
#### .\AsiPlugin\dllmain.cpp -> CustomRes - исправление GUI при использовании нестандартных разрешений экрана;
#### .\AsiPlugin\dllmain.cpp -> A_Cam - "улучшенная" камера, поворот мышью обзора в салонах;
#### .\AsiPlugin\dllmain.cpp -> A_Signals - индикаторы поворота;
#### .\AsiPlugin\dllmain.cpp -> VehicleChanged - проверка, произошла ли смена автомобиля игрока;
#### .\AsiPlugin\dllmain.cpp -> PrepareValues - обновление констант;
#### .\AsiPlugin\dllmain.cpp -> ResetValues - сброс констант;
#### .\AsiPlugin\dllmain.cpp -> AdvancedSounds - дополнительные звуки;
#### .\AsiPlugin\dllmain.cpp -> Update - обновление параметров, которые всё время обновляются;
#### .\AsiPlugin\dllmain.cpp -> Process - вызов функций (каждый кадр).

#### .\AsiPlugin\GameApp.cpp - всякая всячина, вроде принта текста;
#### .\to_delete\ - последняя версия плагина без классов.

#### new_structures.h - новые типы данных (Vector3D и Matrix).

### Благодарности:
#### Юрий Гладышенко, Алексей Воронов

#### [Сообщество во ВК](https://vk.com/rnr_mods)
