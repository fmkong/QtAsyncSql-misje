#pragma once
#include <QAbstractTableModel>
#include "AsyncQueryResult.h"

namespace Database
{
class AsyncQuery;

class AsyncQueryQMLModel : public QAbstractTableModel
{
	Q_OBJECT
	Q_PROPERTY(QString query READ queryString WRITE setQueryString NOTIFY
			queryStringChanged)
	Q_PROPERTY(QStringList columnNames READ columnNames NOTIFY columnNamesChanged)

signals:
	void queryStringChanged(const QString &queryString);
	void columnNamesChanged(const QStringList &columnNames);
	void querySucceeded(const AsyncQueryResult &result);
	void queryFailed(const QString &errorMessage);

public:
	enum PrefixMode
	{
		PrefixTableNameAlways,
		PrefixTableNameOnDuplicate,
		PrefixTableNameNever,
	};
	Q_ENUM(PrefixMode)

	explicit AsyncQueryQMLModel(QObject *parent = nullptr);

	AsyncQuery *asyncQuery() const;
	QString queryString() const;
	QStringList columnNames() const;
	QSqlError error() const;
	AsyncQueryResult result() const;
	PrefixMode prefixMode() const;

	void startExec(const QString &query);
	void clear();

	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
	Q_INVOKABLE QVariant data(int row, const QString &role) const;
	QHash<int, QByteArray> roleNames() const override;

	void setQueryString(const QString &query);
	void setPrefixMode(PrefixMode prefixMode);

public slots:
	void bindValue(const QString &name, const QVariant &value);
	void exec();

private:
	void onExecDone(const Database::AsyncQueryResult &result);
	void updateRoles();
	void setColumnNames(const QStringList &columnNames);
	void updateDuplicateColumnNames(const QSqlRecord &record);
	QString columnName(const QSqlField &field);

	QHash<int, QByteArray> _roleNames;
	QHash<QString, int> _roleIDs;
	QStringList _columnNames;
	AsyncQueryResult _res;
	AsyncQuery *_aQuery;
	PrefixMode _prefixMode;
	QHash<QString, QString> _duplicateColumnNames;
};
};
