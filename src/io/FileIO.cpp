#include "FileIO.h"

#include <STEPControl_Reader.hxx>
#include <STEPControl_Writer.hxx>
#include <IGESControl_Reader.hxx>
#include <IGESControl_Writer.hxx>
#include <BRepTools.hxx>
#include <BRep_Builder.hxx>

#include <QObject>
#include <stdexcept>

TopoDS_Shape FileIO::importSTEP(const QString& path)
{
    STEPControl_Reader reader;
    IFSelect_ReturnStatus status = reader.ReadFile(path.toUtf8().constData());
    if (status != IFSelect_RetDone) {
        throw std::runtime_error("Не удалось прочитать STEP файл");
    }
    reader.TransferRoots();
    return reader.OneShape();
}

TopoDS_Shape FileIO::importIGES(const QString& path)
{
    IGESControl_Reader reader;
    int status = reader.ReadFile(path.toUtf8().constData());
    if (status != IFSelect_RetDone) {
        throw std::runtime_error("Не удалось прочитать IGES файл");
    }
    reader.TransferRoots();
    return reader.OneShape();
}

TopoDS_Shape FileIO::importBRep(const QString& path)
{
    TopoDS_Shape shape;
    BRep_Builder builder;
    if (!BRepTools::Read(shape, path.toUtf8().constData(), builder)) {
        throw std::runtime_error("Не удалось прочитать BRep файл");
    }
    return shape;
}

bool FileIO::exportSTEP(const TopoDS_Shape& shape, const QString& path)
{
    STEPControl_Writer writer;
    IFSelect_ReturnStatus status = writer.Transfer(shape, STEPControl_AsIs);
    if (status != IFSelect_RetDone) return false;
    return writer.Write(path.toUtf8().constData()) == IFSelect_RetDone;
}

bool FileIO::exportIGES(const TopoDS_Shape& shape, const QString& path)
{
    IGESControl_Writer writer;
    writer.AddShape(shape);
    writer.ComputeModel();
    return writer.Write(path.toUtf8().constData()) != 0;
}

bool FileIO::exportBRep(const TopoDS_Shape& shape, const QString& path)
{
    return BRepTools::Write(shape, path.toUtf8().constData());
}

QString FileIO::supportedImportFormats()
{
    return QObject::tr("Все поддерживаемые (*.step *.stp *.iges *.igs *.brep *.brp);;"
                       "Файлы STEP (*.step *.stp);;"
                       "Файлы IGES (*.iges *.igs);;"
                       "Файлы BRep (*.brep *.brp);;"
                       "Все файлы (*)");
}

QString FileIO::supportedExportFormats()
{
    return QObject::tr("Файлы STEP (*.step *.stp);;"
                       "Файлы IGES (*.iges *.igs);;"
                       "Файлы BRep (*.brep *.brp)");
}
