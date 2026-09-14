#include "excelprocessor.h"

#include <QRegularExpression>
#include <QDir>
#include <QDebug>
#include <QStringList>

ExcelProcessor::ExcelProcessor() : excel(nullptr), workbooks(nullptr), workbook(nullptr), worksheet(nullptr) //это присваивание после создания объекта
{

}

ExcelProcessor::~ExcelProcessor()
{
    closeFile();
}

// Открываем файл excel
bool ExcelProcessor::openFile(const QString& filePath)
{
    closeFile();
    lastError.clear();

    excel = new QAxObject("Excel.Application");
    if (!excel || excel->isNull())
    {
        lastError = "Не удалось запустить Excel. Убедитесь, что Excel установлен.";
        delete excel;
        excel = nullptr;
        return false;
    }

    excel->setProperty("Visible", false);
    excel->setProperty("DisplayAlerts", false);

    // Получаем открытые файлы (должен быть пустой список)
    workbooks = excel->querySubObject("Workbooks");
    if (!workbooks)
    {
        lastError = "Не удалось получить доступ к рабочим книгам";
        closeFile();
        return false;
    }

    //преобразуем путь к файлу в понятный формат для WIndows
    QString nativePath = QDir::toNativeSeparators(filePath);

    // Открываем нужный файл
    workbook = workbooks->querySubObject("Open(const QString&)", nativePath);
    if (!workbook)
    {
        lastError = QString("Не удалось открыть файл: %1").arg(filePath);
        closeFile();
        return false;
    }

    // Получаем активный лист (первый лист)
    worksheet = workbook->querySubObject("Worksheets(int)", 1);
    if (!worksheet)
    {
        worksheet = workbook->querySubObject("ActiveSheet");
    }
    if (!worksheet)
    {
        lastError = "Не удалось получить лист Excel";
        closeFile();
        return false;
    }

    return true;
}

//закрытие файла
void ExcelProcessor::closeFile()
{
    if(worksheet)
    {
        delete worksheet;
        worksheet = nullptr;
    }

    if(workbook)
    {
        workbook->dynamicCall("Close(Bool)", false);
        delete workbook;
        workbook = nullptr;
    }

    if (workbooks)
    {
        delete workbooks;
        workbooks = nullptr;
    }

    if (excel)
    {
        excel->dynamicCall("Quit()");
        delete excel;
        excel = nullptr;
    }
}

//чтение строк указанного столбца в массив
QVector<QString> ExcelProcessor::readColumn(const QString columnLetter, int startRow, int endRow)
{
    QVector<QString> result; //объявляем пустой массив

    //проверка открытия файла
    if (!isOpen())
    {
        lastError = "Excel файл не открыт";
        return result;
    }

    QString normalizedColumn = columnLetter.trimmed().toUpper(); //приводим значение к нужному виду
    int col = getColumnNumber(normalizedColumn); //преобразуем букву в цифру

    //проверяем корректность данных
    if(col <= 0 || startRow <= 0 || endRow <= 0 || startRow > endRow)
    {
        lastError = "Неверно указан столбец или диапазон строк";
        return result;
    }

    for(int row = startRow; row <= endRow; row++)
    {
        QAxObject* cell = getCell(row, col);

        //добавляем полученные значения из файла в массив
        if (cell)
        {
            result.append(cell->property("Value").toString());
            delete cell;
        }
        else
        {
            result.append("null");
        }
    }

    return result;
};

//обработка полученных данных
int ExcelProcessor::applyCorrectionsToColumn(const QString& columnLetter, int startRow, int endRow, Corrector::CorrectionType type, QStringList* changeLog)
{
    //проверка открытия файла
    if (!isOpen())
    {
        lastError = "Excel файл не открыт";
        return -1;
    }

    QString normalizedColumn = columnLetter.trimmed().toUpper(); //приводим значение к нужному виду

    //читаем столбец и записываем в массив
    QVector<QString> values = readColumn(normalizedColumn, startRow, endRow);

    //проверяем заполнение массива
    if (values.isEmpty())
    {
        lastError = "Не удалось прочитать данные из столбца";
        return -1;
    }

    Corrector corrector; //объявляем класс
    int modifiedCells = 0;

    for(int i = 0; i < values.size(); ++i)
    {
        QString oldValue = values[i];

        if (oldValue == "null")
        {
            continue;
        }

        Corrector::CorrectionResult result = corrector.correct(oldValue, type); //исправляем значение

        if(result.correctionsCount > 0 && result.CorrectedText != oldValue)
        {
            values[i] = result.CorrectedText; //записываем исправленный вариант
            modifiedCells++;

            if(changeLog)
            {
                int row = startRow + i;
                QString cellAddress = normalizedColumn + QString::number(row); //формируем запись типа "А1"

                //добавляем в лог
                changeLog->append(QString("%1: \"%2\"").arg(cellAddress, result.CorrectedText));
            }
        }
    }

    if (modifiedCells > 0)
    {
        if (!writeColumn(normalizedColumn, startRow, values))
        {
            return -1;
        }

        workbook->dynamicCall("Save()");
    }

    return modifiedCells;
};


//запись исправленных значений в файл
 bool ExcelProcessor::writeColumn(const QString& columnLetter, int startRow, const QVector<QString>& values)
{
    //проверка открытия файла
    if (!isOpen())
    {
        lastError = "Excel файл не открыт";
        return false;
    }

    QString normalizedColumn = columnLetter.trimmed().toUpper(); //приводим значение к нужному виду
    int col = getColumnNumber(normalizedColumn); //преобразуем букву в цифру

    //проверяем корректность данных
    if(col <= 0 || startRow <= 0)
    {
        lastError = "Неверно указан столбец или диапазон строк";
        return false;
    }

    for(int i = 0; i < values.size(); ++i)
    {
        int row = startRow + i;
        QAxObject* cell = getCell(row, col);

        if(!cell)
        {
            lastError = QString("Не удалось получить ячейку %1%2").arg(normalizedColumn).arg(row);
            return false;
        }

        //запись значения в ячейку файла
        cell->setProperty("Value", values[i]);
    }

    return true;
}

//преобразует номер колонки (1,2,3) в буквенный адрес (A,B,C)
QString ExcelProcessor::getColumnLetter(int col)
{
    QString result;
    while (col > 0) {
        col--;  // Переходим к 0-индексации (A=0, B=1, ...)
        result.prepend(QChar('A' + (col % 26)));  // Добавляем букву в начало строки
        col /= 26;  // Переходим к следующему разряду (для AA, AB и т.д.)
    }
    return result;
}

//преобразует буквенный адрес колонки (A, B) в номер (1, 2, 3)
int ExcelProcessor::getColumnNumber(const QString& letter)
{
    int result = 0;
    for (QChar ch : letter) {
        // Каждая буква дает вклад: A=1, B=2, ... Z=26
        // Для "AB": сначала A=1, потом B = 1*26 + 2 = 28
        result = result * 26 + (ch.toUpper().toLatin1() - 'A' + 1);
    }
    return result;
}

//получает COM-объект ячейки по ее координатам
QAxObject* ExcelProcessor::getCell(int row, int col)
{
    if (!worksheet)
    {
        return nullptr;
    }

    //формируем адрес ячейки "A1", "B15"
    QString cellRef = getColumnLetter(col) + QString::number(row);
    //запрашиваем у Excel объект этой ячейки
    return worksheet->querySubObject("Range(const QString&)", cellRef);
}