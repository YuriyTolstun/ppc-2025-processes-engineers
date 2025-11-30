# <Подсчет числа буквенных символов в строке>

- Студент: <Толстун Юрий Андреевич>, группа <3823Б1ПР1>
- Технологии: <SEQ | MPI>
- Вариант: <22>

## 1. Введение
Обработка однотипных данных является типичной задачей в анализе данных и обработке текстовой информации. В данной работе нашей целью является ускорение подсчёта буквенных символов в строке за счёт применения параллельных вычислений с использованием MPI, а также сравнение результатов, чтобы оценить достигнутое ускорение и эффективность распределения нагрузки.

## 2. Постановка задачи
Нам нужно подсчитать число буквенных символов в строке
Входной тип - using InType = std::string; Мы получаем на вход строку, которую будем обрабатывать.
Выходной тип - using OutType = int; Мы возвращаем число буквенных символов в строке.

## 3. Базовый последовательный алгоритм (SEQ)
Для начала мы получаем нашу строку, затем мы подсчитываем количество буквенных символов проходя по каждому символу в строке и проверяя его, подходит ли он под условие задачи, и если подходит, то происходит увеличение переменной количества буквенных символов.

bool TolstunYBukStringCountSEQ::RunImpl() {
  const std::string &stroka = GetInput();

  // Считаем количество букв
  int bukv_count = 0;
  for (char sim : stroka) {
    if (std::isalpha(sim) != 0) {
      bukv_count++;
    }
  }

  GetOutput() = bukv_count;
  return true;
}

## 4. Схема параллелизации
### Распределение данных
В начале мы разбиваем строку на равные интервалы и определяем остаток:

std::size_t interval = size_stroka / static_cast<std::size_t>(size);
std::size_t ostatok = size_stroka % static_cast<std::size_t>(size);

Затем первым osatok процессам мы даем больше на 1 символ, чтобы между ними одинаково распределить остаток. Остальные процессы будут обрабатывать части строки равные interval.

td::size_t start = (rank * interval) + std::min(static_cast<std::size_t>(rank), ostatok);

std::size_t end = start + interval;
if (std::cmp_less(rank, static_cast<int>(ostatok))) {
  end += 1;
}
### Коммуникация и роли рангов
Все процессы получают входные данные через GetInput(). Результаты локальной обработки всех процессов суммируются с помощью MPI_Reduce и операцией MPI_SUM на корневом процессе с рангом = 0. Итоговый результат рассылается всем процессам через MPI_Bcast. В конце выполняется синхронизация MPI_Barrier для завершения.

## 5. Детали реализации
### Структура кода

`common/include/common.hpp` — общие определения типов
`seq/src/ops_seq.cpp` — последовательная реализация
`mpi/src/ops_mpi.cpp` — параллельная реализация
`tests/functional/main.cpp` — функциональные тесты
`tests/performance/main.cpp` — тесты производительности
###  Классы задач

Оба класса являются наследниками базового класса `ppc::task::Task<InType, OutType>`:

  `TolstunYBukStringCountSEQ` — последовательная версия
  `TolstunYBukStringCountMPI` — параллельная версия

Методы обоих классов:
  `ValidationImpl()` — проверка входных данных
  `PreProcessingImpl()` — подготовка данных
  `RunImpl()` — выполнение основного алгоритма
  `PostProcessingImpl()` — проверка результата


## 6. Настройка эксперимента
- **Процессор:** AMD Ryzen 5 5500U with Radeon Graphics 2.10 GHz
- **Количество ядер:** 6
- **ОЗУ:** 16,0 ГБ
- **ОС:** Windows 10 Pro

## 7. Results and Discussion

### 7.1 Корректность
Корректность реализации была достигнута следующими способами:

**Функциональные тесты:** Набор из 5 тестовых случаев, покрывающих следующие случаи:
  - Пустая строка
  - Строка с небуквенными символами
  - Строка с пробелами
  - Строка с только небуквенными символами
  - Строка с специальные символами (табуляция, перевод строки)

**Сравнение с последовательной версией:** Результаты MPI-версии сравниваются с результатами последовательной версии на тех же данных.

**Инварианты:** 
  - Результат всегда неотрицательный
  - Результат не больше количества символов в строке
  - Пустая строка всегда дает 0

### 7.2 Производительность

| Режим | Процессы | Время, с       | Ускорение | Эффективность |
|-------|----------|----------------|-----------|---------------|
| seq   | 1        | 0.1013505459   | 1.00      | N/A           |
| mpi   | 1        | 0.1139962216   | 0.89      | 88.9%         |
| mpi   | 2        | 0.0570976338   | 1.77      | 88.8%         |
| mpi   | 4        | 0.0386252538   | 2.62      | 65.6%         |
| mpi   | 6        | 0.0331322172   | 3.05      | 51.0%         |

**Метрики:**
- Ускорение: S(p) = T_seq / T_mpi(p)
- Эффективность: E(p) = S(p) / p × 100%

где:
- T_seq — время выполнения последовательной версии
- T_mpi(p) — время выполнения MPI-версии на p процессах

## 8. Вывод
В данной работе были сделаны последовательная и параллельная реализации алгоритма подсчета слов в строке.

1. **Корректность:** Обе версии успешно проходят все функциональные тесты.

2. **Параллелизация:** Реализована корректная схема распределения данных между процессами.

3. **Производительность:** Тесты на больших данных (50 миллионов букв, ~50 МБ) показали **положительное ускорение** с высокой эффективностью. 

## 9. References
1. <Курс по параллельному программированию>

## Приложение

### Последовательная версия
bool TolstunYBukStringCountSEQ::RunImpl() {
  const std::string &stroka = GetInput();

  // Считаем количество букв
  int bukv_count = 0;
  for (char sim : stroka) {
    if (std::isalpha(sim) != 0) {
      bukv_count++;
    }
  }

  GetOutput() = bukv_count;
  return true;
}
### Параллельная версия
bool TolstunYBukStringCountMPI::RunImpl() {
  const std::string &stroka = GetInput();
  const std::size_t size_stroka = stroka.size();

  int rank = 0;
  int size = 1;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (size_stroka == 0) {
    GetOutput() = 0;
    MPI_Barrier(MPI_COMM_WORLD);
    return true;
  }

  std::size_t interval = size_stroka / static_cast<std::size_t>(size);
  std::size_t ostatok = size_stroka % static_cast<std::size_t>(size);

  std::size_t start = (rank * interval) + std::min(static_cast<std::size_t>(rank), ostatok);

  std::size_t end = start + interval;
  if (std::cmp_less(rank, static_cast<int>(ostatok))) {
    end += 1;
  }

  start = std::min(start, size_stroka);
  end = std::min(end, size_stroka);

  start = std::min(start, end);  // защита от пересечения

  int local_count = 0;
  if (start < end) {
    local_count = CountBukv(stroka, start, end);
  }

  int global_count = 0;

  MPI_Reduce(&local_count, &global_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  MPI_Bcast(&global_count, 1, MPI_INT, 0, MPI_COMM_WORLD);

  GetOutput() = global_count;

  MPI_Barrier(MPI_COMM_WORLD);
  return true;
}