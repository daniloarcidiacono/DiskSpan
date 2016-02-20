#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"
#include "Item.h"

// Forward declarations
class QFileSystemModel;
class ItemsModel;
class ItemDetailsModel;
class Model;

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindowClass ui;

    // Document
    Model *document;

    // Models
	QFileSystemModel *model;
	ItemsModel *itemsModel;
	ItemDetailsModel *itemDetailsModel;

protected:
	void createModels();
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
    void showCentered();    

	// Service methods
	void removeItems(const QList<Item *> &itemsToRemove);
	void removeCurrentItem();
	void addItem(Item *item);
	void removeEntriesFromCurrentItem(const QList<Item::ItemEntry *> &itemsToRemove);

protected slots:
	void onRootPathChanged(const QString &value);
	void onTreeFSSelectionChanged();
	void onItemsSelectionChanged();
	void onItemDetailsSelectionChanged();
	void on_btnAddItem_clicked();
	void on_btnRemoveItem_clicked();
	void on_btnRemoveFromItem_clicked();
    void about();

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
};

#endif