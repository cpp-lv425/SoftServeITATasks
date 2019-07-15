#ifndef FILESTAT_H
#define FILESTAT_H

#include <QString>
#include <QFile>


class FileStat {
private:
    QString filename;
    unsigned int commentLinesCount;
    unsigned int blankLinesCount;
    unsigned int withCodeLinesCount;
    bool inCommentBlock;
    bool inSquare;
public:
    FileStat(QString _filename): filename(_filename), commentLinesCount(0), blankLinesCount(0),
                                 withCodeLinesCount(0), inCommentBlock(false), inSquare(false) {}

public slots:
    void fileStat();

    inline unsigned int getCommentLinesCount() {
        return commentLinesCount;
    }

    inline unsigned int getBlankLinesCount() {
        return blankLinesCount;
    }

    inline unsigned int getWithCodeLinesCount() {
        return withCodeLinesCount;
    }

    inline void enterToCommentBlock() {
        inCommentBlock = true;
    }

    inline void exitFromCommentBlock() {
        inCommentBlock = false;
    }

    inline bool lineInCommentBlock() {
        return inCommentBlock;
    }

    inline void enterToSquare() {
        inSquare = true;
    }

    inline void exitFromSquare() {
        inSquare = false;
    }

    inline bool lineInSquare() {
        return inSquare;
    }
};

#endif // FILESTAT_H
