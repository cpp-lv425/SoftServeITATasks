#include "filestat.h"

void FileStat::fileStat() {
    QFile file(filename);
    file.open(QIODevice::ReadOnly);
    QString line;

    while(!file.atEnd()) {
        bool commentLineWasAdded = false;
        bool lineWithCodeWasAdded = false;
        exitFromSquare();
        line = file.readLine();
        //Remove all tabs and spaces and if line is empty - increment counter if blank lines
        QVector<char> empty_symbols = {'\t', ' ', '\r', '\n'};
        for(auto it = empty_symbols.begin(); it < empty_symbols.end(); ++it)
            line = line.remove(*it);

        if(line.isEmpty()) ++blankLinesCount;
        for (auto i = line.begin(); i < line.end(); ++i) {
            //If we found construction which open multiline comment (/*)
            if (*i == '/' && *(i + 1) == '*') {
                //If /* doesn't in square and already doesn't in comment-block
                if (!lineInSquare() && !lineInCommentBlock()) {
                    enterToCommentBlock();
                    if(!commentLineWasAdded) ++commentLinesCount;
                    commentLineWasAdded = true;
                    //Next symbol is "*". Has no sense to check it. Go to the next symbol.
                    ++i;
                }
            } else if (*i == '*' && *(i + 1) == '/') {
                //If */ doesn't in square and current line in comment-block
                if (!lineInSquare() && lineInCommentBlock()) {
                    exitFromCommentBlock();
                    if(!commentLineWasAdded) ++commentLinesCount;
                    commentLineWasAdded = true;
                    //Next symbol is "/". Has no sense to check it. Go to the next symbol.
                    ++i;
                }
            } else if (*i == '"') {
                if (!lineInCommentBlock() && !lineInSquare()) enterToSquare();
                else if (!lineInCommentBlock() && lineInSquare()) exitFromSquare();
            } else if (*i == '/' && *(i + 1) == '/' && !lineInSquare()) {
                if(!commentLineWasAdded) ++commentLinesCount;
                break;
            } else if (lineInCommentBlock() && !commentLineWasAdded) {
                ++commentLinesCount;
                commentLineWasAdded = true;
            } else if (!lineInCommentBlock() && !lineWithCodeWasAdded) {
                ++withCodeLinesCount;
                lineWithCodeWasAdded = true;
            }
        }
    }
}
