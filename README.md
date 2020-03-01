# ASI плагин модификации SEMod для игры "Дальнобойщики-2"
## Однако, работает.

### Отмазка:
#### Плагин представлен КАК ЕСТЬ.
#### При использовании кода из плагина указывать ссылку на первоисточник.

### Авторы:
#### Андрей Прожога, Юрий Гладышенко, Алексей Воронов

#### [Сообщество во ВК](https://vk.com/rnr_mods)

### Файлы и функции:
#### .\AsiPlugin\dllmain.cpp - основа:
#### ReadBooleanFromIni - чтение bool'ов из ini;
#### GetPrivateProfileFloat - чтение float из ini;
#### GetPrivateProfileStr - чтение строки из ini;
#### PrintUserLog - запись лога плагина;
#### PrintDebugLog - запись отладочного лога плагина (если отладка включена);
#### PrintWarnLog - запись лога с ошибками (если запись сообщений об ошибках включена);
#### ReadParamsFromIni - чтение параметров плагина из SEMod.ini;
#### IsKeyPressed - проверка нажатия клавиш;
#### GetInput - обработка нажатий клавиш;
#### CustomRes - исправление GUI при использовании нестандартных разрешений экрана;
#### A_Cam - "улучшенная" камера, поворот мышью обзора в салонах;
#### A_Signals - индикаторы поворота;
#### VehicleChanged - проверка, произошла ли смена автомобиля игрока;
#### PrepareValues - обновление констант;
#### ResetValues - сброс констант;
#### AdvancedSounds - дополнительные звуки;
#### Update - обновление параметров, которые всё время обновляются;
#### Process - вызов функций (каждый кадр).

#### .\AsiPlugin\GameApp.cpp - всякая всячина:
#### DisplayScreenMessage - показ нижней панели с текстом;
#### DisplayPagerMessage - показ пейджера с текстом;
#### SearchResourceSND - поиск звукового файла в res;
#### PlaySound_ - воспроизведение звукого файла;
#### PlaySoundLocated - воспроизведение звукого файла в определённой точке (какие координаты нужны этой функции - неизвестно);
#### GetPlayerID - получить ID игрока;
#### GetPlayerCapital - получить количество капитала игрока;
#### GetActionState - проверка нажатия клавиши того или иного внутриигрового действия.

#### .\to_delete\ - последняя версия плагина без классов.

#### new_structures.h - новые типы данных (Vector3D и Matrix).
