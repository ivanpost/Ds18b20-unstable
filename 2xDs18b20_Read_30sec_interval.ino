/*

Дополнить предыдущий пример - если не удалось считать данные 10 раз подряд - считать температуру равной 20°, а до этого считать последнее удачное считывание.

Хорошо, я дополню предыдущий пример, чтобы при неудачном считывании данных 10 раз подряд,
температура считалась равной 20°, а до этого считывалось последнее удачное значение.
чтобы учитывать два датчика DS18B20, подключенных к одному пину, и применять условие с 10 попытками
для каждого датчика. Вот обновленный скетч:*/

#include <OneWire.h>
#include <DallasTemperature.h>

// Подключение датчиков DS18B20 к пину 2
#define ONE_WIRE_BUS 12

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

unsigned long previousMillis = 0;   // Время предыдущего считывания
const unsigned long interval = 30000; // Интервал считывания (30 секунд)

float lastTemperature1 = 20.0; // Последнее удачное считанное значение температуры первого датчика
float lastTemperature2 = 20.0; // Последнее удачное считанное значение температуры второго датчика
int consecutiveFailures1 = 0; // Количество подряд неудачных считываний первого датчика
int consecutiveFailures2 = 0; // Количество подряд неудачных считываний второго датчика

void setup() {
  Serial.begin(9600);

  // Поиск и инициализация датчиков
  sensors.begin();

  // Определение количества найденных датчиков
  int numberOfSensors = sensors.getDeviceCount();
  Serial.print("Number of sensors found: ");
  Serial.println(numberOfSensors);
}

void loop() {
  unsigned long currentMillis = millis();

  // Проверка, прошло ли достаточно времени для считывания
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;  // Обновляем время предыдущего считывания

    sensors.requestTemperatures(); // Запрос температуры с датчиков

    // Чтение значения температуры первого датчика
    float temperature1 = sensors.getTempCByIndex(0);

    if (temperature1 != DEVICE_DISCONNECTED_C) {
      // Успешное считывание температуры первого датчика
      lastTemperature1 = temperature1;
      consecutiveFailures1 = 0;
    } else {
      // Не удалось считать температуру первого датчика
      consecutiveFailures1++;
    }

    // Чтение значения температуры второго датчика
    float temperature2 = sensors.getTempCByIndex(1);

    if (temperature2 != DEVICE_DISCONNECTED_C) {
      // Успешное считывание температуры второго датчика
      lastTemperature2 = temperature2;
      consecutiveFailures2 = 0;
    } else {
      // Не удалось считать температуру второго датчика
      consecutiveFailures2++;
    }

    // Проверка на количество подряд неудачных считываний первого датчика
    if (consecutiveFailures1 >= 10) {
      // Если не удалось считать 10 раз подряд, считаем температуру равной 20°
      temperature1 = 20.0;
    } else {
      // Иначе используем последнее удачное считанное значение
      temperature1 = lastTemperature1;
    }

    // Проверка на количество подряд неудачных считываний второго датчика
    if (consecutiveFailures2 >= 10) {
      // Если не удалось считать 10 раз подряд, считаем температуру равной 20°
      temperature2 = 20.0;
    } else {
      // Иначе используем последнее удачное считанное значение
      temperature2 = lastTemperature2;
    }

    // Вывод значений температур
    Serial.print("Temperature 1: ");
    Serial.println(temperature1);
    Serial.print("Temperature 2: ");
    Serial.println(temperature2);
  }
}


/*В этом обновленном коде добавлены две переменные: `lastTemperature` (последнее удачное считанное значение температуры)
и `consecutiveFailures` (количество подряд неудачных считываний). Если не удалось считать температуру,
увеличиваем `consecutiveFailures`. Если `consecutiveFailures` достигает значения 10, то считаем температуру равной 20°.
В противном случае используем последнее удачное значение из `lastTemperature`.*/
