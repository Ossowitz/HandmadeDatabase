# Используем базовый образ с Linux, содержащий компилятор C и все необходимые инструменты
FROM gcc:latest

# Копируем файлы программы в контейнер
COPY . /app

# Устанавливаем рабочую директорию
WORKDIR /app

# Компилируем программу
RUN gcc -o db db.c

# Запускаем программу
CMD ["./db"]