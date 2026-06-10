#pragma once

#include <QAbstractListModel>
#include <QVector>
#include <QString>
#include "playarchive/ArchiveHandler.h"

class ArchiveModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(float progress READ progress NOTIFY progressChanged)
    Q_PROPERTY(QString status READ status NOTIFY statusChanged)

public:
    enum RoleNames {
        PathRole = Qt::UserRole + 1,
        SizeRole,
        IsDirRole
    };

    ArchiveModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void openArchive(const QString &path);
    Q_INVOKABLE void extractArchive(const QString &archivePath, const QString &destPath);
    Q_INVOKABLE void createArchive(const QString &archivePath, const QStringList &sources);

    float progress() const { return m_progress; }
    QString status() const { return m_status; }

signals:
    void progressChanged();
    void statusChanged();

private:
    struct Entry {
        QString path;
        quint64 size;
        bool isDir;
    };
    QVector<Entry> m_entries;
    float m_progress = 0;
    QString m_status;

    void setProgress(float p);
    void setStatus(const QString &s);
};
