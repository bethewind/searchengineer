#ifndef SVMPREDICT_H
#define SVMPREDICT_H
#include <QString>

class WVTWordList;
struct svm_model;
class SvmPredict
{
public:
    SvmPredict();
    int predict(const QString& fileValue);
    double predict(const QString& fileValue
                   ,WVTWordList* wordList,svm_model* model);
     WVTWordList* wordList1;
     WVTWordList* wordList2;
     WVTWordList* wordList3;
     svm_model* model1;
     svm_model* model2;
     svm_model* model3;
};

#endif // SVMPREDICT_H
