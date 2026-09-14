#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>
#include <QRegularExpression>
#include "ExcelProcessor.h"
#include "Corrector.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_select_file_clicked()
{
    //открываем диалог выбора файла
    QString fileName = QFileDialog::getOpenFileName(this, "Выберите Excel файл", "", "Excel файлы (*.xlsx *.xls);;Все файлы (*)");

    //если файл выбран (не пустая строка)
    if (!fileName.isEmpty()) {
        //записываем путь в поле lineEdit
        ui->patch_file->setText(fileName);

        //для отладки
        qDebug() << "Выбран файл:" << fileName;
    }
}

void MainWindow::on_start_clicked()
{
    ui->logs->clear(); //чистим поле с логами

    //проверяем выбран ли файл
    if(ui->patch_file->text().isEmpty())
    {
        ui->logs->append("ОШИБКА: Не выбран файл для редактирования!");
        return;
    }

    int startRow = ui->start_row->value();
    int endRow = ui->end_row->value();
    QString col = ui->column->text().trimmed().toUpper();

    //проверяем заполнение диапазона строк
    if(startRow == 0 || endRow == 0 || startRow > endRow)
    {
        ui->logs->append("ОШИБКА: Некорректно указан диапазон строк!");
        ui->logs->append("ПОДСКАЗКА: начальная сторка - 1 и конечная строка - 100");
        return;
    }

    //проверяем поле для колонки на русские буквы
    QRegularExpression russianLetters("[А-Яа-я]");
    if(col.contains(russianLetters))
    {
        ui->logs->append("ОШИБКА: В указанном диапазоне используются символы русского регистра!");
        ui->logs->append("ПОДСКАЗКА: Используйте символы английского регистра при указании колонки");
        return;
    }

    //проверяем поле для колонки на наличие цифр
    QRegularExpression digits("[0-9]");
    if(col.contains(digits))
    {
        ui->logs->append("ОШИБКА: Колонка не может содержать цифры!");
        ui->logs->append("ПОДСКАЗКА: Вводите адрес колонки в следующем формате: A, B, C");
        return;
    }

    //проверка на заполнение колонки
    if(col.isEmpty())
    {
        ui->logs->append("ОШИБКА: Колонка не заполнена!");
        ui->logs->append("ПОДСКАЗКА: Заполните значение для колонки и повторите запуск");
        return;
    }

    ui->logs->append("Начало обработки...");

    //определяем тип исправления (0 - EMAIL, 1 - PHONE)
    Corrector::CorrectionType type;
    if (ui->list->currentIndex() == 0) {
        type = Corrector::EMAIL;
        ui->logs->append("Тип: исправление email-адресов");
    } else {
        type = Corrector::PHONE;
        ui->logs->append("Тип: исправление номеров телефонов");
    }

    //создаем объект для работы с Excel
    ExcelProcessor processor;

    ui->logs->append("Открытие файла...");

    //открываем файл
    if (!processor.openFile(ui->patch_file->text())) {
        ui->logs->append("ОШИБКА: " + processor.getLastError());
        return;
    }

    ui->logs->append("Обработка данных...");

    QStringList changeLog; //объявляем объект для передачи логов

    // Применяем исправления
    int result = processor.applyCorrectionsToColumn(col, startRow, endRow, type, &changeLog);

    // Закрываем файл
    processor.closeFile();

    // Выводим результат
    ui->logs->append("----------------------------------------");

    if (result < 0)
    {
        ui->logs->append("ОШИБКА: " + processor.getLastError());
    }
    else if (result == 0)
    {
        ui->logs->append("Изменений не обнаружено.");
    }
    else
    {
        ui->logs->append("ГОТОВО! Исправлено ячеек: " + QString::number(result));

        for (const QString& logLine : changeLog)
        {
            ui->logs->append(logLine);
        }
    }

    ui->logs->append("----------------------------------------");
}
