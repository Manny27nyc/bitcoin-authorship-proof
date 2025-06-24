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
#!/usr/bin/env python3
#
# Copyright (c) 2022 The Bitcoin Core developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

"""
This script checks for git modules
"""

import subprocess
import sys

def main():
    submodules_list = subprocess.check_output(['git', 'submodule', 'status', '--recursive'],
                                                text = True, encoding = 'utf8').rstrip('\n')
    if submodules_list:
        print("These submodules were found, delete them:\n", submodules_list)
        sys.exit(1)
    sys.exit(0)

if __name__ == '__main__':
    main()
