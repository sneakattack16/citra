// Copyright 2016 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include <QAbstractListModel>
#include <QListView>
#include <QDockWidget>

#include "core/hle/kernel/thread.h"
#include "core/hle/kernel/event.h"
#include "core/hle/kernel/semaphore.h"
#include "core/hle/kernel/mutex.h"
#include "core/hle/kernel/session.h"
#include "core/hle/kernel/timer.h"

class EmuThread;

class WaitTreeItem : public QObject {
    Q_OBJECT
public:
    virtual bool IsExpandable() const;
    virtual QString GetText() const = 0;
    virtual ~WaitTreeItem();
};

class WaitTreeText : public WaitTreeItem {
    Q_OBJECT
public:
    WaitTreeText(const QString& text);
    QString GetText() const override;
private:
    QString text;
};

class WaitTreeExpandableItem : public WaitTreeItem {
    Q_OBJECT
public:
    bool IsExpandable() const override;
    virtual std::vector<std::unique_ptr<WaitTreeItem>> GetChildren() const = 0;
};

class WaitTreeWaitObject : public WaitTreeExpandableItem {
    Q_OBJECT
public:
    WaitTreeWaitObject(const Kernel::WaitObject& object);
    static std::unique_ptr<WaitTreeWaitObject> make(const Kernel::WaitObject& object);
    QString GetText() const override;
    std::vector<std::unique_ptr<WaitTreeItem>> GetChildren() const override;
protected:
    const Kernel::WaitObject& object;
};

class WaitTreeObjectList : public WaitTreeExpandableItem {
    Q_OBJECT
public:
    WaitTreeObjectList(const std::vector<Kernel::SharedPtr<Kernel::WaitObject>>& list, bool wait_all);
    QString GetText() const override;
    std::vector<std::unique_ptr<WaitTreeItem>> GetChildren() const override;
private:
    const std::vector<Kernel::SharedPtr<Kernel::WaitObject>>& object_list;
    bool wait_all;
};

class WaitTreeThread : public WaitTreeWaitObject {
    Q_OBJECT
public:
    WaitTreeThread(const Kernel::Thread& thread);
    QString GetText() const override;
    std::vector<std::unique_ptr<WaitTreeItem>> GetChildren() const override;
};

class WaitTreeEvent : public WaitTreeWaitObject {
    Q_OBJECT
public:
    WaitTreeEvent(const Kernel::Event& object);
    std::vector<std::unique_ptr<WaitTreeItem>> GetChildren() const override;
};

class WaitTreeMutex : public WaitTreeWaitObject {
    Q_OBJECT
public:
    WaitTreeMutex(const Kernel::Mutex& object);
    std::vector<std::unique_ptr<WaitTreeItem>> GetChildren() const override;
};

class WaitTreeSemaphore : public WaitTreeWaitObject {
    Q_OBJECT
public:
    WaitTreeSemaphore(const Kernel::Semaphore& object);
    std::vector<std::unique_ptr<WaitTreeItem>> GetChildren() const override;
};

class WaitTreeTimer : public WaitTreeWaitObject {
    Q_OBJECT
public:
    WaitTreeTimer(const Kernel::Timer& object);
    std::vector<std::unique_ptr<WaitTreeItem>> GetChildren() const override;
};

class WaitTreeMutexList : public WaitTreeExpandableItem {
    Q_OBJECT
public:
    WaitTreeMutexList(const boost::container::flat_set<Kernel::SharedPtr<Kernel::Mutex>>& list);
    QString GetText() const override;
    std::vector<std::unique_ptr<WaitTreeItem>> GetChildren() const override;
private:
    const boost::container::flat_set<Kernel::SharedPtr<Kernel::Mutex>>& mutex_list;
};

class WaitTreeThreadList : public WaitTreeExpandableItem {
    Q_OBJECT
public:
    WaitTreeThreadList(const std::vector<Kernel::SharedPtr<Kernel::Thread>>& list);
    QString GetText() const override;
    std::vector<std::unique_ptr<WaitTreeItem>> GetChildren() const override;
private:
    const std::vector<Kernel::SharedPtr<Kernel::Thread>>& thread_list;
};

class WaitTree : public QObject {
    Q_OBJECT
public:
    void Init(std::vector<std::unique_ptr<WaitTreeItem>>&& root_list);
    void Clear();
    void ToggleExpand(size_t index);
    bool IsExpandable(size_t index) const;
    bool IsExpanded(size_t index) const;
    size_t Size() const;
    QString GetText(size_t index) const;
    int GetLevel(size_t index) const;
private:
    std::vector<std::pair<std::unique_ptr<WaitTreeItem>, int>> list;
};

class WaitTreeModel : public QAbstractListModel
{
    Q_OBJECT

public:
    WaitTreeModel(QObject* parent);

    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    void ClearItems();
    void InitItems();
public slots:
    void OnToggleExpandItem(const QModelIndex& index);
private:
    WaitTree wait_tree;
};

class WaitTreeWidget : public QDockWidget
{
    Q_OBJECT

public:
    WaitTreeWidget(QWidget* parent = nullptr);

public slots:
    void OnDebugModeEntered();
    void OnDebugModeLeft();

    void OnEmulationStarting(EmuThread* emu_thread);
    void OnEmulationStopping();

private:
    QListView* view;
    WaitTreeModel* model;
};
