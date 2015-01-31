#include "indexfilenames.h"
#include "segmentinfo.h"
/* Name of the index segment file */
const QString IndexFileNames::SEGMENTS = "segments";

  /* Name of the generation reference file name */
  const QString IndexFileNames::SEGMENTS_GEN = "segments.gen";

  /* Name of the index deletable file (only used in
   * pre-lockless indices) */
  const QString IndexFileNames::DELETABLE = "deletable";

  /* Extension of norms file */
  const QString IndexFileNames::NORMS_EXTENSION = "nrm";

  /* Extension of compound file */
  const QString IndexFileNames::COMPOUND_FILE_EXTENSION = "cfs";

  /* Extension of deletes */
  const QString IndexFileNames::DELETES_EXTENSION = "del";

  /* Extension of plain norms */
  const QString IndexFileNames::PLAIN_NORMS_EXTENSION = "f";

  /* Extension of separate norms */
  const QString IndexFileNames::SEPARATE_NORMS_EXTENSION = "s";

  const QString IndexFileNames::INDEX_EXTENSIONS[14] = {
      "cfs", "fnm", "fdx", "fdt", "tii", "tis", "frq", "prx", "del",
      "tvx", "tvd", "tvf", "gen", "nrm"
  };

  const QString IndexFileNames::INDEX_EXTENSIONS_IN_COMPOUND_FILE[11] =  {
      "fnm", "fdx", "fdt", "tii", "tis", "frq", "prx",
      "tvx", "tvd", "tvf", "nrm"
  };

  const QString IndexFileNames::COMPOUND_EXTENSIONS[7] =  {
    "fnm", "frq", "prx", "fdx", "fdt", "tii", "tis"
  };

  const QString IndexFileNames::VECTOR_EXTENSIONS[3] = {
    "tvx", "tvd", "tvf"
  };
IndexFileNames::IndexFileNames()
{
}
//
QString IndexFileNames::fileNameFromGeneration(QString base, QString extension, __int64 gen)
{
    if (gen == SegmentInfo::NO) {
      return "";
  } else if (gen == SegmentInfo::WITHOUT_GEN) {
      return base + extension;
    } else {
      return base + "_" + QString().setNum(gen,36) + extension;
    }
  }
