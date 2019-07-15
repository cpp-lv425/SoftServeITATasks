#include <iostream>
#include "filestat.h"
#include <QDir>
#include <thread>
#include <QThreadPool>

QFileInfoList recursiveSearch(QString project_path) {
    QDir project(project_path);
    QFileInfoList elements(project.entryInfoList());
    QFileInfoList files;
    for(auto it = elements.begin(); it < elements.end(); ++it)
        if(it->isFile()) files.push_back(it->filePath());
        else if (it->isDir() && it->fileName() != "." && it->fileName() != "..") files += recursiveSearch(it->filePath());
    return files;
}

void worker(FileStat* fileStat){
    fileStat->fileStat();
}

class Task : public QRunnable{
private:
    FileStat* fileStat;
public:
    Task(FileStat* fileStat): fileStat(fileStat){}
    void run(){
        fileStat->fileStat();
    }
};

int main(int argc, char *argv[])
{
    QFileInfoList files = recursiveSearch("C:\\Project");
    QVector<QString> path;
    for(auto it = files.begin(); it < files.end(); ++it) {
        QString extension = *(it->fileName().split('.').end() - 1);
        if (extension == "cpp" || extension == "c" || extension == "hpp" || extension == "h")
            path.push_back(it->filePath());
    }

    QThreadPool* pool = QThreadPool::globalInstance();
    QVector<FileStat*> vec;

    for(auto it : path) {
        FileStat *fileStat = new FileStat(it);
        vec.push_back(fileStat);
        pool->start(new Task(fileStat));
    }

    pool->waitForDone();

    unsigned int blankLinesInProject = 0;
    unsigned int commentLinesInProject = 0;
    unsigned int linesWithCodeInProject = 0;
    for(auto it : vec) {
        blankLinesInProject += it->getBlankLinesCount();
        commentLinesInProject += it->getCommentLinesCount();
        linesWithCodeInProject += it->getWithCodeLinesCount();
        delete it;
    }

    std::cout << "Blank lines in project: " << blankLinesInProject << std::endl;
    std::cout << "Comment lines in project: " << commentLinesInProject << std::endl;
    std::cout << "Lines with code in project: " << linesWithCodeInProject << std::endl;
}
