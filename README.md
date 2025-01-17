# XOR Encryptor

> [!NOTE]
> Приложение позволяет модифицировать входные файлы с помощью различных операций, таких как XOR с 8-байтной переменной. Приложение включает возможность управления входными и выходными файлами, а также настройки параметров выполнения.

## Функциональность

 - Маска входных файлов: Поддержка различных форматов файлов, включая:
```
.txt
testFile.bin
```
 - Удаление входных файлов: Опция для настройки необходимости удаления входных файлов после обработки: **Да | Нет**
 - Путь для сохранения результирующих файлов: Укажите путь, где будут сохранены обработанные файлы.
- Действия при повторении имени выходного файла:
    - Перезапись существующего файла
    - Модификация имени файла с помощью счетчика (например, добавление числа к имени файла)
- Работа по таймеру или разовый запуск
- 8-байтная переменная для бинарной операции модификации файла: Пользователь может вводить значение в 8 байт для выполнения операций модификации, таких как XOR.


## Использование

- Запустите приложение.
- Настройте параметры:
    - Укажите маску входных файлов.
    - Выберите, нужно ли удалять входные файлы.
    - Укажите путь для сохранения результирующих файлов.
    - Настройте поведение при совпадении имен выходных файлов.
    - Выберите режим работы (таймер или разовый запуск).
        - при выборе запуска по таймеру выберите кол-во секунд в соответствующем поле.
    - Введите 8-байтное значение для операции модификации.
    - Нажмите кнопку "Старт" для начала обработки.
