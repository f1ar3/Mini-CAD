#include <QtTest>
#include <QTemporaryDir>
#include "io/FileIO.h"

#include <BRepPrimAPI_MakeBox.hxx>

class TestFileIO : public QObject
{
    Q_OBJECT

private:
    TopoDS_Shape makeTestBox();

private slots:
    void testExportImportSTEP();
    void testExportImportBRep();
    void testExportImportIGES();
};

TopoDS_Shape TestFileIO::makeTestBox()
{
    return BRepPrimAPI_MakeBox(100.0, 200.0, 50.0).Shape();
}

void TestFileIO::testExportImportSTEP()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    QString path = dir.path() + "/test.step";

    TopoDS_Shape box = makeTestBox();
    QVERIFY(!box.IsNull());

    bool ok = FileIO::exportSTEP(box, path);
    QVERIFY(ok);
    QVERIFY(QFile::exists(path));

    TopoDS_Shape imported = FileIO::importSTEP(path);
    QVERIFY(!imported.IsNull());
}

void TestFileIO::testExportImportBRep()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    QString path = dir.path() + "/test.brep";

    TopoDS_Shape box = makeTestBox();
    bool ok = FileIO::exportBRep(box, path);
    QVERIFY(ok);
    QVERIFY(QFile::exists(path));

    TopoDS_Shape imported = FileIO::importBRep(path);
    QVERIFY(!imported.IsNull());
}

void TestFileIO::testExportImportIGES()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    QString path = dir.path() + "/test.iges";

    TopoDS_Shape box = makeTestBox();
    bool ok = FileIO::exportIGES(box, path);
    QVERIFY(ok);
    QVERIFY(QFile::exists(path));

    TopoDS_Shape imported = FileIO::importIGES(path);
    QVERIFY(!imported.IsNull());
}

QTEST_MAIN(TestFileIO)
#include "tst_FileIO.moc"
