#include "ArchiveModel.h"
#include "playarchive/FormatManager.h"
#include <QFuture>
#include <QtConcurrent>

using namespace PlayArchive;

ArchiveModel::ArchiveModel(QObject *parent) : QAbstractListModel(parent) {}

int ArchiveModel::rowCount(const QModelIndex &parent) const {
    return m_entries.count();
}

QVariant ArchiveModel::data(const QModelIndex &index, int role) const {
    if (index.row() < 0 || index.row() >= m_entries.count()) return QVariant();
    const auto &entry = m_entries[index.row()];
    if (role == PathRole) return entry.path;
    if (role == SizeRole) return entry.size;
    if (role == IsDirRole) return entry.isDir;
    return QVariant();
}

QHash<int, QByteArray> ArchiveModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[PathRole] = "path";
    roles[SizeRole] = "size";
    roles[IsDirRole] = "isDir";
    return roles;
}

void ArchiveModel::openArchive(const QString &path) {
    auto handler = FormatManager::instance().getHandler(path.toStdString());
    if (!handler) {
        setStatus("Unsupported format");
        return;
    }

    beginResetModel();
    m_entries.clear();
    auto entries = handler->list(path.toStdString());
    for (const auto &ae : entries) {
        m_entries.append({QString::fromStdString(ae.path), ae.size, ae.isDirectory});
    }
    endResetModel();
    setStatus(QString("Loaded %1 entries").arg(m_entries.count()));
}

void ArchiveModel::extractArchive(const QString &archivePath, const QString &destPath) {
    auto handler = FormatManager::instance().getHandler(archivePath.toStdString());
    if (!handler) return;

    setStatus("Extracting...");
    QtConcurrent::run([=]() {
        handler->extract(archivePath.toStdString(), destPath.toStdString(), [this](uint64_t cur, uint64_t total, const std::string &file) {
            // In a real app, use signals for thread safety
            QMetaObject::invokeMethod(this, [=]() {
                setStatus(QString("Extracting: %1").arg(QString::fromStdString(file)));
            });
        });
        QMetaObject::invokeMethod(this, [=]() {
            setStatus("Extraction complete");
        });
    });
}

void ArchiveModel::createArchive(const QString &archivePath, const QStringList &sources) {
    auto handler = FormatManager::instance().getHandler(archivePath.toStdString());
    if (!handler) return;

    setStatus("Creating archive...");
    std::vector<std::string> stdSources;
    for (const auto &s : sources) stdSources.push_back(s.toStdString());

    QtConcurrent::run([=]() {
        handler->create(archivePath.toStdString(), stdSources, [this](uint64_t cur, uint64_t total, const std::string &file) {
            QMetaObject::invokeMethod(this, [=]() {
                setStatus(QString("Packing: %1").arg(QString::fromStdString(file)));
            });
        });
        QMetaObject::invokeMethod(this, [=]() {
            setStatus("Archive created");
        });
    });
}

void ArchiveModel::setProgress(float p) {
    if (m_progress != p) {
        m_progress = p;
        emit progressChanged();
    }
}

void ArchiveModel::setStatus(const QString &s) {
    if (m_status != s) {
        m_status = s;
        emit statusChanged();
    }
}
