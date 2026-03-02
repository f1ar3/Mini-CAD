#include <QtTest>
#include "commands/CommandHistory.h"

class TestCommandHistory : public QObject
{
    Q_OBJECT

private slots:
    void testPushPopUndo();
    void testPushPopRedo();
    void testCanUndoRedo();
    void testClearRedo();
    void testClearAll();
    void testMaxHistory();
    void testDescriptions();
};

static Snapshot makeTestSnapshot(const QString& desc, int nextId = 1)
{
    Snapshot s;
    s.description = desc;
    s.nextId = nextId;
    Snapshot::ShapeData sd;
    sd.id = 1;
    sd.name = "Test";
    sd.type = "Box";
    s.shapes.append(sd);
    return s;
}

void TestCommandHistory::testPushPopUndo()
{
    CommandHistory h;
    Snapshot s = makeTestSnapshot("op1", 5);
    h.pushUndo(s);

    QVERIFY(h.canUndo());
    Snapshot popped = h.popUndo();
    QCOMPARE(popped.description, QString("op1"));
    QCOMPARE(popped.nextId, 5);
    QCOMPARE(popped.shapes.size(), 1);
    QVERIFY(!h.canUndo());
}

void TestCommandHistory::testPushPopRedo()
{
    CommandHistory h;
    Snapshot s = makeTestSnapshot("redo1", 3);
    h.pushRedo(s);

    QVERIFY(h.canRedo());
    Snapshot popped = h.popRedo();
    QCOMPARE(popped.description, QString("redo1"));
    QCOMPARE(popped.nextId, 3);
    QVERIFY(!h.canRedo());
}

void TestCommandHistory::testCanUndoRedo()
{
    CommandHistory h;
    QVERIFY(!h.canUndo());
    QVERIFY(!h.canRedo());

    h.pushUndo(makeTestSnapshot("a"));
    QVERIFY(h.canUndo());
    QVERIFY(!h.canRedo());

    h.pushRedo(makeTestSnapshot("b"));
    QVERIFY(h.canUndo());
    QVERIFY(h.canRedo());
}

void TestCommandHistory::testClearRedo()
{
    CommandHistory h;
    h.pushRedo(makeTestSnapshot("r1"));
    h.pushRedo(makeTestSnapshot("r2"));
    QVERIFY(h.canRedo());

    h.clearRedo();
    QVERIFY(!h.canRedo());
}

void TestCommandHistory::testClearAll()
{
    CommandHistory h;
    h.pushUndo(makeTestSnapshot("u1"));
    h.pushRedo(makeTestSnapshot("r1"));

    h.clearAll();
    QVERIFY(!h.canUndo());
    QVERIFY(!h.canRedo());
}

void TestCommandHistory::testMaxHistory()
{
    CommandHistory h;
    for (int i = 0; i < 55; ++i) {
        h.pushUndo(makeTestSnapshot(QString("op%1").arg(i)));
    }

    // MAX_HISTORY = 50, so oldest entries are dropped
    int count = 0;
    while (h.canUndo()) {
        h.popUndo();
        count++;
    }
    QCOMPARE(count, 50);
}

void TestCommandHistory::testDescriptions()
{
    CommandHistory h;

    // Empty stacks return empty strings
    QCOMPARE(h.undoDescription(), QString());
    QCOMPARE(h.redoDescription(), QString());

    h.pushUndo(makeTestSnapshot("first"));
    h.pushUndo(makeTestSnapshot("second"));
    QCOMPARE(h.undoDescription(), QString("second"));

    h.pushRedo(makeTestSnapshot("redo_first"));
    QCOMPARE(h.redoDescription(), QString("redo_first"));
}

QTEST_MAIN(TestCommandHistory)
#include "tst_CommandHistory.moc"
