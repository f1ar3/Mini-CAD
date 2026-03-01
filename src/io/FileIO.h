#ifndef FILEIO_H
#define FILEIO_H

#include <QString>
#include <TopoDS_Shape.hxx>

class FileIO
{
public:
    static TopoDS_Shape importSTEP(const QString& path);
    static TopoDS_Shape importIGES(const QString& path);
    static TopoDS_Shape importBRep(const QString& path);

    static bool exportSTEP(const TopoDS_Shape& shape, const QString& path);
    static bool exportIGES(const TopoDS_Shape& shape, const QString& path);
    static bool exportBRep(const TopoDS_Shape& shape, const QString& path);

    static QString supportedImportFormats();
    static QString supportedExportFormats();
};

#endif // FILEIO_H
