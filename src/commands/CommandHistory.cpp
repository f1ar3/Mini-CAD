#include "CommandHistory.h"

void CommandHistory::pushUndo(const Snapshot& snapshot)
{
    m_undoStack.append(snapshot);
    if (m_undoStack.size() > MAX_HISTORY) {
        m_undoStack.removeFirst();
    }
}

void CommandHistory::pushRedo(const Snapshot& snapshot)
{
    m_redoStack.append(snapshot);
}

bool CommandHistory::canUndo() const
{
    return !m_undoStack.isEmpty();
}

bool CommandHistory::canRedo() const
{
    return !m_redoStack.isEmpty();
}

Snapshot CommandHistory::popUndo()
{
    return m_undoStack.takeLast();
}

Snapshot CommandHistory::popRedo()
{
    return m_redoStack.takeLast();
}

void CommandHistory::clearRedo()
{
    m_redoStack.clear();
}

void CommandHistory::clearAll()
{
    m_undoStack.clear();
    m_redoStack.clear();
}

QString CommandHistory::undoDescription() const
{
    if (m_undoStack.isEmpty()) return {};
    return m_undoStack.last().description;
}

QString CommandHistory::redoDescription() const
{
    if (m_redoStack.isEmpty()) return {};
    return m_redoStack.last().description;
}
