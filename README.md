# Microcontroller AVR

## Поставленная задача
Придумать формат описания команд микроконтроллера семейства AVR.Написать программу, которая эммулирует работу микроконтроллера семейства, после отработки которой можно посмотреть данные микроконтроллера.

## Pешение

## Входные данные
### Первый файл(описание функций) 

В первой строке написаны два числа через запятую - это размеры памяти RAM и EEPROM микроконтроллера.

Пример:
`128 256`


Далее идут описания функций, который будут использоваться в коде.Каждая функция на отдельной строке.Пробелы в местах, где они не описаны не допускаются. Одна функция имеет следующий вид:


Название функции в одно слово, пробел;


Число машинных циклов, которое потратит микроконтроллер для выполнение данной функции, пробел;


Аргументы через запятую, пробел;


В конце каждой команды знак точка с запятой.


Пример:


`PUSH 1 Rd STACK=Rd;`


`POP 1 Rd Rd=STACK;`


Команды могут присваивать в регистры значения или класть переменный на стек.Пример:


`LDI 1 Rd,k Rd=k;` // Rd - аргумент-регистр, в который мы присваиваем некую константу(тоже аргумент).


Могут вычислять несложные арифметические выражения, содержащие знаки:  плюс/минус/умножить/делить/брать остаток от деления.Пример:


`REM 2 Rd,Rr1,Rr2 Rd=Rr1%Rr2;`


Можно использовать условие, содержащее &&, ||, ==, !=,  больше или меньше, но только что-то одно из это (в противном случае все выражение сравнивается на ноль). Вид условия: if(...)command.Пример:


`REC_IF 2 Rr,k if(Rr)PC=k;`


`REC_IF_LESS 3 Rr,k,to if(Rr<k)PC=to;`


### Второй файл(исполняемый код)
Содержит функции и аргументы с которыми их необходимо вызывать.Каждый вызов функции на отдельной строке. Аргументы для функции отдельны от имени функции пробелом, а между собой запятыми. Можно вызывать функцию с одним именем несколько раз.Пример:


`LDI R0,1`


`LDI R1,1`


### Реализация
Создан класс Emulator, который считывает и запоминает функции, создает представитель класса ControllerAVR, исполняет написанный код.Реализован специальный интерфейс для общения с пользователем, который запрашивает необходимые для отработки программы данные, после чего предоставляет возможность посмотреть результат выполнения кода.Предусмотрены многие возможные опечатки и ошибки и на их случае в программе реализованны обработка ошибок и исключений. 


Микроконтроллер имеет следующие поля: два вида памяти - RAM и EEPROM, размеры которых задаются во входном файле. RAM память используется для стека, а так же в начале ее хранятся регистры общего назначения(R*) и некоторые специальные регистры.EEPROM память пока не используется, но при завершении программы она сохраняется, тк является в микроконтроллере энергонезависимой.
С регистрами общего назначения можно выполнять операции вида: записать значение, считать значени, использовать в условиях, смотреть значение(output_information->show_particular_peristr).


СПециальные регистры: регистр состояния - SREG. Можно устанавливать его флаги и использовать их(по имени). PC - счетчик команд, с его помощью можно организовывать циклы(есть в примерах).STACK - можно сохранять переменные на стек и доставать их оттуда.

## Сборка
make all 

./emulator
### Удаление бинарных файлов
make clean

## Примеры входных файлов
### Вычисление факториала числа

function_file


`128 256`


`MOV 1 Rd,Rs Rd=Rs;`


`LDI 1 Rd,k Rd=k;`


`REM 2 Rd,Rr1,Rr2 Rd=Rr1%Rr2;`


`REC_IF 2 Rr,k if(Rr)PC=k;`


`MUL 2 Rd,Rr1,Rr2 Rd=Rr1*Rr2;`


`INC 1 Rd Rd=Rd+1;`


`REC_IF_LESS 3 Rr,k,to if(Rr<k)PC=to;`


code_file


`LDI R0,1`    \\ Обнуляем


`LDI R1,1`     \\ Обнуляем 


`MUL R0,R0,R1` \\


`INC R1`       \\


`REC_IF_LESS R1,6,2`


В результате в регистре R0 записан 6! (6 - указано в параметре последней функции).


### Вычисление gcd двух чисел


function_file

`128 256`


`MOV 1 Rd,Rs Rd=Rs;`


`LDI 1 Rd,k Rd=k;`


`REM 2 Rd,Rr1,Rr2 Rd=Rr1%Rr2;`


`REC_IF 2 Rr,k if(Rr)PC=k;`

code_file

`LDI R0,8`       //first number, the largest


`LDI R1,6`       //second number, the least


`REM R2,R0,R1`


`MOV R0,R1`


`MOV R1,R2`


`REC_IF R1,2`


В результате в регистре R0 будет записан наибольший общий делитель двух заданных чисел.


### Числа Фиббоначи

function_file

`128 256`


`REC_IF_LESS 3 Rr,k,to if(Rr<k)PC=to;`


`ADD 1 Rd,Rr Rd=Rr+Rd;`


`CLR 1 Rd Rd=0;`


`INC 1 Rd Rd=Rd+1;`


`MOV 1 Rd,Rs Rd=Rs;`


code_file

`CLR R0`


`CLR R1`


`CLR R5`


`INC R1`


`INC R0`


`INC R5`


`MOV R6,R0`


`MOV R0,R1`


`ADD R6,R0`


`MOV R1,R6`


`REC_IF_LESS R5,7,5`

Вычисляется 7 число Фиббоначи, которое записывается в регистр R0.
