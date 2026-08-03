#include "global.h"

std::function<QString(QString)> xorString = [](QString input) {
    QString result = input;
    int length = input.length();
    length = length % 255; //让长度不超过255

    for (int i = 0; i < length; i++) {
        //对每个字符与长度异或
        result[i] = QChar(static_cast<ushort>(input[i].unicode() ^ static_cast<ushort>(length)));
    }

    return result;
};
