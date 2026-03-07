#include <QtTest>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>
#include <BRepGProp.hxx>
#include <GProp_GProps.hxx>
#include <Bnd_Box.hxx>
#include <BRepBndLib.hxx>

class TestMeasure : public QObject
{
    Q_OBJECT

private slots:
    void testBoxVolume();
    void testBoxSurfaceArea();
    void testBoxBoundingBox();
    void testSphereVolume();
};

void TestMeasure::testBoxVolume()
{
    TopoDS_Shape box = BRepPrimAPI_MakeBox(100.0, 200.0, 50.0).Shape();
    GProp_GProps props;
    BRepGProp::VolumeProperties(box, props);
    // Volume = 100 * 200 * 50 = 1000000
    QVERIFY(qAbs(props.Mass() - 1000000.0) < 1.0);
}

void TestMeasure::testBoxSurfaceArea()
{
    TopoDS_Shape box = BRepPrimAPI_MakeBox(100.0, 200.0, 50.0).Shape();
    GProp_GProps props;
    BRepGProp::SurfaceProperties(box, props);
    // Area = 2*(100*200 + 100*50 + 200*50) = 2*(20000+5000+10000) = 70000
    QVERIFY(qAbs(props.Mass() - 70000.0) < 1.0);
}

void TestMeasure::testBoxBoundingBox()
{
    TopoDS_Shape box = BRepPrimAPI_MakeBox(100.0, 200.0, 50.0).Shape();
    Bnd_Box bbox;
    BRepBndLib::Add(box, bbox);
    QVERIFY(!bbox.IsVoid());

    double xmin, ymin, zmin, xmax, ymax, zmax;
    bbox.Get(xmin, ymin, zmin, xmax, ymax, zmax);

    QVERIFY(qAbs(xmax - xmin - 100.0) < 1.0);
    QVERIFY(qAbs(ymax - ymin - 200.0) < 1.0);
    QVERIFY(qAbs(zmax - zmin - 50.0) < 1.0);
}

void TestMeasure::testSphereVolume()
{
    double radius = 50.0;
    TopoDS_Shape sphere = BRepPrimAPI_MakeSphere(radius).Shape();
    GProp_GProps props;
    BRepGProp::VolumeProperties(sphere, props);
    // Volume = 4/3 * pi * r^3
    double expected = (4.0 / 3.0) * M_PI * radius * radius * radius;
    QVERIFY(qAbs(props.Mass() - expected) < 10.0);
}

QTEST_MAIN(TestMeasure)
#include "tst_Measure.moc"
