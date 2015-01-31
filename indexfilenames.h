#ifndef INDEXFILENAMES_H
#define INDEXFILENAMES_H
#include <QString>
class IndexFileNames
{
public:
    IndexFileNames();

    /* Name of the index segment file */
      static const QString SEGMENTS;

      /* Name of the generation reference file name */
      static const QString SEGMENTS_GEN;

      /* Name of the index deletable file (only used in
       * pre-lockless indices) */
      static const QString DELETABLE;

      /* Extension of norms file */
      static const QString NORMS_EXTENSION;

      /* Extension of compound file */
      static const QString COMPOUND_FILE_EXTENSION;

      /* Extension of deletes */
      static const QString DELETES_EXTENSION;

      /* Extension of plain norms */
      static const QString PLAIN_NORMS_EXTENSION;

      /* Extension of separate norms */
      static const QString SEPARATE_NORMS_EXTENSION;

      static const QString INDEX_EXTENSIONS[14];

      static const QString INDEX_EXTENSIONS_IN_COMPOUND_FILE[11];

      static const QString COMPOUND_EXTENSIONS[7];

      static const QString VECTOR_EXTENSIONS[3];

      static QString fileNameFromGeneration(QString base, QString extension, __int64 gen);
};

#endif // INDEXFILENAMES_H
