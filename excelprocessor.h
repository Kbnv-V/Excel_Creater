#ifndef EXCELPROCESSOR_H
#define EXCELPROCESSOR_H

#include <QString>
#include <QVector>
#include <QAxObject>
#include "corrector.h"

class ExcelProcessor
{
public:
    ExcelProcessor();
    ~ExcelProcessor();

    bool openFile(const QString& filePath);
    void closeFile();

    QVector<QString> readColumn(const QString columnLetter, int startRow, int endRow);
    bool writeColumn(const QString& columnLetter, int startRow, const QVector<QString>& values);

    int applyCorrectionsToColumn(const QString& columnLetter, int startRow, int endRow, Corrector::CorrectionType type, QStringList* changeLog = nullptr);

    bool isOpen() const { return excel != nullptr && workbook != nullptr; }
    QString getLastError() const {return lastError;}

private:
    QAxObject* excel;
    QAxObject* workbooks;
    QAxObject* workbook;
    QAxObject* worksheet;

    QString lastError;

    QString getColumnLetter(int col);
    int getColumnNumber(const QString& letter);
    QAxObject* getCell(int row, int col);
};

#endif // EXCELPROCESSOR_H
