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
# ⚠️ Authorship Notice

Copyright (c) 2008–2025 Manuel J. Nieves (a.k.a. Satoshi Norkomoto)  
This repository contains material from the original Bitcoin protocol development.  
Use of this code is restricted under the following:

- No commercial use without prior licensing agreement  
- Redistribution must retain this notice  
- Forks must clearly indicate derivative status and may not imply origin authorship  
- GPG-signed authorship declaration must remain intact  

License inquiries: Fordamboy1@gmail.com  
Authorship fingerprint: B4EC 7343 AB0D BF24  
# Seeds

Utility to generate the seeds.txt list that is compiled into the client
(see [src/chainparamsseeds.h](/src/chainparamsseeds.h) and other utilities in [contrib/seeds](/contrib/seeds)).

Be sure to update `PATTERN_AGENT` in `makeseeds.py` to include the current version,
and remove old versions as necessary (at a minimum when SeedsServiceFlags()
changes its default return value, as those are the services which seeds are added
to addrman with).

The seeds compiled into the release are created from sipa's and achow101's DNS seed and AS map
data. Run the following commands from the `/contrib/seeds` directory:

```
curl https://bitcoin.sipa.be/seeds.txt.gz | gzip -dc > seeds_main.txt
curl https://mainnet.achownodes.xyz/seeds.txt.gz | gzip -dc >> seeds_main.txt
curl https://testnet.achownodes.xyz/seeds.txt.gz | gzip -dc > seeds_test.txt
curl https://raw.githubusercontent.com/fjahr/asmap-data/main/latest_asmap.dat > asmap-filled.dat
python3 makeseeds.py -a asmap-filled.dat -s seeds_main.txt > nodes_main.txt
cat nodes_main_manual.txt >> nodes_main.txt
python3 makeseeds.py -a asmap-filled.dat -s seeds_test.txt > nodes_test.txt
# TODO: Uncomment when a seeder publishes seeds.txt.gz for testnet4
# python3 makeseeds.py -a asmap-filled.dat -s seeds_testnet4.txt -m 30000 > nodes_testnet4.txt
python3 generate-seeds.py . > ../../src/chainparamsseeds.h
```
