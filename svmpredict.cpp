#include "svmpredict.h"
#include "wvtool.h"
#include "wvtwordlist.h"
#include "stdio.h"
#include <ctype.h>
#include "stdlib.h"
#include <string>
#include <errno.h>
#include "svm.h"
#include <QStringList>
#include <iostream>
using namespace std;
struct svm_model;
SvmPredict::SvmPredict()
{
     wordList1 = new WVTWordList("data/wvlist1.txt");
     wordList2 = new WVTWordList("data/wvlist2.txt");
     wordList3 = new WVTWordList("data/wvlist3.txt");
     model1 = svm_load_model("data/wv1.txt.model");
     model2 = svm_load_model("data/wv2.txt.model");
     model3 = svm_load_model("data/wv3.txt.model");
}
//
int SvmPredict::predict(const QString &fileValue)
{
    double r1 = predict(fileValue,this->wordList1,this->model1);
    double r2 = predict(fileValue,this->wordList2,this->model2);
    double r3 = predict(fileValue,this->wordList3,this->model3);
    if(r1 > 0)
    {
        return 1;
    }
    else
    {
        if(r2 > 0)
            return 2;
        else
        {
            if(r3 > 0)
                return 3;
            else
                return -1;
        }
    }
}
//
double SvmPredict::predict(const QString& fileValue
                           ,WVTWordList* wordList,svm_model* model)
{
     WVTool wvt;
     QString vector =  wvt.createVector(fileValue,wordList);
      QStringList list = vector.split(" ");
      int size = list.size() - 1;
       QListIterator<QString> iterator(list);
       double target = iterator.next().toDouble();
        svm_node *x = new svm_node[size+1];
       // svm_model* model = svm_load_model("../data/wv1.txt.model");

       for(int i = 0;i < size;i++)
       {
           QString inVa = iterator.next();
           QStringList list1 = inVa.split(":");
           int j = 0;
           foreach (QString str, list1)
           {

                if(j == 0)
                    x[i].index = str.toInt();
                else
                {
                    string aa =  str.toStdString();
                    x[i].value = atof(aa.c_str());
                }

                j++;
           }
       }
       x[size].index = -1;
     double result =  svm_predict(model,x);
     return result;
}
