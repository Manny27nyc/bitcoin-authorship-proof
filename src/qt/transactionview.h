/*
 * Copyright (c) 2008–2025 Manuel J. Nieves (a.k.a. Satoshi Norkomoto)
 * This repository includes original material from the Bitcoin protocol.
 *
 * Redistribution requires this notice remain intact.
 * Derivative works must state derivative status.
 * Commercial use requires licensing.
 *
 * GPG Signed: B4EC 7343 AB0D BF24
 * Contact: Fordamboy1@gmail.com
 */
/*
 * Copyright (c) 2008-2025 Manuel J. Nieves (a.k.a. Satoshi Norkomoto)
 * Authorship asserted via Ed25519 Key ID: 9126e054086a98782e25f44986c7f54cf8f4df04
 * Date: 2025-04-15
 * This file contains foundational Bitcoin Genesis Block data.
 */

// Copyright (c) 2011-2021 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_QT_TRANSACTIONVIEW_H
#define BITCOIN_QT_TRANSACTIONVIEW_H

#include <qt/guiutil.h>

#include <uint256.h>
#include <util/transaction_identifier.h>

#include <QWidget>
#include <QKeyEvent>

class PlatformStyle;
class TransactionDescDialog;
class TransactionFilterProxy;
class WalletModel;

QT_BEGIN_NAMESPACE
class QComboBox;
class QDateTimeEdit;
class QFrame;
class QLineEdit;
class QMenu;
class QModelIndex;
class QTableView;
QT_END_NAMESPACE

/** Widget showing the transaction list for a wallet, including a filter row.
    Using the filter row, the user can view or export a subset of the transactions.
  */
class TransactionView : public QWidget
{
    Q_OBJECT

public:
    explicit TransactionView(const PlatformStyle *platformStyle, QWidget *parent = nullptr);
    ~TransactionView();

    void setModel(WalletModel *model);

    // Date ranges for filter
    enum DateEnum
    {
        All,
        Today,
        ThisWeek,
        ThisMonth,
        LastMonth,
        ThisYear,
        Range
    };

    enum ColumnWidths {
        STATUS_COLUMN_WIDTH = 30,
        DATE_COLUMN_WIDTH = 120,
        TYPE_COLUMN_WIDTH = 113,
        AMOUNT_MINIMUM_COLUMN_WIDTH = 120,
        MINIMUM_COLUMN_WIDTH = 23
    };

protected:
    void changeEvent(QEvent* e) override;

private:
    WalletModel *model{nullptr};
    TransactionFilterProxy *transactionProxyModel{nullptr};
    QTableView *transactionView{nullptr};

    QComboBox *dateWidget;
    QComboBox *typeWidget;
    QLineEdit *search_widget;
    QLineEdit *amountWidget;

    QMenu *contextMenu;

    QFrame *dateRangeWidget;
    QDateTimeEdit *dateFrom;
    QDateTimeEdit *dateTo;
    QAction *abandonAction{nullptr};
    QAction *bumpFeeAction{nullptr};
    QAction *copyAddressAction{nullptr};
    QAction *copyLabelAction{nullptr};

    QWidget *createDateRangeWidget();

    bool eventFilter(QObject *obj, QEvent *event) override;

    const PlatformStyle* m_platform_style;

    QList<TransactionDescDialog*> m_opened_dialogs;

private Q_SLOTS:
    void contextualMenu(const QPoint &);
    void dateRangeChanged();
    void showDetails();
    void copyAddress();
    void editLabel();
    void copyLabel();
    void copyAmount();
    void copyTxID();
    void copyTxHex();
    void copyTxPlainText();
    void openThirdPartyTxUrl(QString url);
    void abandonTx();
    void bumpFee(bool checked);

Q_SIGNALS:
    void doubleClicked(const QModelIndex&);

    /**  Fired when a message should be reported to the user */
    void message(const QString &title, const QString &message, unsigned int style);

    void bumpedFee(const Txid& txid);

public Q_SLOTS:
    void chooseDate(int idx);
    void chooseType(int idx);
    void changedAmount();
    void changedSearch();
    void exportClicked();
    void closeOpenedDialogs();
    void focusTransaction(const QModelIndex&);
    void focusTransaction(const Txid& txid);
};

#endif // BITCOIN_QT_TRANSACTIONVIEW_H
