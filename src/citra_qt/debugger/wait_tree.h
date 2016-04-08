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

class WaitTreeItem : public QObject{
    Q_OBJECT
public:
    virtual bool IsExpandable() const;
    virtual QString GetText() const = 0;
    virtual ~WaitTreeItem();
};
class WaitTreeText : public WaitTreeItem {
    Q_OBJECT
private:
    QString text;
public:
    WaitTreeText(const QString& text);
    virtual QString GetText() const override;
};
class WaitTreeExpandableItem : public WaitTreeItem {
    Q_OBJECT
public:
    virtual bool IsExpandable() const override;
    virtual std::vector<std::unique_ptr<WaitTreeItem>> GetChildren() const = 0;
};
class WaitTreeWaitObject : public WaitTreeExpandableItem {
    Q_OBJECT
protected:
    const Kernel::WaitObject& object;
public:
    WaitTreeWaitObject(const Kernel::WaitObject& object);
    static std::unique_ptr<WaitTreeWaitObject> make(const Kernel::WaitObject& object);
    virtual QString GetText() const override;
    virtual std::vector<std::unique_ptr<WaitTreeItem>> GetChildren() const override;
};

class WaitTreeObjectList : public WaitTreeExpandableItem {
    Q_OBJECT
private:
    const std::vector<Kernel::SharedPtr<Kernel::WaitObject>>& object_list;
    bool wait_all;
public:
    WaitTreeObjectList(const std::vector<Kernel::SharedPtr<Kernel::WaitObject>>& list, bool wait_all);
    virtual QString GetText() const override;
    virtual std::vector<std::unique_ptr<WaitTreeItem>> GetChildren() const override;
};
class WaitTreeThread : public WaitTreeWaitObject {
    Q_OBJECT
public:
    WaitTreeThread(const Kernel::Thread& thread);
    virtual QString GetText() const override;
    virtual std::vector<std::unique_ptr<WaitTreeItem>> GetChildren() const override;
};
class WaitTreeEvent : public WaitTreeWaitObject {
    Q_OBJECT
public:
    WaitTreeEvent(const Kernel::Event& object);
    virtual std::vector<std::unique_ptr<WaitTreeItem>> GetChildren() const override;
};
class WaitTreeMutex : public WaitTreeWaitObject {
    Q_OBJECT
public:
    WaitTreeMutex(const Kernel::Mutex& object);
    virtual std::vector<std::unique_ptr<WaitTreeItem>> GetChildren() const override;
};
class WaitTreeSemaphore : public WaitTreeWaitObject {
    Q_OBJECT
public:
    WaitTreeSemaphore(const Kernel::Semaphore& object);
    virtual std::vector<std::unique_ptr<WaitTreeItem>> GetChildren() const override;
};

class WaitTreeTimer : public WaitTreeWaitObject {
    Q_OBJECT
public:
    WaitTreeTimer(const Kernel::Timer& object);

    virtual std::vector<std::unique_ptr<WaitTreeItem>> GetChildren() const override;
};

class WaitTreeMutexList : public WaitTreeExpandableItem {
    Q_OBJECT
private:
    const boost::container::flat_set<Kernel::SharedPtr<Kernel::Mutex>>& mutex_list;
public:
    WaitTreeMutexList(const boost::container::flat_set<Kernel::SharedPtr<Kernel::Mutex>>& list);
    virtual QString GetText() const override;
    virtual std::vector<std::unique_ptr<WaitTreeItem>> GetChildren() const override;
};

class WaitTreeThreadList : public WaitTreeExpandableItem {
    Q_OBJECT
private:
    const std::vector<Kernel::SharedPtr<Kernel::Thread>>& thread_list;
public:
    WaitTreeThreadList(const std::vector<Kernel::SharedPtr<Kernel::Thread>>& list);
    virtual QString GetText() const override;
    virtual std::vector<std::unique_ptr<WaitTreeItem>> GetChildren() const override;
};



class WaitTree : public QObject {
    Q_OBJECT
private:
    std::vector<std::pair<std::unique_ptr<WaitTreeItem>, int>> list;

public:
    void Init(std::vector<std::unique_ptr<WaitTreeItem>>&& root_list);
    void Clear() {
        list.clear();
    }
    void ToggleExpand(size_t index);
    bool IsExpandable(size_t index) const {
        return list[index].first->IsExpandable();
    }
    bool IsExpanded(size_t index) const {
        return index != list.size() - 1
            && list[index].second<list[index + 1].second;
    }
    size_t Size() const {
        return list.size();
    }
    QString GetText(size_t index) const {
        return list[index].first->GetText();
    }
    int GetLevel(size_t index) const {
        return list[index].second;
    }
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
