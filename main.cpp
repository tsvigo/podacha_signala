//#include <QCoreApplication>

//int main(int argc, char *argv[])
//{
//    QCoreApplication a(argc, argv);

//    return a.exec();
//}
//###########################################################################
#include <QCoreApplication>
#include <QDataStream>
#include <QFile>
#include <QImage>
#include <iostream>
#include <vector>

// Функция для преобразования пикселей изображения в числа unsigned unsigned long long
std::vector<unsigned long long> imageToNumbers(const QString &fileName)
{
    QImage image;
    if (!image.load(fileName)) {
        std::cerr << "Failed to load image." << std::endl;
        return {};
    }

    std::vector<unsigned long long> numbers;
    numbers.reserve(image.width() * image.height());

    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            QColor pixelColor = image.pixelColor(x, y);
            unsigned long long number = static_cast<unsigned long long>(pixelColor.rgb());
            numbers.push_back(number);
        }
    }

    return numbers;
}

// Функция для чтения чисел из бинарного файла
std::vector<unsigned long long> readNumbersFromFile(const QString &fileName)
{
    std::vector<unsigned long long> numbers;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        std::cerr << "Failed to open file for reading." << std::endl;
        return numbers;
    }

    QDataStream in(&file);
    unsigned long long number;
    while (!in.atEnd()) {
        in >> number;
        numbers.push_back(number);
    }

    file.close();
    return numbers;
}

// Функция для записи чисел в бинарный файл
bool writeNumbersToFile(const std::vector<unsigned long long> &numbers, const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        std::cerr << "Failed to open file for writing." << std::endl;
        return false;
    }

    QDataStream out(&file);
    for (const auto &number : numbers) {
        out << number;
    }

    file.close();
    return true;
}

// Главная функция
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    const QString imageFileName
        = "/home/viktor/Загрузки/data/none/300/masshtab/black-white/1.bmp"; // Имя BMP файла

    const QString binaryFileName
        = "/home/viktor/my_projects_qt_2/sgenerirovaty_long_long_neyroni/random_numbers.bin";
    //"/home/viktor/my_projects_qt_2/generatsiya_neyronov/"
    //               "random_numbers.bin"; // Имя бинарного файла с числами
    const QString outputFileName = "/home/viktor/my_projects_qt_2/podacha_signala/"
                                   "combined_numbers.bin"; // Имя выходного бинарного файла

    // Считываем картинку и преобразуем пиксели в числа
    std::vector<unsigned long long> imageNumbers = imageToNumbers(imageFileName);
    if (imageNumbers.size() != 100) {
        std::cerr << "Image does not have exactly 100 pixels." << std::endl;
        return 1;
    }

    // Читаем числа из бинарного файла
    std::vector<unsigned long long> binaryNumbers = readNumbersFromFile(binaryFileName);
    if (binaryNumbers.size() != 205) {
        std::cerr << "Binary file does not contain exactly 205 numbers." << std::endl;
        return 1;
    }

    // Добавляем недостающие числа к вектору с картинкой
    imageNumbers.resize(205);
    std::copy(binaryNumbers.begin() + 100, binaryNumbers.end(), imageNumbers.begin() + 100);

    // Записываем итоговый вектор в бинарный файл
    if (!writeNumbersToFile(imageNumbers, outputFileName)) {
        std::cerr << "Failed to write combined numbers to file." << std::endl;
        return 1;
    }

    // Проверка правильности записи и вывод чисел в консоль
    std::vector<unsigned long long> resultNumbers = readNumbersFromFile(outputFileName);
    if (resultNumbers != imageNumbers) {
        std::cerr << "Numbers were not written or read correctly." << std::endl;
        return 1;
    }
    // exit(0);
    for (const auto &number : resultNumbers) {
        std::cout << number << std::endl;
    }

    return 0;
}
