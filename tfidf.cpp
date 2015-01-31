#include "tfidf.h"
#include "wvtwordlist.h"
#include "wvtwordvector.h"
#include <cmath>
TFIDF::TFIDF()
{
}
//create Vector weighted by TFIDF
 WVTWordVector* TFIDF::createVector(int* ai, int i, WVTWordList* wvtwordlist, WVTDocumentInfo* wvtdocumentinfo)
 {
     int length = wvtwordlist->wordList.size();
     int j = wvtwordlist->getNumDocuments();
     int* ai1 = wvtwordlist->getDocumentFrequencies();
     WVTWordVector* wvtwordvector = new WVTWordVector();
     double* ad = new double[length];
     if(i > 0)
     {
         double d = 0.0;
         for(int l = 0; l < length; l++)
         {

             double d1 = log((double)j / (double)ai1[l]);
             ad[l] = ((double)ai[l] / (double)i) * d1;
             d += ad[l] * ad[l];
         }

         d = sqrt(d);
         if(d > 0.0)
         {
             for(int i1 = 0; i1 < length; i1++)
                 ad[i1] = ad[i1] / d;

         }
     } else
     {
         for(int k = 0; k < length; k++)
             ad[k] = 0.0;

     }
     wvtwordvector->setDocumentInfo(wvtdocumentinfo);
     wvtwordvector->setValues(ad);
     delete[] ai;
     delete[] ai1;
     return wvtwordvector;
 
 }
