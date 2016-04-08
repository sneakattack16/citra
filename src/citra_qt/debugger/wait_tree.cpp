// Copyright 2016 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.


#include "core/core.h"

#include "citra_qt/util/util.h"
#include "citra_qt/debugger/wait_tree.h"

bool WaitTreeItem::IsExpandable() const {
    return false;
}
WaitTreeItem::~WaitTreeItem() {

}


WaitTreeText::WaitTreeText(const QString& text)
    : text(text){
}
QString WaitTreeText::GetText() const {
    return text;
}

bool WaitTreeExpandableItem::IsExpandable() const {
    return true;
}


WaitTreeWaitObject::WaitTreeWaitObject(const Kernel::WaitObject& object)
    : object(object) {
}
QString WaitTreeWaitObject::GetText() const {
    return tr("[%1]%2 %3").arg(object.GetObjectId())
        .arg(QString::fromStdString(object.GetTypeName()),
            QString::fromStdString(object.GetName()));
}
std::unique_ptr<WaitTreeWaitObject> WaitTreeWaitObject::make(const Kernel::WaitObject& object) {
    switch (object.GetHandleType()) {
    case Kernel::HandleType::Event:
        return std::make_unique<WaitTreeEvent>(reinterpret_cast<const Kernel::Event&>(object));
    case Kernel::HandleType::Mutex:
        return std::make_unique<WaitTreeMutex>(reinterpret_cast<const Kernel::Mutex&>(object));
    case Kernel::HandleType::Semaphore:
        return std::make_unique<WaitTreeSemaphore>(reinterpret_cast<const Kernel::Semaphore&>(object));
    case Kernel::HandleType::Timer:
        return std::make_unique<WaitTreeTimer>(reinterpret_cast<const Kernel::Timer&>(object));
    case Kernel::HandleType::Thread:
        return std::make_unique<WaitTreeThread>(reinterpret_cast<const Kernel::Thread&>(object));
    default:
        return std::make_unique<WaitTreeWaitObject>(object);
    }
}
std::vector<std::unique_ptr<WaitTreeItem>> WaitTreeWaitObject::GetChildren() const {
    std::vector<std::unique_ptr<WaitTreeItem>> list;

    const auto& threads=object.GetWaitingThreads();
    if (threads.empty()) {
        list.push_back(std::make_unique<WaitTreeText>(tr("waited by no thread")));
    } else {
        list.push_back(std::make_unique<WaitTreeThreadList>(threads));
    }
    return list;
}

WaitTreeObjectList::WaitTreeObjectList(const std::vector<Kernel::SharedPtr<Kernel::WaitObject>>& list, bool wait_all)
    : object_list(list), wait_all(wait_all) {
}
QString WaitTreeObjectList::GetText() const {
    if (wait_all) {
        return tr("waiting for all of objects");
    } else {
        return tr("waiting for one of objects");
    }
}
std::vector<std::unique_ptr<WaitTreeItem>> WaitTreeObjectList::GetChildren() const {
    std::vector<std::unique_ptr<WaitTreeItem>> list(object_list.size());
    std::transform(object_list.begin(), object_list.end(), list.begin(),
        [](const auto& t){return WaitTreeWaitObject::make(*t); }
    );
    return list;
}

WaitTreeThread::WaitTreeThread(const Kernel::Thread& thread)
    : WaitTreeWaitObject(thread) {
}
QString WaitTreeThread::GetText() const {
    const auto& thread = reinterpret_cast<const Kernel::Thread&>(object);
    QString status;
    switch (thread.status) {
    case THREADSTATUS_RUNNING:
        status = tr("running");
        break;
    case THREADSTATUS_READY:
        status = tr("ready");
        break;
    case THREADSTATUS_WAIT_ARB:
        status = tr("waiting for address 0x%1")
            .arg(thread.wait_address,8, 16, QLatin1Char('0'));
        break;
    case THREADSTATUS_WAIT_SLEEP:
        status = tr("sleeping");
        break;
    case THREADSTATUS_WAIT_SYNCH:
        status = tr("waiting for objects");
        break;
    case THREADSTATUS_DORMANT:
        status = tr("dormant");
        break;
    case THREADSTATUS_DEAD:
        status = tr("dead");
        break;
    }
    return WaitTreeWaitObject::GetText() + " (" + status + ")";
}
std::vector<std::unique_ptr<WaitTreeItem>> WaitTreeThread::GetChildren() const {
    std::vector<std::unique_ptr<WaitTreeItem>> list(WaitTreeWaitObject::GetChildren());
    const auto& thread = reinterpret_cast<const Kernel::Thread&>(object);
    list.push_back(std::make_unique<WaitTreeText>(tr("thread id = %1")
        .arg(thread.GetThreadId())));
    list.push_back(std::make_unique<WaitTreeText>(tr("priority = %1(current) / %2(normal)")
        .arg(thread.current_priority)
        .arg(thread.nominal_priority)));
    list.push_back(std::make_unique<WaitTreeText>(tr("last running ticks = %1")
        .arg(thread.last_running_ticks)));
    if (thread.held_mutexes.empty()) {
        list.push_back(std::make_unique<WaitTreeText>(tr("not holding mutex")));
    } else {
        list.push_back(std::make_unique<WaitTreeMutexList>(thread.held_mutexes));
    }
    if (thread.status == THREADSTATUS_WAIT_SYNCH) {
        list.push_back(std::make_unique<WaitTreeObjectList>(thread.wait_objects,thread.wait_all));
    }

    return list;
}

WaitTreeEvent::WaitTreeEvent(const Kernel::Event& object)
    : WaitTreeWaitObject(object) {
}
std::vector<std::unique_ptr<WaitTreeItem>> WaitTreeEvent::GetChildren() const {
    std::vector<std::unique_ptr<WaitTreeItem>> list(WaitTreeWaitObject::GetChildren());

    QString reset_type;
    switch (reinterpret_cast<const Kernel::Event&>(object).reset_type) {
    case Kernel::ResetType::OneShot:
        reset_type = tr("one shot");
        break;
    case Kernel::ResetType::Sticky:
        reset_type = tr("sticky");
        break;
    case Kernel::ResetType::Pulse:
        reset_type = tr("pulse");
        break;
    }
    list.push_back(std::make_unique<WaitTreeText>(tr("reset type = %1").arg(reset_type)));
    return list;
}

WaitTreeMutex::WaitTreeMutex(const Kernel::Mutex& object)
    : WaitTreeWaitObject(object) {

}
std::vector<std::unique_ptr<WaitTreeItem>> WaitTreeMutex::GetChildren() const {
    std::vector<std::unique_ptr<WaitTreeItem>> list(WaitTreeWaitObject::GetChildren());

    const auto& mutex = reinterpret_cast<const Kernel::Mutex&>(object);
    if (mutex.lock_count) {
        list.push_back(std::make_unique<WaitTreeText>(tr("locked %1 times by thread:")
            .arg(mutex.lock_count)));
        list.push_back(std::make_unique<WaitTreeThread>(*mutex.holding_thread));
    }
    else {
        list.push_back(std::make_unique<WaitTreeText>(tr("free")));
    }
    return list;
}

WaitTreeSemaphore::WaitTreeSemaphore(const Kernel::Semaphore& object)
    : WaitTreeWaitObject(object) {
}
std::vector<std::unique_ptr<WaitTreeItem>> WaitTreeSemaphore::GetChildren() const {
    std::vector<std::unique_ptr<WaitTreeItem>> list(WaitTreeWaitObject::GetChildren());

    const auto& semaphore = reinterpret_cast<const Kernel::Semaphore&>(object);
    list.push_back(std::make_unique<WaitTreeText>(tr("available count = %1")
        .arg(semaphore.available_count)));
    list.push_back(std::make_unique<WaitTreeText>(tr("max count = %1")
        .arg(semaphore.max_count)));
    return list;
}

WaitTreeTimer::WaitTreeTimer(const Kernel::Timer& object)
    : WaitTreeWaitObject(object){
}
std::vector<std::unique_ptr<WaitTreeItem>> WaitTreeTimer::GetChildren() const{
    std::vector<std::unique_ptr<WaitTreeItem>> list(WaitTreeWaitObject::GetChildren());

    const auto& timer = reinterpret_cast<const Kernel::Timer&>(object);
    list.push_back(std::make_unique<WaitTreeText>(tr("initial delay = %1")
        .arg(timer.initial_delay)));
    list.push_back(std::make_unique<WaitTreeText>(tr("interval_delay = %1")
        .arg(timer.interval_delay)));
    return list;
}

WaitTreeMutexList::WaitTreeMutexList(const boost::container::flat_set<Kernel::SharedPtr<Kernel::Mutex>>& list)
    : mutex_list(list) {
}
QString WaitTreeMutexList::GetText() const {
    return tr("holding mutexes");
}
std::vector<std::unique_ptr<WaitTreeItem>> WaitTreeMutexList::GetChildren() const {
    std::vector<std::unique_ptr<WaitTreeItem>> list(mutex_list.size());
    std::transform(mutex_list.begin(), mutex_list.end(), list.begin(),
        [](const auto& t){return std::make_unique<WaitTreeMutex>(*t); });
    return list;
}

WaitTreeThreadList::WaitTreeThreadList(const std::vector<Kernel::SharedPtr<Kernel::Thread>>& list)
    : thread_list(list) {
}
QString WaitTreeThreadList::GetText() const {
    return tr("waited by thread");
}
std::vector<std::unique_ptr<WaitTreeItem>> WaitTreeThreadList::GetChildren() const  {
    std::vector<std::unique_ptr<WaitTreeItem>> list(thread_list.size());
    std::transform(thread_list.begin(), thread_list.end(), list.begin(),
        [](const auto& t){return std::make_unique<WaitTreeThread>(*t); });
    return list;
}





template<typename T>
std::vector<std::pair<T, int>> AssignLevel(std::vector<T>&& list, int level) {
    std::vector<std::pair<T, int>> new_list(list.size());
    std::transform(std::make_move_iterator(list.begin()),
        std::make_move_iterator(list.end()),
        new_list.begin(), [level](T&& t){return std::make_pair(std::move(t), level); });
    return new_list;
}

void WaitTree::Init(std::vector<std::unique_ptr<WaitTreeItem>>&& root_list) {
    list = AssignLevel(std::move(root_list), 0);
}

void WaitTree::ToggleExpand(size_t index) {
    if (!list[index].first->IsExpandable()) return;
    if (IsExpanded(index)) {
        int level = list[index].second;
        auto child_begin = list.begin() + index + 1;
        auto child_end = std::find_if_not(child_begin, list.end(),
            [level](const auto& item){return item.second>level; });
        list.erase(child_begin, child_end);
    }
    else{
        auto child = AssignLevel(reinterpret_cast<WaitTreeExpandableItem&>(*list[index].first).GetChildren(),
            list[index].second + 1);
        list.insert(list.begin() + index + 1,
            std::make_move_iterator(child.begin()),
            std::make_move_iterator(child.end())
            );
    }

}


WaitTreeModel::WaitTreeModel(QObject* parent)
    : QAbstractListModel(parent){

}
int WaitTreeModel::columnCount(const QModelIndex& parent) const {
    return 1;
}
int WaitTreeModel::rowCount(const QModelIndex& parent) const {
    return (int)wait_tree.Size();
}
QVariant WaitTreeModel::data(const QModelIndex& index, int role) const {
    switch (role) {
    case Qt::DisplayRole:
    {
        int i = index.row();
        char expand_mark = wait_tree.IsExpandable(i) ? wait_tree.IsExpanded(i) ?
            '-' : '+' : ' ';
        return QString(wait_tree.GetLevel(i) * 4, ' ')
            + expand_mark + ' ' + wait_tree.GetText(i);
    }
    case Qt::FontRole:
        return GetMonospaceFont();
    }
    return QVariant();

}

void WaitTreeModel::ClearItems() {
    wait_tree.Clear();
    emit layoutChanged();
}
void WaitTreeModel::InitItems() {
    const auto& threads(Kernel::GetThreadList());

    std::vector<std::unique_ptr<WaitTreeItem>> list(threads.size());
    std::transform(threads.begin(), threads.end(), list.begin(),
        [](const auto& t){return std::make_unique<WaitTreeThread>(*t); });
    wait_tree.Init(std::move(list));
    emit layoutChanged();
}

void WaitTreeModel::OnToggleExpandItem(const QModelIndex& index) {
    wait_tree.ToggleExpand(index.row());
    emit layoutChanged();
}

WaitTreeWidget::WaitTreeWidget(QWidget* parent)
    : QDockWidget(tr("Wait Tree"), parent) {
    setObjectName("WaitTreeWidget");
    view = new QListView();
    setWidget(view);
    setEnabled(false);
}

void WaitTreeWidget::OnDebugModeEntered() {
    if (!Core::g_app_core) return;
    model->InitItems();
    setEnabled(true);
}

void WaitTreeWidget::OnDebugModeLeft() {
    setEnabled(false);
    model->ClearItems();
}

void WaitTreeWidget::OnEmulationStarting(EmuThread* emu_thread) {
    model = new WaitTreeModel(this);
    view->setModel(model);
    connect(view, SIGNAL(clicked(const QModelIndex &)),
        model, SLOT(OnToggleExpandItem(const QModelIndex &)));
    setEnabled(false);
}

void WaitTreeWidget::OnEmulationStopping() {
    view->setModel(nullptr);
    delete model;
    setEnabled(false);
}