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

#ifndef BITCOIN_QT_BANTABLEMODEL_H
#define BITCOIN_QT_BANTABLEMODEL_H

#include <addrdb.h>
#include <net.h>

#include <memory>

#include <QAbstractTableModel>
#include <QStringList>

class BanTablePriv;

namespace interfaces {
    class Node;
}

struct CCombinedBan {
    CSubNet subnet;
    CBanEntry banEntry;
};

class BannedNodeLessThan
{
public:
    BannedNodeLessThan(int nColumn, Qt::SortOrder fOrder) :
        column(nColumn), order(fOrder) {}
    bool operator()(const CCombinedBan& left, const CCombinedBan& right) const;

private:
    int column;
    Qt::SortOrder order;
};

/**
   Qt model providing information about banned peers, similar to the
   "getpeerinfo" RPC call. Used by the rpc console UI.
 */
class BanTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit BanTableModel(interfaces::Node& node, QObject* parent);
    ~BanTableModel();
    void startAutoRefresh();
    void stopAutoRefresh();

    enum ColumnIndex {
        Address = 0,
        Bantime = 1
    };

    /** @name Methods overridden from QAbstractTableModel
        @{*/
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    void sort(int column, Qt::SortOrder order) override;
    /*@}*/

    bool shouldShow();

    bool unban(const QModelIndex& index);

public Q_SLOTS:
    void refresh();

private:
    interfaces::Node& m_node;
    QStringList columns;
    std::unique_ptr<BanTablePriv> priv;
};

#endif // BITCOIN_QT_BANTABLEMODEL_H
