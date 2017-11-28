// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "assert.h"

#include "chainparams.h"
#include "main.h"

#include <boost/assign/list_of.hpp>

using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"

int64_t CChainParams::GetProofOfWorkReward(int nHeight, int64_t nFees) const
{
    // miner's coin base reward
    int64_t nSubsidy = 0;

    if (nHeight <= 1)
        nSubsidy = PREMINE * COIN;
    else if(nHeight >= 1 && nHeight < ONE_YEAR_BLOCKS * 4)
    {
        nSubsidy = 20 * COIN;
    }
    else if(nHeight >= ONE_YEAR_BLOCKS * 4 && nHeight < ONE_YEAR_BLOCKS * 8)
    {
        nSubsidy = 10 * COIN;
    }
        else if(nHeight >= ONE_YEAR_BLOCKS * 8 && nHeight < ONE_YEAR_BLOCKS * 12)
    {
        nSubsidy = 5 * COIN;
    }
        else if(nHeight >= ONE_YEAR_BLOCKS * 12 && nHeight < ONE_YEAR_BLOCKS * 16)
    {
        nSubsidy = 2.50 * COIN;
    }
        else if(nHeight >= ONE_YEAR_BLOCKS * 16 && nHeight < ONE_YEAR_BLOCKS * 20)
    {
        nSubsidy = 1.25 * COIN;
    }

    if (fDebug && GetBoolArg("-printcreation"))
        LogPrintf("GetProofOfWorkReward() : create=%s nSubsidy=%d\n", FormatMoney(nSubsidy).c_str(), nSubsidy);

    return nSubsidy + nFees;
};


int64_t CChainParams::GetProofOfStakeReward(const CBlockIndex* pindexPrev, int64_t nCoinAge, int64_t nFees) const
{
    // miner's coin stake reward based on coin age spent (coin-days)
    int64_t nDiv = (365 * 33 + 8);
    int64_t nSubsidy = 0;

    if( pindexPrev->nHeight < ONE_YEAR_BLOCKS)
    {
        nSubsidy = 4 * COIN_YEAR_REWARD * nCoinAge *33 / nDiv;
    }
    else if( pindexPrev->nHeight < 2 * ONE_YEAR_BLOCKS)
    {
        nSubsidy = 3 * COIN_YEAR_REWARD * nCoinAge * 33 / nDiv;
    }
    else if ( pindexPrev->nHeight < 3 * ONE_YEAR_BLOCKS)
    {
        nSubsidy = 2 * COIN_YEAR_REWARD * nCoinAge * 33 / nDiv;
    }
    else
    {
        nSubsidy = COIN_YEAR_REWARD * nCoinAge * 33 / nDiv;
    }

    if (fDebug && GetBoolArg("-printcreation"))
        LogPrintf("GetProofOfStakeReward(): create=%s nCoinAge=%d\n", FormatMoney(nSubsidy).c_str(), nCoinAge);

    return nSubsidy + nFees;
}

//
// Main network
//

// Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress> &vSeedsOut, const SeedSpec6 *data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7*24*60*60;
    for (unsigned int i = 0; i < count; i++)
    {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

// Convert the pnSeeds array into usable address objects.
static void convertSeeds(std::vector<CAddress> &vSeedsOut, const unsigned int *data, unsigned int count, int port)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7*24*60*60;
    for (unsigned int k = 0; k < count; ++k)
    {
        struct in_addr ip;
        unsigned int i = data[k], t;

        // -- convert to big endian
        t =   (i & 0x000000ff) << 24u
            | (i & 0x0000ff00) << 8u
            | (i & 0x00ff0000) >> 8u
            | (i & 0xff000000) >> 24u;

        memcpy(&ip, &t, sizeof(ip));

        CAddress addr(CService(ip, port));
        addr.nTime = GetTime()-GetRand(nOneWeek)-nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

class CBaseChainParams : public CChainParams {
public:
    CBaseChainParams() {
        const char* pszTimestamp = "2017/11/27 Politics: Congress Returns to Intense Pressure to End Secrecy Over Sex Harassment";
        CTransaction txNew;
        txNew.nTime = GENESIS_BLOCK_TIME;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 0 << CBigNum(42) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].SetEmpty();
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime    = GENESIS_BLOCK_TIME;

        vSeeds.push_back(CDNSSeedData("14.63.226.166",  "172.30.1.1"));
        vSeeds.push_back(CDNSSeedData("14.63.226.166",  "172.30.1.7"));
    }
    virtual const CBlock& GenesisBlock() const { return genesis; }
    virtual const std::vector<CAddress>& FixedSeeds() const {
        return vFixedSeeds;
    }
protected:
    CBlock genesis;
    std::vector<CAddress> vFixedSeeds;
};

class CMainParams : public CBaseChainParams {
public:
    CMainParams() {
        strNetworkID = "main";

        // The message start string is designed to be unlikely to occur in normal data.
        // The characters are rarely used upper ASCII, not valid as UTF-8, and produce
        // a large 4-byte int at any alignment.
        pchMessageStart[0] = 0xdf;
        pchMessageStart[1] = 0xb2;
        pchMessageStart[2] = 0x94;
        pchMessageStart[3] = 0x2c;

        //vAlertPubKey = ParseHex("031d5def92b2d59943e57aaa8b1adbb110ff215fc4ebdc6fb5c9a797e2b1dea527");
        vAlertPubKey = ParseHex("04e6480b31b13352355cd4072def967e52d928130cb33eded45d995251e627b3ecbe6637cfab15dc7017fb4de258daebd89ed3adfb0a2867c1d723d039c4392e4a");

        nDefaultPort = 13800;
        nRPCPort = 13801;
        nBIP44ID = 0x80000023;

        nLastPOWBlock = ONE_YEAR_BLOCKS *LAST_POW_YEARS;     

        nFirstPosv2Block = nLastPOWBlock + 1; // pskbank
        nFirstPosv3Block = nLastPOWBlock + 2; // pskbank

        bnProofOfWorkLimit = CBigNum(~uint256(0) >> 20); // "standard" scrypt target limit for proof of work, results with 0,000244140625 proof-of-work difficulty
        bnProofOfStakeLimit = CBigNum(~uint256(0) >> 20);
		bnProofOfStakeLimitV2 = CBigNum(~uint256(0) >> 48);


        genesis.nBits    = bnProofOfWorkLimit.GetCompact();
        genesis.nNonce   = 930540; 

        if (false  && (genesis.GetHash() != hashGenesisBlock)) {    // pskbank

                // This will figure out a valid hash and Nonce if you're
                // creating a different genesis block:

                    fprintf(stderr,"Searching for genesis block...\n");
                    uint256 hashTarget = CBigNum().SetCompact(genesis.nBits).getuint256();
                    uint256 thash;
                    while (true)
                       {

                          thash = genesis.GetHash();
                          if (thash <= hashTarget)
                              break;
                          if ((genesis.nNonce & 0xFFF) == 0)
                          {
                              fprintf(stderr,"nonce %08X: hash = %s (target = %s)\n", genesis.nNonce, genesis.GetHash().ToString().c_str(), genesis.hashMerkleRoot.ToString().c_str());
                          }

                           ++genesis.nNonce;
                           if (genesis.nNonce == 0)
                           {
                               printf("NONCE WRAPPED, incrementing time");
                               ++genesis.nTime;
                           }

                       }
        }
/*
        //// debug print
        genesis.print();

        printf("genesis.GetHash() == %s\n", genesis.GetHash().ToString().c_str());
        printf("genesis.hashMerkleRoot == %s\n", genesis.hashMerkleRoot.ToString().c_str());
        printf("genesis.nTime = %u \n", genesis.nTime);
        printf("genesis.nNonce = %u \n", genesis.nNonce);

*/
        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0x00000151e29d3893aab55a046c7c56258001b6cd214c3b49efe719e4f4bc6c9a"));
        assert(genesis.hashMerkleRoot == uint256("0x58cb626cec3e2e4cc63324e07d5bbce4269ba5dff781c988f5f80c1a416fb74a"));

        base58Prefixes[PUBKEY_ADDRESS]      = list_of(63).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[SCRIPT_ADDRESS]      = list_of(125).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[SECRET_KEY]          = list_of(191).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[STEALTH_ADDRESS]     = list_of(75).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_PUBLIC_KEY]      = list_of(0xEE)(0x80)(0x28)(0x6A).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY]      = list_of(0xEE)(0x80)(0x31)(0xE8).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_KEY_HASH]        = list_of(137).convert_to_container<std::vector<unsigned char> >();         // x
        base58Prefixes[EXT_ACC_HASH]        = list_of(83).convert_to_container<std::vector<unsigned char> >();          // a
        base58Prefixes[EXT_PUBLIC_KEY_BTC]  = list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >(); // xprv
        base58Prefixes[EXT_SECRET_KEY_BTC]  = list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >(); // xpub

        //convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));
        convertSeeds(vFixedSeeds, pnSeed, ARRAYLEN(pnSeed), nDefaultPort);
    }

    virtual Network NetworkID() const { return CChainParams::MAIN; }
};
static CMainParams mainParams;

//
// Testnet
//

class CTestNetParams : public CBaseChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        strDataDir = "testnet";

        // The message start string is designed to be unlikely to occur in normal data.
        // The characters are rarely used upper ASCII, not valid as UTF-8, and produce
        // a large 4-byte int at any alignment.
        pchMessageStart[0] = 0xa7;
        pchMessageStart[1] = 0xd1;
        pchMessageStart[2] = 0xc3;
        pchMessageStart[3] = 0xa1;

        //vAlertPubKey = ParseHex("0373d8dce43eb98374bcfff2352cd559e6774fd6a87eef73b2fbdb39b2b0bc0082");
        vAlertPubKey = ParseHex("046f791cd0652c59aea3f327e5d4fd816e32db1c9816a53a81ebff59c67bbabba4ca12204d1ea83d6795993e0398a6b435df1ff91a4ba7f93c6869dbd775789e0b");
        nDefaultPort = 23800;
        nRPCPort = 23801;
        nBIP44ID = 0x80000001;

        nLastPOWBlock = 110;
        nLastFairLaunchBlock = 10;

        nFirstPosv2Block = 1;
        nFirstPosv3Block = 2;

        bnProofOfWorkLimit = CBigNum(~uint256(0) >> 20);
        bnProofOfStakeLimit = CBigNum(~uint256(0) >> 16);

        genesis.nBits  = bnProofOfWorkLimit.GetCompact();
        genesis.nNonce = 930540;
        if (false  && (genesis.GetHash() != hashGenesisBlock)) {    // pskbank

                // This will figure out a valid hash and Nonce if you're
                // creating a different genesis block:

                    fprintf(stderr,"Searching for testnet genesis block...\n");
                    uint256 hashTarget = CBigNum().SetCompact(genesis.nBits).getuint256();
                    uint256 thash;
                    while (true)
                       {

                          thash = genesis.GetHash();
                          if (thash <= hashTarget)
                              break;
                          if ((genesis.nNonce & 0xFFF) == 0)
                          {
                              fprintf(stderr,"nonce %08X: hash = %s (target = %s)\n", genesis.nNonce, genesis.GetHash().ToString().c_str(), genesis.hashMerkleRoot.ToString().c_str());
                          }

                           ++genesis.nNonce;
                           if (genesis.nNonce == 0)
                           {
                               printf("NONCE WRAPPED, incrementing time");
                               ++genesis.nTime;
                           }

                       }
        }
/*
        //// debug print
        genesis.print();

        printf("test genesis.GetHash() == %s\n", genesis.GetHash().ToString().c_str());
        printf("test genesis.hashMerkleRoot == %s\n", genesis.hashMerkleRoot.ToString().c_str());
        printf("test genesis.nTime = %u \n", genesis.nTime);
        printf("test genesis.nNonce = %u \n", genesis.nNonce);
*/
        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("00000151e29d3893aab55a046c7c56258001b6cd214c3b49efe719e4f4bc6c9a"));

        base58Prefixes[PUBKEY_ADDRESS]      = list_of(127).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[SCRIPT_ADDRESS]      = list_of(196).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[SECRET_KEY]          = list_of(255).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[STEALTH_ADDRESS]     = list_of(40).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_PUBLIC_KEY]      = list_of(0x76)(0xC0)(0xFD)(0xFB).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY]      = list_of(0x76)(0xC1)(0x07)(0x7A).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_KEY_HASH]        = list_of(75).convert_to_container<std::vector<unsigned char> >();          // X
        base58Prefixes[EXT_ACC_HASH]        = list_of(23).convert_to_container<std::vector<unsigned char> >();          // A
        base58Prefixes[EXT_PUBLIC_KEY_BTC]  = list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >(); // tprv
        base58Prefixes[EXT_SECRET_KEY_BTC]  = list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >(); // tpub

        //convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));
        convertSeeds(vFixedSeeds, pnTestnetSeed, ARRAYLEN(pnTestnetSeed), nDefaultPort);
    }
    virtual Network NetworkID() const { return CChainParams::TESTNET; }
};
static CTestNetParams testNetParams;


//
// Regression test
//
/*
class CRegTestParams : public CTestNetParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        strDataDir = "regtest";

        nFirstPosv2Block = -1;
        nFirstPosv3Block = -1;

        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0xbf;
        pchMessageStart[2] = 0xb5;
        pchMessageStart[3] = 0xda;
        bnProofOfWorkLimit = CBigNum(~uint256(0) >> 20);
        genesis.nTime = GENESIS_BLOCK_TIME;
        genesis.nBits  = bnProofOfWorkLimit.GetCompact();
        genesis.nNonce = 0;
        hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 18444;
        assert(hashGenesisBlock == uint256("0x0000060d6aaaf8d5f835567060dcfbff843677d10b92ef0e0036cb9b126a9ff0"));

        vSeeds.clear();  // Regtest mode doesn't have any DNS seeds.
    }

    virtual bool RequireRPCPassword() const { return false; }
    virtual Network NetworkID() const { return CChainParams::REGTEST; }
};
*/
//static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = &mainParams;

const CChainParams &Params() {
    return *pCurrentParams;
}

const CChainParams &TestNetParams() {
    return testNetParams;
}

const CChainParams &MainNetParams() {
    return mainParams;
}

void SelectParams(CChainParams::Network network)
{
    switch (network)
    {
        case CChainParams::MAIN:
            pCurrentParams = &mainParams;
            break;
        case CChainParams::TESTNET:
            pCurrentParams = &testNetParams;
            break;
        //case CChainParams::REGTEST:
        //    pCurrentParams = &regTestParams;
        //    break;
        default:
            assert(false && "Unimplemented network");
            return;
    };
};

bool SelectParamsFromCommandLine()
{
    bool fRegTest = GetBoolArg("-regtest", false);
    bool fTestNet = GetBoolArg("-testnet", false);

    if (fTestNet && fRegTest)
    {
        return false;
    };

    if (fRegTest)
    {
        SelectParams(CChainParams::REGTEST);
    } else
    if (fTestNet)
    {
        SelectParams(CChainParams::TESTNET);
    } else
    {
        SelectParams(CChainParams::MAIN);
    };

    return true;
}
