#include <QtTest>
#include "model/Document.h"

class TestRebuildShape : public QObject
{
    Q_OBJECT

private slots:
    void testBuildBox();
    void testBuildCylinder();
    void testBuildSphere();
    void testBuildCone();
    void testBuildTorus();
    void testBuildWedge();
    void testUnknownType();
};

void TestRebuildShape::testBuildBox()
{
    QMap<QString, double> params;
    params["Width (X)"] = 100.0;
    params["Depth (Y)"] = 200.0;
    params["Height (Z)"] = 50.0;

    TopoDS_Shape shape = Document::rebuildShape("Box", params);
    QVERIFY(!shape.IsNull());
    QCOMPARE(shape.ShapeType(), TopAbs_SOLID);
}

void TestRebuildShape::testBuildCylinder()
{
    QMap<QString, double> params;
    params["Radius"] = 30.0;
    params["Height"] = 80.0;

    TopoDS_Shape shape = Document::rebuildShape("Cylinder", params);
    QVERIFY(!shape.IsNull());
    QCOMPARE(shape.ShapeType(), TopAbs_SOLID);
}

void TestRebuildShape::testBuildSphere()
{
    QMap<QString, double> params;
    params["Radius"] = 50.0;

    TopoDS_Shape shape = Document::rebuildShape("Sphere", params);
    QVERIFY(!shape.IsNull());
    QCOMPARE(shape.ShapeType(), TopAbs_SOLID);
}

void TestRebuildShape::testBuildCone()
{
    QMap<QString, double> params;
    params["Bottom Radius"] = 40.0;
    params["Top Radius"] = 10.0;
    params["Height"] = 60.0;

    TopoDS_Shape shape = Document::rebuildShape("Cone", params);
    QVERIFY(!shape.IsNull());
    QCOMPARE(shape.ShapeType(), TopAbs_SOLID);
}

void TestRebuildShape::testBuildTorus()
{
    QMap<QString, double> params;
    params["Major Radius"] = 50.0;
    params["Minor Radius"] = 15.0;

    TopoDS_Shape shape = Document::rebuildShape("Torus", params);
    QVERIFY(!shape.IsNull());
    QCOMPARE(shape.ShapeType(), TopAbs_SOLID);
}

void TestRebuildShape::testBuildWedge()
{
    QMap<QString, double> params;
    params["Width (X)"] = 100.0;
    params["Height (Z)"] = 80.0;
    params["Depth (Y)"] = 60.0;
    params["Top Width (Xmin)"] = 30.0;

    TopoDS_Shape shape = Document::rebuildShape("Wedge", params);
    QVERIFY(!shape.IsNull());
    QCOMPARE(shape.ShapeType(), TopAbs_SOLID);
}

void TestRebuildShape::testUnknownType()
{
    QMap<QString, double> params;
    params["Radius"] = 10.0;

    TopoDS_Shape shape = Document::rebuildShape("Unknown", params);
    QVERIFY(shape.IsNull());
}

QTEST_MAIN(TestRebuildShape)
#include "tst_RebuildShape.moc"
