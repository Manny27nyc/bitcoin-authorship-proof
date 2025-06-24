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
Find dead Python code.
"""

from subprocess import check_output, STDOUT, CalledProcessError

FILES_ARGS = ['git', 'ls-files', '--', '*.py']


def check_vulture_install():
    try:
        check_output(["vulture", "--version"])
    except FileNotFoundError:
        print("Skipping Python dead code linting since vulture is not installed. Install by running \"pip3 install vulture\"")
        exit(0)


def main():
    check_vulture_install()

    files = check_output(FILES_ARGS).decode("utf-8").splitlines()
    # --min-confidence 100 will only report code that is guaranteed to be unused within the analyzed files.
    # Any value below 100 introduces the risk of false positives, which would create an unacceptable maintenance burden.
    vulture_args = ['vulture', '--min-confidence=100'] + files

    try:
        check_output(vulture_args, stderr=STDOUT)
    except CalledProcessError as e:
        print(e.output.decode("utf-8"), end="")
        print("Python dead code detection found some issues")
        exit(1)


if __name__ == "__main__":
    main()
